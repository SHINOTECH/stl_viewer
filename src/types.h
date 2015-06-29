#ifndef TYPES_H
#define TYPES_H

// useful types

/*
 * A shorter name for often used QVector3D - less typing.
 */

class QVector3D; typedef QVector3D V3D;

/*
 * An exception that carries a message.
 */
#include <QException>
#include <QString>

class Error: public QException {
public:
  QString message;

  Error(QString const& msg): message(msg) {
  }

  Error(Error const& that): message(that.message) {
  }

  ~Error() throw();

  void   raise() const { throw *this; }
  Error* clone() const { return new Error(*this); }

  // Use this method to throw an exception.
  static void raise(QString const& message);
};

#endif
