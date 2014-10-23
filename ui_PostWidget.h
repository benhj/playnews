/********************************************************************************
** Form generated from reading UI file 'PostWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POSTWIDGET_H
#define UI_POSTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PostWidget
{
public:
    QGridLayout *gridLayout_2;
    QPlainTextEdit *postEdit;
    QFrame *buttonFrame;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *postButton;
    QPushButton *cancelButton;
    QFrame *entryFrame;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *fromEdit;
    QLineEdit *groupEdit;
    QLabel *label_2;
    QLineEdit *subjectEdit;
    QLabel *label_3;

    void setupUi(QWidget *PostWidget)
    {
        if (PostWidget->objectName().isEmpty())
            PostWidget->setObjectName(QStringLiteral("PostWidget"));
        PostWidget->resize(618, 424);
        PostWidget->setStyleSheet(QStringLiteral(""));
        gridLayout_2 = new QGridLayout(PostWidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        postEdit = new QPlainTextEdit(PostWidget);
        postEdit->setObjectName(QStringLiteral("postEdit"));
        postEdit->setStyleSheet(QStringLiteral("border: none"));

        gridLayout_2->addWidget(postEdit, 2, 0, 1, 1);

        buttonFrame = new QFrame(PostWidget);
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
        horizontalLayout_4 = new QHBoxLayout(buttonFrame);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        postButton = new QPushButton(buttonFrame);
        postButton->setObjectName(QStringLiteral("postButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/icons/export_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        postButton->setIcon(icon);
        postButton->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(postButton);

        cancelButton = new QPushButton(buttonFrame);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/icons/hand_contra_icon&48.png"), QSize(), QIcon::Normal, QIcon::Off);
        cancelButton->setIcon(icon1);
        cancelButton->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(cancelButton);


        gridLayout_2->addWidget(buttonFrame, 3, 0, 1, 1);

        entryFrame = new QFrame(PostWidget);
        entryFrame->setObjectName(QStringLiteral("entryFrame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(entryFrame->sizePolicy().hasHeightForWidth());
        entryFrame->setSizePolicy(sizePolicy);
        entryFrame->setStyleSheet(QLatin1String("\n"
"\n"
"#entryFrame {\n"
"background-image: url(:icons/backgrounds/291.jpg);\n"
"border: none;\n"
"padding: 0px;\n"
"}\n"
"\n"
"\n"
"#entryFrame QLabel {\n"
"color: white;\n"
"border: none;\n"
"min-width: 80px;\n"
"}\n"
"\n"
"#entryFrame QLineEdit {\n"
"color: #333;\n"
"border: 2px solid #555;\n"
"border-radius: 11px;\n"
"padding: 5px;\n"
"background: white;\n"
"min-width: 80px;\n"
"}"));
        entryFrame->setFrameShape(QFrame::StyledPanel);
        entryFrame->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(entryFrame);
        formLayout->setContentsMargins(0, 0, 0, 0);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        label = new QLabel(entryFrame);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(80, 32));
        label->setMaximumSize(QSize(102, 32));
        label->setBaseSize(QSize(102, 32));
        label->setAutoFillBackground(false);
        label->setStyleSheet(QStringLiteral(""));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label->setIndent(0);

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        fromEdit = new QLineEdit(entryFrame);
        fromEdit->setObjectName(QStringLiteral("fromEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(fromEdit->sizePolicy().hasHeightForWidth());
        fromEdit->setSizePolicy(sizePolicy1);
        fromEdit->setMinimumSize(QSize(94, 0));
        fromEdit->setMaximumSize(QSize(900, 16777215));
        fromEdit->setBaseSize(QSize(900, 0));

        formLayout->setWidget(1, QFormLayout::FieldRole, fromEdit);

        groupEdit = new QLineEdit(entryFrame);
        groupEdit->setObjectName(QStringLiteral("groupEdit"));
        sizePolicy1.setHeightForWidth(groupEdit->sizePolicy().hasHeightForWidth());
        groupEdit->setSizePolicy(sizePolicy1);
        groupEdit->setMinimumSize(QSize(94, 0));
        groupEdit->setBaseSize(QSize(900, 0));

        formLayout->setWidget(2, QFormLayout::FieldRole, groupEdit);

        label_2 = new QLabel(entryFrame);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(80, 32));
        label_2->setMaximumSize(QSize(102, 32));
        label_2->setBaseSize(QSize(102, 32));
        label_2->setStyleSheet(QStringLiteral(""));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2->setIndent(0);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        subjectEdit = new QLineEdit(entryFrame);
        subjectEdit->setObjectName(QStringLiteral("subjectEdit"));
        sizePolicy1.setHeightForWidth(subjectEdit->sizePolicy().hasHeightForWidth());
        subjectEdit->setSizePolicy(sizePolicy1);
        subjectEdit->setMinimumSize(QSize(94, 0));
        subjectEdit->setBaseSize(QSize(900, 0));

        formLayout->setWidget(3, QFormLayout::FieldRole, subjectEdit);

        label_3 = new QLabel(entryFrame);
        label_3->setObjectName(QStringLiteral("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(80, 32));
        label_3->setMaximumSize(QSize(102, 32));
        label_3->setBaseSize(QSize(102, 32));
        label_3->setStyleSheet(QStringLiteral(""));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3->setIndent(0);

        formLayout->setWidget(3, QFormLayout::LabelRole, label_3);


        gridLayout_2->addWidget(entryFrame, 1, 0, 1, 1);


        retranslateUi(PostWidget);
        QObject::connect(cancelButton, SIGNAL(clicked()), PostWidget, SLOT(close()));

        QMetaObject::connectSlotsByName(PostWidget);
    } // setupUi

    void retranslateUi(QWidget *PostWidget)
    {
        PostWidget->setWindowTitle(QApplication::translate("PostWidget", "Form", 0));
        postButton->setText(QString());
        cancelButton->setText(QString());
        label->setText(QApplication::translate("PostWidget", "From", 0));
        fromEdit->setText(QApplication::translate("PostWidget", "anon@anon.anon", 0));
        groupEdit->setText(QApplication::translate("PostWidget", "alt.test", 0));
        label_2->setText(QApplication::translate("PostWidget", "NewsGroups", 0));
        subjectEdit->setText(QApplication::translate("PostWidget", "test", 0));
        label_3->setText(QApplication::translate("PostWidget", "Subject", 0));
    } // retranslateUi

};

namespace Ui {
    class PostWidget: public Ui_PostWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POSTWIDGET_H
