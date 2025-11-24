#include "sharefiledialog.h"
#include "ui_sharefiledialog.h"
#include<QButtonGroup>
#include <QRegularExpression>
#include<QRandomGenerator>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QByteArray>
#include <QClipboard>
#include <QGuiApplication>
#include<QMessageBox>
ShareFileDialog::ShareFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareFileDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setPlaceholderText("请输入4位提取码");
    this->setFixedSize(770,560);
    QButtonGroup g1,g2;
    g1.addButton(ui->randomBtn);
    g1.addButton(ui->selfBtn);
    g2.addButton(ui->encodeBtn);
    g2.addButton(ui->unencodeBtn);
    g1.setExclusive(true);
    g2.setExclusive(true);
    // 连接 ui->selfBtn 的 toggled 信号到槽函数
    connect(ui->selfBtn, &QAbstractButton::toggled, this, &ShareFileDialog::onSelfBtnToggled);
    // 初始化 ui->lineEdit 的可用性，根据 ui->selfBtn 的初始状态设置
    ui->lineEdit->setEnabled(ui->selfBtn->isChecked());
    ui->pushButton_2->setEnabled(false);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &ShareFileDialog::checkExtractionCodeInput);
}

ShareFileDialog::~ShareFileDialog()
{
    delete ui;
}
void ShareFileDialog::onSelfBtnToggled(bool checked)
{
    // 根据 ui->selfBtn 的选中状态设置 ui->lineEdit 的可用性
    ui->lineEdit->setEnabled(checked);
}
void ShareFileDialog::on_pushButton_clicked()
{
    ui->pushButton_2->setEnabled(true);
    QString extractionCode;
    if(ui->randomBtn->isChecked()){
        qDebug()<<"随机";
        extractionCode = generateRandomExtractionCode();
    }
    else if(ui->selfBtn->isChecked()){
        qDebug()<<"自行设置";
        extractionCode = ui->lineEdit->text();
    }
    baseUrl=GlobalData::instance()->getSharedString()+"/api/FileItem/";
    QString link;
    if (ui->encodeBtn->isChecked()) {  // 加密链接
        link = encryptLink();
        link += "&encrypted=1"; // 添加加密标识
        isencoded = true;
        qDebug() << decryptLink(link);
    } else {
        link = baseUrl + QString::number(fileId);
        link += "&encrypted=0"; // 添加未加密标识
    }


    if (ui->checkBox->isChecked()) {
        link += "?code=" + extractionCode;
    }

    FinalUrl=link;
    ui->UrlLine->setText(link);
    ui->ExtractLine->setText(extractionCode);
    // 发送请求更新文件信息
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(baseUrl + QString::number(fileId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["Pin"] = extractionCode;
    json["Permission"] = 1;

    QJsonDocument jsonDoc(json);
    QByteArray requestData = jsonDoc.toJson();
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QNetworkReply *reply = manager->put(request, requestData);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "文件信息更新成功";
        } else {
            qDebug() << "文件信息更新失败: " << reply->errorString();
        }
        reply->deleteLater();
    });

}
void ShareFileDialog::checkExtractionCodeInput()
{
    QString input = ui->lineEdit->text();
    QRegularExpression regex("^[a-zA-Z0-9]{4}$");
    if (regex.match(input).hasMatch()) {
        ui->label_2->setStyleSheet("color: black;");
        ui->label_2->setText("仅支持数字及英文字母");
    } else {
        ui->label_2->setStyleSheet("color: red;");
        ui->label_2->setText("格式错误，提取码应为四位包含数字和字母的字符");
    }
}

QString ShareFileDialog::generateRandomExtractionCode()
{
    QString characters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString code;
    for (int i = 0; i < 4; ++i) {
        int index = QRandomGenerator::global()->bounded(characters.length());
        code += characters.at(index);
    }
    return code;
}
QString ShareFileDialog::decryptLink(const QString& encryptedUrl)
{
    QString baseUrl = GlobalData::instance()->getSharedString()+"/api/FileItem/";
    QString encryptedFileId = encryptedUrl.mid(baseUrl.length());
    int index = encryptedFileId.indexOf("?");
    if (index != -1) {
        encryptedFileId = encryptedFileId.left(index);
    }
    QByteArray byteArray = QByteArray::fromBase64(encryptedFileId.toUtf8());
    QString fileIdStr = QString::fromUtf8(byteArray);
    return baseUrl + fileIdStr;
}


QString ShareFileDialog::encryptLink()
{
    QString fileIdStr = QString::number(fileId);
    QByteArray byteArray = fileIdStr.toUtf8();
    QString encryptedFileId = byteArray.toBase64();
    return baseUrl + encryptedFileId;
}

void ShareFileDialog::on_pushButton_2_clicked()
{
    QString s = "通过轻协云储分享的文件：  链接：" + ui->UrlLine->text() + " 提取码：" + ui->ExtractLine->text();
    // 获取系统剪贴板对象
    QClipboard *clipboard = QGuiApplication::clipboard();
    // 将字符串 s 设置到剪贴板中
    clipboard->setText(s);
    QMessageBox::information(this, "操作成功", "成功复制链接和提取码");
}

