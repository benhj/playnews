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

#include "HeaderExtractor.h"
#include "ConnectorBuilder.h"
#include <sstream>

namespace core {

    HeaderExtractor::HeaderExtractor(QString const& groupName, ConnectionInfo const &connectionInfo,
                                    int const count, QObject *callbackObject)
      : m_groupName(groupName)
      , m_connectionInfo(connectionInfo)
      , m_count(count)
      , m_callbackObject(callbackObject)
    {

    }

    HeaderExtractor::~HeaderExtractor()
    {
        if(m_worker.isRunning()) {
            m_worker.quit();
        }
    }

    void
    HeaderExtractor::process()
    {
        this->moveToThread(&m_worker);
        QObject::connect(&m_worker, SIGNAL(started()), this, SLOT(extractNHeadersUsingXOverCommand()),
                         Qt::QueuedConnection);
        m_worker.start();
    }

    // more efficient way of extracting header subject lines
    void
    HeaderExtractor::extractNHeadersUsingXOverCommand()
    {

        NNTPConnector connector;
        ConnectorBuilder::buildReferencedConnector(m_connectionInfo, connector, m_callbackObject);

        Headers().swap(m_headers);

        if(!(m_groupName.toStdString()).empty()) {
            int status = connector.setGroup(m_groupName);
            if(status == 211) {
                connector.issueStatCommand();
                int statId = connector.getStatReturn();

                qDebug() << statId;

                if(statId > -1) {
                    int lower = statId - m_count;
                    if(lower < 1) {
                        lower = 1;
                    }
                    int upper = statId;
                    qDebug() << lower << "\t" <<upper;
                    qDebug() << "upper - lower: "<< (upper-lower);
                    connector.issueXOverCommand(lower, upper);
                    processXOverData(connector);
                }
            }

        }

        QObject::disconnect(&m_worker, SIGNAL(started()), this,
                            SLOT(extractNHeadersUsingXOverCommand()));
        ConnectorBuilder::tearDownReferencedConnector(connector);
        if(m_worker.isRunning()) {
            m_worker.quit();
        }

    }

    struct PartSorter
    {
        bool operator()(CompositeMessagePart const &left, CompositeMessagePart const &right)
        {
            return left.thisPart < right.thisPart;
        }
    };

    void
    HeaderExtractor::processXOverData(NNTPConnector &connector)
    {
        auto response = connector.getResponseString(1, "\r\n.\r\n", false);
        std::stringstream ss;
        ss << response;
        int status;
        ss >> status;
        qDebug() << "status: " << status;
        if(status == 224) {
            std::string responseLine;
            int c = 0;
            int index = 0;
            CompositeMessageStore compositeMessageStore;
            while(std::getline(ss, responseLine)) {
                //qDebug() << "c: "<<c;
                //qDebug()<<"responseLine: "<<responseLine.c_str();
                if(c > 0) {

                    std::string token;
                    std::stringstream tokenStream;
                    tokenStream << responseLine;
                    std::getline(tokenStream, token, '\t');
                    int articleId = atoi(token.c_str());
                    std::getline(tokenStream, token, '\t');
                    std::stringstream withSubject;
                    withSubject << "Subject: " << token;
                    std::string subject(withSubject.str());
                    std::getline(tokenStream, token, '\t');
                    std::stringstream authorLine;
                    authorLine << "Author: " << token;
                    std::string author = authorLine.str();
                    std::getline(tokenStream, token, '\t');
                    std::stringstream dateLine;
                    dateLine << "Date: "<<token;
                    std::string date = dateLine.str();
                    Header header;
                    header.messageId = articleId;
                    header.author = author;
                    header.subject = subject;
                    header.date = date;
                    header.hasBeenSelected = false;
                    header.downloadPath = "";
                    //
                    // Deal with composite messages (articles having multiple parts)
                    // and store in a 'composite cache' messageStore
                    //
                    OptionalCompositeMessagePart ocmp = articleIsComposite(subject, articleId,
                                                                           author, date);
                    if(ocmp) {
                        std::string baseName = ocmp->baseName;
                        compositeMessageStore[baseName].push_back(ocmp.get());
                    } else {
                        //
                        // Store a normal non composite header
                        //
                        header.isComposite =false;
                        header.index = index;
                        index++;
                        m_headers.push_back(header);
                    }
                }

                ++c;
            }

            // Fill in m_headers with composite parts
            for(auto const & it : compositeMessageStore) {
                auto baseName = it.first;
                Header header;
                header.subject = baseName;
                header.isComposite = true;
                header.date = it.second.begin()->date;
                header.author = it.second.begin()->author;
                header.compositeMessageParts.assign(it.second.begin(), it.second.end());
                std::sort(header.compositeMessageParts.begin(), header.compositeMessageParts.end(), PartSorter());
                header.index = index;
                index++;
                m_headers.push_back(header);
            }

        }

        HeadersData hd;
        hd.headers = m_headers;
        hd.status = status;
        emit headersReadFinishedSignal(hd);
    }


