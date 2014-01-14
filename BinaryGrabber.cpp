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

#include "BinaryGrabber.h"
#include <QDir>
#include <QThread>
#include <fstream>

BinaryGrabber::BinaryGrabber(ConnectionPtr &connection,
                             QString const &groupName,
                             Header &header,
                             QThread &worker,
                             bool const compositeReadMode) :
                            m_connection(connection),
                            m_groupName(groupName),
                            m_header(header),
                            m_worker(worker),
                            m_compositeReadMode(compositeReadMode),
                            m_finished(false)
{
    QObject::connect(&m_yencDecoder, SIGNAL(partDecoded()), this, SLOT(partDecodedSlot()));
}

void
BinaryGrabber::readMultiPartBinary()
{
    //
    // Loop over composite article codes
    //
    CompositeMessageParts::iterator cmpsIt;
    std::vector<int> compositeCodes;
    if(!m_header.compositeMessageParts.empty()) {
        for(cmpsIt = m_header.compositeMessageParts.begin();
            cmpsIt!= m_header.compositeMessageParts.end();
            ++cmpsIt) {
            compositeCodes.push_back(cmpsIt->articleId);
        }

        //
        // make sure codes are in order to preserve ordering of parts
        // (this is hypothetical and is not certain since parts might
        // exist in an inconsistent ordering on the server)
        //
        //std::sort(compositeCodes.begin(), compositeCodes.end());

        //
        // check that number of parts matches expected number of parts
        //
        int numberOf = compositeCodes.size();
        if(numberOf == m_header.compositeMessageParts[0].totalParts) {
            emit resetProgressBarSignal();
            emit setProgressBarMaximum(numberOf);

            //
            // Process composite article codes in a CompositeArticleLoaderThread (TODO)
            // chunking all of the resulting data together and then processing it
            // as one large chunk (perhaps)
            //
            // 26th May 2013 -- m_connection now starts threading process as
            // m_connection is the connection manager
            //
            QObject::connect(m_connection.data(), SIGNAL(compositeDataReadSignal()), this,
                             SLOT(handleBinaryData()));
            m_connection->selectAndReadCollection(m_groupName, compositeCodes);
        } else {
            //
            // Not all parts present
            //
        }
    }
}

void
BinaryGrabber::decodeHeadPart(int const id, std::ofstream &out)
{

    std::ostringstream ss;
    ss << "/" << id;

    QString tempDir = QDir::tempPath() + ss.str().c_str();
    std::ifstream articleStream(tempDir.toStdString().c_str());
    if(m_yencDecoder.setHead(articleStream)) { // if not, not a binary file
        std::string filename = m_yencDecoder.getFileName();

        QString fileName = QDir::homePath() + "/pbnews/cache/";
        fileName = fileName + QString(filename.c_str());
        if (!fileName.isEmpty()) {

            out.open(fileName.toStdString().c_str(),
                          std::ios_base::binary);
            m_header.downloadPath = fileName;
            if(out.is_open()) {
                qDebug() << "writing to cache..";
                articleStream.seekg(0, articleStream.beg);
                m_yencDecoder.decode(articleStream, out);
            }
        }
    }
    articleStream.close();
}

void
BinaryGrabber::decodePart(int const id, std::ofstream &out)
{
    std::ostringstream ss;
    ss << "/" << id;
    QString tempDir = QDir::tempPath() + ss.str().c_str();
    std::ifstream articleStream(tempDir.toStdString().c_str());
    m_yencDecoder.decode(articleStream, out);
    articleStream.close();
}

void
BinaryGrabber::handleComposite()
{

    int articleId = m_header.compositeMessageParts.at(0).articleId;
    std::ofstream out;
    this->decodeHeadPart(articleId, out);

    CompositeMessageParts::iterator codeIt;
    for(codeIt = m_header.compositeMessageParts.begin() + 1;
        codeIt != m_header.compositeMessageParts.end();
        ++codeIt) {
        this->decodePart(codeIt->articleId, out);
    }
    out.close();
}

void
BinaryGrabber::handleSingle()
{
    int articleId = m_header.messageId;
    std::ofstream out;
    this->decodeHeadPart(articleId, out);
    out.close();
}

void
BinaryGrabber::handleBinaryData()
{
    qDebug() << "here "<<m_header.isComposite;
    if(m_header.isComposite) {
        this->handleComposite();
    } else {
        this->handleSingle();
    }
    emit binaryHasBeenReadSignal(m_header, false);
}

bool
BinaryGrabber::hasFinished()
{
    return m_finished;
}

void
BinaryGrabber::partDecodedSlot()
{
    emit partDecodedSignal();
}
