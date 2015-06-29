#include "stl.h"
#include <QDataStream>

namespace stl {

QDataStream& operator>>(QDataStream& is, Facet& f) throw (Error) {
  is >> f.normal >> f.vertex1 >> f.vertex2 >> f.vertex3;

  /* All coordinates ought to be positive numbers.
   * But since some .stl files that were tried contained non-positive coodinates,
   * this the test is disabled. Put it back if you want.
   */
  /*
  auto isPositive = [](V3D const& v) {
    return v.x()>0 && v.y()>0 && v.z()>0;
  };

  if (!isPositive(f.vertex1) || !isPositive(f.vertex2) || !isPositive(f.vertex3))
    Error::raise("invalid facet coordinates");
  */

  return is;
}

QDataStream& operator>>(QDataStream& is, facet_vec& facets) throw (Error) {
  facets.clear();

  // prepare the stream
  is.setByteOrder(QDataStream::LittleEndian);
  is.setFloatingPointPrecision(QDataStream::SinglePrecision);

  auto isOk = [&]() { return QDataStream::Ok == is.status(); };

  // skip the header
  int const HDR_LEN = 80;
  if (HDR_LEN != is.skipRawData(HDR_LEN)) Error::raise("bad header");

  // load facets
  quint32 numFacets;
  is >> numFacets;

  for (uint n=0; isOk() && (n<numFacets); ++n) {
    Facet facet; quint16 attributeCount;
    is >> facet >> attributeCount;

    if (0 != attributeCount) Error::raise("bad attribute count");

    facets.append(facet);
  }

  if (!isOk())      Error::raise("not good");
  if (!is.atEnd())  Error::raise("bad file length");

  return is;
}

}

// eof
