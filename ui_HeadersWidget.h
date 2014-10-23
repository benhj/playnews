/********************************************************************************
** Form generated from reading UI file 'HeadersWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HEADERSWIDGET_H
#define UI_HEADERSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <TableWidget.h>

QT_BEGIN_NAMESPACE

class Ui_HeadersWidget
{
public:
    QVBoxLayout *verticalLayout;
    TableWidget *headersTableWidget;
    QFrame *buttonFrame;
    QGridLayout *gridLayout_2;
    QProgressBar *progressBar;
    QPushButton *readButton;
    QLCDNumber *bytesDisplay;
    QPushButton *postButton;
    QPushButton *searchButton;
    QPushButton *showAllButton;
    QPushButton *bulkDLButton;

    void setupUi(QWidget *HeadersWidget)
    {
        if (HeadersWidget->objectName().isEmpty())
            HeadersWidget->setObjectName(QStringLiteral("HeadersWidget"));
        HeadersWidget->resize(606, 300);
        HeadersWidget->setStyleSheet(QStringLiteral("background:white"));
        verticalLayout = new QVBoxLayout(HeadersWidget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        headersTableWidget = new TableWidget(HeadersWidget);
        if (headersTableWidget->columnCount() < 3)
            headersTableWidget->setColumnCount(3);
        headersTableWidget->setObjectName(QStringLiteral("headersTableWidget"));
        headersTableWidget->setStyleSheet(QLatin1String("border:none;\n"
"selection-background-color: rgba(0, 0, 125, 100);"));
        headersTableWidget->setAlternatingRowColors(true);
        headersTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        headersTableWidget->setShowGrid(false);
        headersTableWidget->setGridStyle(Qt::DashLine);
        headersTableWidget->setRowCount(0);
        headersTableWidget->setColumnCount(3);
        headersTableWidget->horizontalHeader()->setVisible(false);
        headersTableWidget->horizontalHeader()->setMinimumSectionSize(4);
        headersTableWidget->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(headersTableWidget);

        buttonFrame = new QFrame(HeadersWidget);
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
        gridLayout_2 = new QGridLayout(buttonFrame);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        progressBar = new QProgressBar(buttonFrame);
        progressBar->setObjectName(QStringLiteral("progressBar"));
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

        gridLayout_2->addWidget(progressBar, 1, 7, 1, 1);

        readButton = new QPushButton(buttonFrame);
        readButton->setObjectName(QStringLiteral("readButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/1367895229_folder_open.png"), QSize(), QIcon::Normal, QIcon::Off);
        readButton->setIcon(icon);
        readButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(readButton, 1, 1, 1, 1);

        bytesDisplay = new QLCDNumber(buttonFrame);
        bytesDisplay->setObjectName(QStringLiteral("bytesDisplay"));
        bytesDisplay->setDigitCount(10);
        bytesDisplay->setSegmentStyle(QLCDNumber::Flat);

        gridLayout_2->addWidget(bytesDisplay, 1, 6, 1, 1);

        postButton = new QPushButton(buttonFrame);
        postButton->setObjectName(QStringLiteral("postButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/1367895375_doc_empty.png"), QSize(), QIcon::Normal, QIcon::Off);
        postButton->setIcon(icon1);
        postButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(postButton, 1, 3, 1, 1);

        searchButton = new QPushButton(buttonFrame);
        searchButton->setObjectName(QStringLiteral("searchButton"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/icons/1367893725_zoom.png"), QSize(), QIcon::Normal, QIcon::Off);
        searchButton->setIcon(icon2);
        searchButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(searchButton, 1, 4, 1, 1);

        showAllButton = new QPushButton(buttonFrame);
        showAllButton->setObjectName(QStringLiteral("showAllButton"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icons/icons/1367893874_playback_reload.png"), QSize(), QIcon::Normal, QIcon::Off);
        showAllButton->setIcon(icon3);
        showAllButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(showAllButton, 1, 5, 1, 1);

        bulkDLButton = new QPushButton(buttonFrame);
        bulkDLButton->setObjectName(QStringLiteral("bulkDLButton"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icons/icons/layers_1_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        bulkDLButton->setIcon(icon4);
        bulkDLButton->setIconSize(QSize(32, 32));

        gridLayout_2->addWidget(bulkDLButton, 1, 2, 1, 1);


        verticalLayout->addWidget(buttonFrame);


        retranslateUi(HeadersWidget);

        QMetaObject::connectSlotsByName(HeadersWidget);
    } // setupUi

    void retranslateUi(QWidget *HeadersWidget)
    {
        HeadersWidget->setWindowTitle(QApplication::translate("HeadersWidget", "Form", 0));
        readButton->setText(QString());
        postButton->setText(QString());
        searchButton->setText(QString());
        showAllButton->setText(QString());
        bulkDLButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class HeadersWidget: public Ui_HeadersWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HEADERSWIDGET_H
