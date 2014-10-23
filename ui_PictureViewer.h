/********************************************************************************
** Form generated from reading UI file 'PictureViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PICTUREVIEWER_H
#define UI_PICTUREVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PictureViewer
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *pictureEdit;
    QFrame *buttonFrame;
    QGridLayout *gridLayout;
    QPushButton *saveButton;
    QPushButton *closeButton;

    void setupUi(QWidget *PictureViewer)
    {
        if (PictureViewer->objectName().isEmpty())
            PictureViewer->setObjectName(QStringLiteral("PictureViewer"));
        PictureViewer->resize(400, 300);
        PictureViewer->setStyleSheet(QStringLiteral("background:white"));
        verticalLayout = new QVBoxLayout(PictureViewer);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        pictureEdit = new QTextEdit(PictureViewer);
        pictureEdit->setObjectName(QStringLiteral("pictureEdit"));
        pictureEdit->setStyleSheet(QStringLiteral("border:none"));

        verticalLayout->addWidget(pictureEdit);

        buttonFrame = new QFrame(PictureViewer);
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
        saveButton = new QPushButton(buttonFrame);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/save_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveButton->setIcon(icon);
        saveButton->setIconSize(QSize(32, 32));

        gridLayout->addWidget(saveButton, 0, 0, 1, 1);

        closeButton = new QPushButton(buttonFrame);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/delete_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon1);
        closeButton->setIconSize(QSize(32, 32));

        gridLayout->addWidget(closeButton, 0, 1, 1, 1);


        verticalLayout->addWidget(buttonFrame);


        retranslateUi(PictureViewer);

        QMetaObject::connectSlotsByName(PictureViewer);
    } // setupUi

    void retranslateUi(QWidget *PictureViewer)
    {
        PictureViewer->setWindowTitle(QApplication::translate("PictureViewer", "Form", 0));
        saveButton->setText(QString());
        closeButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PictureViewer: public Ui_PictureViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PICTUREVIEWER_H
