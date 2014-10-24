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

#include "ManagedNNTPConnection.h"
#include "ArticleSelectAndReadStructure.h"

#include <QDir>
#include <QApplication>
#include <sstream>
#include <fstream>

namespace core {

    ManagedNNTPConnection::ManagedNNTPConnection()
    {
    }


    void
    ManagedNNTPConnection::setUsernameAndPassword(QString const &username, QString const &password)
    {
        m_username = username;
        m_password = password;
    }

    void
    ManagedNNTPConnection::loadGroups()
    {

        qDebug() << m_server << "\t"
                 << m_port << "\t"
                 << m_ssl << "\t"
                 << m_username << "\t"
                 << m_password;

        emit resetBytesReadSignal();

        ConnectionInfo connectionInfo(m_server,
                                      m_port,
                                      m_ssl,
                                      m_username,
                                      m_password);

        m_groupLoaderPtr.reset(new GroupLoader(connectionInfo, this));
        QObject::connect(m_groupLoaderPtr.data(), SIGNAL(groupsLoadFinishedSignal()),
                         this, SLOT(groupsLoadFinishedSlot()));
        m_groupLoaderPtr->process();
    }

    Groups
    ManagedNNTPConnection::getLoadedGroups()
    {
        return m_groupLoaderPtr->getLoadedGroups();
    }

    void
    ManagedNNTPConnection::extractNHeadersUsingXOverCommand(QString const &groupName, int const headerCount)
    {
        emit resetBytesReadSignal();

        ConnectionInfo connectionInfo(m_server,
                                      m_port,
                                      m_ssl,
                                      m_username,
                                      m_password);

        m_headerExtractorPtr.reset(new HeaderExtractor(groupName, connectionInfo, headerCount, this));
        QObject::connect(m_headerExtractorPtr.data(),
                         SIGNAL(headersReadFinishedSignal(HeadersData)),
                         this,
                         SLOT(headersReadFinishedSlot(HeadersData)),
                         Qt::QueuedConnection);
        m_headerExtractorPtr->process();
    }

    void
    ManagedNNTPConnection::groupAddedSlot(QString str)
    {
        emit groupAddedSignal(str);
    }

    void
    ManagedNNTPConnection::groupsLoadFinishedSlot()
    {
        emit groupsLoadFinishedSignal();
        QObject::disconnect(m_groupLoaderPtr.data(), SIGNAL(groupsLoadFinishedSignal()),
                         this, SLOT(groupsLoadFinishedSlot()));
    }

    void
    ManagedNNTPConnection::headersReadFinishedSlot(HeadersData hd)
    {
        qDebug() << "finished reading headers";
        emit headersReadFinishedSignal(hd);
        QObject::disconnect(m_headerExtractorPtr.data(), SIGNAL(headersReadFinishedSignal(HeadersData)),
                         this, SLOT(headersReadFinishedSlot(HeadersData)));
    }

    void
    ManagedNNTPConnection::readArticle(QString const &group, int const articleId)
    {
        qDebug() << m_server << "\t" << m_port << "\t" << m_ssl;
        qDebug() << "Resetting article reader";

        emit resetBytesReadSignal();

        ConnectionInfo connectionInfo(m_server,
                                      m_port,
                                      m_ssl,
                                      m_username,
                                      m_password);
        int isBinary = 0;
        m_articleReaderPtr.reset(new ArticleReader(connectionInfo, group, articleId, isBinary, this));

        // return in a struct the article data together with the binaryData int
        qDebug() << "Reading...";
        QObject::connect(m_articleReaderPtr.data(), SIGNAL(articleDataReadSignal(ArticleData&)),
                         this, SLOT(finishedReadingArticle(ArticleData&)));
        m_articleReaderPtr->process();
    }

    void
    ManagedNNTPConnection::finishedReadingArticle(ArticleData &data)
    {

        emit articleDataReadSignal(data);
        qDebug() << "finished reading";
        QObject::disconnect(m_articleReaderPtr.data(), SIGNAL(articleDataReadSignal(ArticleData&)),
                            this, SLOT(finishedReadingArticle(ArticleData&)));
    }

    void
    ManagedNNTPConnection::singleArticleExtractedSlot()
    {
        emit singleArticleExtractedSignal();
    }

    void
    ManagedNNTPConnection::issuedLASTCommandSlot()
    {
        emit issuedLASTCommandSignal();
    }

    void
    ManagedNNTPConnection::finishedIssuingLASTCommandsSlot()
    {
        emit finishedIssuingLASTCommandsSignal();
    }


    void
    ManagedNNTPConnection::selectAndRead(QString const &groupName, int const articleId)
    {
        qDebug() << "Reading article...";
        readArticle(groupName, articleId);
    }

    void
    ManagedNNTPConnection::selectAndReadCollection(QString const &groupName, std::vector<int> codes)
    {

        emit resetBytesReadSignal();

        qDebug() << "yes collection";

        ConnectionInfo connectionInfo(m_server,
                                      m_port,
                                      m_ssl,
                                      m_username,
                                      m_password);

        m_compositeCheckerPtr.reset(new CompositeArticleLoaderChecker(codes, connectionInfo, groupName, this));
        QObject::connect(m_compositeCheckerPtr.data(), SIGNAL(compositeFinishedSignal()),
                         this, SLOT(compositeReadSlot()));

        m_compositeCheckerPtr->doLoad();
    }

    void
    ManagedNNTPConnection::compositeReadSlot()
    {
        qDebug() << "in compositeReadSlot";
        QObject::disconnect(m_compositeCheckerPtr.data(), SIGNAL(compositeFinishedSignal()),
                         this, SLOT(compositeReadSlot()));
        emit compositeDataReadSignal();
    }

    QString
    ManagedNNTPConnection::getCompositeData()
    {
        return m_compositeData;
    }

    void
    ManagedNNTPConnection::setServerAndPort(QString const &server, int const port, bool const ssl)
    {
        m_server = server;
        m_port = port;
        m_ssl = ssl;
    }

    void
    ManagedNNTPConnection::doPost()
    {
        //connection.doPost(m_postData, m_postGroup, m_postFrom, m_postSubject);
    }

    void
    ManagedNNTPConnection::finishedPostingSlot(int const status)
    {
        emit finishedPostingSignal(status);
    }

    void
    ManagedNNTPConnection::setPostStuff(QString const &postData,
                                        QString const &postGroup,
                                        QString const &postFrom,
                                        QString const &postSubject)
    {
        m_postData = postData;
        m_postGroup = postGroup;
        m_postFrom = postFrom;
        m_postSubject = postSubject;
    }

    void
    ManagedNNTPConnection::statusMessageSlot(QString message)
    {
        emit statusSignal(message);
    }

    void
    ManagedNNTPConnection::headCommandsIssuedSlot()
    {
        emit headCommandsIssuedSignal();
    }

    void
    ManagedNNTPConnection::bytesReadSlot(int const bytesRead)
    {
        emit bytesReadSignal(bytesRead);
    }

    void
    ManagedNNTPConnection::readBeginSlot(int const count)
    {
        emit readBeginSignal(count);
    }

    void
    ManagedNNTPConnection::readBitOfDataSlot()
    {
        emit readBitOfDataSignal();
    }
}
