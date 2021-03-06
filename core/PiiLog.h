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

#ifndef _PIILOG_H
#define _PIILOG_H

/**
 * @group logging Application-wide logging facilities.
 *
 * The logging system in Into is built on Qt's debug message system
 * (qDebug(), qWarning() etc.) There are however a few notable
 * enhancements:
 *
 * - Log messages are categorized not only by their severity but
 * also by their source. It is possible to enable/disable messages
 * from a named module such as a plug-in at run time.
 *
 * - Message filters are separated from output handlers. This makes
 * it possible to filter log messages independent of the handler that
 * actually writes the messages.
 *
 * - Log messages can be forced to a uniform format by a global
 * message formatter.
 *
 * To start using Into logging one only needs to replace qDebug() with
 * piiDebug(), qWarning() with piiWarning() etc. The usage is
 * otherwise equivalent. If you want to use the module naming
 * capabilities, compile your module with PII_LOG_MODULE defined. The
 * easy way to do this is to put the following line into your qmake
 * project file:
 *
 * ~~~(c++)
 * DEFINES += PII_LOG_MODULE=MyModule
 * ~~~
 *
 * Another way is to define PII_LOG_MODULE before including any header
 * file from Into:
 *
 * ~~~(c++)
 * #define PII_LOG_MODULE MyModule
 * #include <PiiGlobal.h>
 * ~~~
 */

#include <QString>
#include <cstdarg>
#include "PiiGlobal.h"
#include "PiiPreprocessor.h"

#ifndef PII_NO_QT
#  include <QDebug>
#else
enum QtMsgType
{
  QtDebugMsg,
  QtWarningMsg,
  QtCriticalMsg,
  QtFatalMsg,
  QtSystemMsg = QtCriticalMsg
};
#endif

#ifndef PII_LOG_MODULE
#  define PII_LOG_MODULE Into
#endif

/**
 * Write a log message. This version of the function takes a
 * `va_list` argument instead of a variable number of arguments. This
 * function does not call `va_end` on `argp`; the caller is
 * responsible for it.
 *
 * @see piiLog()
 */
PII_CORE_EXPORT void piiLogv(const char* module, QtMsgType level, const char* msg, va_list argp);

/**
 * Write a log message. This function can be used directly with a
 * module name, but it is usually more convenient to call piiDebug(),
 * piiWarning() etc. directly. You must use this function if you need
 * to use different module names in a single translation unit.
 *
 * This function formats the log message based on the current log
 * format (see piiSetLogMessageFormat()) and uses Qt's debug message
 * system to write it out. Suppressing Qt's own debug and warning
 * outputs with QT_NO_DEBUG_OUTPUT and QT_NO_WARNING_OUTPUT has no
 * effect on Into logging.
 *
 * @param module the name of the module sending the message. Module
 * names are user-specifiable. Typically, plug-in names are used as
 * log module names.
 *
 * @param level the severity of the message.
 *
 * @param msg the log message in printf format.
 */
inline void PII_PRINTF_ATTR(3,4) piiLog(const char* module, QtMsgType level, const char* msg, ...)
{
  va_list argp;
  va_start(argp, msg);
  piiLogv(module, level, msg, argp);
  va_end(argp);
}

inline void piiLog(const char* module, QtMsgType level, const QString& msg)
{
  piiLog(module, level, "%s", piiPrintable(msg));
}

namespace PiiLog
{
  /**
   * A typedef for a pointer to a message filter function. See
   * setMessageFilter() for details.
   */
  typedef bool (*MessageFilter)(const char* module, QtMsgType level);

  /**
   * A function that writes messages to a log file. This function is
   * intended to be used with qInstallMsgHandler(). It writes messages
   * to the file set by setLogFile() and rotates log files when they
   * become too big. Once the [size limit](setMaxFileSize()) of a
   * log file is exceeded, log files are rotated so that file.log
   * becomes file.log.1, file.log.1 becomes file.log.2 and so on. If
   * the [maximum number of archived files](setMaxArchivedFiles())
   * has been reached, the oldest log file will be deleted.
   *
   * ~~~(c++)
   * PiiLog::setLogFile("/var/log/my.log");
   * qInstallMsgHandler(PiiLog::writeToFile);
   * ~~~
   */
  PII_CORE_EXPORT void writeToFile(QtMsgType type, const char* msg);

  /**
   * A message handler that drops all messages to a black hole.
   */
  PII_CORE_EXPORT void ignoreMessage(QtMsgType type, const char* msg);

