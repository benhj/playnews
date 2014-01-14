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

#include "MainApplication.h"
#include "LoginDialog.h"
#include "ManagedNNTPConnection.h"
#include "ui_mainwidget.h"
#include "MessageReader.h"
#include "ui_MessageReader.h"
#include "ui_groupwidget.h"
#include "ArticleCountDialog.h"
#include "ui_articlecountdialog.h"
#include "ManagedGroupTab.h"
#include "ConnectionThread.h"
#include "LoginThread.h"
#include "SearchDialog.h"
#include "StatusMessageDisplayer.h"
#include "ArticleReader.h"
#include <QGridLayout>
#include <QWidget>
#include <QTapAndHoldGesture>
#include <QScopedPointer>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <sstream>
#include <cassert>
#include <fstream>

MainApplication::MainApplication(QObject *parent):
    QObject(parent),
    m_connected(false),
    m_authorized(false),
    m_prefsRead(false),
    m_connectionPtr(new ManagedNNTPConnection),
    m_connectionThread(NULL),
    m_loginThread(NULL),
    m_groupsAdded(false)
{
    connectSignalsToSlots();
    qRegisterMetaType<ArticleData>("ArticleData&");
    qRegisterMetaType<HeadersData>("HeadersData");
    checkForPrefsFile();
    checkForGroupCache();

/*
    tapper = new QTapAndHoldGesture(w.ui->groupsView);
    tapper->setTimeout(300);
    tapper->installEventFilter(this);
*/
}

void
MainApplication::connectSignalsToSlots()
{

    QObject::connect(this, SIGNAL(statusMessageSignal(QString)), this, SLOT(statusMessageSlot(QString)),
                     Qt::QueuedConnection);

    QObject::connect(m_w.ui->groupWidget->ui->connectButton, SIGNAL(clicked()), this,
                     SLOT(displayPrefsWidget()));
    QObject::connect(m_w.ui->groupWidget->ui->groupLoadButton, SIGNAL(clicked()), this,
                     SLOT(extractHeadersSlot()));

    //
    // Once active group list has been read from server
    //
    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(groupsLoadFinishedSignal()), this,
                     SLOT(groupsLoadFinishedSlot()));

    /*
    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(loginFinishedSignal(int)), this,
                     SLOT(connectFinishedSlot(int)));
                     */

    /*
    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(authorizedSignal(bool)), this,
                     SLOT(loginFinishedSlot(bool)));
                     */

    //
    // Not really used in practice (for future imp if required)
    //
    QObject::connect(m_w.ui->groupWidget->ui->groupsView, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(groupSelectedEvent(QListWidgetItem*)));

    //
    // When group headers have been read
    //
    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(headersReadFinishedSignal(HeadersData)), this,
                     SLOT(headersReadFinishedSlot(HeadersData)));

    //
    // When a group is selected, add it to pane
    //
    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(groupAddedSignal(QString)), this,
                     SLOT(groupAddedSlot(QString)));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(singleArticleExtractedSignal()), this,
                     SLOT(singleArticleExtractedSlot()));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(issuedLASTCommandSignal()), this,
                     SLOT(issuedLASTCommandSlot()));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(finishedIssuingLASTCommandsSignal()), this,
                     SLOT(finishedIssuingLASTCommandsSlot()));

    QObject::connect(m_w.ui->tabWidget,
                     SIGNAL(tabCloseRequested(int)),
                     this,
                     SLOT(closeTabSlot(int)));

    QObject::connect(m_w.ui->groupWidget->ui->searchButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(searchButtonSlot()));

    QObject::connect(m_w.ui->groupWidget->ui->showAllButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(showAllButtonSlot()));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(statusSignal(QString)),
                     this,
                     SLOT(statusMessageSlot(QString)));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(headCommandsIssuedSignal()),
                     this,
                     SLOT(headCommandsIssuedSlot()));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(bytesReadSignal(int)),
                     this,
                     SLOT(bytesReadSlot(int)));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(readBeginSignal(int)),
                     this,
                     SLOT(readBeginSlot(int)));

    QObject::connect(m_connectionPtr.data(),
                     SIGNAL(readBitOfDataSignal()),
                     this,
                     SLOT(readBitOfDataSlot()), Qt::QueuedConnection);

    QObject::connect(m_connectionPtr.data(), SIGNAL(resetBytesReadSignal()),
                     this, SLOT(resetBytesReadSlot()), Qt::QueuedConnection);
}

