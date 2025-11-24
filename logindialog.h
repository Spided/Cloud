#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include"globaldata.h"
#include <QDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QListWidget>
#include<QJsonArray>
#include<QStack>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
namespace Ui {
class LoginDialog;
}
struct TeamData{
    int TeamId=-1;
    int RootId=-1;
    QString name="";
    int role=-1;//owner=0,member=1
};
class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_Finish_clicked();

    void on_returnbtn_clicked();

    void on_Registerbtn_clicked();

    void on_Loginbtn_clicked();
    void queryTeamInfo(int userId);
signals:
     void sendID(int UserId, int rootId, int trashBinId,TeamData td);
private:
    Ui::LoginDialog *ui;
    int UserId;
    int rootId;
    int trashBinId;
    TeamData teamData;
};

#endif // LOGINDIALOG_H
