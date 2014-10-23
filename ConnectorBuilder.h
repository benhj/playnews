/*
Copyright (c) 2013-2014, BenHJ

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CONNECTORBUILDER_H
#define CONNECTORBUILDER_H

#include "NNTPConnector.h"
#include "ConnectionInfo.h"
#include <QString>
#include <QMutex>

namespace ConnectorBuilder
{

    static int connections = 0;
    static int allowedConnections = 10; // default -1: unlimited
    static QMutex connectionMutex;

    static void setAllowedConnections(int const con)
    {
        allowedConnections = con;
    }

    static bool socketsAvailable()
    {
        //connectionMutex.lock();
        int con = connections;

        if(allowedConnections > -1) {
            if(con < allowedConnections) {
                return true;
            } else {
                return false;
            }
        }
        //connectionMutex.unlock();
        return true;
    }


    static void tearDownReferencedConnector(NNTPConnector &connector)
    {
        connector.nntpClose();
        --connections;
    }

    static void buildReferencedConnector(ConnectionInfo const &connectionInfo, NNTPConnector &connector, QObject *callbackObject)
    {

        qDebug() << allowedConnections;
        connectionMutex.lock();
        while(!socketsAvailable()){}
        ++connections;

        QObject::connect(&connector, SIGNAL(bytesReadSignal(int)), callbackObject, SLOT(bytesReadSlot(int)),
                         Qt::QueuedConnection);
        QObject::connect(&connector, SIGNAL(statusSignal(QString)), callbackObject, SLOT(statusMessageSlot(QString)),
                         Qt::QueuedConnection);

        qDebug() << "in build connector!";
        qDebug() << connectionInfo.m_server << "\t" <<
                    connectionInfo.m_port << "\t" <<
                    connectionInfo.m_ssl << "\t" <<
                    connectionInfo.m_username << "\t" <<
                    connectionInfo.m_password;

        connector.nntpConnect(connectionInfo.m_server, connectionInfo.m_port, connectionInfo.m_ssl);
        if(!connectionInfo.m_username.toStdString().empty() && !connectionInfo.m_password.toStdString().empty()) {
            connector.supplyCredentials(connectionInfo.m_username.toStdString().c_str(),
                                        connectionInfo.m_password.toStdString().c_str());
        }
        connectionMutex.unlock();
    }
}

#endif // CONNECTORBUILDER_H
