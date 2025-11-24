/********************************************************************************
** Form generated from reading UI file 'handlevideodialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANDLEVIDEODIALOG_H
#define UI_HANDLEVIDEODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_HandleVideoDialog
{
public:

    void setupUi(QDialog *HandleVideoDialog)
    {
        if (HandleVideoDialog->objectName().isEmpty())
            HandleVideoDialog->setObjectName("HandleVideoDialog");
        HandleVideoDialog->resize(400, 300);

        retranslateUi(HandleVideoDialog);

        QMetaObject::connectSlotsByName(HandleVideoDialog);
    } // setupUi

    void retranslateUi(QDialog *HandleVideoDialog)
    {
        HandleVideoDialog->setWindowTitle(QCoreApplication::translate("HandleVideoDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HandleVideoDialog: public Ui_HandleVideoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANDLEVIDEODIALOG_H
