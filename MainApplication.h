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

#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include "mainwidget.h"
#include "managednntpconnection.h"
#include "prefswidget.h"
#include "statusmessagedisplayer.h"


#include <QObject>
#include <QThread>
#include <QListWidgetItem>
#include <QTapAndHoldGesture>
#include <QGestureEvent>
#include <QTimer>
#include <QScopedPointer>
#include <deque>

class QTapAndHoldGesture;
class ManagedGroupTab;
class ConnectionThread;
class LoginThread;

typedef QScopedPointer<QTimer> TimerPtr;

typedef std::vector<ManagedGroupTab*> GroupTabs;

class MainApplication : public QObject
{
    Q_OBJECT
public:
    explicit MainApplication(QObject *parent = 0);
    ~MainApplication();

    void createPersistentPrefsFile();
    void createPersistentGroupsFile();
    MainWidget m_w;

signals:
    void statusMessageSignal(QString);

public slots:
    void displayPrefsWidget();
    void groupsLoadFinishedSlot();
    void groupAddedSlot(QString str);
    void groupSelectedEvent(QListWidgetItem* itme);
    void tapGestureSlot();
    void extractHeadersSlot();
    void headersReadFinishedSlot(HeadersData hd);
    void singleArticleExtractedSlot();
    void connectFinishedSlot(int status);
    void loginFinishedSlot(bool);
    void prefsUpdatedSlot();
    void issuedLASTCommandSlot();
    void finishedIssuingLASTCommandsSlot();
    void closeTabSlot(int index);
    void searchButtonSlot();
    void showAllButtonSlot();
    void statusMessageSlot(QString);
    void hideStatusDisplaySlot();
    void headCommandsIssuedSlot();
    void bytesReadSlot(int const);
    void readBeginSlot(int const count);
    void readBitOfDataSlot();
    void resetBytesReadSlot();

private:


    ConnectionPtr m_connectionPtr;
    QTapAndHoldGesture *tapper;
    QString m_selectedGroup;
    ConnectionThread *m_connectionThread;
    LoginThread *m_loginThread;
    PrefsWidgetPtr m_prefsWidgetPtr;
    bool m_connected;
    bool m_authorized;
    bool m_prefsRead;
    GroupTabs m_groupTabs;
    Groups m_groups;
    QString m_server;
    QString m_username;
    QString m_password;
    QString m_from;
    QString m_org;
    bool m_ssl;
    bool m_deleteCacheOnExit;
    QThread m_workerThread;
    StatusMessageDisplayer m_statusMessageDisplayer;
    TimerPtr m_timerPtr;
    std::deque<QString> m_messageBuffer;
    int m_port;
    bool m_groupsAdded;
    void disableButtons();
    void connectSignalsToSlots();
    void checkForGroupCache();
    void checkForPrefsFile();
    void updateGroupTabProgressBars(int const val);
    void setGroupTabProgressBarMaximums(int const val);
    void resetGroupTabProgressBars();

protected:
    bool eventFilter(QObject *obj, QGestureEvent *event);

};

#endif // MAINAPPLICATION_H
