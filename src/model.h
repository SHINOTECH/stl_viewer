#ifndef MODEL_H
#define MODEL_H

#include "types.h"
#include "stl.h"

#include <qopengl.h>
#include <QDataStream>
#include <QVector>
#include <QMatrix4x4>

class QOpenGLBuffer;

/*
 * The model - triangles - that is created from a binary STL file.
 * Scaled and translated to fit in a bounding box, xyz between -1 and +1.
 * Ready to be rendered in GL.
 */

class Model {
  friend class Canvas;

public:
  Model();
  ~Model();

public:
  /*
   * Factory methods that create a model and load STL data.
   * Return a new model. The caller takes ownership and is responsible
   * for deleting the model later.
   */
  static Model* fromStl(QString const&) throw(Error);
  static Model* fromStl(QDataStream&)   throw(Error);

private:
  void setFacets(stl::facet_vec const&);
  void allocate(QOpenGLBuffer&);
  void drawTriangles();

public:
  // GL data
  GLfloat const* constData() const { return glData.constData(); }
  int count()                const { return glData.count();     }
  int vertexCount()          const { return count()/6;          }

private:
  QVector<GLfloat> glData;

public:
  // world transformation is part of the model
  QMatrix4x4 world;
};

// Automatic pointer type.
typedef std::unique_ptr<Model> model_ptr;

#endif
