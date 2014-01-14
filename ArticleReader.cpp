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

#include "ArticleReader.h"
#include "ConnectorBuilder.h"
#include <QDir>
#include <sstream>
#include <fstream>
#include <cassert>

ArticleReader::ArticleReader(ConnectionInfo const &connectionInfo,
                             QString const & group, int const articleId,
                             int isBinary, QObject *callbackObject) :
    m_connectionInfo(connectionInfo),
    m_group(group),
    m_articleId(articleId),
    m_isBinary(isBinary),
    m_callbackObject(callbackObject)
{

}

void
ArticleReader::process()
{
    this->moveToThread(&m_worker);
    QObject::connect(&m_worker, SIGNAL(started()), this, SLOT(readArticle()));
    m_worker.start();

}

void
ArticleReader::readArticle()
{

    NNTPConnector connector;
    ConnectorBuilder::buildReferencedConnector(m_connectionInfo, connector, m_callbackObject);

    QString articleData = "";
    int binaryData = 0;

    qDebug()<<"In readArticle";

    if(connector.setGroup(m_group) == 211) {
        std::stringstream ss;
        ss << "BODY " << m_articleId <<"\r\n";
        connector.pushCommand(ss.str());
        connector.popAndIssueCommand();
        std::vector<int> codeChecks;
        codeChecks.push_back(222); // article follows
        codeChecks.push_back(430); // no article with given id
        codeChecks.push_back(412); // no group selected
        codeChecks.push_back(423); // no article number
        codeChecks.push_back(420); // invalid article number
        std::ostringstream comp;
        std::string response = connector.getResponseString(1, "\n");
        int status = connector.getStatus(response);
        comp << response;
        qDebug() << "read article status: "<<status;
        if(status == 222) {

            //
            // Do check here to see if yencoded binary data; currently only
            // yenc encoded data supported; might also add support
            // for uuencoded/mime/base-64 data at a later time
            //
            // If binary data, save to disk
            //
            //std::string ybeginCheck = connector.getResponseString(1, "\r\n");
            //comp << ybeginCheck;
            //qDebug() << "response: "<<response.c_str();
            if(m_isBinary) {

                qDebug() << "saving binary part to disk..";

                std::ostringstream ss;
                ss << "/" << m_articleId;
                QString tempDir = QDir::tempPath() + ss.str().c_str();
                if(!QFile(tempDir).exists()) {
                    std::ofstream out(tempDir.toStdString().c_str(), std::ios_base::binary);
                    assert(out.is_open());
                    out << comp.str().c_str();
                    readBinaryArticle(out, connector);
                    out.close();
                } else {
                    //response = connector.getResponseString(1, "\n.\r\n", false);
                }
                binaryData = 1;
                //emit binaryPartDownloadedSignal();

            } else {
                response = connector.getResponseString(1, "\n.\r\n", false);
                comp << response;
                articleData = QString(comp.str().c_str());
            }
            articleData = articleData.toStdString().empty() ? "" : chopArticleData(articleData);
        } else {
            articleData = "ERROR; not 222";
        }
        ConnectorBuilder::tearDownReferencedConnector(connector);
        ArticleData data;
        data.binary = binaryData;
        data.data = articleData;
        qDebug() << "about to emit data";
        emit articleDataReadSignal(data);
        qDebug() << "data emitted";
        QObject::disconnect(&m_worker, SIGNAL(started()), this, SLOT(readArticle()));
        m_worker.quit();

    }

}

void
ArticleReader::readBinaryArticle(std::ostream &out, NNTPConnector &connector)
{
    connector.getResponseStream(out, 1, "\n.\r\n");
}

QString
ArticleReader::chopArticleData(QString const &original)
{
    std::string chopped(original.toStdString());
    std::string::iterator it = chopped.begin();
    while(*it != '\n') {
        ++it;
    }
    std::string without222(it, chopped.end());
    return QString(without222.c_str());
}
