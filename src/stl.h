#ifndef STL_H
#define STL_H

#include "types.h"
#include <QVector>
#include <QVector3D>

/*
 * To read binary STL files.
 * See: http://www.fabbers.com/tech/STL_Format
 */

namespace stl {

struct Facet {
  V3D normal, vertex1, vertex2, vertex3;
};

typedef QVector<Facet> facet_vec;

/*
 * Reads one facet from a stream.
 */
QDataStream& operator>>(QDataStream&, Facet&)     throw(Error);

/*
 * Reads the whole content of an STL stream.
 * Performs basic validity checks of the data.
 */
QDataStream& operator>>(QDataStream&, facet_vec&) throw(Error);

}

#endif
