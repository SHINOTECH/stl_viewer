#include "camera.h"
#include "canvas.h"
#include <qmath.h>

Camera::Camera(): clearColor(Qt::darkGray),
  // Initial light colours are hardcoded. TODO.
  col1(.8,1,.8), col2(1,.8,.8) {
  setEye();
}

Camera::~Camera() {
}

void Camera::setEye() {
  eye.setToIdentity();
  // assumes that the model is scaled to -1 .. +1
  eye.lookAt(V3D(0,0,2),V3D(0,0,0),V3D(0,1,0));

  eye.rotate(rotation.x(), 1, 0, 0);
  eye.rotate(rotation.y(), 0, 1, 0);
}

void Camera::setAspectRatio(qreal ratio) {
  projection.setToIdentity();
  // assumes that the model is scaled to -1 .. +1
  projection.perspective(40, ratio, .01, 100);
}

void Camera::rotateBy(QPoint const& dpos) {
  int const FACTOR = 1; // 1 seems good
  rotateTo(QPoint(rotation.x() + FACTOR*dpos.y(),
                  rotation.y() + FACTOR*dpos.x()));
}

void Camera::rotateTo(QPoint const& r) {
  auto normalize = [](float angle) {
    return angle - qFloor(angle/360) * 360;
  };

  rotation.setX(normalize(r.x()));
  rotation.setY(normalize(r.y()));

  setEye();
}

QPoint Camera::rotatedTo() const {
  return QPoint(rotation.x(),rotation.y());
}

void Camera::changeLights() {
  // random small incremental changes
  auto rnd = [](float v) {
    float const STEP = .1f;
    return qBound(0.0f, v + STEP*(float(qrand())/RAND_MAX - .5f), 1.0f);
  };

  auto rand = [&] (V3D& col) {
    col.setX(rnd(col.x()));
    col.setY(rnd(col.y()));
    col.setZ(rnd(col.z()));
  };

  rand(col1); rand(col2);
}

// eof
