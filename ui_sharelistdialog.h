/********************************************************************************
** Form generated from reading UI file 'sharelistdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHARELISTDIALOG_H
#define UI_SHARELISTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>
#include <downloadprogresstablewidget.h>
#include "customlistwidget.h"

QT_BEGIN_NAMESPACE

class Ui_ShareListDialog
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLineEdit *UrlLine;
    QLabel *label;
    QLineEdit *codeLine;
    QLabel *label_2;
    QPushButton *pushButton;
    QWidget *page_2;
    CustomListWidget *FileListWidget;
    QPushButton *lastBtn;
    QWidget *page_3;
    DownloadProgressTableWidget *ProgessWidget;
    QPushButton *pushButton_2;

    void setupUi(QDialog *ShareListDialog)
    {
        if (ShareListDialog->objectName().isEmpty())
            ShareListDialog->setObjectName("ShareListDialog");
        ShareListDialog->resize(802, 658);
        stackedWidget = new QStackedWidget(ShareListDialog);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(20, 10, 761, 671));
        page = new QWidget();
        page->setObjectName("page");
        UrlLine = new QLineEdit(page);
        UrlLine->setObjectName("UrlLine");
        UrlLine->setGeometry(QRect(230, 130, 391, 41));
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(140, 130, 71, 31));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        codeLine = new QLineEdit(page);
        codeLine->setObjectName("codeLine");
        codeLine->setGeometry(QRect(230, 220, 221, 41));
        label_2 = new QLabel(page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(140, 220, 71, 31));
        label_2->setFont(font);
        pushButton = new QPushButton(page);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(230, 340, 151, 41));
        pushButton->setFont(font);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        FileListWidget = new CustomListWidget(page_2);
        FileListWidget->setObjectName("FileListWidget");
        FileListWidget->setGeometry(QRect(10, 30, 750, 600));
        lastBtn = new QPushButton(page_2);
        lastBtn->setObjectName("lastBtn");
        lastBtn->setGeometry(QRect(10, 0, 31, 31));
        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        ProgessWidget = new DownloadProgressTableWidget(page_3);
        ProgessWidget->setObjectName("ProgessWidget");
        ProgessWidget->setGeometry(QRect(10, 30, 750, 600));
        pushButton_2 = new QPushButton(page_3);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(10, 0, 75, 24));
        stackedWidget->addWidget(page_3);

        retranslateUi(ShareListDialog);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ShareListDialog);
    } // setupUi

    void retranslateUi(QDialog *ShareListDialog)
    {
        ShareListDialog->setWindowTitle(QCoreApplication::translate("ShareListDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ShareListDialog", "\345\210\206\344\272\253\351\223\276\346\216\245\357\274\232", nullptr));
        codeLine->setText(QString());
        label_2->setText(QCoreApplication::translate("ShareListDialog", "\346\217\220\345\217\226\347\240\201\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("ShareListDialog", "\347\241\256\345\256\232", nullptr));
        lastBtn->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("ShareListDialog", "\350\277\224\345\233\236\345\210\227\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShareListDialog: public Ui_ShareListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHARELISTDIALOG_H
