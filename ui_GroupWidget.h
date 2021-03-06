/********************************************************************************
** Form generated from reading UI file 'GroupWidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPWIDGET_H
#define UI_GROUPWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ListWidget.h"

QT_BEGIN_NAMESPACE

class Ui_GroupWidget
{
public:
    QVBoxLayout *verticalLayout;
    ListWidget *groupsView;
    QFrame *buttonFrame;
    QGridLayout *gridLayout_2;
    QProgressBar *progressBar;
    QPushButton *groupLoadButton;
    QPushButton *connectButton;
    QPushButton *showAllButton;
    QPushButton *searchButton;
    QLCDNumber *byteDisplay;

    void setupUi(QWidget *GroupWidget)
    {
        if (GroupWidget->objectName().isEmpty())
            GroupWidget->setObjectName(QString::fromUtf8("GroupWidget"));
        GroupWidget->resize(606, 300);
        GroupWidget->setStyleSheet(QString::fromUtf8("background:white"));
        verticalLayout = new QVBoxLayout(GroupWidget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupsView = new ListWidget(GroupWidget);
        groupsView->setObjectName(QString::fromUtf8("groupsView"));
        groupsView->setStyleSheet(QString::fromUtf8("border: 0px solid white"));

        verticalLayout->addWidget(groupsView);

        buttonFrame = new QFrame(GroupWidget);
        buttonFrame->setObjectName(QString::fromUtf8("buttonFrame"));
        buttonFrame->setStyleSheet(QString::fromUtf8("\n"
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
"\n"
"\n"
""));
        buttonFrame->setFrameShape(QFrame::StyledPanel);
        buttonFrame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(buttonFrame);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        progressBar = new QProgressBar(buttonFrame);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy);
        progressBar->setMaximumSize(QSize(150, 16777215));
        progressBar->setMaximum(1);
        progressBar->setValue(-1);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        gridLayout_2->addWidget(progressBar, 0, 5, 1, 1);

        groupLoadButton = new QPushButton(buttonFrame);
        groupLoadButton->setObjectName(QString::fromUtf8("groupLoadButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/1367893718_arrow_bottom.png"), QSize(), QIcon::Normal, QIcon::Off);
        groupLoadButton->setIcon(icon);
        groupLoadButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(groupLoadButton, 0, 1, 1, 1);

        connectButton = new QPushButton(buttonFrame);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/1367893332_globe_3.png"), QSize(), QIcon::Normal, QIcon::Off);
        connectButton->setIcon(icon1);
        connectButton->setIconSize(QSize(48, 32));

        gridLayout_2->addWidget(connectButton, 0, 0, 1, 1);

        showAllButton = new QPushButton(buttonFrame);
        showAllButton->setObjectName(QString::fromUtf8("showAllButton"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/1367893874_playback_reload.png"), QSize(), QIcon::Normal, QIcon::Off);
        showAllButton->setIcon(icon2);
        showAllButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(showAllButton, 0, 3, 1, 1);

        searchButton = new QPushButton(buttonFrame);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/1367893725_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchButton->setIcon(icon3);
        searchButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(searchButton, 0, 2, 1, 1);

        byteDisplay = new QLCDNumber(buttonFrame);
        byteDisplay->setObjectName(QString::fromUtf8("byteDisplay"));
        byteDisplay->setNumDigits(10);
        byteDisplay->setDigitCount(10);
        byteDisplay->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_2->addWidget(byteDisplay, 0, 4, 1, 1);


        verticalLayout->addWidget(buttonFrame);


        retranslateUi(GroupWidget);

        QMetaObject::connectSlotsByName(GroupWidget);
    } // setupUi

    void retranslateUi(QWidget *GroupWidget)
    {
        GroupWidget->setWindowTitle(QApplication::translate("GroupWidget", "Form", 0, QApplication::UnicodeUTF8));
        groupLoadButton->setText(QString());
        connectButton->setText(QString());
        showAllButton->setText(QString());
        searchButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GroupWidget: public Ui_GroupWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPWIDGET_H
