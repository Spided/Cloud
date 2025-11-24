/********************************************************************************
** Form generated from reading UI file 'handleimagedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANDLEIMAGEDIALOG_H
#define UI_HANDLEIMAGEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_HandleImageDialog
{
public:

    void setupUi(QDialog *HandleImageDialog)
    {
        if (HandleImageDialog->objectName().isEmpty())
            HandleImageDialog->setObjectName("HandleImageDialog");
        HandleImageDialog->resize(400, 300);

        retranslateUi(HandleImageDialog);

        QMetaObject::connectSlotsByName(HandleImageDialog);
    } // setupUi

    void retranslateUi(QDialog *HandleImageDialog)
    {
        HandleImageDialog->setWindowTitle(QCoreApplication::translate("HandleImageDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HandleImageDialog: public Ui_HandleImageDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANDLEIMAGEDIALOG_H
