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

#ifndef _PIISIMPLEPROCESSOR_H
#define _PIISIMPLEPROCESSOR_H

#include "PiiOperationProcessor.h"

class PiiDefaultOperation;
class QMutex;

/**
 * A simple processor that calls the parent operation's process()
 * function immediately at the reception of a new input object. There
 * is no separate processing thread. It logically follows that this
 * processor cannot be used for operations that produce data with no
 * input from external sources. Such operations must be processed in
 * parallel. PiiSimpleProcessor is the right choice when the overhead
 * in parallel processing exceeds the complexity of the operation. 
 * Such operations are, for example, simple logical and arithmetic
 * calculations.
 *
 * @internal
 * @ingroup Ydin
 */
class PiiSimpleProcessor : public PiiOperationProcessor
{
public:
  /**
   * Create a new PiiSimpleProcessor.
   */
  PiiSimpleProcessor(PiiDefaultOperation* parent);
  
  /**
   * Returns true.
   */
  bool wait(unsigned long time = ULONG_MAX);

  /**
   * Invoked when a new object appears on any input socket. This
   * function first calls @ref PiiFlowController::prepareProcess() and
   * then the parent operation's @ref PiiDefaultOperation::process()
   * "process()" function, if necessary. Thus, the operation is
   * processed in the context of the thread that emitted the object to
   * @a sender.
   */
  bool tryToReceive(PiiAbstractInputSocket* sender, const PiiVariant& object) throw ();

  /**
   * Sets the state to @p Running.
   */
  void start();

  /**
   * Sets the state to @p Stopped.
   */
  void interrupt();

  /**
   * If the operation has connected inputs, sets the state to @p
   * Pausing. Otherwise sets the state to @p Paused and sends a pause
   * tag.
   */
  void pause();

  /**
   * If the operation has connected inputs, sets the state to @p
   * Stopping. Otherwise sets the state to @p Stopped and sends a stop
   * tag.
   */
  void stop();
  
  void reconfigure(const QString& propertySetName);

  void check(bool reset);

  void setProcessingPriority(QThread::Priority priority);
  QThread::Priority processingPriority() const;

  int activeInputGroup() const;

private:
  void stop(PiiOperation::State finalState);
  
  volatile bool _bReset;
  bool _bProcessing;
  QMutex* _pStateMutex;
};

#endif //_PIISIMPLEPROCESSOR_H