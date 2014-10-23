/********************************************************************************
** Form generated from reading UI file 'MessageReader.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEREADER_H
#define UI_MESSAGEREADER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageReader
{
public:
    QGridLayout *gridLayout_2;
    QFrame *buttonFrame;
    QGridLayout *gridLayout;
    QPushButton *replyButton;
    QPushButton *closeButton;
    QTextEdit *messageEdit;

    void setupUi(QWidget *MessageReader)
    {
        if (MessageReader->objectName().isEmpty())
            MessageReader->setObjectName(QStringLiteral("MessageReader"));
        MessageReader->resize(400, 300);
        MessageReader->setStyleSheet(QStringLiteral("background: white"));
        gridLayout_2 = new QGridLayout(MessageReader);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        buttonFrame = new QFrame(MessageReader);
        buttonFrame->setObjectName(QStringLiteral("buttonFrame"));
        buttonFrame->setStyleSheet(QLatin1String("\n"
"#buttonFrame {\n"
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
        gridLayout = new QGridLayout(buttonFrame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        replyButton = new QPushButton(buttonFrame);
        replyButton->setObjectName(QStringLiteral("replyButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/round_and_up_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        replyButton->setIcon(icon);
        replyButton->setIconSize(QSize(32, 32));

        gridLayout->addWidget(replyButton, 0, 0, 1, 1);

        closeButton = new QPushButton(buttonFrame);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/delete_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon1);
        closeButton->setIconSize(QSize(32, 32));

        gridLayout->addWidget(closeButton, 0, 1, 1, 1);


        gridLayout_2->addWidget(buttonFrame, 1, 0, 1, 1);

        messageEdit = new QTextEdit(MessageReader);
        messageEdit->setObjectName(QStringLiteral("messageEdit"));
        messageEdit->setStyleSheet(QStringLiteral("border:none"));

        gridLayout_2->addWidget(messageEdit, 0, 0, 1, 1);


        retranslateUi(MessageReader);
        QObject::connect(closeButton, SIGNAL(clicked()), MessageReader, SLOT(close()));

        QMetaObject::connectSlotsByName(MessageReader);
    } // setupUi

    void retranslateUi(QWidget *MessageReader)
    {
        MessageReader->setWindowTitle(QApplication::translate("MessageReader", "Form", 0));
        replyButton->setText(QString());
        closeButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MessageReader: public Ui_MessageReader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEREADER_H
