/********************************************************************************
** Form generated from reading UI file 'addsharedfiledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDSHAREDFILEDIALOG_H
#define UI_ADDSHAREDFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_AddSharedFileDialog
{
public:
    QTreeWidget *folderTree;
    QPushButton *newfoldbtn;
    QPushButton *confirmbtn;
    QPushButton *cancelbtn;

    void setupUi(QDialog *AddSharedFileDialog)
    {
        if (AddSharedFileDialog->objectName().isEmpty())
            AddSharedFileDialog->setObjectName("AddSharedFileDialog");
        AddSharedFileDialog->resize(603, 716);
        folderTree = new QTreeWidget(AddSharedFileDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        folderTree->setHeaderItem(__qtreewidgetitem);
        folderTree->setObjectName("folderTree");
        folderTree->setGeometry(QRect(20, 10, 571, 651));
        newfoldbtn = new QPushButton(AddSharedFileDialog);
        newfoldbtn->setObjectName("newfoldbtn");
        newfoldbtn->setGeometry(QRect(80, 670, 91, 31));
        confirmbtn = new QPushButton(AddSharedFileDialog);
        confirmbtn->setObjectName("confirmbtn");
        confirmbtn->setGeometry(QRect(450, 670, 91, 31));
        cancelbtn = new QPushButton(AddSharedFileDialog);
        cancelbtn->setObjectName("cancelbtn");
        cancelbtn->setGeometry(QRect(300, 670, 91, 31));

        retranslateUi(AddSharedFileDialog);

        QMetaObject::connectSlotsByName(AddSharedFileDialog);
    } // setupUi

    void retranslateUi(QDialog *AddSharedFileDialog)
    {
        AddSharedFileDialog->setWindowTitle(QCoreApplication::translate("AddSharedFileDialog", "Dialog", nullptr));
        newfoldbtn->setText(QCoreApplication::translate("AddSharedFileDialog", "\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        confirmbtn->setText(QCoreApplication::translate("AddSharedFileDialog", "\347\241\256\345\256\232", nullptr));
        cancelbtn->setText(QCoreApplication::translate("AddSharedFileDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddSharedFileDialog: public Ui_AddSharedFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDSHAREDFILEDIALOG_H
