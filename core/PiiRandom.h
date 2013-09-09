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

#ifndef _PIIRANDOM_H
#define _PIIRANDOM_H

#include "PiiGlobal.h"
#include <cstdlib>
#include <PiiMatrix.h>

/**
 * @file
 *
 * Functions for generating different types of random numbers. It works
 * around compiler differences in generating uniformly distributed
 * random numbers.
 *
 * @ingroup Core
 */
namespace Pii
{
  /**
   * Returns a uniformly distributed random number in [0,1].
   */
  inline double uniformRandom()
  {
#ifdef Q_OS_WIN
    return double(rand()) * 1.0/RAND_MAX;
#else
    return drand48();
#endif
  }

  /**
   * Returns a @a rows x @a columns matrix filled with uniformly
   * distributed random numbers in [0,1].
   */
  PII_CORE_EXPORT PiiMatrix<double> uniformRandomMatrix(int rows, int columns);

  /**
   * Returns a uniformly distributed random number in [@a min, @a
   * max].
   */
  inline double uniformRandom(double min, double max)
  {
    return uniformRandom() * (max-min) + min;
  }

  /**
   * Returns a @a rows x @a columns matrix filled with uniformly
   * distributed random numbers in [@a min, @a max].
   */
  PII_CORE_EXPORT PiiMatrix<double> uniformRandomMatrix(int rows, int columns,
                                                        double min, double max);

  /**
   * Returns a random number from a distribution that follows N(0,1)
   * (zero mean, unit variance Gaussian distribution). To convert x in
   * N(0,1) to N(m,v), where m is a non-zero mean and v a non-unit
   * variance, calculate x*v+m.
   */
  PII_CORE_EXPORT double normalRandom();

  /**
   * Returns a @a rows x @a columns matrix filled with normally
   * distributed random numbers.
   */
  PII_CORE_EXPORT PiiMatrix<double> normalRandomMatrix(int rows, int columns);
  
  /**
   * Initializes the random number generator from system clock. Note
   * that successive inits within the same millisecond have no effect. 
   * Note also that this function must be called before any random
   * numbers are generated if you don't want a similar sequence each
   * time the program is run.
   */
  PII_CORE_EXPORT void seedRandom();
  
  /**
   * Seeds the random number generator with your favourite value.
   */
  inline void seedRandom(long value)
  {
#ifdef Q_OS_WIN
    srand(static_cast<unsigned int>(value));
#else
    srand48(value);
#endif
  }
}

#endif //_PIIRANDOM_H