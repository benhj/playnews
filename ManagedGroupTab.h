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

#ifndef MANAGEDGROUPTAB_H
#define MANAGEDGROUPTAB_H

#include "MainWidget.h"
#include "ManagedNNTPConnection.h"
#include "HeadersWidget.h"
#include "StatusMessageDisplayer.h"
#include "BinaryGrabber.h"
#include "BulkDownloader.h"
#include <QObject>
#include <QString>
#include <QThread>
#include <map>

class ArticleLoaderThread;
class QWidget;
class QGridLayout;
class QTabWidget;

class ManagedGroupTab : public QObject
{
    Q_OBJECT
public:
    explicit ManagedGroupTab(QObject *parent,
                    QString const &groupName,
                    MainWidget &w,
                    ConnectionPtr &connection,
                    ConnectionInfo const& connectionInfo,
                    QThread &worker,
                    StatusMessageDisplayer &statusMessageDisplayer,
                    Headers &headers);
    virtual ~ManagedGroupTab();
    void addGroupTab();

signals:
    void partDecodedSignal();
    void pictureSavedSignal(QString message);
    void loadingMediaSignal(QString message);
    void resetHeadersList(QString message);

public slots:
    void readArticleSlot();
    //void postSlot();
    void displayArticleSlot(ArticleData &data);
    //void replySlot(QString const &articleData);
    void searchSlot();
    void showAllSlot();
    void partDecodedSlot();
    void pictureSavedSlot(QString message);
    void openBinary(Header header, bool const autoSave = false);
    void updateProgressBar(int const val);
    void setProgressBarMaximum(int const val);
    void updateBytesDisplay(double const val);
    void resetProgressBar();
    //void bulkDownloadSlot();

private:
    ArticleLoaderThread *m_articleLoaderThread;
    QString m_groupName;
    MainWidget &m_w;
    ConnectionPtr m_connection;
    Headers m_headers;
    QWidget *m_selectedGroupTab;
    QGridLayout *m_gridLayout;
    HeadersWidget *m_headersWidget;
    QThread &m_worker;
    std::vector<int> m_headerIndices;
    StatusMessageDisplayer &m_statusMessageDisplayer;
    QTabWidget *m_parentWidget;
    int m_articleCode;
    std::string m_subject;
    std::string m_date;
    std::string m_author;
    int m_selectedArticleIndex;
    BinaryGrabberPtr m_binaryGrabberPtr;
    BulkDownloaderPtr m_bulkDownloaderPtr;
    ConnectionInfo m_connectionInfo;
};

#endif // MANAGEDGROUPTAB_H
