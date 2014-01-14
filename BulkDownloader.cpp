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


#include "BulkDownloader.h"
#include <QThread>

BulkDownloader::BulkDownloader(ConnectionPtr &connection,
                               QString const &groupName,
                               QThread &worker) :
                                m_connection(connection),
                                m_groupName(groupName),
                                m_worker(worker)
{
}

void
BulkDownloader::addHeader(Header *header)
{
    m_headers.push_back(header);
}

void
BulkDownloader::bulkDownload()
{

    m_headerIterator = m_headers.begin();
    this->downloadAndIterate();

}

void
BulkDownloader::downloadAndIterate()
{
    if(m_headerIterator != m_headers.end()) {
        if((*m_headerIterator)->isComposite) {

            m_binaryGrabberPtr.reset(new BinaryGrabber(m_connection,
                                                       m_groupName,
                                                       *(*m_headerIterator),
                                                       m_worker,
                                                       true));

            QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(binaryHasBeenReadSignal(Header, bool)),
                             this, SLOT(openBinary(Header, bool)));

            QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(resetProgressBarSignal()),
                             this, SLOT(resetProgressBar()));

            QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(setProgressBarMaximum(int)),
                             this, SLOT(setProgressBarMaximum(int)));

            QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(partDecodedSignal()),
                             this, SLOT(partDecodedSlot()));

            m_binaryGrabberPtr->readMultiPartBinary();

        } else {
            m_binaryGrabberPtr.reset(new BinaryGrabber(m_connection,
                                                       m_groupName,
                                                       *(*m_headerIterator),
                                                       m_worker,
                                                       false));
            QObject::connect(m_binaryGrabberPtr.data(), SIGNAL(binaryHasBeenReadSignal(Header, bool)),
                             this, SLOT(openBinary(Header, bool)));
            m_binaryGrabberPtr->handleBinaryData();
        }
        ++m_headerIterator;
    }
}

void
BulkDownloader::openBinary(Header header, bool)
{
    qDebug() << "in BulkDownloader openBinary..";
    QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(binaryHasBeenReadSignal(Header, bool)),
                     this, SLOT(openBinary(Header, bool)));

    QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(resetProgressBarSignal()),
                     this, SLOT(resetProgressBar()));

    QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(setProgressBarMaximum(int)),
                     this, SLOT(setProgressBarMaximum(int)));

    QObject::disconnect(m_binaryGrabberPtr.data(), SIGNAL(partDecodedSignal()),
                     this, SLOT(partDecodedSlot()));


    qDebug() <<"a";
    emit openBinarySignal(header, true);
    qDebug() <<"b";
    sleep(1);
    downloadAndIterate();
    qDebug() <<"c";

}

void
BulkDownloader::resetProgressBar()
{
    emit resetProgressBarSignal();
}

void
BulkDownloader::setProgressBarMaximum(int maximum)
{
    emit setProgressBarMaximumSignal(maximum);
}

void
BulkDownloader::partDecodedSlot()
{
    emit partDecodedSignal();
}
