/* This file is part of Into. 
 * Copyright (C) Intopii 2013.
 * All rights reserved.
 *
 * Licensees holding a commercial Into license may use this file in
 * accordance with the commercial license agreement. Please see
 * LICENSE.commercial for commercial licensing terms.
 *
 * Alternatively, this file may be used under the terms of the GNU
 * Affero General Public License version 3 as published by the Free
 * Software Foundation. In addition, Intopii gives you special rights
 * to use Into as a part of open source software projects. Please
 * refer to LICENSE.AGPL3 for details.
 */

#include "PiiRoi.h"
#include <PiiInputSocket.h>
#include <PiiYdinTypes.h>

namespace PiiImage
{
  const char* roiRectangleSizeError =
    QT_TRANSLATE_NOOP("PiiRoi", "Region-of-interest rectangle (%1, %2), (%3-by-%4) exceeds image boundaries (%5-by-%6).");
  const char* roiMaskSizeError =
    QT_TRANSLATE_NOOP("PiiRoi", "Region-of-interest mask size (%1-by-%2) doesn't match image size (%3-by-%4).");
  
  PiiMatrix<bool> toRoiMask(const PiiVariant& obj)
  {
    switch (obj.type())
      {
      case PiiYdin::BoolMatrixType:
      case PiiYdin::CharMatrixType:
      case PiiYdin::UnsignedCharMatrixType:
        // HACK: All 8-bit data types can be treated as bool
        return obj.valueAs<PiiMatrix<bool> >();
      case PiiYdin::ShortMatrixType:
        return PiiMatrix<bool>(obj.valueAs<PiiMatrix<short> >());
      case PiiYdin::IntMatrixType:
        return PiiMatrix<bool>(obj.valueAs<PiiMatrix<int> >());
      case PiiYdin::Int64MatrixType:
        return PiiMatrix<bool>(obj.valueAs<PiiMatrix<qint64> >());
      case PiiYdin::UnsignedShortMatrixType:
        return PiiMatrix<bool>(obj.valueAs<PiiMatrix<short> >());
      case PiiYdin::UnsignedIntMatrixType:
        return PiiMatrix<bool>(obj.valueAs<PiiMatrix<int> >());
      case PiiYdin::UnsignedInt64MatrixType:
        return PiiMatrix<bool>(obj.valueAs<PiiMatrix<quint64> >());
      default:
        PII_THROW(PiiExecutionException,
                  QCoreApplication::translate("PiiRoi",
                                              "Cannot convert an object of type 0x%1 "
                                              "to a region-of-interest mask.").arg(obj.type(), 0, 16));
      }
    return PiiMatrix<bool>();
  }

  PiiMatrix<bool> createRoiMask(int rows, int columns,
                                const PiiMatrix<int>& rectangles)
  {
    PiiMatrix<bool> result(rows, columns);
    for (int r=0; r<rows; ++r)
      {
        const PiiRectangle<int>& rect = rectangles.rowAs<PiiRectangle<int> >(r);
        if (rect.x >=0 && rect.x < columns &&
            rect.y >=0 && rect.y < rows &&
            rect.width > 0 &&
            rect.height > 0 &&
            rect.x + rect.width <= columns &&
            rect.y + rect.height <= rows)
          result(rect.y, rect.x, rect.height, rect.width) = true;
      }
    return result;
  }

  bool overlapping(const PiiMatrix<int>& rectangles)
  {
    const int iRows = rectangles.rows();
    if (iRows <= 1)
      return false;
    for (int r1=0; r1<iRows; ++r1)
      {
        const PiiRectangle<int>& rect = rectangles.rowAs<PiiRectangle<int> >(r1);
        for (int r2=r1+1; r2<iRows; ++r2)
          if (rect.intersects(rectangles.rowAs<PiiRectangle<int> >(r2)))
            return true;
      }
    return false;
  }
}