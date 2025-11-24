/********************************************************************************
** Form generated from reading UI file 'handlepdfworddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANDLEPDFWORDDIALOG_H
#define UI_HANDLEPDFWORDDIALOG_H

#include <QtAxContainer/QAxWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_HandlePdfWordDialog
{
public:
    QAxWidget *axWidget;

    void setupUi(QDialog *HandlePdfWordDialog)
    {
        if (HandlePdfWordDialog->objectName().isEmpty())
            HandlePdfWordDialog->setObjectName("HandlePdfWordDialog");
        HandlePdfWordDialog->resize(765, 801);
        axWidget = new QAxWidget(HandlePdfWordDialog);
        axWidget->setObjectName("axWidget");
        axWidget->setProperty("geometry", QVariant(QRect(70, 10, 541, 741)));

        retranslateUi(HandlePdfWordDialog);

        QMetaObject::connectSlotsByName(HandlePdfWordDialog);
    } // setupUi

    void retranslateUi(QDialog *HandlePdfWordDialog)
    {
        HandlePdfWordDialog->setWindowTitle(QCoreApplication::translate("HandlePdfWordDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HandlePdfWordDialog: public Ui_HandlePdfWordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANDLEPDFWORDDIALOG_H
