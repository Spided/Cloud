/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *label_3;
    QPushButton *Registerbtn;
    QLabel *label;
    QPushButton *Loginbtn;
    QLineEdit *Password;
    QLineEdit *EmailLine;
    QLabel *label_2;
    QWidget *page_2;
    QLineEdit *Password_Conf;
    QPushButton *returnbtn;
    QLineEdit *UsernameLine_2;
    QLabel *label_7;
    QLineEdit *Password_2;
    QPushButton *Finish;
    QLabel *label_6;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_8;
    QLineEdit *Email;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName("LoginDialog");
        LoginDialog->resize(550, 399);
        stackedWidget = new QStackedWidget(LoginDialog);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(10, 0, 531, 391));
        page = new QWidget();
        page->setObjectName("page");
        label_3 = new QLabel(page);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(210, 20, 111, 41));
        QFont font;
        font.setPointSize(20);
        label_3->setFont(font);
        Registerbtn = new QPushButton(page);
        Registerbtn->setObjectName("Registerbtn");
        Registerbtn->setGeometry(QRect(270, 260, 121, 41));
        Registerbtn->setFont(font);
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 90, 101, 41));
        label->setFont(font);
        Loginbtn = new QPushButton(page);
        Loginbtn->setObjectName("Loginbtn");
        Loginbtn->setGeometry(QRect(90, 260, 121, 41));
        Loginbtn->setFont(font);
        Password = new QLineEdit(page);
        Password->setObjectName("Password");
        Password->setGeometry(QRect(190, 170, 261, 51));
        EmailLine = new QLineEdit(page);
        EmailLine->setObjectName("EmailLine");
        EmailLine->setGeometry(QRect(190, 90, 261, 51));
        label_2 = new QLabel(page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 170, 121, 41));
        label_2->setFont(font);
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        Password_Conf = new QLineEdit(page_2);
        Password_Conf->setObjectName("Password_Conf");
        Password_Conf->setGeometry(QRect(190, 250, 261, 51));
        returnbtn = new QPushButton(page_2);
        returnbtn->setObjectName("returnbtn");
        returnbtn->setGeometry(QRect(280, 330, 121, 41));
        returnbtn->setFont(font);
        UsernameLine_2 = new QLineEdit(page_2);
        UsernameLine_2->setObjectName("UsernameLine_2");
        UsernameLine_2->setGeometry(QRect(190, 70, 261, 51));
        label_7 = new QLabel(page_2);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(70, 250, 121, 41));
        label_7->setFont(font);
        Password_2 = new QLineEdit(page_2);
        Password_2->setObjectName("Password_2");
        Password_2->setGeometry(QRect(190, 190, 261, 51));
        Finish = new QPushButton(page_2);
        Finish->setObjectName("Finish");
        Finish->setGeometry(QRect(100, 330, 121, 41));
        Finish->setFont(font);
        label_6 = new QLabel(page_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(70, 190, 121, 41));
        label_6->setFont(font);
        label_4 = new QLabel(page_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(70, 70, 101, 41));
        label_4->setFont(font);
        label_5 = new QLabel(page_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(180, 20, 151, 41));
        label_5->setFont(font);
        label_8 = new QLabel(page_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(70, 130, 121, 41));
        label_8->setFont(font);
        Email = new QLineEdit(page_2);
        Email->setObjectName("Email");
        Email->setGeometry(QRect(190, 130, 261, 51));
        stackedWidget->addWidget(page_2);

        retranslateUi(LoginDialog);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QCoreApplication::translate("LoginDialog", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("LoginDialog", "\350\275\273\345\215\217\344\272\221\345\202\250", nullptr));
        Registerbtn->setText(QCoreApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        label->setText(QCoreApplication::translate("LoginDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        Loginbtn->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        Password->setText(QCoreApplication::translate("LoginDialog", "123", nullptr));
        EmailLine->setText(QCoreApplication::translate("LoginDialog", "3292160288@qq.com", nullptr));
        label_2->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225\345\257\206\347\240\201\357\274\232", nullptr));
        Password_Conf->setText(QString());
        returnbtn->setText(QCoreApplication::translate("LoginDialog", "\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
        label_7->setText(QCoreApplication::translate("LoginDialog", "\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        Password_2->setText(QString());
        Finish->setText(QCoreApplication::translate("LoginDialog", "\347\241\256\350\256\244", nullptr));
        label_6->setText(QCoreApplication::translate("LoginDialog", "\347\231\273\345\275\225\345\257\206\347\240\201\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("LoginDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("LoginDialog", "\344\272\221\345\255\230\345\202\250\350\275\257\344\273\266", nullptr));
        label_8->setText(QCoreApplication::translate("LoginDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        Email->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
