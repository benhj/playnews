/********************************************************************************
** Form generated from reading UI file 'PrefsWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFSWIDGET_H
#define UI_PREFSWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrefsWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_7;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QWidget *widget_6;
    QFormLayout *formLayout;
    QCheckBox *deleteOnExitCheck;
    QLineEdit *emailEdit;
    QLabel *label_13;
    QWidget *widget_3;
    QFormLayout *formLayout_3;
    QLineEdit *serverEdit;
    QWidget *widget_5;
    QLabel *label_9;
    QLineEdit *portEdit;
    QLabel *label_10;
    QLineEdit *usernameEdit;
    QLabel *label_11;
    QLineEdit *passwordEdit;
    QLabel *label_12;
    QLineEdit *connectionCountEdit;
    QLabel *label_3;
    QCheckBox *sslCheck;
    QFrame *buttonFrame;
    QGridLayout *gridLayout_2;
    QPushButton *connectButton;
    QPushButton *cancelbutton;

    void setupUi(QWidget *PrefsWidget)
    {
        if (PrefsWidget->objectName().isEmpty())
            PrefsWidget->setObjectName(QString::fromUtf8("PrefsWidget"));
        PrefsWidget->resize(756, 414);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PrefsWidget->sizePolicy().hasHeightForWidth());
        PrefsWidget->setSizePolicy(sizePolicy);
        PrefsWidget->setLayoutDirection(Qt::RightToLeft);
        PrefsWidget->setStyleSheet(QString::fromUtf8("#PrefsWidget {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"stop: 0 #a6a6a6, stop: 0.08 #7f7f7f,\n"
"stop: 0.39999 #717171, stop: 0.4 #626262,\n"
"stop: 0.9 #4c4c4c, stop: 1 #333333);\n"
"border: none\n"
"}\n"
"\n"
"#PrefsWidget QLineEdit {\n"
"color: #333;\n"
"border: 2px solid #555;\n"
"border-radius: 11px;\n"
"padding: 5px;\n"
"background: white;\n"
"min-width: 80px;\n"
"}"));
        verticalLayout = new QVBoxLayout(PrefsWidget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        widget = new QWidget(PrefsWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(50, 30));
        widget->setStyleSheet(QString::fromUtf8("background-image: url(:icons/backgrounds/291.jpg);\n"
"\n"
""));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        label_7->setFont(font);
        label_7->setStyleSheet(QString::fromUtf8("color: white;"));

        horizontalLayout_2->addWidget(label_7);


        verticalLayout->addWidget(widget, 0, Qt::AlignTop);

        widget_2 = new QWidget(PrefsWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy2);
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        widget_6 = new QWidget(widget_2);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_6->sizePolicy().hasHeightForWidth());
        widget_6->setSizePolicy(sizePolicy3);
        formLayout = new QFormLayout(widget_6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        deleteOnExitCheck = new QCheckBox(widget_6);
        deleteOnExitCheck->setObjectName(QString::fromUtf8("deleteOnExitCheck"));

        formLayout->setWidget(2, QFormLayout::LabelRole, deleteOnExitCheck);

        emailEdit = new QLineEdit(widget_6);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(emailEdit->sizePolicy().hasHeightForWidth());
        emailEdit->setSizePolicy(sizePolicy4);
        emailEdit->setMinimumSize(QSize(94, 0));
        emailEdit->setMaximumSize(QSize(400, 16777215));
        emailEdit->setBaseSize(QSize(200, 0));
        emailEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::LabelRole, emailEdit);

        label_13 = new QLabel(widget_6);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label_13);


        horizontalLayout->addWidget(widget_6);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        sizePolicy3.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy3);
        widget_3->setLayoutDirection(Qt::LeftToRight);
        formLayout_3 = new QFormLayout(widget_3);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        serverEdit = new QLineEdit(widget_3);
        serverEdit->setObjectName(QString::fromUtf8("serverEdit"));
        sizePolicy4.setHeightForWidth(serverEdit->sizePolicy().hasHeightForWidth());
        serverEdit->setSizePolicy(sizePolicy4);
        serverEdit->setMinimumSize(QSize(94, 0));
        serverEdit->setBaseSize(QSize(200, 0));
        serverEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, serverEdit);

        widget_5 = new QWidget(widget_3);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, widget_5);

        label_9 = new QLabel(widget_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy5);

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_9);

        portEdit = new QLineEdit(widget_3);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));
        sizePolicy4.setHeightForWidth(portEdit->sizePolicy().hasHeightForWidth());
        portEdit->setSizePolicy(sizePolicy4);
        portEdit->setMinimumSize(QSize(94, 0));
        portEdit->setBaseSize(QSize(200, 0));
        portEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, portEdit);

        label_10 = new QLabel(widget_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        sizePolicy5.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy5);

        formLayout_3->setWidget(3, QFormLayout::LabelRole, label_10);

        usernameEdit = new QLineEdit(widget_3);
        usernameEdit->setObjectName(QString::fromUtf8("usernameEdit"));
        sizePolicy4.setHeightForWidth(usernameEdit->sizePolicy().hasHeightForWidth());
        usernameEdit->setSizePolicy(sizePolicy4);
        usernameEdit->setMinimumSize(QSize(94, 0));
        usernameEdit->setBaseSize(QSize(200, 0));
        usernameEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, usernameEdit);

        label_11 = new QLabel(widget_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        sizePolicy5.setHeightForWidth(label_11->sizePolicy().hasHeightForWidth());
        label_11->setSizePolicy(sizePolicy5);

        formLayout_3->setWidget(4, QFormLayout::LabelRole, label_11);

        passwordEdit = new QLineEdit(widget_3);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        sizePolicy4.setHeightForWidth(passwordEdit->sizePolicy().hasHeightForWidth());
        passwordEdit->setSizePolicy(sizePolicy4);
        passwordEdit->setMinimumSize(QSize(94, 0));
        passwordEdit->setBaseSize(QSize(200, 0));
        passwordEdit->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoAutoUppercase|Qt::ImhNoPredictiveText);
        passwordEdit->setEchoMode(QLineEdit::Password);
        passwordEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(4, QFormLayout::FieldRole, passwordEdit);

        label_12 = new QLabel(widget_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        sizePolicy5.setHeightForWidth(label_12->sizePolicy().hasHeightForWidth());
        label_12->setSizePolicy(sizePolicy5);
        label_12->setMinimumSize(QSize(0, 0));

        formLayout_3->setWidget(5, QFormLayout::LabelRole, label_12);

        connectionCountEdit = new QLineEdit(widget_3);
        connectionCountEdit->setObjectName(QString::fromUtf8("connectionCountEdit"));
        sizePolicy4.setHeightForWidth(connectionCountEdit->sizePolicy().hasHeightForWidth());
        connectionCountEdit->setSizePolicy(sizePolicy4);
        connectionCountEdit->setMinimumSize(QSize(94, 0));
        connectionCountEdit->setBaseSize(QSize(200, 0));
        connectionCountEdit->setLayoutDirection(Qt::RightToLeft);
        connectionCountEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(5, QFormLayout::FieldRole, connectionCountEdit);

        label_3 = new QLabel(widget_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy5.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy5);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_3);

        sslCheck = new QCheckBox(widget_3);
        sslCheck->setObjectName(QString::fromUtf8("sslCheck"));
        sslCheck->setLayoutDirection(Qt::RightToLeft);
        sslCheck->setAutoFillBackground(false);
        sslCheck->setChecked(false);

        formLayout_3->setWidget(6, QFormLayout::FieldRole, sslCheck);


        horizontalLayout->addWidget(widget_3);


        verticalLayout->addWidget(widget_2);

        buttonFrame = new QFrame(PrefsWidget);
        buttonFrame->setObjectName(QString::fromUtf8("buttonFrame"));
        QSizePolicy sizePolicy6(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(buttonFrame->sizePolicy().hasHeightForWidth());
        buttonFrame->setSizePolicy(sizePolicy6);
        buttonFrame->setMinimumSize(QSize(0, 38));
        buttonFrame->setStyleSheet(QString::fromUtf8("#buttonFrame {\n"
"background-image: url(:icons/backgrounds/291.jpg);\n"
"border: none;\n"
"padding: 0px;\n"
"}\n"
"\n"
"#buttonFrame QPushButton {\n"
"color: #333;\n"
"border: none;\n"
"border-radius: 0px;\n"
"padding: 0px;\n"
"background: none;\n"
"}\n"
"\n"
"\n"
"#buttonFrame QProgressBar {\n"
"color: #222;\n"
"border: 2px solid #555;\n"
"border-radius: 5px;\n"
"padding: 5px;\n"
"background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"fx: 0.3, fy: -0.4,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #433);\n"
"min-width: 80px;\n"
"}\n"
"\n"
"#buttonFrame QLCDNumber {\n"
"color: #222;\n"
"border: 2px solid #555;\n"
"border-radius: 5px;\n"
"padding: 5px;\n"
"background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"fx: 0.3, fy: -0.4,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #433);\n"
"min-width: 80px;\n"
"}\n"
"\n"
"\n"
""));
        buttonFrame->setFrameShape(QFrame::StyledPanel);
        buttonFrame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(buttonFrame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(-1);
        gridLayout_2->setContentsMargins(0, 13, 0, 15);
        connectButton = new QPushButton(buttonFrame);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/hand_pro_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        connectButton->setIcon(icon);
        connectButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(connectButton, 0, 1, 1, 1);

        cancelbutton = new QPushButton(buttonFrame);
        cancelbutton->setObjectName(QString::fromUtf8("cancelbutton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/delete_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        cancelbutton->setIcon(icon1);
        cancelbutton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(cancelbutton, 0, 0, 1, 1);


        verticalLayout->addWidget(buttonFrame, 0, Qt::AlignBottom);


        retranslateUi(PrefsWidget);

        QMetaObject::connectSlotsByName(PrefsWidget);
    } // setupUi

    void retranslateUi(QWidget *PrefsWidget)
    {
        PrefsWidget->setWindowTitle(QApplication::translate("PrefsWidget", "Form", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PrefsWidget", "Setup", 0, QApplication::UnicodeUTF8));
        deleteOnExitCheck->setText(QApplication::translate("PrefsWidget", "Delete cache on exit", 0, QApplication::UnicodeUTF8));
        emailEdit->setText(QApplication::translate("PrefsWidget", "anon@anon.anon", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("PrefsWidget", "Email from", 0, QApplication::UnicodeUTF8));
        serverEdit->setText(QApplication::translate("PrefsWidget", "news.virginmedia.com", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("PrefsWidget", "Port", 0, QApplication::UnicodeUTF8));
        portEdit->setText(QApplication::translate("PrefsWidget", "119", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("PrefsWidget", "Username", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("PrefsWidget", "Password", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("PrefsWidget", "#connections", 0, QApplication::UnicodeUTF8));
        connectionCountEdit->setText(QApplication::translate("PrefsWidget", "10", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PrefsWidget", "Server", 0, QApplication::UnicodeUTF8));
        sslCheck->setText(QApplication::translate("PrefsWidget", "SSL", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QString());
        cancelbutton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PrefsWidget: public Ui_PrefsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFSWIDGET_H