    /**
     * @brief NNTPConnector::articleIsComposite checks if a subject line contains
     * the string (a/b) e.g. (1/5) to indicate article is part 1 of 5 parts. In other
     * works it checks the subject line as a point of reference in determining if
     * the article is part of a composite message as in the case of multipart binaries
     * @param subjectLine the subject line which might contain (a/b)
     * @param articleId id of this article
     * @return an OptionalCompositeMessagePart if the message is found to include (a/b_
     */
    OptionalCompositeMessagePart
    HeaderExtractor::articleIsComposite(std::string &subjectLine, int const articleId,
                                        std::string &author, std::string &date )
    {
        std::string::reverse_iterator rit;
        bool firstBracketFound = false;
        std::string::reverse_iterator stopPosition;
        std::stringstream baseNameBuilder;
        if(!subjectLine.empty()) {
            for(rit = subjectLine.rbegin(); rit != subjectLine.rend(); ++rit) {

                if(*rit == '(') {
                    firstBracketFound = true;
                    stopPosition = rit;
                    ++rit;
                }

                if(!firstBracketFound) {
                    continue;
                } else {
                    baseNameBuilder << *rit;
                }
            }
            if(!firstBracketFound) {
                return OptionalCompositeMessagePart();
            }
            auto baseNameReversed = baseNameBuilder.str();
            std::string baseName(baseNameReversed.rbegin(), baseNameReversed.rend());
            std::stringstream partBuilder;
            for(rit = stopPosition; rit >= subjectLine.rbegin(); --rit) {
                partBuilder << *rit;
            }
            auto partDescriptor = partBuilder.str();
            std::string::iterator it;
            std::stringstream number1;
            bool slashFound = false;
            std::string::iterator slashPosition;
            for(it = partDescriptor.begin()+1; it != partDescriptor.end(); ++it) {
                if(*it != '/') {
                    number1 << *it;
                } else {
                    slashFound = true;
                    slashPosition = it;
                    break;
                }
            }
            if(!slashFound) {
                return OptionalCompositeMessagePart();
            }
            std::stringstream number2;
            bool secondBracketFound = false;
            for(it = slashPosition + 1; it != partDescriptor.end(); ++it) {
                if(*it != ')') {
                    number2 << *it;
                } else {
                    secondBracketFound = true;
                    break;
                }
            }
            if(!secondBracketFound) {
                return OptionalCompositeMessagePart();
            }
            int partNumber;
            int totalParts;
            try {
                partNumber = atoi(number1.str().c_str());
            } catch (...) {
                return OptionalCompositeMessagePart();
            }
            try {
                totalParts = atoi(number2.str().c_str());
            } catch (...) {
                return OptionalCompositeMessagePart();
            }
            CompositeMessagePart cmp;
            cmp.thisPart = partNumber;
            cmp.totalParts = totalParts;
            cmp.baseName = baseName;
            cmp.originalName = subjectLine;
            cmp.articleId = articleId;
            cmp.author = author;
            cmp.date = date;
            return OptionalCompositeMessagePart(cmp);
        }
        return OptionalCompositeMessagePart();
    }

}
