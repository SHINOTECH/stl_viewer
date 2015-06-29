#ifndef CAMERA_H
#define CAMERA_H

#include "model.h"
class Canvas;

#include <QColor>
#include <QMatrix4x4>

/*
 * A camera abstraction.
 */
class Camera {
  friend class Canvas;

public:
  Camera();
  ~Camera();

  void setEye();
  void setAspectRatio(qreal);

  // QPoint is used as a handy x-y pair.
  // A proper solution would use three angles or, better, quaternions!
  void   rotateBy(QPoint const&);
  void   rotateTo(QPoint const&);
  QPoint rotatedTo() const;

private:
  QColor      clearColor;
  QMatrix4x4  projection, eye;
  V3D         rotation;

public:
  // Christmas!
  void changeLights();

private:
  // The colours of the two lights used in shading.
  // V3D instead of QColor because alpha is not needed.
  V3D col1, col2;
};

#endif