void
MainApplication::checkForGroupCache()
{
    QString homeDir = QDir::homePath();
    QString configDir = "/pbnews/config";
    QString cacheFile = "/groupList.txt";
    QString fullPath = homeDir + configDir + cacheFile;
    QFile filePath(fullPath);
    if(filePath.exists()) {
        std::fstream groupStream(fullPath.toStdString().c_str(), std::fstream::in);
        std::string line;
        while(std::getline(groupStream, line)) {
            Group group;
            group.groupName = line;
            m_groups.push_back(group);
            m_w.addItem(QString(line.c_str()));
        }
        groupStream.close();
        m_groupsAdded = true;
    } else {
        if(m_prefsRead) {
            loginFinishedSlot(true);
        }
    }
}

void
MainApplication::checkForPrefsFile()
{
    QString homeDir = QDir::homePath();
    QString configDir = "/pbnews/config";
    QString prefsFile = "/prefs.txt";
    QString fullPath = homeDir + configDir + prefsFile;
    QFile filePath(fullPath);
    if(filePath.exists()) {
        std::fstream credStream(fullPath.toStdString().c_str(), std::fstream::in);
        std::string line;
        std::getline(credStream, line);
        m_server = QString(line.c_str());
        std::getline(credStream, line);
        m_port = atoi(line.c_str());
        std::getline(credStream, line);
        m_ssl = atoi(line.c_str());
        std::getline(credStream, line);
        m_username = QString(line.c_str());
        std::getline(credStream, line);
        m_password = QString(line.c_str());
        std::getline(credStream, line);
        m_from = QString(line.c_str());
        std::getline(credStream, line);
        m_org = QString(line.c_str());
        std::getline(credStream, line);
        m_deleteCacheOnExit = atoi(line.c_str());
        std::getline(credStream, line);
        int connections = atoi(line.c_str());
        ConnectorBuilder::setAllowedConnections(connections);
        credStream.close();
        m_prefsRead = true;
        m_connectionPtr->setServerAndPort(m_server, m_port, m_ssl);
        m_connectionPtr->setUsernameAndPassword(m_username, m_password);
        if(!m_w.isVisible()) {
            m_w.resize(1024,600);
            m_w.show();
        }
    } else {
        displayPrefsWidget();
    }
}

void
MainApplication::disableButtons()
{
    m_w.ui->groupWidget->ui->searchButton->setEnabled(false);
}

void
MainApplication::displayPrefsWidget()
{
    m_prefsWidgetPtr.reset(new PrefsWidget);

    if(m_prefsRead) {
        m_prefsWidgetPtr->setServer(m_server);
        m_prefsWidgetPtr->setPort(m_port);
        m_prefsWidgetPtr->setSsl(m_ssl);
        m_prefsWidgetPtr->setUsername(m_username);
        m_prefsWidgetPtr->setPassword(m_password);
        m_prefsWidgetPtr->setFrom(m_from);
        m_prefsWidgetPtr->setOrg(m_org);
        m_prefsWidgetPtr->setDeleteCacheOnExit(m_deleteCacheOnExit);
        m_prefsWidgetPtr->setAllowedConnections(ConnectorBuilder::allowedConnections);
    }

    //
    // To force re-caching of group list
    //
    //m_groupsAdded = false;

    //loginDialog->setUsername( "Luca" );  // optional
    connect( m_prefsWidgetPtr->getConnectButton(),
             SIGNAL(clicked()),
             this,
             SLOT(prefsUpdatedSlot()));
    m_prefsWidgetPtr->show();
    m_prefsWidgetPtr->resize(1024,600);

}

