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

#include "MainWidget.h"
#include "ManagedGroupTab.h"
#include "ManagedNNTPConnection.h"
#include "LoginDialog.h"
#include "StatusMessageDisplayer.h"

#include <QObject>
#include <QThread>
#include <QListWidgetItem>
#include <QTimer>
#include <QScopedPointer>
#include <deque>

typedef QScopedPointer<QTimer> TimerPtr;

typedef std::vector<ManagedGroupTabPtr> GroupTabs;

class MainApplication : public QObject
{
    Q_OBJECT
  public:
    explicit MainApplication(QObject *parent = 0);
    ~MainApplication();
    MainWidget m_mainWidget;

  signals:
    void statusMessageSignal(QString);

  public slots:
    void displayLoginDialog();
    void groupsLoadFinishedSlot();

    /// stores the currently selected group so that
    /// headers for group can be later loaded if desired
    void groupSelectedEvent(QListWidgetItem* item);

    /// called after the group load button has been pressed
    void extractHeadersSlot();

    /// indicates that header data for a given group has been downloaded
    /// can therefore populate a new group tab with data
    void headersReadFinishedSlot(core::HeadersData hd);

    void closeTabSlot(int index);

    /// allow searching of particular group in list of groups
    void searchButtonSlot();

    /// show all groups (e.g. after having searched for specific ones)
    void showAllButtonSlot();

    /// flashes up server status messages on screen
    void statusMessageSlot(QString);

    /// when the status display flasher has timed out, this
    /// slot is triggered to hide it
    void hideStatusDisplaySlot();

    /// for updateing how many bytes have been read from the server
    void bytesReadSlot(int const);

    /// puts bytes read back to zero
    void resetBytesReadSlot();

    /// after login dialog has popped up and the user has accepted
    /// the input credentials, this function is triggered
    void loginDialogAcceptedSlot(QString server,
                                 QString username,
                                 QString password);

  private:

    /// core server parameters
    QString m_server;
    QString m_username;
    QString m_password;
    int m_port;
    bool m_ssl;
    core::ManagedConnectionPtr m_managedConPtr;

    /// The loaded groups
    core::Groups m_groups;

    /// currently selected group
    QString m_selectedGroup;

    /// A pop-up asking for server, username and pw etc.
    LoginDialogPtr m_loginDialogPtr;

    /// Holds headers of loaded groups
    GroupTabs m_groupTabs;

    /// Flashes up messages coming back from the server
    StatusMessageDisplayer m_statusMessageDisplayer;

    /// For setting a timer on how long message flasher
    /// should stay open for
    TimerPtr m_timerPtr;

    /// connects the different gui components to this components
    void connectSignalsToSlots();

    /// connects managed connection to this gui component
    void setManagedConSignalsAndSlots();

    /// ensures that progress bars in all other tabs are
    /// also updated
    void updateGroupTabProgressBars(int const val);

    /// sets upper limit on all other tabs' progress bars
    void setGroupTabProgressBarMaximums(int const val);

    /// puts group tab progress bars back to zero
    void resetGroupTabProgressBars();

    /// connect and load groups
    void connectAndLoadGroups();

};
