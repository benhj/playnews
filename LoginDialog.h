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

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>
#include <QScopedPointer>

class LoginDialog;
typedef QScopedPointer<LoginDialog> LoginDialogPtr;


class LoginDialog : public QDialog
{
    Q_OBJECT

private:
    /*!
      * A label for the username component.
      */
    QLabel* labelUsername;


    /*!
      * A label for the password.
      */
    QLabel* labelPassword;

    QLabel* labelServer;

    QLineEdit* editServer;

    /*!
      * An editable combo box for allowing the user
      * to enter his username or select it from a list.
      */
    QLineEdit* editUsername;


    /*!
      * A field to let the user enters his password.
      */
    QLineEdit* editPassword;

    /*!
      * The standard dialog button box.
      */
    QDialogButtonBox* buttons;

    /*!
      * A method to set up all dialog components and
      * initialize them.
      */
    void setUpGUI();

public:
    explicit LoginDialog(QWidget *parent = 0);

    void setServer(QString &server);

    /*!
      * Sets the proposed username, that can come for instance
      * from a shared setting.
      *\param username the string that represents the current username
      * to display
      */
    void setUsername( QString& username );

    /*!
      * Sets the current password to propose to the user for the login.
      * \param password the password to fill into the dialog form
      */
    void setPassword( QString& password );


signals:

    /*!
      * A signal emitted when the login is performed.
      * \param username the username entered in the dialog
      * \param password the password entered in the dialog
      */
    void acceptLogin( QString server, QString username, QString password );

public slots:
    /*!
      * A lot to adjust the emitting of the signal.
      */
    void slotAcceptLogin();

protected:
    void paintEvent(QPaintEvent *e);

};
