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

#include "PiiHistogramOperation.h"
#include "PiiRoi.h"
#include "PiiHistogram.h"
#include <PiiYdinTypes.h>
#include <PiiColor.h>

template <class T> struct PiiHistogramOperation::GrayHistogram :
  PiiGrayHistogramHandler<T>
{
  GrayHistogram(PiiHistogramOperation* parent) : pParent(parent) {}

  void send()
  {
    for (int i=0; i<3; ++i)
      pParent->emitObject(this->varHistogram, i);
  }

  PiiHistogramOperation* pParent;
};

template <class T> struct PiiHistogramOperation::ColorHistogram :
  PiiColorHistogramHandler<T>
{
  ColorHistogram(PiiHistogramOperation* parent) : pParent(parent)
  {
    for (int i=0; i<3; ++i)
      this->baCalculate[i] = pParent->outputAt(i)->isConnected();
  }

  void send()
  {
    for (int i=0; i<3; ++i)
      if (this->varHistograms[i].isValid())
        pParent->emitObject(this->varHistograms[i], i);
  }

  PiiHistogramOperation* pParent;
};

template <class T> struct PiiHistogramOperation::Histogram :
  Pii::IfClass<Pii::IsPrimitive<T>,
               PiiHistogramOperation::GrayHistogram<T>,
               PiiHistogramOperation::ColorHistogram<T> >::Type
{
  typedef typename Pii::IfClass<Pii::IsPrimitive<T>,
                                PiiHistogramOperation::GrayHistogram<T>,
                                PiiHistogramOperation::ColorHistogram<T> >::Type SuperType;
  Histogram(PiiHistogramOperation* parent) : SuperType(parent) {}
};

PiiHistogramOperation::Data::Data() :
  iLevels(256),
  bNormalized(false),
  roiType(PiiImage::AutoRoi),
  pHistogram(0),
  uiPreviousType(PiiVariant::InvalidType)
{
}

PiiHistogramOperation::Data::~Data()
{
  delete pHistogram;
}

PiiHistogramOperation::PiiHistogramOperation() :
  PiiDefaultOperation(new Data)
{
  setThreadCount(1);
  PII_D;
  addSocket(d->pImageInput = new PiiInputSocket("image"));
  addSocket(d->pRoiInput = new PiiInputSocket("roi"));
  d->pRoiInput->setOptional(true);

  addSocket(new PiiOutputSocket("red"));
  addSocket(new PiiOutputSocket("green"));
  addSocket(new PiiOutputSocket("blue"));
}

int PiiHistogramOperation::levels() const { return _d()->iLevels; }
void PiiHistogramOperation::setLevels(int levels) { _d()->iLevels = levels; }

void PiiHistogramOperation::setNormalized(bool normalize) { _d()->bNormalized = normalize; }
bool PiiHistogramOperation::normalized() const { return _d()->bNormalized; }

void PiiHistogramOperation::setRoiType(PiiImage::RoiType roiType) { _d()->roiType = roiType; }
PiiImage::RoiType PiiHistogramOperation::roiType() const { return _d()->roiType; }

void PiiHistogramOperation::aboutToChangeState(State state)
{
  PII_D;
  if (state == Stopped)
    {
      delete d->pHistogram;
      d->pHistogram = 0;
      d->uiPreviousType = PiiVariant::InvalidType;
    }
}

void PiiHistogramOperation::process()
{
  PII_D;
  PiiVariant obj = d->pImageInput->firstObject();

  switch (obj.type())
    {
      PII_INT_GRAY_IMAGE_CASES(histogram, obj);
      PII_INT_COLOR_IMAGE_CASES(histogram, obj);
    default:
      PII_THROW_UNKNOWN_TYPE(d->pImageInput);
    }
}

template <class T> void PiiHistogramOperation::histogram(const PiiVariant& obj)
{
  PII_D;
  const PiiMatrix<T> image = obj.valueAs<PiiMatrix<T> >();
  // If the histogram handler we already have is suitable for this
  // type of input, we won't bother recreating it.
  if (d->uiPreviousType != Pii::typeId<T>())
    {
      delete d->pHistogram;
      d->pHistogram = new Histogram<T>(this);
      d->uiPreviousType = Pii::typeId<T>();
    }

  Histogram<T>& hist = *static_cast<Histogram<T>*>(d->pHistogram);
  hist.initialize(d->iLevels, d->bNormalized);

  PiiImage::handleRoiInput(d->pRoiInput, d->roiType, image, hist);

  if (d->bNormalized)
    hist.normalize();

  hist.send();
}

