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

#include "ArticleHeader.h"
#include "ManagedNNTPConnection.h"
#include "BinaryGrabber.h"
#include <QScopedPointer>

class QThread;

namespace core {

    class BulkDownloader;

    typedef QScopedPointer<BulkDownloader> BulkDownloaderPtr;

    class BulkDownloader : public QObject
    {
        Q_OBJECT
    public:
        BulkDownloader(ManagedConnectionPtr &connection, QString const &groupName, QThread &worker);
        void addHeader(Header * header);
        void bulkDownload();
        void downloadAndIterate();

    public slots:
        void openBinary(Header, bool autoSave = true);
        void resetProgressBar();
        void setProgressBarMaximum(int);
        void partDecodedSlot();

    signals:
        void openBinarySignal(Header, bool const);
        void resetProgressBarSignal();
        void setProgressBarMaximumSignal(int);
        void partDecodedSignal();

    private:
        HeaderPtrs m_headers;
        ManagedConnectionPtr &m_connection;
        QString m_groupName;
        BinaryGrabberPtr m_binaryGrabberPtr;
        HeaderPtrs::iterator m_headerIterator;
        QThread &m_worker;
    };

}
