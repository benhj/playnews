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

#ifndef MANAGEDNNTPCONNECTION_H
#define MANAGEDNNTPCONNECTION_H

#include "NNTPConnector.h"
#include "HeaderExtractor.h"
#include "GroupLoader.h"
#include "ArticleReader.h"
#include "HeadersExtractorStructure.h"
#include "CompositeArticleLoaderChecker.h"
#include <QObject>
#include <QSharedPointer>

class ManagedNNTPConnection;

typedef QSharedPointer<ManagedNNTPConnection> ConnectionPtr;

class ManagedNNTPConnection : public QObject
{
    Q_OBJECT
public:
    explicit ManagedNNTPConnection();


    void setServerAndPort(QString const &server, int const port, bool const ssl);


    void setUsernameAndPassword(QString const &username, QString const &password);


    /**
     * @brief getLoadedGroups
     * @return a vector of loaded groups
     */
    Groups getLoadedGroups();

    //QString getArticleData();
    QString getCompositeData();

    void setPostStuff(QString const &postData,
                      QString const &postGroup,
                      QString const &postFrom,
                      QString const &postSubject);

signals:
    // for resending added group back up to main application
    void groupAddedSignal(QString);
    // for indicating when all groups have been loaded
    void groupsLoadFinishedSignal();
    // for indicating when group headers have been read
    void headersReadFinishedSignal(HeadersData);
    void singleArticleExtractedSignal();
    void loginFinishedSignal(int);
    void authorizedSignal(bool);
    void articleDataReadSignal(ArticleData&);
    void issuedLASTCommandSignal();
    void finishedIssuingLASTCommandsSignal();
    void finishedPostingSignal(int status);
    void statusSignal(QString);
    void headCommandsIssuedSignal();
    void compositeDataReadSignal();
    void bytesReadSignal(int const bytesRead);
    void resetBytesReadSignal();
    void readBeginSignal(int const count);
    void readBitOfDataSignal();

public slots:
    // catches from nntpconnector
    void groupAddedSlot(QString);
    void groupsLoadFinishedSlot();
    void headersReadFinishedSlot(HeadersData);
    void singleArticleExtractedSlot();
    void moveBackToMainThread();

    /**
     * @brief loadGroups does what it says
     */
    void loadGroups();

    /**
     * @brief extractNHeadersUsingXOverCommand more efficient way of getting header data
     */
    void extractNHeadersUsingXOverCommand(QString const &groupName, int const headerCount);

    /**
     * @brief readArticle
     */
    void readArticle(QString const &group, int const articleId);
    void finishedReadingArticle(ArticleData &data);

    void selectAndRead(QString const &groupName, int const articleId);
    void selectAndReadCollection(QString const &groupName, std::vector<int> codes);

    /**
     * @brief doPost posts article data
     */
    void doPost();

    void issuedLASTCommandSlot();

    void finishedIssuingLASTCommandsSlot();

    void finishedPostingSlot(int const status);

    void statusMessageSlot(QString);

    void headCommandsIssuedSlot();

    void bytesReadSlot(int const bytesRead);

    void readBeginSlot(int const count);
    void readBitOfDataSlot();

    void compositeReadSlot();

private:
    Groups groups;
    QString m_server;
    int m_port;
    bool m_ssl;
    QString m_username;
    QString m_password;
    QString m_postData;
    QString m_postGroup;
    QString m_postFrom;
    QString m_postSubject;
    QString m_compositeData;
    HeaderExtractorPtr m_headerExtractorPtr;
    GroupLoaderPtr m_groupLoaderPtr;
    ArticleReaderPtr m_articleReaderPtr;
    CompositeArticleLoaderChecker* m_compositeCheckerPtr;
};

#endif // MANAGEDNNTPCONNECTION_H
