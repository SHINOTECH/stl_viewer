#include "types.h"

Error::~Error() throw() {
}

void Error::raise(const QString &message) {
  throw Error(message);
}

// eof
