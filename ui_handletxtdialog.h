/********************************************************************************
** Form generated from reading UI file 'handletxtdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANDLETXTDIALOG_H
#define UI_HANDLETXTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_HandleTxtDialog
{
public:

    void setupUi(QDialog *HandleTxtDialog)
    {
        if (HandleTxtDialog->objectName().isEmpty())
            HandleTxtDialog->setObjectName("HandleTxtDialog");
        HandleTxtDialog->resize(400, 300);

        retranslateUi(HandleTxtDialog);

        QMetaObject::connectSlotsByName(HandleTxtDialog);
    } // setupUi

    void retranslateUi(QDialog *HandleTxtDialog)
    {
        HandleTxtDialog->setWindowTitle(QCoreApplication::translate("HandleTxtDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HandleTxtDialog: public Ui_HandleTxtDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANDLETXTDIALOG_H
