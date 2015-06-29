#include "canvas.h"
#include "camera.h"
#include "model.h"

#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>

Canvas::Canvas(Camera& c): camera(c), model(NULL), shaderProgram(NULL) {
}

Canvas::~Canvas() {
  delete shaderProgram;
}

void Canvas::setModel(Model* m) {
  model = m;
  camera.setEye();
  initializeGL(); // to allocate and initialize glBuffer
  update();
}

void Canvas::rotateTo(QPoint const& rot) {
  camera.rotateTo(rot);
  update();
}

void Canvas::rotateBy(QPoint const& rot) {
  camera.rotateBy(rot);
  update();
}

/*
 * These GPU programs and the following GL initialization and painting code
 * are adapted from the Qt 'hellogl2' example.
 * I do not claim their complete understanding.
 */
static char const *vertexShaderSource =
  "attribute vec4 vertex; attribute vec3 normal;"
  "varying vec3 vert, vertNormal;"
  "uniform mat4 projMatrix, modelMatrix;"
  "uniform mat3 normalMatrix;"
  "void main() {"
  "  vert = vertex.xyz;"
  "  vertNormal = normalMatrix * normal;"
  "  gl_Position = projMatrix * modelMatrix * vertex;"
  "}";

static char const *fragmentShaderSource =
  "varying highp vec3 vert, vertNormal;"
  "uniform highp vec3 lightPos1, lightPos2, lightCol1, lightCol2;"
  "void main() {"
  "  highp vec3 L1 = normalize(lightPos1-vert);"
  "  highp vec3 L2 = normalize(lightPos2-vert);"
  "  highp float NL1 = max(dot(normalize(vertNormal), L1), 0.0);"
  "  highp float NL2 = max(dot(normalize(vertNormal), L2), 0.0);"
  "  highp vec3 color1 = lightCol1;"
  "  highp vec3 color2 = lightCol2;"
  "  highp vec3 col1 = color1 * 0.2 + color1 * 0.6 * NL1;"
  "  highp vec3 col2 = color2 * 0.2 + color2 * 0.6 * NL2;"
  "  highp vec3 col  = clamp(col1 + col2,0.0,1.0);"
  "  gl_FragColor = vec4(col, 1.0);"
  "}";

/*
 * initializeGL can be called multiple times - every time a new model is set
 */
void Canvas::initializeGL() {
  delete shaderProgram;
  shaderProgram = new QOpenGLShaderProgram;

  initializeOpenGLFunctions();

  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
  shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
  shaderProgram->bindAttributeLocation("vertex", 0);
  shaderProgram->bindAttributeLocation("normal", 1);
  shaderProgram->link();

  shaderProgram->bind();
  projMatLoc   = shaderProgram->uniformLocation("projMatrix");
  modelMatLoc  = shaderProgram->uniformLocation("modelMatrix");
  normalMatLoc = shaderProgram->uniformLocation("normalMatrix");
  lightPosLoc1 = shaderProgram->uniformLocation("lightPos1");
  lightPosLoc2 = shaderProgram->uniformLocation("lightPos2");
  lightColLoc1 = shaderProgram->uniformLocation("lightCol1");
  lightColLoc2 = shaderProgram->uniformLocation("lightCol2");

  if (!vao.isCreated()) vao.create();
  QOpenGLVertexArrayObject::Binder bindVao(&vao);

  // vertex buffer
  if (!glBuffer.isCreated()) glBuffer.create();
  glBuffer.bind();
  if (model) model->allocate(glBuffer);

  // vertex attribute bindings
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

  shaderProgram->release();
}

void Canvas::paintGL() {
  QColor &col = camera.clearColor;
  glClearColor(col.redF(),col.greenF(),col.blueF(),1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (model) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QOpenGLVertexArrayObject::Binder bindVao(&vao);
    shaderProgram->bind();

    shaderProgram->setUniformValue(projMatLoc,   camera.projection);
    shaderProgram->setUniformValue(modelMatLoc,  camera.eye * model->world);
    shaderProgram->setUniformValue(normalMatLoc, model->world.normalMatrix());
    shaderProgram->setUniformValue(lightPosLoc1, QVector3D(0,0,10));
    shaderProgram->setUniformValue(lightPosLoc2, QVector3D(10,0,-10));
    shaderProgram->setUniformValue(lightColLoc1, camera.col1);
    shaderProgram->setUniformValue(lightColLoc2, camera.col2);

    model->drawTriangles();

    shaderProgram->release();
  }
}

void Canvas::resizeGL(int w, int h) {
  camera.setAspectRatio(h>0 ? qreal(w)/h : 1);
}

void Canvas::mousePressEvent(QMouseEvent* event) {
  lastMousePos = event->pos();
}

void Canvas::mouseMoveEvent(QMouseEvent* event) {
  QPoint pos = event->pos(), dpos = pos - lastMousePos;
  camera.rotateBy(dpos);
  lastMousePos = pos;
  update();
  emit rotated();
}

void Canvas::mouseReleaseEvent(QMouseEvent*) {
  // nothing here
}

// eof
