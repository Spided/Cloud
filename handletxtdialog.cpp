#include "handletxtdialog.h"
#include "ui_handletxtdialog.h"
#include <QTemporaryFile>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTimer>

HandleTxtDialog::HandleTxtDialog(const QUrl& txtUrl, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::HandleTxtDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_NativeWindow, true);
    setParent(nullptr);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    setStyleSheet("background-color: white;");
    this->setWindowTitle("txt预览");
    this->resize(800,400);
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HandleTxtDialog::downloadFinished);
    tempFile = new QTemporaryFile(this);
    textEdit = new QTextEdit(this);
    layout = new QVBoxLayout(this);
    layout->addWidget(textEdit);
    setLayout(layout);

    previewTxt(txtUrl);
}

HandleTxtDialog::~HandleTxtDialog()
{
    // 删除临时文件
    if (!tempFilePath.isEmpty()) {
        QFile file(tempFilePath);
        if (file.exists()) {
            file.remove();
        }
    }
    delete ui;
}

void HandleTxtDialog::previewTxt(const QUrl& txtUrl)
{
    // 创建请求
    QNetworkRequest request(txtUrl);

    // 配置 SSL 以忽略自签名证书（不推荐用于生产环境）
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // 发起下载请求
    manager->get(request);
}

void HandleTxtDialog::downloadFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Download error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    // 确保临时文件写入完成
    tempFile->setFileTemplate(QDir::tempPath() + "/XXXXXX.txt");
    if (!tempFile->open() || !tempFile->write(reply->readAll())) {
        qDebug() << "Failed to write temp file";
        return;
    }
    tempFile->close();
    tempFilePath = tempFile->fileName();
    qDebug() << tempFilePath;
    QString finalFilePath=tempFilePath;
    // 延迟 100ms 确保文件系统同步
    QTimer::singleShot(100, this, [this, finalFilePath]() {
        QFile file(tempFilePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = file.readAll();
            textEdit->setText(content);
            file.close();
        } else {
            qDebug() << "Failed to open temp file for reading";
        }
    });

    reply->deleteLater();
}
