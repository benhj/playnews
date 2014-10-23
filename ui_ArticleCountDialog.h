/********************************************************************************
** Form generated from reading UI file 'ArticleCountDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARTICLECOUNTDIALOG_H
#define UI_ARTICLECOUNTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArticleCountDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *articleCountEdit;

    void setupUi(QDialog *ArticleCountDialog)
    {
        if (ArticleCountDialog->objectName().isEmpty())
            ArticleCountDialog->setObjectName(QStringLiteral("ArticleCountDialog"));
        ArticleCountDialog->resize(392, 87);
        ArticleCountDialog->setStyleSheet(QLatin1String("\n"
"#ArticleCountDialog {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"stop: 0 #a6a6a6, stop: 0.08 #7f7f7f,\n"
"stop: 0.39999 #717171, stop: 0.4 #626262,\n"
"stop: 0.9 #4c4c4c, stop: 1 #333333);\n"
"border: none\n"
"}\n"
"\n"
"#ArticleCountDialog QPushButton {\n"
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
"\n"
"#ArticleCountDialog QPushButton:pressed {\n"
"background: qradialgradient(cx: 0.4, cy: -0.1,\n"
"fx: 0.4, fy: -0.1,\n"
"radius: 1.35, stop: 0 #fff, stop: 1 #ddd);\n"
"}\n"
""));
        buttonBox = new QDialogButtonBox(ArticleCountDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(110, 50, 271, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalLayoutWidget = new QWidget(ArticleCountDialog);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(60, 10, 324, 24));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        articleCountEdit = new QLineEdit(horizontalLayoutWidget);
        articleCountEdit->setObjectName(QStringLiteral("articleCountEdit"));

        horizontalLayout->addWidget(articleCountEdit);


        retranslateUi(ArticleCountDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ArticleCountDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ArticleCountDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ArticleCountDialog);
    } // setupUi

    void retranslateUi(QDialog *ArticleCountDialog)
    {
        ArticleCountDialog->setWindowTitle(QApplication::translate("ArticleCountDialog", "Dialog", 0));
        label->setText(QApplication::translate("ArticleCountDialog", "Number of articles to load:", 0));
    } // retranslateUi

};

namespace Ui {
    class ArticleCountDialog: public Ui_ArticleCountDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARTICLECOUNTDIALOG_H
