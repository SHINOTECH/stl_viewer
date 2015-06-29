#include "model.h"
#include "stl.h"

#include <QFile>
#include <QOpenGLBuffer>

Model::Model() {
  world.setToIdentity();
}

Model::~Model() {
}

// create new model, load from STL file by fileName
Model* Model::fromStl(QString const& fileName) throw (Error) {
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly)) Error::raise("cannot open file");

  QDataStream is(&file);
  return fromStl(is);
}

// create new model, load from STL stream
Model* Model::fromStl(QDataStream& is) throw(Error) {
  model_ptr model(new Model); // exception-safe

  stl::facet_vec facets;
  is >> facets;

  model->setFacets(facets);

  return model.release();
}

// prepare GL data from facets
void Model::setFacets(stl::facet_vec const& facets) {
  glData.clear();
  if (facets.isEmpty()) return;

  // STL data are in arbitrary units
  // let us scale the model to -1 .. +1

  // compute the bounding box
  V3D const& fv = facets.first().vertex1;
  float minX = fv.x(), minY = fv.y(), minZ = fv.z(),
        maxX = minX,   maxY = minY,   maxZ = minZ;

  auto minMax = [&](V3D const& v) {
    minX = qMin(minX,v.x());
    minY = qMin(minY,v.y());
    minZ = qMin(minZ,v.z());
    maxX = qMax(maxX,v.x());
    maxY = qMax(maxY,v.y());
    maxZ = qMax(maxZ,v.z());
  };

  foreach (stl::Facet const& facet, facets) {
    minMax(facet.vertex1);
    minMax(facet.vertex2);
    minMax(facet.vertex3);
  }

  V3D minV(minX,minY,minZ), maxV(maxX,maxY,maxZ);

  // now, center and scale the data
  V3D center = minV + (maxV-minV)/2;
  float scale = qMax(maxX-minX,qMax(maxY-minY,maxZ-minZ));

  // scale should be > 0, but not necessarily
  scale = scale > 0 ? 1/scale : 1;

  // transform each vertex, interleave with normal vectors
  auto addTransformed = [&](V3D const& v,V3D const& n) {
    V3D tr = (v - center) * scale;
    glData.append(tr.x()); glData.append(tr.y()); glData.append(tr.z());
    glData.append(n.x());  glData.append(n.y());  glData.append(n.z());
  };

  foreach (stl::Facet const& facet, facets) {
    addTransformed(facet.vertex1,facet.normal);
    addTransformed(facet.vertex2,facet.normal);
    addTransformed(facet.vertex3,facet.normal);
  }
}

// allocate GL buffer
void Model::allocate(QOpenGLBuffer& glBuffer) {
  glBuffer.allocate(constData(), count() * sizeof(GLfloat));
}

// render the data
void Model::drawTriangles() {
  glDrawArrays(GL_TRIANGLES, 0, vertexCount());
}

// eof
