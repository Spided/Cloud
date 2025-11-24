/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <downloadprogresstablewidget.h>
#include "customlistwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QStackedWidget *stackedWidget;
    QWidget *page_noteam;
    QPushButton *createTeam_btn;
    QLabel *Inteamlabel;
    QWidget *page_team;
    QGroupBox *groupBox;
    QListWidget *TeamListWidget;
    QGroupBox *groupBox_2;
    QPushButton *Add_btn;
    QPushButton *Changename_btn;
    QPushButton *Remove_btn;
    QPushButton *Dismiss_btn;
    QLabel *label_3;
    QLabel *TeamNameLabel;
    CustomListWidget *FileListWidget_team;
    QPushButton *returnBtn_team;
    QPushButton *refreshBtn_team;
    QPushButton *upload_team;
    QWidget *My_Data_page;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *UsernameLine;
    QLineEdit *EmailLine;
    QPushButton *upload_btn;
    QPushButton *changeDate_btn;
    QPushButton *Finish_btn;
    QLabel *png_label;
    QPushButton *changePassword_btn;
    QWidget *FileListPage;
    CustomListWidget *FileListWidget;
    QPushButton *returnBtn;
    QPushButton *upload;
    QPushButton *refreshBtn;
    QWidget *UploadPage;
    QTableWidget *UploadProcessWidget;
    QPushButton *stopallprocess_2;
    QPushButton *deleteallprocess_2;
    QPushButton *startallprocess_2;
    QWidget *ProgressPage;
    DownloadProgressTableWidget *ProgessWidget;
    QPushButton *stopallprocess;
    QPushButton *startallprocess;
    QPushButton *deleteallprocess;
    QWidget *TrashPage;
    CustomListWidget *TrashList;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *sideBar;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton_4;
    QToolButton *toolButton_5;
    QToolButton *toolButton_6;
    QToolButton *toolButton_7;
    QToolButton *toolButton_8;
    QLabel *UserNameLabel;
    QLabel *png_label_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(871, 671);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(110, 0, 761, 651));
        page_noteam = new QWidget();
        page_noteam->setObjectName("page_noteam");
        createTeam_btn = new QPushButton(page_noteam);
        createTeam_btn->setObjectName("createTeam_btn");
        createTeam_btn->setGeometry(QRect(310, 120, 91, 31));
        Inteamlabel = new QLabel(page_noteam);
        Inteamlabel->setObjectName("Inteamlabel");
        Inteamlabel->setGeometry(QRect(40, 110, 231, 41));
        stackedWidget->addWidget(page_noteam);
        page_team = new QWidget();
        page_team->setObjectName("page_team");
        groupBox = new QGroupBox(page_team);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(520, 40, 231, 271));
        TeamListWidget = new QListWidget(groupBox);
        TeamListWidget->setObjectName("TeamListWidget");
        TeamListWidget->setGeometry(QRect(10, 20, 211, 241));
        groupBox_2 = new QGroupBox(page_team);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(520, 310, 231, 211));
        Add_btn = new QPushButton(groupBox_2);
        Add_btn->setObjectName("Add_btn");
        Add_btn->setGeometry(QRect(13, 20, 210, 31));
        Changename_btn = new QPushButton(groupBox_2);
        Changename_btn->setObjectName("Changename_btn");
        Changename_btn->setGeometry(QRect(13, 70, 210, 31));
        Remove_btn = new QPushButton(groupBox_2);
        Remove_btn->setObjectName("Remove_btn");
        Remove_btn->setGeometry(QRect(13, 120, 210, 31));
        Dismiss_btn = new QPushButton(groupBox_2);
        Dismiss_btn->setObjectName("Dismiss_btn");
        Dismiss_btn->setGeometry(QRect(13, 170, 210, 31));
        label_3 = new QLabel(page_team);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(520, 10, 61, 16));
        TeamNameLabel = new QLabel(page_team);
        TeamNameLabel->setObjectName("TeamNameLabel");
        TeamNameLabel->setGeometry(QRect(580, 10, 141, 16));
        FileListWidget_team = new CustomListWidget(page_team);
        FileListWidget_team->setObjectName("FileListWidget_team");
        FileListWidget_team->setGeometry(QRect(20, 40, 491, 581));
        returnBtn_team = new QPushButton(page_team);
        returnBtn_team->setObjectName("returnBtn_team");
        returnBtn_team->setGeometry(QRect(20, 0, 31, 31));
        refreshBtn_team = new QPushButton(page_team);
        refreshBtn_team->setObjectName("refreshBtn_team");
        refreshBtn_team->setGeometry(QRect(60, 0, 31, 31));
        upload_team = new QPushButton(page_team);
        upload_team->setObjectName("upload_team");
        upload_team->setGeometry(QRect(100, 0, 91, 31));
        QFont font;
        font.setPointSize(12);
        upload_team->setFont(font);
        stackedWidget->addWidget(page_team);
        My_Data_page = new QWidget();
        My_Data_page->setObjectName("My_Data_page");
        label = new QLabel(My_Data_page);
        label->setObjectName("label");
        label->setGeometry(QRect(150, 220, 61, 41));
        label->setFont(font);
        label_2 = new QLabel(My_Data_page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(150, 270, 61, 41));
        label_2->setFont(font);
        UsernameLine = new QLineEdit(My_Data_page);
        UsernameLine->setObjectName("UsernameLine");
        UsernameLine->setEnabled(false);
        UsernameLine->setGeometry(QRect(240, 220, 241, 41));
        EmailLine = new QLineEdit(My_Data_page);
        EmailLine->setObjectName("EmailLine");
        EmailLine->setEnabled(false);
        EmailLine->setGeometry(QRect(240, 270, 241, 41));
        upload_btn = new QPushButton(My_Data_page);
        upload_btn->setObjectName("upload_btn");
        upload_btn->setGeometry(QRect(470, 150, 75, 24));
        changeDate_btn = new QPushButton(My_Data_page);
        changeDate_btn->setObjectName("changeDate_btn");
        changeDate_btn->setGeometry(QRect(200, 340, 91, 41));
        Finish_btn = new QPushButton(My_Data_page);
        Finish_btn->setObjectName("Finish_btn");
        Finish_btn->setEnabled(false);
        Finish_btn->setGeometry(QRect(380, 340, 91, 41));
        png_label = new QLabel(My_Data_page);
        png_label->setObjectName("png_label");
        png_label->setGeometry(QRect(270, 20, 151, 151));
        changePassword_btn = new QPushButton(My_Data_page);
        changePassword_btn->setObjectName("changePassword_btn");
        changePassword_btn->setGeometry(QRect(470, 180, 75, 24));
        stackedWidget->addWidget(My_Data_page);
        FileListPage = new QWidget();
        FileListPage->setObjectName("FileListPage");
        FileListWidget = new CustomListWidget(FileListPage);
        FileListWidget->setObjectName("FileListWidget");
        FileListWidget->setGeometry(QRect(10, 40, 750, 600));
        returnBtn = new QPushButton(FileListPage);
        returnBtn->setObjectName("returnBtn");
        returnBtn->setGeometry(QRect(10, 3, 31, 31));
        upload = new QPushButton(FileListPage);
        upload->setObjectName("upload");
        upload->setGeometry(QRect(90, 3, 91, 31));
        upload->setFont(font);
        refreshBtn = new QPushButton(FileListPage);
        refreshBtn->setObjectName("refreshBtn");
        refreshBtn->setGeometry(QRect(50, 3, 31, 31));
        stackedWidget->addWidget(FileListPage);
        UploadPage = new QWidget();
        UploadPage->setObjectName("UploadPage");
        UploadProcessWidget = new QTableWidget(UploadPage);
        UploadProcessWidget->setObjectName("UploadProcessWidget");
        UploadProcessWidget->setGeometry(QRect(10, 40, 750, 601));
        stopallprocess_2 = new QPushButton(UploadPage);
        stopallprocess_2->setObjectName("stopallprocess_2");
        stopallprocess_2->setGeometry(QRect(10, 10, 75, 24));
        deleteallprocess_2 = new QPushButton(UploadPage);
        deleteallprocess_2->setObjectName("deleteallprocess_2");
        deleteallprocess_2->setGeometry(QRect(190, 10, 75, 24));
        startallprocess_2 = new QPushButton(UploadPage);
        startallprocess_2->setObjectName("startallprocess_2");
        startallprocess_2->setGeometry(QRect(100, 10, 75, 24));
        stackedWidget->addWidget(UploadPage);
        ProgressPage = new QWidget();
        ProgressPage->setObjectName("ProgressPage");
        ProgessWidget = new DownloadProgressTableWidget(ProgressPage);
        ProgessWidget->setObjectName("ProgessWidget");
        ProgessWidget->setGeometry(QRect(10, 40, 750, 600));
        stopallprocess = new QPushButton(ProgressPage);
        stopallprocess->setObjectName("stopallprocess");
        stopallprocess->setGeometry(QRect(10, 10, 75, 24));
        startallprocess = new QPushButton(ProgressPage);
        startallprocess->setObjectName("startallprocess");
        startallprocess->setGeometry(QRect(90, 10, 75, 24));
        deleteallprocess = new QPushButton(ProgressPage);
        deleteallprocess->setObjectName("deleteallprocess");
        deleteallprocess->setGeometry(QRect(180, 10, 75, 24));
        stackedWidget->addWidget(ProgressPage);
        TrashPage = new QWidget();
        TrashPage->setObjectName("TrashPage");
        TrashList = new CustomListWidget(TrashPage);
        TrashList->setObjectName("TrashList");
        TrashList->setGeometry(QRect(10, 40, 750, 611));
        pushButton = new QPushButton(TrashPage);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(10, 10, 75, 24));
        pushButton_2 = new QPushButton(TrashPage);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(90, 10, 75, 24));
        stackedWidget->addWidget(TrashPage);
        sideBar = new QWidget(centralwidget);
        sideBar->setObjectName("sideBar");
        sideBar->setGeometry(QRect(0, 90, 61, 711));
        QFont font1;
        font1.setPointSize(8);
        sideBar->setFont(font1);
        toolButton = new QToolButton(sideBar);
        toolButton->setObjectName("toolButton");
        toolButton->setGeometry(QRect(0, 0, 60, 60));
        toolButton->setFont(font1);
        toolButton->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("user-home");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        toolButton->setIcon(icon);
        toolButton->setAutoExclusive(true);
        toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_2 = new QToolButton(sideBar);
        toolButton_2->setObjectName("toolButton_2");
        toolButton_2->setGeometry(QRect(0, 60, 60, 60));
        toolButton_2->setFont(font1);
        toolButton_2->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        toolButton_2->setIcon(icon);
        toolButton_2->setAutoExclusive(true);
        toolButton_2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_3 = new QToolButton(sideBar);
        toolButton_3->setObjectName("toolButton_3");
        toolButton_3->setGeometry(QRect(0, 120, 60, 60));
        toolButton_3->setFont(font1);
        toolButton_3->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        toolButton_3->setIcon(icon);
        toolButton_3->setAutoExclusive(true);
        toolButton_3->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_4 = new QToolButton(sideBar);
        toolButton_4->setObjectName("toolButton_4");
        toolButton_4->setGeometry(QRect(0, 180, 60, 60));
        toolButton_4->setFont(font1);
        toolButton_4->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        toolButton_4->setIcon(icon);
        toolButton_4->setAutoExclusive(true);
        toolButton_4->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_5 = new QToolButton(sideBar);
        toolButton_5->setObjectName("toolButton_5");
        toolButton_5->setGeometry(QRect(0, 240, 60, 60));
        toolButton_5->setFont(font1);
        toolButton_5->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        toolButton_5->setIcon(icon);
        toolButton_5->setAutoExclusive(true);
        toolButton_5->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_6 = new QToolButton(sideBar);
        toolButton_6->setObjectName("toolButton_6");
        toolButton_6->setGeometry(QRect(0, 300, 60, 60));
        toolButton_6->setFont(font1);
        toolButton_6->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        toolButton_6->setIcon(icon);
        toolButton_6->setAutoExclusive(true);
        toolButton_6->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_7 = new QToolButton(sideBar);
        toolButton_7->setObjectName("toolButton_7");
        toolButton_7->setGeometry(QRect(0, 360, 60, 60));
        toolButton_7->setFont(font1);
        toolButton_7->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        toolButton_7->setIcon(icon);
        toolButton_7->setAutoExclusive(true);
        toolButton_7->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolButton_8 = new QToolButton(sideBar);
        toolButton_8->setObjectName("toolButton_8");
        toolButton_8->setGeometry(QRect(0, 420, 60, 60));
        toolButton_8->setFont(font1);
        toolButton_8->setStyleSheet(QString::fromUtf8("/* \351\273\230\350\256\244 */\n"
"QToolButton{   \n"
"	border-top: 3px outset transparent;           /* \344\270\212\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\351\200\217\346\230\216 */\n"
"	border-bottom: 7px outset transparent;\n"
"	border-right: 3px outset transparent;\n"
"	border-left: 3px outset transparent;\n"
"\n"
"	background-color: rgb(228, 228, 228);\n"
"}\n"
"\n"
"/* \351\274\240\346\240\207\346\202\254\345\201\234 */\n"
"QToolButton:hover{\n"
"	background-color: rgb(205, 205, 205);\n"
"}\n"
"\n"
"/* \347\202\271\345\207\273\345\222\214\346\214\211\344\270\213 */\n"
"QToolButton:pressed,QToolButton:checked{\n"
"	border-left: 3px outset rgb(93, 95, 97);  /* \345\267\246\350\276\271\346\241\206\345\256\275\345\272\2463\345\203\217\347\264\240\343\200\201\347\252\201\345\207\272\346\230\276\347\244\272\343\200\201\351\242\234\350\211\262\344\270\272\346\267\261\346"
                        "\267\261\347\201\260 */\n"
"	background-color: rgb(246, 246, 246);	\n"
"}\n"
"\n"
"QPushButton:default {\n"
"    border-color: navy; /* make the default button prominent */\n"
"}"));
        toolButton_8->setIcon(icon);
        toolButton_8->setAutoExclusive(true);
        toolButton_8->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        UserNameLabel = new QLabel(centralwidget);
        UserNameLabel->setObjectName("UserNameLabel");
        UserNameLabel->setGeometry(QRect(0, 50, 91, 16));
        png_label_2 = new QLabel(centralwidget);
        png_label_2->setObjectName("png_label_2");
        png_label_2->setGeometry(QRect(0, 0, 51, 51));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 871, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        createTeam_btn->setText(QCoreApplication::translate("MainWindow", "\345\210\233\345\273\272\345\233\242\351\230\237", nullptr));
        Inteamlabel->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\346\262\241\346\234\211\345\212\240\345\205\245\345\233\242\351\230\237\357\274\214\347\202\271\345\207\273\345\210\233\345\273\272\350\207\252\345\267\261\347\232\204\345\233\242\351\230\237\357\274\201", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\345\233\242\351\230\237\346\210\220\345\221\230", nullptr));
        groupBox_2->setTitle(QString());
        Add_btn->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\346\210\220\345\221\230", nullptr));
        Changename_btn->setText(QCoreApplication::translate("MainWindow", "\345\233\242\351\230\237\351\207\215\345\221\275\345\220\215", nullptr));
        Remove_btn->setText(QCoreApplication::translate("MainWindow", "\347\247\273\351\231\244\346\211\200\351\200\211\346\210\220\345\221\230", nullptr));
        Dismiss_btn->setText(QCoreApplication::translate("MainWindow", "\350\247\243\346\225\243\345\233\242\351\230\237", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\233\242\351\230\237\345\220\215\347\247\260\357\274\232", nullptr));
        TeamNameLabel->setText(QCoreApplication::translate("MainWindow", "TeamName", nullptr));
        returnBtn_team->setText(QString());
        refreshBtn_team->setText(QString());
        upload_team->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\351\202\256\347\256\261\357\274\232", nullptr));
        upload_btn->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240\345\244\264\345\203\217", nullptr));
        changeDate_btn->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271\344\277\241\346\201\257", nullptr));
        Finish_btn->setText(QCoreApplication::translate("MainWindow", "\347\241\256\345\256\232", nullptr));
        png_label->setText(QString());
        changePassword_btn->setText(QCoreApplication::translate("MainWindow", "\344\277\256\346\224\271\345\257\206\347\240\201", nullptr));
        returnBtn->setText(QString());
        upload->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240", nullptr));
        refreshBtn->setText(QString());
        stopallprocess_2->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\346\232\202\345\201\234", nullptr));
        deleteallprocess_2->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\345\210\240\351\231\244", nullptr));
        startallprocess_2->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\345\274\200\345\247\213", nullptr));
        stopallprocess->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\346\232\202\345\201\234", nullptr));
        startallprocess->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\345\274\200\345\247\213", nullptr));
        deleteallprocess->setText(QCoreApplication::translate("MainWindow", "\345\205\250\351\203\250\345\210\240\351\231\244", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\350\277\224\345\233\236\345\210\227\350\241\250", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\233\236\346\224\266\347\253\231", nullptr));
        toolButton->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\345\210\227\350\241\250", nullptr));
        toolButton_2->setText(QCoreApplication::translate("MainWindow", "\344\270\212\344\274\240\347\233\256\345\275\225", nullptr));
        toolButton_3->setText(QCoreApplication::translate("MainWindow", "\344\270\213\350\275\275\347\233\256\345\275\225", nullptr));
        toolButton_4->setText(QCoreApplication::translate("MainWindow", "\345\233\236\346\224\266\347\253\231", nullptr));
        toolButton_5->setText(QCoreApplication::translate("MainWindow", "\345\205\261\344\272\253\351\223\276\346\216\245", nullptr));
        toolButton_6->setText(QCoreApplication::translate("MainWindow", "\351\207\215\346\226\260\347\231\273\345\275\225", nullptr));
        toolButton_7->setText(QCoreApplication::translate("MainWindow", "\345\233\242\351\230\237\345\215\217\344\275\234", nullptr));
        toolButton_8->setText(QCoreApplication::translate("MainWindow", "\346\210\221\347\232\204", nullptr));
        UserNameLabel->setText(QCoreApplication::translate("MainWindow", "UserName", nullptr));
        png_label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
