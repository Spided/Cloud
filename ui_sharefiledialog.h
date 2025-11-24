/********************************************************************************
** Form generated from reading UI file 'sharefiledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREFILEDIALOG_H
#define UI_SHAREFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShareFileDialog
{
public:
    QLabel *label;
    QWidget *widget;
    QRadioButton *randomBtn;
    QRadioButton *selfBtn;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QWidget *widget_2;
    QRadioButton *encodeBtn;
    QRadioButton *unencodeBtn;
    QLabel *label_5;
    QLabel *label_6;
    QWidget *widget_3;
    QLineEdit *UrlLine;
    QLineEdit *ExtractLine;
    QPushButton *pushButton;
    QCheckBox *checkBox;
    QPushButton *pushButton_2;

    void setupUi(QDialog *ShareFileDialog)
    {
        if (ShareFileDialog->objectName().isEmpty())
            ShareFileDialog->setObjectName("ShareFileDialog");
        ShareFileDialog->resize(770, 554);
        label = new QLabel(ShareFileDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 30, 61, 31));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        widget = new QWidget(ShareFileDialog);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(130, 40, 451, 111));
        randomBtn = new QRadioButton(widget);
        randomBtn->setObjectName("randomBtn");
        randomBtn->setGeometry(QRect(70, 10, 191, 21));
        randomBtn->setFont(font);
        randomBtn->setChecked(true);
        selfBtn = new QRadioButton(widget);
        selfBtn->setObjectName("selfBtn");
        selfBtn->setGeometry(QRect(70, 50, 16, 20));
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setEnabled(false);
        lineEdit->setGeometry(QRect(90, 45, 141, 31));
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(240, 50, 151, 21));
        widget_2 = new QWidget(ShareFileDialog);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(130, 200, 451, 71));
        encodeBtn = new QRadioButton(widget_2);
        encodeBtn->setObjectName("encodeBtn");
        encodeBtn->setGeometry(QRect(70, 10, 191, 21));
        encodeBtn->setFont(font);
        encodeBtn->setChecked(true);
        unencodeBtn = new QRadioButton(widget_2);
        unencodeBtn->setObjectName("unencodeBtn");
        unencodeBtn->setGeometry(QRect(70, 40, 191, 21));
        unencodeBtn->setFont(font);
        label_5 = new QLabel(ShareFileDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(60, 190, 61, 31));
        label_5->setFont(font);
        label_6 = new QLabel(ShareFileDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(60, 320, 61, 31));
        label_6->setFont(font);
        widget_3 = new QWidget(ShareFileDialog);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(130, 330, 451, 121));
        UrlLine = new QLineEdit(widget_3);
        UrlLine->setObjectName("UrlLine");
        UrlLine->setEnabled(false);
        UrlLine->setGeometry(QRect(90, 10, 311, 41));
        ExtractLine = new QLineEdit(widget_3);
        ExtractLine->setObjectName("ExtractLine");
        ExtractLine->setEnabled(false);
        ExtractLine->setGeometry(QRect(90, 60, 311, 41));
        pushButton = new QPushButton(ShareFileDialog);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(260, 480, 111, 41));
        pushButton->setFont(font);
        checkBox = new QCheckBox(ShareFileDialog);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(200, 280, 171, 20));
        checkBox->setChecked(true);
        pushButton_2 = new QPushButton(ShareFileDialog);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(600, 360, 151, 41));
        pushButton_2->setFont(font);

        retranslateUi(ShareFileDialog);

        QMetaObject::connectSlotsByName(ShareFileDialog);
    } // setupUi

    void retranslateUi(QDialog *ShareFileDialog)
    {
        ShareFileDialog->setWindowTitle(QCoreApplication::translate("ShareFileDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("ShareFileDialog", "\346\217\220\345\217\226\347\240\201\357\274\232", nullptr));
        randomBtn->setText(QCoreApplication::translate("ShareFileDialog", "\347\263\273\347\273\237\351\232\217\346\234\272\347\224\237\346\210\220\346\217\220\345\217\226\347\240\201", nullptr));
        selfBtn->setText(QString());
        lineEdit->setText(QString());
        label_2->setText(QCoreApplication::translate("ShareFileDialog", "\344\273\205\346\224\257\346\214\201\346\225\260\345\255\227\345\217\212\350\213\261\346\226\207\345\255\227\346\257\215", nullptr));
        encodeBtn->setText(QCoreApplication::translate("ShareFileDialog", "\345\212\240\345\257\206", nullptr));
        unencodeBtn->setText(QCoreApplication::translate("ShareFileDialog", "\344\270\215\345\212\240\345\257\206", nullptr));
        label_5->setText(QCoreApplication::translate("ShareFileDialog", "\345\212\240\345\257\206\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("ShareFileDialog", "\351\223\276\346\216\245\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("ShareFileDialog", "\345\210\233\345\273\272\351\223\276\346\216\245", nullptr));
        checkBox->setText(QCoreApplication::translate("ShareFileDialog", "\345\210\206\344\272\253\351\223\276\346\216\245\350\207\252\345\212\250\345\241\253\345\205\205\346\217\220\345\217\226\347\240\201", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ShareFileDialog", "\345\244\215\345\210\266\351\223\276\346\216\245\345\217\212\346\217\220\345\217\226\347\240\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShareFileDialog: public Ui_ShareFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREFILEDIALOG_H
