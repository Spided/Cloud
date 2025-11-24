#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("登录/注册");
    this->setFixedSize(550,400);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_Finish_clicked()
{
    QString email = ui->Email->text();
    QString username=ui->UsernameLine_2->text();
    QString password = ui->Password_2->text();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(GlobalData::instance()->getSharedString()+"/api/Account/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QJsonObject json;
    json["UserName"] = username;
    json["Email"] = email;
    json["Password"] = password;
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        if (jsonResponse.isNull()) {
            qDebug() << "Failed to parse JSON data:" << responseData;            
        } else {
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject.contains("id")) {
                UserId = jsonObject["id"].toInt();                
            } else if (jsonObject.contains("message")) {
                QString message = jsonObject["message"].toString();                
            } else {
                qDebug() << "Unexpected JSON data:" << jsonObject;               
            }
        }
        reply->deleteLater();
    });
}


void LoginDialog::on_returnbtn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}


void LoginDialog::on_Registerbtn_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void LoginDialog::queryTeamInfo(int userId) {
    QNetworkRequest request;
    request.setUrl(QUrl(GlobalData::instance()->getSharedString()+QString("/api/TeamInfo/Account/%1").arg(userId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            // 处理团队信息查询结果
            if (jsonObject.contains("teamId") && jsonObject.contains("rootId") &&
                jsonObject.contains("name") && jsonObject.contains("role")) {
                // 解析团队信息并存储到Temadata结构体
                teamData.TeamId = jsonObject["teamId"].toInt();
                teamData.RootId = jsonObject["rootId"].toInt();
                teamData.name = jsonObject["name"].toString();
                teamData.role = jsonObject["role"].toInt();
            } else {
                // 处理无团队归属的情况
                teamData.TeamId = -1;
                teamData.RootId = -1;
                teamData.name = "";
                teamData.role = -1;

            }
        }
        else{
            // 处理无团队归属的情况
            teamData.TeamId = -1;
            teamData.RootId = -1;
            teamData.name = "";
            teamData.role = -1;
        }
        reply->deleteLater();
    });
}
void LoginDialog::on_Loginbtn_clicked()
{
    QString email = ui->EmailLine->text();
    QString password = ui->Password->text();
    QNetworkRequest request;
    request.setUrl(QUrl(GlobalData::instance()->getSharedString()+"/api/Account/Login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QJsonObject json;
    json["Email"] = email;
    json["Password"] = password;

    QByteArray data = QJsonDocument(json).toJson();

    QNetworkReply *reply = manager->post(request, data);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();
            if (jsonObject.contains("id") && jsonObject.contains("rootId") && jsonObject.contains("trashBinId")) {
                UserId = jsonObject["id"].toInt();
                rootId = jsonObject["rootId"].toInt();  // 保存 rootId
                trashBinId = jsonObject["trashBinId"].toInt();  // 保存 trashBinId
                queryTeamInfo(UserId);
                QMessageBox::information(this, "登录状态", QString("登陆成功"));
                emit sendID(UserId, rootId, trashBinId,teamData);
                this->close();

            } else if (jsonObject.contains("message")) {
                QString message = jsonObject["message"].toString();
                QMessageBox::warning(this, "登录失败", message);
            } else {
                qDebug() << "Unexpected JSON data:" << jsonObject;
                QMessageBox::warning(this, "登录失败", "Unexpected server response.");
            }
        } else {
            QMessageBox::warning(this, "登录失败", "请检查密码和邮箱是否正确！");
        }
        reply->deleteLater();
    });
}

