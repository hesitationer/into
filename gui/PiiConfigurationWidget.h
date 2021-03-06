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

#ifndef _PIICONFIGURATIONWIDGET_H
#define _PIICONFIGURATIONWIDGET_H

#include "PiiGui.h"
#include <QWidget>

/**
 * An interface that provides different UI components a way of
 * communicating with the controlling program. The UI components
 * provided by operations (see PiiOperation::createUiComponent()) are
 * all derived from QWidget. Since the creator of a UI component
 * usually has no knowledge of the actual type of the component, an
 * abstract way of delivering certain information is provided by the
 * PiiConfigurationWidget interface. If the UI component implements this
 * interface, the creator can send and receive information that
 * controls the behavior of the UI. Neither this interface nor the
 * internal Listener interface use signals and slots, because Qt does
 * not support multiple inheritance.
 *
 * The interface has a default implementation for each method so that
 * only the necessary methods need to be overridden.
 *
 */
class PII_GUI_EXPORT PiiConfigurationWidget : public QWidget
{
  Q_OBJECT

public:
  /**
   * Create a new PiiConfigurationWidget with the given parent.
   */
  PiiConfigurationWidget(QWidget* parent = 0);

  ~PiiConfigurationWidget();

  /**
   * Check whether the UI component can be accepted. This is needed,
   * for example, with configuration dialogs that can be accepted only
   * if certain values have been correctly set. The default
   * implementation returns the value set by setAcceptable(). The
   * default value is `true`.
   */
  virtual bool canAccept() const;
  /**
   * See if the component can be reset to the state it was upon
   * creation or after the last acceptChanges() call. The default
   * implementation returns `false`.
   */
  virtual bool canReset() const;
  /**
   * See if the component has default values it can be initialized
   * with. The default implementation returns `false`.
   */
  virtual bool hasDefaults() const;
  /**
   * Check whether the component's state has changed since its
   * creation or the last acceptChanges() call. The default implementation
   * returns the value set by setChanged(). The default value is
   * `false`.
   */
  virtual bool hasChanged() const;

public slots:
  /**
   * Accept the UI component. The implementation of this method should
   * store the changes made so far.
   */
  virtual void acceptChanges();

  /**
   * Reset the component to the state it was upon creation or after
   * the last acceptChanges() call.
   */
  virtual void reset();

  /**
   * Set the default values.
   */
  virtual void setDefaults();

signals:
  /**
   * This signal indicates that the contents of the widget have been
   * altered from their initial values.
   */
  void contentsChanged(bool changed);

  /**
   * This signal indicates that the configurator is either acceptable
   * or unacceptable.
   */
  void acceptStateChanged(bool acceptable);

protected:
  /**
   * Set the changed state. This method also sends a contentsChanged()
   * signal.
   */
  void setChanged(bool changed);

  /**
   * Set the acceptable state. This method also sends a
   * acceptStateChanged() signal.
   */
  void setAcceptable(bool acceptable);

private:
  class Data
  {
  public:
    Data();
    bool bAcceptable, bChanged;
  } *d;
};

#endif //_PIIEXTENDEDWIDGET_H
