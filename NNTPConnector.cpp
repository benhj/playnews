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

#include "NNTPConnector.h"
#include <QTcpSocket>
#include <QSslSocket>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>

namespace core {

    NNTPConnector::NNTPConnector(QObject *parent)
      : QObject(parent)
      , connected(false)
      , authorized(false)
      , m_bytesAll(0)
    {
    }

    void
    NNTPConnector::setConnectionData(QString const &str,
                                   int const port,
                                   bool const ssl,
                                   QString const &username,
                                   QString const &password)
    {
        m_server = str;
        m_port = port;
        m_ssl = ssl;
        m_username = username.toStdString();
        m_password = password.toStdString();
    }

    int
    NNTPConnector::nntpConnect(QString const &str,
                               int const port,
                               bool const ssl)
    {
        m_server = str;
        m_port = port;
        m_ssl = ssl;

        if(nntp.state() == QAbstractSocket::ConnectedState){
            nntpClose();
        }

        if(ssl) { // encrypted
            qDebug() << "encrypted";
            nntp.connectToHostEncrypted(str, port);
        } else { // standard unencrypted
            qDebug() << "standard";
            nntp.connectToHost(str, port);
        }

        if((!ssl) ? (nntp.waitForConnected()) :
        (nntp.waitForEncrypted())) {
            qDebug() << "Connected!";
            connected = true;

            //
            // Return status: note, should be 200 on success for
            // a server with posting capability or 201 for a server
            // without a posting capability
            //
            std::vector<int> codeChecks;
            codeChecks.push_back(200); // service available, posting allowed
            codeChecks.push_back(201); // posting not allowed
            codeChecks.push_back(400); // temp unavailable
            codeChecks.push_back(502); // permanently unavilable
            m_bytesAll = 0;
            std::string response = getResponseString(1, "\n");
            int status = getStatus(response);
            return status;

        } else {
            qDebug() << "Not connected!";
            return -1;
        }
    }

    int
    NNTPConnector::supplyCredentials(char const *username,
                                     char const *password)
    {
        m_username = std::string(username);
        m_password = std::string(password);

        //
        // Supply username and password
        //
        std::stringstream ss;
        ss << "AUTHINFO USER " << username << "\r\n";
        m_commandQueue.push(ss.str());
        popAndIssueCommand();
        std::vector<int> codeChecks;
        codeChecks.push_back(281);
        codeChecks.push_back(381);
        codeChecks.push_back(502);
        codeChecks.push_back(501);
        std::string response = getResponseString(1, "\n");
        int status = getStatus(response);

        //qDebug()<<"supply credentials status: "<<status;

        //
        // If returned code is 381 then we also need to supply password
        // (note should return 281 on proper credentials)
        //
        if(status == 381) {
            qDebug()<<"Yes 381";
            ss.str("");
            ss << "AUTHINFO PASS " << password << "\r\n";
            m_commandQueue.push(ss.str());
            popAndIssueCommand();
            response = getResponseString(1, "\n");
            status = getStatus(response);
        }
        if(status == 501) { // syntax error (not recognized)
            //
            // AUTHINFO command not recognized so we just assume
            // username and pass not required so force the authority
            //
            status = 281;
        }
        // status 0 is a hack for passwordless logins sometimes
        // not working correctly, at least when tested with virginmedia
        if(status == 281 || status == 0) {
            authorized = true;
        }
        qDebug()<<"supply credentials status: "<<status;
        return status;
    }

    void
    NNTPConnector::nntpClose()
    {
        m_commandQueue.push("QUIT\r\n");
        popAndIssueCommand();
        (void)this->getResponseString(1, "\n", false);
        //nntp.close();
    }


    void
    NNTPConnector::issueStatCommand()
    {
        std::stringstream ss;
        ss << "STAT " << m_lastArticleId.c_str() << "\r\n";
        m_commandQueue.push(ss.str());
        popAndIssueCommand();
    }

    void
    NNTPConnector::issueLASTCommand()
    {
        m_commandQueue.push("LAST\r\n");
        popAndIssueCommand();
    }

    int
    NNTPConnector::getStatReturn()
    {
        std::string response = getResponseString(1, "\n", false);
        std::stringstream ss;
        int status;
        ss << response.c_str();
        ss >> status;
        if(status != 223) {
            return -1;
        }
        int id;
        ss >> id;
        return id;
    }

    void
    NNTPConnector::issueHeadCommand(int const articleId)
    {
        std::stringstream ss;
        ss << "HEAD " << articleId << "\r\n";
        m_commandQueue.push(ss.str());
        popAndIssueCommand();
    }

    void
    NNTPConnector::issueXOverCommand(int const lower, int const upper)
    {
        std::stringstream ss;
        ss << "XOVER " << lower <<"-"<<upper<<"\r\n";
        m_commandQueue.push(ss.str());
        popAndIssueCommand();
    }

    bool containsCode(std::string const &line, std::vector<int> const &codes)
    {
        std::vector<int>::const_iterator it;
        for(it = codes.begin(); it != codes.end(); ++it) {
            std::stringstream ss;
            ss << *it;
            if(line.find(ss.str()) != std::string::npos) {
                break;
            }
        }
    }


    int NNTPConnector::setGroup(QString const &group)
    {

        std::stringstream ss;
        ss << "GROUP " << group.toStdString() <<"\r\n";
        qDebug() << "Pushing command";
        m_commandQueue.push(ss.str());
        popAndIssueCommand();
        qDebug() << "popped and issued";
        std::vector<int> codeChecks;
        codeChecks.push_back(211); // success
        codeChecks.push_back(411); // no such group
        m_bytesAll = 0;
        qDebug() << "getting response";
        std::string response = getResponseString(1, "\r\n");
        qDebug() << "got response "<<response.c_str();
        int status = getStatus(response);
        //
        // Parse out very latest article id of group
        //
        std::stringstream groupParse(response);
        std::string scrap;
        std::getline(groupParse, scrap, ' ');
        std::getline(groupParse, scrap, ' ');
        std::getline(groupParse, scrap, ' ');
        std::getline(groupParse, scrap, ' ');
        m_lastArticleId = scrap;

        return status;
    }

    void
    NNTPConnector::pushCommand(std::string const& command)
    {
        m_commandQueue.push(command);
    }

    void
    NNTPConnector::popAndIssueCommand()
    {

        if(nntp.state() != QAbstractSocket::ConnectedState) {
            this->nntpConnect(m_server, m_port, m_ssl);
            if(!m_username.empty() && !m_password.empty()) {
                this->supplyCredentials(m_username.c_str(), m_password.c_str());
            }
        }
        std::string command = m_commandQueue.front();
        //m_sockMutex.lock();
        m_commandQueue.pop();
        qDebug() << "command: "<<command.c_str();
        nntp.write(command.c_str());
        nntp.flush();
        nntp.waitForBytesWritten();
        //m_sockMutex.unlock();
    }

    /**
     * @brief The ReadFunctor struct reads data from the nntp socket and stores
     * in an array of chars
     */
    struct ReadFunctor
    {
        ReadFunctor(QSslSocket &sock, std::vector<char> &buffer) : m_sock(sock), m_buffer(buffer){}

        qint64 operator()()
        {
            if(m_sock.waitForReadyRead(2000)) {
                try {
                qint64 bytesAvailable = m_sock.bytesAvailable();
                m_buffer.resize(bytesAvailable);
                return m_sock.read(&m_buffer[0], bytesAvailable);
                } catch(...) {
                    return 0;
                }
            }
            return 0;
        }
        QSslSocket &m_sock;
        std::vector<char> &m_buffer;
    };

    /**
     * @brief The StreamWriter struct is a functor that writes to an output stream
     * and checks if a given escape sequence appears in which case an escape
     * sequence counter is updated
     */
    struct StreamWriter
    {
        StreamWriter(std::ostream &out, int const count, std::string const &escapeSequence) :
            m_out(out),
            m_count(count),
            m_escapeSequence(escapeSequence),
            m_breakCount(0),
            m_escapeIterator(0)
        {

        }

