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

#ifndef HEADEREXTRACTOR_H
#define HEADEREXTRACTOR_H

#include "NNTPConnector.h"
#include "ConnectionInfo.h"
#include "CompositeMessagePart.h"
#include "ArticleHeader.h"
#include <QScopedPointer>
#include <QThread>

typedef std::map<std::string, CompositeMessageParts> CompositeMessageStore;

class HeaderExtractor;
typedef QScopedPointer<HeaderExtractor> HeaderExtractorPtr;

class HeaderExtractor : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief HeaderExtractor extracts a bunch of headers from designated group
     * @param groupName the name of the group from which to extract
     * @param connection an NNTPConnector that wraps around a QTcpSocket
     */
    HeaderExtractor(QString const& groupName, ConnectionInfo const &connectionInfo, int const count,
                    QObject *callbackObject);

    ~HeaderExtractor();



    void process();

public slots:

    /**
     * @brief extractNHeadersUsingXOverCommand extracts count sets of article headers
     * @param count number of articles for which headers will be extracted
     */
    void extractNHeadersUsingXOverCommand();

    /**
     * @brief processXOverData processes the header data returned from the server
     * after having issued an xover command
     */
    void processXOverData(NNTPConnector &connector);

signals:
    void headersReadFinishedSignal(HeadersData);

private:
    QThread m_worker;
    QString m_groupName;
    ConnectionInfo m_connectionInfo;
    Headers m_headers;
    int m_count;
    QObject *m_callbackObject;

    /**
     * @brief articleIsComposite based on the subject line of the header, determines whether the
     * article specified by articleId is likely to be part of a composite message (e.g. multi-part
     * binary). If the subject line contains a string on the end (a/b) e.g. (1/2) then this
     * function will indicate that 'this' is part 1 of 2 parts
     * @param subjectLine the subject line to check for (a/b)
     * @param articleId the id of the article
     * @return a CompositeMessagePart if the article is determined to be a composite part
     */
    OptionalCompositeMessagePart articleIsComposite(std::string &subjectLine, int const articleId,
                                                    std::string &author, std::string &date);
};

#endif // HEADEREXTRACTOR_H