  /**
   * Sets the global log message filter and return a pointer to the
   * previous filter (which may be 0). Set the filter to 0 to disable
   * filtering. By default, the message filter is set to
   * [defaultMessageFilter](defaultMessageFilter()).
   *
   * The message filter is used to select which messages are actually
   * logged. The filter will be called with the module name and
   * message type as parameters. If it returns `true`, the message
   * will be logged. Otherwise it will be discarded. Having no message
   * filter means everything will be logged.
   *
   * ~~~(c++)
   * bool myMessageFilter(const char* module, QtMsgType level)
   * {
   *   // Discard everything from DisabledModule
   *   if (QString("DisabledModule") == module)
   *     return false;
   *   // Discard debug messages from all modules
   *   else if (level < 1)
   *     return false;
   *   // Otherwise accept
   *   return true;
   * }
   *
   * PiiLog::setMessageFilter(myMessageFilter);
   * ~~~
   */
  PII_CORE_EXPORT MessageFilter setMessageFilter(MessageFilter filter);

  /**
   * The default message filter. This filter inspects the
   * [PII_LOG_LEVEL] environment variable. If the variable is not set or
   * the `level` is higher than or equal to `PII_LOG_LEVEL`, `true`
   * will be returned. Otherwise, `false` will be returned. The log
   * level should be set to a numeric value that corresponds to
   * QtMsgType values. That is, 0 means everything will be logged, 1
   * means that debug messages will not be logged etc.
   */
  PII_CORE_EXPORT bool defaultMessageFilter(const char* module, QtMsgType level);

  /**
   * Sets the log format. The default log format is an empty string,
   * which means that only the message itself will be logged. The
   * following variables are recognized in the format string:
   *
   * - `$time` - the current time. The default format is "yyyy-MM-dd
   * hh:mm" (see QDateTime). The format can be changed as in
   * `${time format}`, e.g. `${time dd.MM.yyyy}`.
   *
   * - `$type` - the type of the message (Debug, Warning, Critical,
   * or Fatal). If only a fixed-length part of the type is wanted, the
   * length can be given as in `${type 1}`, which takes the
   * first letter only.
   *
   * - `$module` - the name of the module that issued the log
   * message.
   *
   * - `$message` - the message string.
   */
  PII_CORE_EXPORT void setLogFormat(const QString& format);

  /**
   * Returns the log format string.
   */
  PII_CORE_EXPORT QString logFormat();

  /**
   * Sets the name of the log file. Use this function to set the file
   * to which writeToFile() logs messages.
   */
  PII_CORE_EXPORT void setLogFile(const QString& file);

  /**
   * Returns the name of the log file.
   */
  PII_CORE_EXPORT QString logFile();

  /**
   * Sets the maximum size of a log file in bytes.
   */
  PII_CORE_EXPORT void setMaxFileSize(qint64 maxSize);

  /**
   * Returns the maximum size of a log file in bytes.
   */
  PII_CORE_EXPORT qint64 maxFileSize();

  /**
   * Sets the maximum number of log files to be stored.
   */
  PII_CORE_EXPORT void setMaxArchivedFiles(int maxCount);

  /**
   * Returns the maximum number of log files to be stored.
   */
  PII_CORE_EXPORT int maxArchivedFiles();
}

/// @endgroup

/// @hide
static const char* piiLogModuleName = PII_STRINGIZE(PII_LOG_MODULE);

#define PII_LOG_FUNCTION(NAME) \
static inline void PII_PRINTF_ATTR(1,2) pii##NAME(const char* msg, ...) \
{ \
  va_list argp; \
  va_start(argp, msg); \
  piiLogv(piiLogModuleName, Qt##NAME##Msg, msg, argp); \
  va_end(argp); \
} \
static inline void pii##NAME(const QString& msg) { pii##NAME("%s", piiPrintable(msg)); }

#define PII_LOG_FUNCTION_NOOP(NAME) \
  static inline void PII_PRINTF_ATTR(1,2) pii##NAME(const char*, ...) {} \
  static inline void pii##NAME(const QString&) {}

#ifndef PII_LOG_LEVEL
# define PII_LOG_LEVEL 0
#endif

#if PII_LOG_LEVEL < 1
PII_LOG_FUNCTION(Debug)
#else
PII_LOG_FUNCTION_NOOP(Debug)
#endif

#if PII_LOG_LEVEL < 2
PII_LOG_FUNCTION(Warning)
#else
PII_LOG_FUNCTION_NOOP(Warning)
#endif

#if PII_LOG_LEVEL < 3
PII_LOG_FUNCTION(Critical)
#else
PII_LOG_FUNCTION_NOOP(Critical)
#endif

#if PII_LOG_LEVEL < 4
PII_LOG_FUNCTION(Fatal)
#else
PII_LOG_FUNCTION_NOOP(Fatal)
#endif

/// @endhide

#endif //_PIILOG_H
