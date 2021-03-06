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

#ifndef _PIINETWORKOUTPUTOPERATION_H
#define _PIINETWORKOUTPUTOPERATION_H

#include "PiiNetworkOperation.h"

class PiiNetworkClient;
class PiiHttpDevice;

/**
 * PiiNetworkOutputOperation description
 *
 * Inputs
 * ------
 *
 * @in server uri - the URI to which data is to be sent (QString).
 * This input is optional. If it is not connected, the [serverUri]
 * property will be used.
 *
 * @in inputX - a configurable number of input sockets. X is a
 * zero-based index. The number of input sockets and their alias names
 * can be configured with the [inputNames] property.
 *
 * Outputs
 * -------
 *
 * @out outputX - a configurable number of input sockets. If the
 * server responds with properly encoded values, they will be sent to
 * the corresponding output sockets. The number of output sockets and
 * their alias names can be configured with the [outputNames] property.
 *
 */
class PiiNetworkOutputOperation : public PiiNetworkOperation
{
  Q_OBJECT

  /**
   * The URI of the server to send the data to, for example
   * "tcp://localhost:8080/sum". See PiiHttpServer for supported URI
   * schemes. There is no default value.
   */
  Q_PROPERTY(QString serverUri READ serverUri WRITE setServerUri);

  /**
   * HTTP request method. The default is `PostRequest`.
   */
  Q_PROPERTY(RequestMethod requestMethod READ requestMethod WRITE setRequestMethod);
  Q_ENUMS(RequestMethod);

  PII_OPERATION_SERIALIZATION_FUNCTION
public:
  /**
   * Supported HTTP request methods.
   *
   * - `PostRequest` - input objects are marshalled and sent as a
   * sequence of HTTP POST requests.
   *
   * - `GetRequest` - input objects are encoded into the request URI.
   * A HTTP GET request will be sent. This method works only if all
   * input objects are primitive types or strings. It cannot be used
   * with the `body` input connected.
   */
  enum RequestMethod { PostRequest, GetRequest };

  PiiNetworkOutputOperation();
  ~PiiNetworkOutputOperation();

  void check(bool reset);

  void setServerUri(const QString& serverUri);
  QString serverUri() const;
  void setRequestMethod(const RequestMethod& requestMethod);
  RequestMethod requestMethod() const;

protected:
  void process();

private:
  void sendPostRequest();
  void sendGetRequest();
  void readResponse(PiiHttpDevice& h);

  /// @internal
  class Data : public PiiNetworkOperation::Data
  {
  public:
    Data();

    PiiNetworkClient* pNetworkClient;
    QString strServerUri;
    QString strHost, strUri;
    RequestMethod requestMethod;
  };
  PII_D_FUNC;
};


#endif //_PIINETWORKOUTPUTOPERATION_H
