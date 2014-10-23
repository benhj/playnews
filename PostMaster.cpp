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

#include "PostMaster.h"
#include "PostWidget.h"
#include "NNTPConnector.h"
#include "ui_PostWidget.h"

PostMaster::PostMaster(ConnectionInfo const &connectionInfo, QString const &groupName, QObject *callbackObject) :
    m_connectionInfo(connectionInfo),
    m_groupName(groupName),
    m_callbackObject(callbackObject),
    pw(NULL)
{

}

void
PostMaster::process()
{
    this->moveToThread(&m_worker);
    QObject::connect(&m_worker, SIGNAL(started()), this, SLOT(doPost()));
    m_worker.start();
}

PostMaster::~PostMaster()
{
    if(pw) {
        delete pw;
    }
}

void
PostMaster::displayPostWidget(QString const &data)
{
    pw = new PostWidget;
    connect(pw->ui->postButton, SIGNAL(clicked()), this, SLOT(process()), Qt::QueuedConnection);
    pw->ui->groupEdit->setText(m_groupName);
    pw->setAttribute(Qt::WA_DeleteOnClose);
    if(!data.toStdString().empty()) {
        pw->setText(data);
    }
    pw->resize(1024,600);
    pw->show();
}


void
PostMaster::doPost()
{
    NNTPConnector connector;
    ConnectorBuilder::buildReferencedConnector(m_connectionInfo, connector, m_callbackObject);

    QString postData = pw->getText();
    pw->close();
    qDebug() << "Post data: " << postData;

    connector.doPost(postData, pw->getGroup(), pw->getFrom(), pw->getSubject());

    ConnectorBuilder::tearDownReferencedConnector(connector);
    QObject::disconnect(&m_worker, SIGNAL(started()), this, SLOT(doPost()));
    m_worker.quit();
}


