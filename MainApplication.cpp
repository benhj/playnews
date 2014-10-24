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
#include "ui_MainWidget.h"
#include "MessageReader.h"
#include "ui_MessageReader.h"
#include "ui_GroupWidget.h"
#include "ArticleCountDialog.h"
#include "ui_ArticleCountDialog.h"
#include "SearchDialog.h"
#include "StatusMessageDisplayer.h"
#include "ArticleReader.h"
#include <QGridLayout>
#include <QWidget>
#include <QScopedPointer>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <sstream>
#include <cassert>
#include <fstream>

namespace {

    void centerWidget(QWidget* parent, QWidget* child)
    {
        QPoint centerparent(
        parent->x() + ((parent->frameGeometry().width() - child->frameGeometry().width()) /2),
        parent->y() + ((parent->frameGeometry().height() - child->frameGeometry().height()) /2));

        auto pDesktop = QApplication::desktop();
        auto sgRect = pDesktop->screenGeometry(pDesktop->screenNumber(parent));
        auto childFrame = child->frameGeometry();

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

    int getPortFromString(QString const &str)
    {
        auto strings = str.split(":");
        if(strings.size() > 1) {
            return strings[1].toInt();
        }
        return 119;
    }

    QString getServerFromString(QString const &str)
    {
        auto strings = str.split(":");
        return strings[0];
    }

}

MainApplication::MainApplication(QObject *parent)
  : QObject(parent)
  , m_server()
  , m_username()
  , m_password()
  , m_port(119)
  , m_ssl(false)
  , m_managedConPtr()
  , m_groups()
  , m_selectedGroup()
  , m_loginDialogPtr()
  , m_groupTabs()
  , m_statusMessageDisplayer()
  , m_timerPtr()
{
    connectSignalsToSlots();
    qRegisterMetaType<core::ArticleData>("ArticleData&");
    qRegisterMetaType<core::HeadersData>("HeadersData");
    displayLoginDialog();
}

void
MainApplication::connectSignalsToSlots()
{
    // display status messages emitted by 'this'
    QObject::connect(this, SIGNAL(statusMessageSignal(QString)),
                     this, SLOT(statusMessageSlot(QString)),
                     Qt::QueuedConnection);

    QObject::connect(m_mainWidget.ui->groupWidget->ui->connectButton, SIGNAL(clicked()),
                     this, SLOT(displayLoginDialog()));

    // For pulling in headers of particular group
    QObject::connect(m_mainWidget.ui->groupWidget->ui->groupLoadButton, SIGNAL(clicked()),
                     this, SLOT(extractHeadersSlot()));

    QObject::connect(m_mainWidget.ui->tabWidget, SIGNAL(tabCloseRequested(int)),
                     this, SLOT(closeTabSlot(int)));

    QObject::connect(m_mainWidget.ui->groupWidget->ui->searchButton, SIGNAL(clicked()),
                     this, SLOT(searchButtonSlot()));

    QObject::connect(m_mainWidget.ui->groupWidget->ui->showAllButton, SIGNAL(clicked()),
                     this, SLOT(showAllButtonSlot()));

    QObject::connect(m_mainWidget.ui->groupWidget->ui->groupsView, SIGNAL(itemClicked(QListWidgetItem*)),
                     this, SLOT(groupSelectedEvent(QListWidgetItem*)));
}

void
MainApplication::displayLoginDialog()
{
    m_loginDialogPtr.reset(new LoginDialog);

    QObject::connect(m_loginDialogPtr.data(),
                     SIGNAL(acceptLogin(QString, QString, QString)),
                     this,
                     SLOT(loginDialogAcceptedSlot(QString , QString , QString )));

    m_loginDialogPtr->show();
}

void
MainApplication::groupsLoadFinishedSlot()
{

    // All groups have been loaded and cached; insert them into list widget.
    core::Groups().swap(m_groups);
    m_groups = m_managedConPtr->getLoadedGroups();
    for(auto const & it : m_groups) {
         m_mainWidget.addItem(QString(it.groupName.c_str()));
    }

    // Turn off busy status of progress bar
    m_mainWidget.ui->groupWidget->ui->progressBar->setMaximum(1);
    this->bytesReadSlot(0);
    this->setGroupTabProgressBarMaximums(1);
    this->updateGroupTabProgressBars(1);

    // Activate group-centric buttons
    m_mainWidget.ui->groupWidget->ui->connectButton->setEnabled(true);
    m_mainWidget.ui->groupWidget->ui->searchButton->setEnabled(true);
}

void
MainApplication::groupSelectedEvent(QListWidgetItem* item)
{
    qDebug() << "in groupSelectedEvent function";
    m_selectedGroup = item->text();
    qDebug() << m_selectedGroup;
}

void
MainApplication::extractHeadersSlot()
{
    QWidget widget;
    ArticleCountDialog acd(&widget);
    std::stringstream ss;
    int articlesToExtract = 500;
    ss << 500;
    acd.ui->articleCountEdit->setText(QString(ss.str().c_str()));
    if(acd.exec() == QDialog::Accepted) {
        auto returned = acd.ui->articleCountEdit->text();
        articlesToExtract = atoi(returned.toStdString().c_str());
        m_managedConPtr->extractNHeadersUsingXOverCommand(m_selectedGroup, articlesToExtract);

        // Setting maximum of progress bar
        m_mainWidget.ui->groupWidget->ui->progressBar->setMaximum(articlesToExtract);
        this->setGroupTabProgressBarMaximums(articlesToExtract);
    }
}

void
MainApplication::headersReadFinishedSlot(core::HeadersData hd)
{
    // Stop progress bar busy status
    qDebug() << "finihsed headers!!";
    m_mainWidget.ui->groupWidget->ui->progressBar->reset();
    m_mainWidget.ui->groupWidget->ui->progressBar->setMaximum(1);
    this->setGroupTabProgressBarMaximums(1);

    if(hd.status == 224)
    {

        // Create a new tab with selected group name and push back
        // into managed group tab vector
        core::ConnectionInfo connectionInfo(m_server,
                                            m_port,
                                            m_ssl,
                                            m_username,
                                            m_password);
        ManagedGroupTabPtr managedGroupTab(new ManagedGroupTab(m_mainWidget.ui->tabWidget,
                                                               m_selectedGroup,
                                                               m_managedConPtr,
                                                               connectionInfo,
                                                               m_statusMessageDisplayer,
                                                               hd.headers));
        QObject::connect(managedGroupTab.data(), SIGNAL(pictureSavedSignal(QString)),
                         this, SLOT(statusMessageSlot(QString)));
        QObject::connect(managedGroupTab.data(), SIGNAL(loadingMediaSignal(QString)),
                         this, SLOT(statusMessageSlot(QString)));
        QObject::connect(managedGroupTab.data(), SIGNAL(resetHeadersList(QString)),
                         this, SLOT(statusMessageSlot(QString)));
        m_groupTabs.push_back(managedGroupTab);
    }
}

void
MainApplication::connectAndLoadGroups()
{
    qDebug() << "MainApplication: authenticated";

    // automatically commits suicide on finish
    m_managedConPtr->loadGroups();

    // Setting maximum of progress bar to 0 swicthes on its busy status
    m_mainWidget.ui->groupWidget->ui->progressBar->setMaximum(0);
    this->setGroupTabProgressBarMaximums(0);
}

void
MainApplication::closeTabSlot(int index)
{
    // Never allow closing of tab 0 which is the groups tab
    if(index > 0) {
        m_mainWidget.ui->tabWidget->removeTab(index);
    }
}

void
MainApplication::searchButtonSlot()
{
    QWidget widget;
    SearchDialog sd(&widget);
    if(sd.exec() == QDialog::Accepted) {
        QString returned = sd.getText();
        m_mainWidget.removeAllItems();
        for(auto const & it : m_groups) {
            if(std::string(returned.toStdString()).empty()) {
                m_mainWidget.addItem(QString(it.groupName.c_str()));
            } else if(std::string(it.groupName).find(returned.toStdString()) != std::string::npos) {
                m_mainWidget.addItem(QString(it.groupName.c_str()));
            }
        }
    }
}

void
MainApplication::showAllButtonSlot()
{
    emit statusMessageSignal("Showing all groups...");
    m_mainWidget.removeAllItems();
    for(auto const & it : m_groups) {
        m_mainWidget.addItem(QString(it.groupName.c_str()));
    }
}

void
MainApplication::statusMessageSlot(QString statusMessage)
{
    if(m_statusMessageDisplayer.isVisible()) {
        if(m_timerPtr) {
            m_timerPtr->stop();
        }
        m_statusMessageDisplayer.hide();
    }

    m_statusMessageDisplayer.setMessage(statusMessage);
    m_statusMessageDisplayer.showMaximized();
    centerWidget(&m_mainWidget, &m_statusMessageDisplayer);
    m_statusMessageDisplayer.setFocus();
    m_statusMessageDisplayer.setVisible(true);
    m_statusMessageDisplayer.raise();

    // display message for a short time as indicated by timer
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
        m_mainWidget.update();
    }
}

