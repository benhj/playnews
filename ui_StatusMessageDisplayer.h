/********************************************************************************
** Form generated from reading UI file 'StatusMessageDisplayer.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATUSMESSAGEDISPLAYER_H
#define UI_STATUSMESSAGEDISPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatusMessageDisplayer
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;

    void setupUi(QWidget *StatusMessageDisplayer)
    {
        if (StatusMessageDisplayer->objectName().isEmpty())
            StatusMessageDisplayer->setObjectName(QString::fromUtf8("StatusMessageDisplayer"));
        StatusMessageDisplayer->resize(1024, 102);
        StatusMessageDisplayer->setMinimumSize(QSize(1024, 102));
        StatusMessageDisplayer->setMaximumSize(QSize(1024, 102));
        StatusMessageDisplayer->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(StatusMessageDisplayer);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(80, -1, 80, -1);
        label = new QLabel(StatusMessageDisplayer);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("color: #333;\n"
"border: hidden;\n"
"border-radius: 11px;\n"
"padding: 5px;\n"
"background-color: rgba(200,200,200, 230);\n"
"min-width: 80px;"));
        label->setLineWidth(0);
        label->setWordWrap(true);

        horizontalLayout->addWidget(label);


        retranslateUi(StatusMessageDisplayer);

        QMetaObject::connectSlotsByName(StatusMessageDisplayer);
    } // setupUi

    void retranslateUi(QWidget *StatusMessageDisplayer)
    {
        StatusMessageDisplayer->setWindowTitle(QApplication::translate("StatusMessageDisplayer", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StatusMessageDisplayer", "Server status:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StatusMessageDisplayer: public Ui_StatusMessageDisplayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATUSMESSAGEDISPLAYER_H
