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

#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QSslSocket>
#include <String>
#include <vector>
#include <deque>
#include <queue>
#include <QSharedPointer>
#include <QScopedPointer>

namespace core {

    /**
     * Convenience vector typedefs
     */
    typedef std::vector<int> ArticleCodes;

    typedef std::queue<std::string> CommandQueue;

    class NNTPConnector;

    typedef QSharedPointer<NNTPConnector> NNTPConnectorPtr;

    typedef std::deque<NNTPConnectorPtr> NNTPConnectorPtrs;

    class NNTPConnector : public QObject
    {
        Q_OBJECT
    public:
        explicit NNTPConnector(QObject *parent = 0);
        ~NNTPConnector()
        {
            nntpClose();
        }

        void setConnectionData(QString const &str,
                               int const port,
                               bool const ssl,
                               QString const &username,
                               QString const &password);

        /**
         * @brief nntpConnect
         * @param str server on which to connect
         * @param port port on which to connect
         * @return status of connection attempt (e.g. 200 or 201)
         */
        int nntpConnect(QString const &str,
                        int const port,
                        bool const ssl);

        /**
         * @brief supplyCredentials
         * @param username
         * @param password
         * @return status code of authorization attempt
         */
        int supplyCredentials(char const *username,
                              char const *password);

        /**
         * @brief nntpClose closes the connection to the nntp server
         */
        void nntpClose();

        /**
         * @brief setGroup sets group using GROUP command
         * @param group the name of the group
         * @return status of trying to set group
         */
        int setGroup(QString const &group);

        /**
         * @brief doPost allows posting of text article
         * @param postData article body
         * @param postGroup group to post to
         * @param postFrom author
         * @param postSubject subject
         */
        void doPost(QString const &postData,
                    QString const &postGroup,
                    QString const &postFrom,
                    QString const &postSubject);

        /**
         * @brief resetBytesRecorder reset m_bytesAll which records the
         * number of bytes read from the server since last reset
         */
        void resetBytesRecorder();

        void getResponseStream(std::ostream &out, int const count, std::string const &escapeSequence);

    signals:
        /**
         * @brief singleArticleExtractedSignal signals when one header has been read
         */
        void singleArticleExtractedSignal();

        /**
         * @brief statusSignal a signal reporting the status string returned by the server
         * @param statusString the status string
         */
        void statusSignal(QString statusString);

        /**
         * @brief bytesReadSignal indicates bytes read
         * @param bytesRead
         */
        void bytesReadSignal(int bytesRead);

        /**
         * @brief readOneBitOfDataSignal when a single item of data has been read
         * determined to be the presence of one escape sequence
         */
        void readOneBitOfDataSignal();

    private:

        friend class HeaderExtractor;
        friend class GroupLoader;
        friend class ArticleReader;

        QSslSocket nntp;
        bool connected;
        bool authorized;
        std::string m_lastArticleId;
        long m_bytesAll;

        QString m_server;
        int m_port;
        bool m_ssl;
        std::string m_username;
        std::string m_password;

        // For storing commands to be issued sequentially to the server
        CommandQueue m_commandQueue;

        QMutex m_sockMutex;

        /**
         * @brief issueNextCommand issues the NEXT command to get article id of
         * next message in group
         */
        void issueLASTCommand();

        /**
         * @brief issueStatCommand issues STAT command to get article id of current message
         */
        void issueStatCommand();

        /**
         * @brief getStatReturn get the article id returned by server after stat
         * @return stat article code
         */
        int getStatReturn();

        /**
         * @brief issueXOverCommand A more effective way of grabbing the header data is to issue
         * an xover command which allows one to specify a range of headers rather than having
         * to iteratively issue N head commands sequentially
         * @param lower article id lower
         * @param upper article id upper
         */
        void issueXOverCommand(int const lower, int const upper);

        /**
         * @brief issueHeadCommand
         * @param aritlceId
         */
        void issueHeadCommand(int const aritlceId);

         /**
          * @brief issueCommand send a string of command data to the server
          * @param data the data to send
          */
         void popAndIssueCommand();

         void pushCommand(std::string const& command);

        /**
         * @brief readFromSocket reads data from the socket while it is available
         * @return array of data read from socket
         */
        void readFromSocket(std::ostream &writeToMe,
                            int const count = -1, std::string const &escapeSequence = "\r\n.\r\n");

        /**
         * @brief getResponseString get response from server
         * @param count number of escape sequences to search for before breaking
         * @param escapeSequence what to escape on
         * @param doEmit whether to emit statusSignal
         * @return
         */
        std::string getResponseString(int const count, std::string const &escapeSequence,
                                      bool const doEmit = true);

        /**
         * @brief getStatus parses status integer from status response string
         * @param response string containing status integer at its beginning
         * @return status number
         */
        int getStatus(std::string const &response);

    };

}