void
MainApplication::updateGroupTabProgressBars(int const val)
{
    for(auto const & it : m_groupTabs) {
        (*it).updateProgressBar(val);
    }
}

void
MainApplication::setGroupTabProgressBarMaximums(int const val)
{
    for(auto const & it : m_groupTabs) {
        (*it).setProgressBarMaximum(val);
    }
}

void
MainApplication::resetGroupTabProgressBars()
{
    for(auto const & it : m_groupTabs) {
        (*it).resetProgressBar();
    }
}

void MainApplication::setManagedConSignalsAndSlots()
{
    // Once active group list has been read from server
    QObject::connect(m_managedConPtr.data(),
                     SIGNAL(groupsLoadFinishedSignal()), this,
                     SLOT(groupsLoadFinishedSlot()));

    // When group headers have been read
    QObject::connect(m_managedConPtr.data(),
                     SIGNAL(headersReadFinishedSignal(core::HeadersData)), this,
                     SLOT(headersReadFinishedSlot(core::HeadersData)));

    // When a status message is flashed up on screen
    QObject::connect(m_managedConPtr.data(),
                     SIGNAL(statusSignal(QString)),
                     this,
                     SLOT(statusMessageSlot(QString)));

    // Captures when bytes have been read by the connector
    QObject::connect(m_managedConPtr.data(),
                     SIGNAL(bytesReadSignal(int)),
                     this,
                     SLOT(bytesReadSlot(int)));

    // For putting the bytes read back in to a ground state
    QObject::connect(m_managedConPtr.data(), SIGNAL(resetBytesReadSignal()),
                     this, SLOT(resetBytesReadSlot()), Qt::QueuedConnection);
}

