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

#ifndef PREFSWIDGET_H
#define PREFSWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <QPushButton>

namespace Ui {
class PrefsWidget;
}

class PrefsWidget;
typedef QScopedPointer<PrefsWidget> PrefsWidgetPtr;

class PrefsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrefsWidget(QWidget *parent = 0);
    QString getUsername();
    QString getPassword();
    QString getServer();
    int getPort();
    QString getFrom();
    QString getOrg();
    bool ssl();
    bool deleteCacheOnExit();
    int allowedConnections();

    void setUsername(QString const& username);
    void setPassword(QString const& password);
    void setServer(QString const& server);
    void setPort(int const port);
    void setFrom(QString const& from);
    void setOrg(QString const& org);
    void setSsl(bool const ssl);
    void setDeleteCacheOnExit(bool const yes);
    void setAllowedConnections(int const con);

    QPushButton *getConnectButton();

    ~PrefsWidget();

public slots:
    void closeSlot();

private:
    Ui::PrefsWidget *ui;
};

#endif // PREFSWIDGET_H
