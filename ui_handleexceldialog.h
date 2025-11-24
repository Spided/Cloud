/********************************************************************************
** Form generated from reading UI file 'handleexceldialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANDLEEXCELDIALOG_H
#define UI_HANDLEEXCELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_HandleExcelDialog
{
public:

    void setupUi(QDialog *HandleExcelDialog)
    {
        if (HandleExcelDialog->objectName().isEmpty())
            HandleExcelDialog->setObjectName("HandleExcelDialog");
        HandleExcelDialog->resize(400, 300);

        retranslateUi(HandleExcelDialog);

        QMetaObject::connectSlotsByName(HandleExcelDialog);
    } // setupUi

    void retranslateUi(QDialog *HandleExcelDialog)
    {
        HandleExcelDialog->setWindowTitle(QCoreApplication::translate("HandleExcelDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HandleExcelDialog: public Ui_HandleExcelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANDLEEXCELDIALOG_H
