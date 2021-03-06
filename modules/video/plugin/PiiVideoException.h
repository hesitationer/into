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

#ifndef _PIIVIDEOEXCEPTION_H
#define _PIIVIDEOEXCEPTION_H

#include <PiiException.h>
#include <PiiVideoGlobal.h>

/**
 * PiiVideoException is thrown when errors occur in video operations.
 */
class PII_VIDEO_EXPORT PiiVideoException : public PiiException
{
public:
  /**
   * Construct a new PiiVideoException.
   */
  PiiVideoException(const QString& message = "", const QString& location = "") : PiiException(message, location) {}
};


#endif //_PIIVIDEOEXCEPTION_H
