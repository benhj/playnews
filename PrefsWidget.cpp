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

#include "PrefsWidget.h"
#include "ui_PrefsWidget.h"
#include <sstream>

PrefsWidget::PrefsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrefsWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(closeSlot()));
}

PrefsWidget::~PrefsWidget()
{
    delete ui;
}

QString
PrefsWidget::getUsername()
{
    return ui->usernameEdit->text();
}

QString
PrefsWidget::getPassword()
{
    return ui->passwordEdit->text();
}

QString
PrefsWidget::getServer()
{
    return ui->serverEdit->text();
}

int
PrefsWidget::getPort()
{
    return atoi(ui->portEdit->text().toStdString().c_str());
}

QString
PrefsWidget::getFrom()
{
    return ui->emailEdit->text();
}

QString
PrefsWidget::getOrg()
{
    //return ui->organizationEdit->text();
    return QString("noOrg");
}

bool
PrefsWidget::ssl()
{
    return ui->sslCheck->isChecked();
}

bool
PrefsWidget::deleteCacheOnExit()
{
    return ui->deleteOnExitCheck->isChecked();
}

void
PrefsWidget::setUsername(QString const& username)
{
    ui->usernameEdit->setText(username);
}

void
PrefsWidget::setPassword(QString const& password)
{
    ui->passwordEdit->setText(password);
}

void
PrefsWidget::setServer(QString const& server)
{
    ui->serverEdit->setText(server);
}

void
PrefsWidget::setPort(int const port)
{
    std::stringstream portStream;
    portStream << port;
    ui->portEdit->setText(QString(portStream.str().c_str()));
}

void
PrefsWidget::setFrom(QString const& from)
{
    ui->emailEdit->setText(from);
}

void
PrefsWidget::setOrg(QString const& org)
{
    //ui->organizationEdit->setText(org);
}

void
PrefsWidget::setSsl(bool const ssl)
{
    ui->sslCheck->setChecked(ssl);
}

void
PrefsWidget::setDeleteCacheOnExit(bool const yes)
{
    ui->deleteOnExitCheck->setChecked(yes);
}

void
PrefsWidget::setAllowedConnections(int const con)
{
    std::stringstream conStream;
    conStream << con;
    ui->connectionCountEdit->setText(QString(conStream.str().c_str()));
}

QPushButton *
PrefsWidget::getConnectButton()
{
    return ui->connectButton;
}

int
PrefsWidget::allowedConnections()
{
    return atoi(ui->connectionCountEdit->text().toStdString().c_str());
}

void
PrefsWidget::closeSlot()
{
    this->hide();
}