void
MainApplication::bytesReadSlot(int const bytesRead)
{
    // following magic number converts bytes to MB
    double d = (double)(bytesRead / 1048576.0);
    double orig = m_mainWidget.ui->groupWidget->ui->byteDisplay->value();
    auto temp = QString::number(d+orig, 'f', 2);
    m_mainWidget.ui->groupWidget->ui->byteDisplay->display(temp);

    for(auto const & it : m_groupTabs) {
        (*it).updateBytesDisplay(d+orig);
    }
}

void
MainApplication::resetBytesReadSlot()
{
    m_mainWidget.ui->groupWidget->ui->byteDisplay->display(0);
    for(auto const & it : m_groupTabs) {
        (*it).updateBytesDisplay(0);
    }
}

void MainApplication::loginDialogAcceptedSlot(QString server,
                                              QString username,
                                              QString password)
{
    m_loginDialogPtr->hide();
    m_server = getServerFromString(server);
    m_port = getPortFromString(server);
    m_ssl = (m_port == 563); // TODO
    m_username = username;
    m_password = password;
    m_managedConPtr = core::ManagedConnectionPtr(new core::ManagedNNTPConnection(m_server,
                                                                                 m_port,
                                                                                 m_ssl,
                                                                                 m_username,
                                                                                 m_password));
    this->setManagedConSignalsAndSlots();
    this->connectAndLoadGroups();

    if(!m_mainWidget.isVisible()) {
        m_mainWidget.resize(1024,600);
        m_mainWidget.show();
    }
}

MainApplication::~MainApplication()
{
}