        int operator()(std::vector<char> &buffer)
        {
            std::vector<char>::iterator it;
            for(auto const & it : buffer) {
                m_out << it;
                if(it == m_escapeSequence.at(m_escapeIterator)) {
                    assert(m_escapeIterator < m_escapeSequence.size());
                    if(m_escapeIterator == m_escapeSequence.size() - 1) {
                        ++m_breakCount;
                        m_escapeIterator = 0;
                    } else {
                        ++m_escapeIterator;
                    }
                } else {
                    m_escapeIterator = 0;
                    if(it == m_escapeSequence.at(m_escapeIterator)) {
                        ++m_escapeIterator;
                    }
                }
            }
            return m_breakCount;
        }

        std::ostream &m_out;
        int const m_count;
        std::string const& m_escapeSequence;
        int m_breakCount;
        int m_escapeIterator;
    };


    void
    NNTPConnector::readFromSocket(std::ostream &writeToMe,
                                  int const count,
                                  std::string const &escapeSequence)
    {
        //m_sockMutex.lock();
        qint64 bytesRead = 0;
        int breakCount = 0;
        StreamWriter streamWriter(writeToMe, count, escapeSequence);
        do {
            std::vector<char> buffer;
            ReadFunctor readFunctor(nntp, buffer);
            bytesRead = readFunctor();
            m_bytesAll += bytesRead;
            emit bytesReadSignal(bytesRead);
            breakCount = streamWriter(buffer);
            if(breakCount == count) {
                break;
            }

            //
            // Note, when breakCount >= count, there will be no more
            // incoming data and so we can safely break out and not bother
            // issuing a waitForReadyRead
            //
        } while (bytesRead > 0);
        //m_sockMutex.unlock();
    }

    void
    NNTPConnector::doPost(QString const &postData,
                          QString const &postGroup,
                          QString const &postFrom,
                          QString const &postSubject)
    {
        m_commandQueue.push("POST\r\n");
        popAndIssueCommand();
        m_bytesAll = 0;
        std::string response = getResponseString(1, "\n");
        int status = getStatus(response);
        qDebug() << "first post status: "<<status;
        qDebug() << "postData: "<<postData;
        if(status == 340) {
            qDebug() << "Good to post now!";
            std::stringstream composer;
            qDebug() << "postFrom: "<<postFrom;
            composer << "From: "<<postFrom.toStdString()<<"\r\n";
            m_commandQueue.push(composer.str());
            popAndIssueCommand();
            composer.str("");
            composer << "Newsgroups: "<<postGroup.toStdString()<<"\r\n";
            m_commandQueue.push(composer.str());
            popAndIssueCommand();
            composer.str("");
            composer << "Subject: "<<postSubject.toStdString()<<"\r\n";
            m_commandQueue.push(composer.str());
            popAndIssueCommand();
            composer.str("");
            composer << "Organization: none\r\n";
            m_commandQueue.push(composer.str());
            popAndIssueCommand();
            composer.str("");
            composer << "\r\n"<<postData.toStdString() <<"\r\n";
            m_commandQueue.push(composer.str());
            popAndIssueCommand();
            composer.str("");
            composer<<".\r\n";
            m_commandQueue.push(composer.str());
            popAndIssueCommand();
            composer.str("");
            m_commandQueue.push(composer.str());
            popAndIssueCommand();
            std::string response = getResponseString(1, "\n");
            status = getStatus(response);
        }

        emit finishedPostingSignal(status);
    }

    std::string
    NNTPConnector::getResponseString(int const count,
                                     std::string const &escapeSequence,
                                     bool const doEmit)
    {
        std::ostringstream ss;
        readFromSocket(ss, count, escapeSequence);
        std::string response = ss.str();
        if(doEmit) {
            std::stringstream ss(response);
            std::string statusLine;
            std::getline(ss,statusLine);
            emit statusSignal(QString(statusLine.c_str()));
        }
        return response;
    }

    void
    NNTPConnector::getResponseStream(std::ostream &out,
                                     int const count,
                                     std::string const &escapeSequence)
    {
        readFromSocket(out, count, escapeSequence);
    }

    int
    NNTPConnector::getStatus(std::string const &response)
    {
        return atoi(std::string(response.begin(), response.begin() + 3).c_str());
    }

    void
    NNTPConnector::resetBytesRecorder()
    {
        m_bytesAll = 0;
    }

}