//
// Need to trigger this once all groups have been loaded!
//
void
MainApplication::createPersistentGroupsFile()
{
    QString homeDir = QDir::homePath();
    QString configDir = "/pbnews/config";
    QString cacheFile = "/groupList.txt";
    QString fullPath = homeDir + configDir + cacheFile;
    std::fstream groupStream(fullPath.toStdString().c_str(), std::fstream::out);
    for(Groups::iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        groupStream << it->groupName << "\n";
    }
    groupStream.close();
}

void
MainApplication::createPersistentPrefsFile()
{
    QString homeDir = QDir::homePath();
    QString configDir = "/pbnews/config";
    QString prefsFile = "/prefs.txt";
    QString fullPath = homeDir + configDir + prefsFile;
    std::fstream out(fullPath.toStdString().c_str(), std::fstream::out);
    out << std::string(m_server.toStdString()) << "\n"
        << m_port << "\n"
        << m_ssl << "\n"
        << std::string(m_username.toStdString()) << "\n"
        << std::string(m_password.toStdString()) <<"\n"
        << std::string(m_from.toStdString()) << "\n"
        << std::string(m_org.toStdString()) << "\n"
        << m_deleteCacheOnExit << "\n"
        << ConnectorBuilder::allowedConnections << "\n";
    out.close();
}

void
MainApplication::prefsUpdatedSlot()
{
    m_prefsWidgetPtr->hide();
    m_server = m_prefsWidgetPtr->getServer();
    m_port = m_prefsWidgetPtr->getPort();
    m_ssl = m_prefsWidgetPtr->ssl();
    m_username = m_prefsWidgetPtr->getUsername();
    m_password = m_prefsWidgetPtr->getPassword();
    m_from = m_prefsWidgetPtr->getFrom();
    m_org = m_prefsWidgetPtr->getOrg();
    m_deleteCacheOnExit = m_prefsWidgetPtr->deleteCacheOnExit();
    ConnectorBuilder::setAllowedConnections(m_prefsWidgetPtr->allowedConnections());
    m_connectionPtr->setServerAndPort(m_server, m_port, m_ssl);
    m_connectionPtr->setUsernameAndPassword(m_username, m_password);
    m_prefsRead = true;
    loginFinishedSlot(true);
    createPersistentPrefsFile();
    if(!m_w.isVisible()) {
        m_w.resize(1024,600);
        m_w.show();
    }
}

void
MainApplication::groupsLoadFinishedSlot()
{
    //
    // All groups have been loaded and cached; insert them into list widget.
    // Also cache them to disk for later easier loading
    //
    Groups().swap(m_groups);
    m_groups = m_connectionPtr->getLoadedGroups();
    for(Groups::iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        m_w.addItem(QString(it->groupName.c_str()));
    }

    createPersistentGroupsFile();

    m_groupsAdded = true;

    //
    // Turn off busy status of progress bar
    //
    m_w.ui->groupWidget->ui->progressBar->setMaximum(1);
    this->bytesReadSlot(0);
    this->setGroupTabProgressBarMaximums(1);
    this->updateGroupTabProgressBars(1);

    //
    // Activate group-centric buttons
    //
    m_w.ui->groupWidget->ui->connectButton->setEnabled(true);
    m_w.ui->groupWidget->ui->searchButton->setEnabled(true);
}

//
// Probably should show non-deterministic progress bar as groups
// are being loaded
//
void
MainApplication::groupAddedSlot(QString str)
{
    m_w.ui->groupWidget->ui->groupsView->addItem(str);
}

void
MainApplication::groupSelectedEvent(QListWidgetItem* item)
{

    qDebug() << "in groupSelectedEvent function";
    m_selectedGroup = item->text();
    qDebug() << m_selectedGroup;
}

void
MainApplication::tapGestureSlot()
{
    if(tapper->state() == Qt::GestureFinished) {
        qDebug() << "in tap gesture slot";
    }
}

