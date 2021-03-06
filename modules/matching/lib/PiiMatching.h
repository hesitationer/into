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

#ifndef _PIIMATCHING_H
#define _PIIMATCHING_H

#include <PiiMatrix.h>
#include <QVector>
#include <QObject>

#include "PiiMatchingPlugin.h"

#ifdef Q_MOC_RUN
class PiiMatching
#else
namespace PiiMatching
#endif
{
#ifdef Q_MOC_RUN
  Q_GADGET

  Q_ENUMS(ModelMatchingMode);
  Q_FLAGS(InvarianceFlag InvarianceFlags);
public:
#endif
  /// @internal
#ifndef PII_NO_QT
  extern PII_MATCHING_EXPORT const QMetaObject staticMetaObject;
#endif

  /**
   * Feature point matching modes.
   *
   * - `MatchOneModel` - match exactly one model. The search for
   * matching models will be terminated once the best matching model
   * is found.
   *
   * - `MatchAllModels` - match any number of models. Many matches to
   * the same model are allowed. For example, the same model may match
   * a query many times at different orientations.
   *
   * - `MatchDifferentModels` - match any number of models, but at
   * most one match is allowed for each model.
   */
  enum ModelMatchingMode { MatchOneModel, MatchAllModels, MatchDifferentModels };

  /**
   * Invariance levels. Some feature point descriptors and matching
   * algorithms have controllable invariance properties. The values in
   * this enumeration can be used as flags to control the behavior of
   * such algorithms.
   *
   * - `NonInvariant` - the algorithm is not invariant to any changes
   * in view geometry
   *
   * - `RotationInvariant` - the algorithm is invariant against
   * 2D (in-plane) rotations.
   *
   * - `ScaleInvariant` - the algorithm is invariant against 2D
   * scaling.
   *
   * - AffineInvariant - the algorithm is invariant against 2D
   * affine transformations (including rotation and scaling).
   */
  enum InvarianceFlag
    {
      NonInvariant = 0,
      RotationInvariant = 1,
      ScaleInvariant = 2,
      AffineInvariant = 4
    };
  Q_DECLARE_FLAGS(InvarianceFlags, InvarianceFlag);

  /**
   * Calculates the shape context descriptor to a selected set of key
   * points. The shape context descriptor is a two-dimensional polar
   * histogram that encodes the locations of boundary points with
   * respect to the key points.
   *
   * @param boundaryPoints all points of a boundary that is going to
   * be described
   *
   * @param keyPoints selected key points on which the shape context
   * descriptor should be calculated. This matrix can be the same as
   * *boundaryPoints*.
   *
   * @param angles the number of quantization levels for angle in the
   * polar histogram. Typically, 12 quantization levels are used.
   *
   * @param distances quantization boundaries for distance in the
   * polar histogram. The values must be in ascending order.
   * Everything closer than the first distance limit will be put to
   * distance bin 0 and so on. For best performance, logarithmically
   * growing distances should be used. The last distance limit may be
   * infinity, in which case everything beyond the second-to-last
   * distance will be put into the same bin. Usually, five bins are
   * used. Note that the algorithm uses squared distances for speed.
   * Thus, the distance limits must also be given as squares.
   *
   * @param boundaryDirections boundary directions at key points. If
   * boundaryDirections is non-empty, its length must be
   * keyPoints.rows(). The information is used in normalizing the
   * descriptor against rotations.
   *
   * @param invariance either `NonInvariant` or `ScaleInvariant`. In
   * `ScaleInvariant` mode, all distances will be divided by the mean
   * (squared) distance between key points. Thus, *distances* must
   * not be absolute values but relative to the mean distance.
   */
  PII_MATCHING_EXPORT PiiMatrix<float> shapeContextDescriptor(const PiiMatrix<int>& boundaryPoints,
                                                              const PiiMatrix<int>& keyPoints,
                                                              int angles,
                                                              const QVector<double>& distances,
                                                              const QVector<double>& boundaryDirections = QVector<double>(),
                                                              InvarianceFlags invariance = NonInvariant);

  /**
   * Returns the direction of the boundary for each point in
   * *boundaryPoints*. Boundary direction at a point is the angle (in
   * radians) of the vector that connects the points before and after
   * it. Note that there must be at least three points in
   * *boundaryPoints*. Otherwise, all angles will be zero. If the first
   * and last boundary points are equal, the length of the returned
   * vector will be one less than the number of rows in
   * *boundaryPoints*.
   */
  PII_MATCHING_EXPORT QVector<double> boundaryDirections(const PiiMatrix<int>& boundaryPoints);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PiiMatching::InvarianceFlags);

#endif //_PIIMATCHING_H
