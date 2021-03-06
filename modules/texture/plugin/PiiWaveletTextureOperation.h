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

#ifndef _PIIWAVELETTEXTUREOPERATION_H
#define _PIIWAVELETTEXTUREOPERATION_H

#include <PiiDefaultOperation.h>
#include <PiiWavelet.h>

/**
 * An operation that extracts texture features with discrete wavelet
 * decomposition. The feature vector contains the norm of some of the
 * decomposition results. The feature extraction method is a
 * generalization of that used by R. Porter and N. Canagarajah. For
 * details see Porter, R., Canagarajah, N., 1997. Robust
 * rotation-invariant texture classification: wavelet, gabor filter
 * and gmrf based schemes. IEE Proc. Vis. Image Signal Process. 144
 * (3), 180-188.
 *
 * Inputs
 * ------
 *
 * @in image - an image whose texture features are to be
 * calculated. (Any gray-scale image)
 *
 * Outputs
 * -------
 *
 * @out features - feature vector. For each image, four values are
 * calculated and output as as 1-by-4 matrix. (PiiMatrix<float>)
 *
 */
class PiiWaveletTextureOperation : public PiiDefaultOperation
{
  Q_OBJECT

  /**
   * The number of wavelet decomposition levels. Must be larger than
   * zero. The default is three. The number of features is equal to
   * the levels * featuresPerLevel + 1.
   */
  Q_PROPERTY(int levels READ levels WRITE setLevels);
  /**
   * The number of features to calculate for each decomposition
   * level. Feature counts have the following meanings:
   *
   * - 1 - rotation invariant texture descriptor as described in the
   * aforementioned paper.
   *
   * - 2 - consider only horizontal and vertical details
   *
   * - 3 - consider also diagonal details (default value)
   *
   * - 4 - use also the approximation coefficients on each
   * level. This information is redundant.
   */
  Q_PROPERTY(int featuresPerLevel READ featuresPerLevel WRITE setFeaturesPerLevel);
  /**
   * The norm used in extracting features from the decomposition
   * coefficients. The L1 norm is used by default, L2 is commonly
   * referred to as the "energy" of texture. Any norm larger than 0
   * will do.
   */
  Q_PROPERTY(int norm READ norm WRITE setNorm);
  /**
   * The name of the wavelet to use. Known values are "Haar" and
   * "Daubechies1" ... "Daubechies10". The default is "Daubechies2".
   */
  Q_PROPERTY(QString wavelet READ wavelet WRITE setWavelet);

  PII_OPERATION_SERIALIZATION_FUNCTION

public:
  PiiWaveletTextureOperation();

  int levels() const;
  void setLevels(int levels);

  int featuresPerLevel() const;
  void setFeaturesPerLevel(int features);

  QString wavelet() const;
  void setWavelet(const QString& name);

  int norm() const;
  void setNorm(int norm);

protected:
  void process();

private:
  template <class T> void waveletNormFloat(const PiiVariant& obj);
  template <class T> void waveletNormInt(const PiiVariant& obj);
  template <class T> void waveletNorm(const PiiMatrix<T>& mat);

  /// @internal
  class Data : public PiiDefaultOperation::Data
  {
  public:
    Data();
    int iLevels, iFeaturesPerLevel, iNorm;
    PiiDsp::WaveletFamily waveletFamily;
    int iWaveletIndex;

    PiiInputSocket *pImageInput;
    PiiOutputSocket *pFeatureOutput;
  };
  PII_D_FUNC;
};

#endif //_PIIWAVELETTEXTUREOPERATION_H
