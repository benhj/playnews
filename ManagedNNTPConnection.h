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

#pragma once

#include "NNTPConnector.h"
#include "HeaderExtractor.h"
#include "GroupLoader.h"
#include "ArticleReader.h"
#include "CompositeArticleLoaderChecker.h"
#include <QObject>
#include <QSharedPointer>

namespace core {

    class ManagedNNTPConnection;

    typedef QSharedPointer<ManagedNNTPConnection> ManagedConnectionPtr;

    class ManagedNNTPConnection : public QObject
    {
        Q_OBJECT
    public:
        ManagedNNTPConnection(QString const &server,
                              int const port,
                              bool const ssl,
                              QString const &username,
                              QString const &password);

        /// retrieves a list of groups from the server
        void loadGroups();

        /// retrieves list of loaded groups
        Groups getLoadedGroups();

        /// loads headers for given group
        void extractNHeadersUsingXOverCommand(QString const &groupName, int const headerCount);

        /// reads a single-part article
        void readArticle(QString const &groupName, int const articleId);

        /// reads a multi-part article
        void readCompositeArticle(QString const &groupName,
                                  std::vector<int> const &articleIDs);

    signals:
        // for indicating when all groups have been loaded
        void groupsLoadFinishedSignal();

        // for indicating when group headers have been read
        void headersReadFinishedSignal(core::HeadersData);

        // indicate that article data has been read and is up for grabs
        void articleDataReadSignal(core::ArticleData&);

        // for sending messages back to process
        void statusSignal(QString);

        // used by BinaryGrabber to indicate that binary data
        // has been read
        void compositeDataReadSignal();

        // to signify that data has been read from the server
        void bytesReadSignal(int const bytesRead);

        // to signify that we are at the end of bytes having been read
        void resetBytesReadSignal();

    public slots:
        /// all groups from server have been loaded
        void groupsLoadFinishedSlot();

        /// all headers for a specific group have been read
        /// HeadersData containers the header data for the group
        void headersReadFinishedSlot(HeadersData);

        /// signifies that article has been read
        void finishedReadingArticle(ArticleData &data);

        /// passing back status messages
        void statusMessageSlot(QString);

        /// signifies that bytes have been read by the connector
        void bytesReadSlot(int const bytesRead);

        /// signified that reading of composite data has finished
        void compositeReadFinishedSlot();

    private:

        QString m_server;
        int m_port;
        bool m_ssl;
        QString m_username;
        QString m_password;

        /// for grabbing headers
        HeaderExtractorPtr m_headerExtractorPtr;

        /// for building group list
        GroupLoaderPtr m_groupLoaderPtr;

        /// for reading a single, 'simple' article
        ArticleReaderPtr m_articleReaderPtr;

        /// for reading a collection of articles in the form
        /// of a composite multi-part message
        CompositeCheckerPtr m_compositeCheckerPtr;

        /// called from selectAndRead, for reading a single, 'simple' article
        void doReadArticle(QString const &group, int const articleId);

        /// for reading a multi-part article
        void doReadCompositeArticle(QString const &groupName,
                                    std::vector<int> const &articleIDs);
    };
}
