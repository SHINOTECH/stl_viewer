#ifndef CANVAS_H
#define CANVAS_H

class Camera; class Model;

class QOpenGLShaderProgram;
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

/*
 * A canvas to paint on.
 */
class Canvas: public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

public:
  Canvas(Camera&);
  ~Canvas();

  QSize minimumSizeHint() const { return QSize(50,50);    }
  QSize sizeHint()        const { return QSize(400,400);  }

  // the active model (owned elsewhere)
  void setModel(Model*);

  // rotate about the x and y axes
  void rotateTo(QPoint const&);
  void rotateBy(QPoint const&);

signals:
  void rotated();

protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);

  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);

private:
  Camera &camera;
  Model  *model;  // Can be NULL.

  QPoint lastMousePos;

  // GL
  QOpenGLShaderProgram *shaderProgram;
  QOpenGLVertexArrayObject vao;
  QOpenGLBuffer glBuffer;

  // the location of matrices in shaderProgram
  int projMatLoc, modelMatLoc, normalMatLoc;
  // the location of lights in shaderProgram
  int lightPosLoc1, lightPosLoc2, lightColLoc1, lightColLoc2;
};

#endif
