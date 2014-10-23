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

#include "LoginDialog.h"
#include <QPainter>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{

    setUpGUI();
    setWindowTitle( tr("User Login") );
    setModal( true );
}

void LoginDialog::setUpGUI(){
    // set up the layout
    setAttribute( Qt::WA_TranslucentBackground, true );
    auto formGridLayout = new QGridLayout( this );
    editServer = new QLineEdit(this);
    editServer->setText("news.gmane.org:119");

    // initialize the username combo box so that it is editable
    editUsername = new QLineEdit( this );
    // initialize the password field so that it does not echo
    // characters
    editPassword = new QLineEdit( this );
    editPassword->setEchoMode( QLineEdit::Password );

    // initialize the labels
    labelServer = new QLabel(this);
    labelUsername = new QLabel( this );
    labelPassword = new QLabel( this );
    labelServer->setText(tr("Server"));
    labelServer->setBuddy( editServer );
    labelUsername->setText( tr( "Username" ) );
    labelUsername->setBuddy( editUsername );
    labelPassword->setText( tr( "Password" ) );
    labelPassword->setBuddy( editPassword );

    // initialize buttons
    buttons = new QDialogButtonBox( this );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Cancel );
    buttons->button( QDialogButtonBox::Ok )->setText( tr("Login") );
    buttons->button( QDialogButtonBox::Cancel )->setText( tr("Abort") );

    // connects slots
    connect( buttons->button( QDialogButtonBox::Cancel ),
             SIGNAL(clicked()),
             this,
             SLOT(close())
             );

    connect( buttons->button( QDialogButtonBox::Ok ),
             SIGNAL(clicked()),
             this,
             SLOT(slotAcceptLogin()) );

    // place components into the dialog
    formGridLayout->addWidget( labelServer, 0, 0);
    formGridLayout->addWidget( editServer, 0, 1 );
    formGridLayout->addWidget( labelUsername, 1, 0 );
    formGridLayout->addWidget( editUsername, 1, 1 );
    formGridLayout->addWidget( labelPassword, 2, 0 );
    formGridLayout->addWidget( editPassword, 2, 1 );
    formGridLayout->addWidget( buttons, 3, 0, 1, 2 );

    setLayout( formGridLayout );

    this->setObjectName(QString::fromUtf8("passDialog"));
    this->setStyleSheet(QString::fromUtf8("\n"
    "#passDialog {\n"
    "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
    "stop: 0 #a6a6a6, stop: 0.08 #7f7f7f,\n"
    "stop: 0.39999 #717171, stop: 0.4 #626262,\n"
    "stop: 0.9 #4c4c4c, stop: 1 #333333);\n"
    "border: 2px solid #555;\n"
    "border-radius: 11px;\n"
    "padding: 5px;\n"
    "}\n"
    "\n"
    "#passDialog QLineEdit {\n"
    "border: 2px solid #555;\n"
    "border-radius: 11px;\n"
    "padding: 5px;\n"
    "}\n"
    "\n"
    "#passDialog QComboBox {\n"
    "border: 2px solid #555;\n"
    "border-radius: 11px;\n"
    "padding: 5px;\n"
    "}\n"
    "\n"
    "#passDialog QPushButton {\n"
    "color: #333;\n"
    "border: 2px solid #555;\n"
    "border-radius: 11px;\n"
    "padding: 5px;\n"
    "background: qradialgradient(cx: 0.3, cy: -0.4,\n"
    "fx: 0.3, fy: -0.4,\n"
    "radius: 1.35, stop: 0 #fff, stop: 1 #888);\n"
    "min-width: 80px;\n"
    "}\n"
    "\n"
    "#passDialog QPushButton:hover {\n"
    "background: qradialgradient(cx: 0.3, cy: -0.4,\n"
    "fx: 0.3, fy: -0.4,\n"
    "radius: 1.35, stop: 0 #fff, stop: 1 #bbb);\n"
    "}\n"
    "\n"
    "#passDialog QPushButton:pressed {\n"
    "background: qradialgradient(cx: 0.4, cy: -0.1,\n"
    "fx: 0.4, fy: -0.1,\n"
    "radius: 1.35, stop: 0 #fff, stop: 1 #ddd);\n"
    "}\n"
    ""));

    int playbookWidthDiv2 = 1024 / 2;
    int playbookHeightDiv2 = 300;
    this->resize(400,200);
    QSize windowSize;
    windowSize = this->size();
    int width = windowSize.width();
    int height = windowSize.height();
    int x = (playbookWidthDiv2 - (width/2));
    int y = (playbookHeightDiv2 - (height / 2));
    this->move( x, y-150 );
    //setWindowFlags(Qt::CustomizeWindowHint);

}


void LoginDialog::setUsername(QString &username){
    editUsername->setText( username );
    // place the focus on the password field
    editPassword->setFocus();
}


void LoginDialog::setPassword(QString &password){
    editPassword->setText( password );
}

void LoginDialog::setServer(QString &server) {
    editServer->setText( server );

    // place the focus on the password field
    editUsername->setFocus();
}

void LoginDialog::slotAcceptLogin(){
    QString server   = editServer->text();
    QString username = editUsername->text();
    QString password = editPassword->text();

    emit acceptLogin( server,    // current server
                      username,  // current username
                      password   // current password
                      );

    // close this dialog
    close();
}

void
LoginDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter;
    painter.begin(this);

    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(200,200,200,240));
    gradient.setColorAt(1, QColor(40,40,40,200));
    QBrush brush( gradient);
    brush.setStyle(Qt::LinearGradientPattern);
    painter.setBrush( brush );		// set the yellow brush
    painter.setPen( Qt::NoPen );
    painter.drawRoundedRect(0,0,this->width(), this->height(), 10, 10);
    painter.end();
}