bool
MainApplication::eventFilter(QObject *obj, QGestureEvent *event)
{
    if (QGesture *tapAndHold = event->gesture(Qt::TapAndHoldGesture)) {
        qDebug() << "gesture event!";
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

void
MainApplication::extractHeadersSlot()
{
    if(m_groupsAdded) {
        QWidget widget;
        ArticleCountDialog acd(&widget);
        std::stringstream ss;
        int articlesToExtract = 500;
        ss << 500;
        acd.ui->articleCountEdit->setText(QString(ss.str().c_str()));
        if(acd.exec() == QDialog::Accepted) {
            QString returned = acd.ui->articleCountEdit->text();
            articlesToExtract = atoi(returned.toStdString().c_str());
            m_connectionPtr->extractNHeadersUsingXOverCommand(m_selectedGroup, articlesToExtract);

            //
            // Setting maximum of progress bar
            //
            m_w.ui->groupWidget->ui->progressBar->setMaximum(articlesToExtract);
            this->setGroupTabProgressBarMaximums(articlesToExtract);
        }
    }
}

void
MainApplication::singleArticleExtractedSlot()
{
    int const oldValue = m_w.ui->groupWidget->ui->progressBar->value();
    m_w.ui->groupWidget->ui->progressBar->setValue(oldValue + 1);
    this->updateGroupTabProgressBars(oldValue + 1);
}

//
// For when group headers have finished loading, we
// can do whatever. E.g. open a new tab and display
// the article headers in a list ready to be selected
// by user :-)
//
void
MainApplication::headersReadFinishedSlot(HeadersData hd)
{
    //
    // Stop progress bar busy status
    //
    qDebug() << "finihsed headers!!";
    m_w.ui->groupWidget->ui->progressBar->reset();
    m_w.ui->groupWidget->ui->progressBar->setMaximum(1);
    this->setGroupTabProgressBarMaximums(1);

    if(hd.status == 224)
    {
        //
        // Create a new tab with selected group name and push back
        // into managed group tab vector
        //
        ConnectionInfo connectionInfo(m_server,
                                      m_port,
                                      m_ssl,
                                      m_username,
                                      m_password);
        ManagedGroupTab *managedGroupTab = new ManagedGroupTab(m_w.ui->tabWidget,
                                                  m_selectedGroup,
                                                  m_w,
                                                  m_connectionPtr,
                                                  connectionInfo,
                                                  m_workerThread,
                                                  m_statusMessageDisplayer,
                                                  hd.headers);
        QObject::connect(managedGroupTab, SIGNAL(pictureSavedSignal(QString)), this, SLOT(statusMessageSlot(QString)));
        QObject::connect(managedGroupTab, SIGNAL(loadingMediaSignal(QString)), this, SLOT(statusMessageSlot(QString)));
        QObject::connect(managedGroupTab, SIGNAL(resetHeadersList(QString)), this, SLOT(statusMessageSlot(QString)));
        m_groupTabs.push_back(managedGroupTab);
    }
}

void
MainApplication::connectFinishedSlot(int status)
{
    /*
    if(status == 200 || status == 201) {
        m_connected = true;
        //
        // automatically commits suicide on finish
        //
        if(!m_username.toStdString().empty() && !m_password.toStdString().empty()) {
            m_loginThread = new LoginThread(m_connectionPtr, m_username, m_password);
            m_loginThread->start();
        } else {
            loginFinishedSlot(true);
        }
    }*/
}

void
MainApplication::loginFinishedSlot(bool authorized) {

    m_authorized = authorized;
    if(authorized && m_prefsRead) {

        qDebug() << "MainApplication: authenticated";

        //
        // Create a new group load thread while transferring thread ownsership
        // of m_connectionPtr to the thread and start the thread
        //
        if(!m_groupsAdded) {
            //
            // automatically commits suicide on finish
            //
            m_connectionPtr->loadGroups();

            //
            // Setting maximum of progress bar to 0 swicthes on its busy status
            //
            m_w.ui->groupWidget->ui->progressBar->setMaximum(0);
            this->setGroupTabProgressBarMaximums(0);
            //m_w.ui->groupWidget->ui->connectButton->setEnabled(false);
        } else {
            //
            // Turn off busy status of progress bar
            //
            qDebug() << "Turning off busy status..";
            m_w.ui->groupWidget->ui->progressBar->setMaximum(1);
            this->setGroupTabProgressBarMaximums(1);

            //
            // Activate group-centric buttons
            //
            m_w.ui->groupWidget->ui->connectButton->setEnabled(true);
            m_w.ui->groupWidget->ui->searchButton->setEnabled(true);
        }
    }
}

void
MainApplication::issuedLASTCommandSlot()
{
    int const oldValue = m_w.ui->groupWidget->ui->progressBar->value();
    m_w.ui->groupWidget->ui->progressBar->setValue(oldValue + 1);
    this->updateGroupTabProgressBars(oldValue + 1);
}

void
MainApplication::finishedIssuingLASTCommandsSlot()
{
    m_w.ui->groupWidget->ui->progressBar->reset();
    m_w.ui->groupWidget->ui->progressBar->setMaximum(0);
    this->setGroupTabProgressBarMaximums(0);
}

void
MainApplication::closeTabSlot(int index)
{
    //
    // Never allow closing of tab 0 which is the groups tab
    //
    if(index > 0) {
        m_w.ui->tabWidget->removeTab(index);
    }
}

void
MainApplication::searchButtonSlot()
{
    QWidget widget;
    SearchDialog sd(&widget);
    if(sd.exec() == QDialog::Accepted) {
        QString returned = sd.getText();
        m_w.removeAllItems();
        for(Groups::iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
            if(std::string(returned.toStdString()).empty()) {
                m_w.addItem(QString(it->groupName.c_str()));
            } else if(std::string(it->groupName).find(returned.toStdString()) != std::string::npos) {
                m_w.addItem(QString(it->groupName.c_str()));
            }
        }
    }
}

void
MainApplication::showAllButtonSlot()
{
    emit statusMessageSignal("Showing all groups...");
    m_w.removeAllItems();
    for(Groups::iterator it = m_groups.begin(); it != m_groups.end(); ++it) {
        m_w.addItem(QString(it->groupName.c_str()));
    }
}

void centerWidget(QWidget* parent, QWidget* child)
{
    QPoint centerparent(
    parent->x() + ((parent->frameGeometry().width() - child->frameGeometry().width()) /2),
    parent->y() + ((parent->frameGeometry().height() - child->frameGeometry().height()) /2));

    QDesktopWidget * pDesktop = QApplication::desktop();
    QRect sgRect = pDesktop->screenGeometry(pDesktop->screenNumber(parent));
    QRect childFrame = child->frameGeometry();

    if(centerparent.x() < sgRect.left())
        centerparent.setX(sgRect.left());
    else if((centerparent.x() + childFrame.width()) > sgRect.right())
        centerparent.setX(sgRect.right() - childFrame.width());

    if(centerparent.y() < sgRect.top())
        centerparent.setY(sgRect.top());
    else if((centerparent.y() + childFrame.height()) > sgRect.bottom())
        centerparent.setY(sgRect.bottom() - childFrame.height());

    child->move(centerparent);
}

void
MainApplication::statusMessageSlot(QString statusMessage)
{
    //qDebug() << "statusMessage: "<< statusMessage;

    if(m_statusMessageDisplayer.isVisible()) {
        if(m_timerPtr) {
            m_timerPtr->stop();
        }
        m_statusMessageDisplayer.hide();
    }

    /*
    m_messageBuffer.push_front(statusMessage);
    if(m_messageBuffer.size() > 1) {
        m_messageBuffer.pop_back();
    }

    QString concat("");
    std::deque<QString>::iterator it;
    for(it = m_messageBuffer.begin(); it != m_messageBuffer.end(); ++it) {
        concat += *it;
        concat += "\n";
    }

    qDebug() << "concat: "<<concat;
*/

    m_statusMessageDisplayer.setMessage(statusMessage);
    m_statusMessageDisplayer.showMaximized();
    centerWidget(&m_w, &m_statusMessageDisplayer);
    m_statusMessageDisplayer.setFocus();
    m_statusMessageDisplayer.setVisible(true);
    m_statusMessageDisplayer.raise();


    m_timerPtr.reset(new QTimer);
    QObject::connect(m_timerPtr.data(),
                     SIGNAL(timeout()),
                     this,
                     SLOT(hideStatusDisplaySlot()));
    m_timerPtr->setInterval(2000);
    m_timerPtr->start();

}

void
MainApplication::hideStatusDisplaySlot()
{

    if(m_statusMessageDisplayer.isVisible()) {
        qDebug() << "Hiding...";
        m_statusMessageDisplayer.hide();
        m_w.update();
    }
}

void
MainApplication::headCommandsIssuedSlot()
{
    m_w.ui->groupWidget->ui->progressBar->setValue(0);
    this->updateGroupTabProgressBars(0);
}

void
MainApplication::updateGroupTabProgressBars(int const val)
{
    GroupTabs::iterator it;
    for(it = m_groupTabs.begin(); it != m_groupTabs.end(); ++it) {
        (*it)->updateProgressBar(val);
    }
}

void
MainApplication::setGroupTabProgressBarMaximums(int const val)
{
    GroupTabs::iterator it;
    for(it = m_groupTabs.begin(); it != m_groupTabs.end(); ++it) {
        (*it)->setProgressBarMaximum(val);
    }
}

void
MainApplication::resetGroupTabProgressBars()
{
    GroupTabs::iterator it;
    for(it = m_groupTabs.begin(); it != m_groupTabs.end(); ++it) {
        (*it)->resetProgressBar();
    }
}

void
MainApplication::bytesReadSlot(int const bytesRead)
{
    double d = (double)(bytesRead / 1048576.0);
    double orig = m_w.ui->groupWidget->ui->byteDisplay->value();
    QString temp = QString::number(d+orig, 'f', 2);
    //temp = temp + "KB";
    m_w.ui->groupWidget->ui->byteDisplay->display(temp);

    GroupTabs::iterator it;
    for(it = m_groupTabs.begin(); it != m_groupTabs.end(); ++it) {
        (*it)->updateBytesDisplay(d+orig);
    }
}

void
MainApplication::readBeginSlot(int const count)
{
    m_w.ui->groupWidget->ui->progressBar->setMaximum(count);
    this->setGroupTabProgressBarMaximums(count);
}

void
MainApplication::resetBytesReadSlot()
{
    m_w.ui->groupWidget->ui->byteDisplay->display(0);
    GroupTabs::iterator it;
    for(it = m_groupTabs.begin(); it != m_groupTabs.end(); ++it) {
        (*it)->updateBytesDisplay(0);
    }
}

void
MainApplication::readBitOfDataSlot()
{
    int oldValue = m_w.ui->groupWidget->ui->progressBar->value();
    m_w.ui->groupWidget->ui->progressBar->setValue(oldValue + 1);
    this->updateGroupTabProgressBars(oldValue + 1);
    if(oldValue + 1 == m_w.ui->groupWidget->ui->progressBar->maximum()) {
        m_w.ui->groupWidget->ui->progressBar->reset();
        this->resetGroupTabProgressBars();
    }
}

MainApplication::~MainApplication()
{
    //
    // Remove cache on program exit
    //
    /*
    QString homeDir = QDir::homePath();
    QString pbnews = "/pbnews";
    QString cachePath = homeDir + pbnews + "/cache";
    QDir dir(cachePath);
    if (dir.exists(cachePath)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot
                                                    | QDir::System
                                                    | QDir::Hidden
                                                    | QDir::AllDirs
                                                    | QDir::Files,
                                                    QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    */
}
