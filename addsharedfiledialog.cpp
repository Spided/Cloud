#include "addsharedfiledialog.h"
#include "ui_addsharedfiledialog.h"
#include<QInputDialog>
#include<QNetworkRequest>
#include<QMessageBox>
AddSharedFileDialog::AddSharedFileDialog(QWidget *parent,int id,int id2) :
    QDialog(parent),
    ui(new Ui::AddSharedFileDialog)
{
    ui->setupUi(this);
    UserId = id;
    fileId=id2;
    ui->folderTree->setHeaderLabels({"文件夹名称"});
    this->setFixedSize(610,720);
    // 发送请求获取根文件夹
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Root/%1").arg(UserId);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            onRootFilesReceived(reply);
        }
        reply->deleteLater();
    });
}
void AddSharedFileDialog::onRootFilesReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (jsonDoc.isArray()) {
            QJsonArray rootFiles = jsonDoc.array();
            auto file=rootFiles.at(0);
            rootId=file["parentId"].toInt();
            qDebug()<<"rootId="<<rootId;
            buildFolderTree(rootFiles);
        }
        else qDebug()<<"jsonDoc有问题";
    }
}
void AddSharedFileDialog::onDirectoryFilesReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (jsonDoc.isArray()) {
            QJsonArray directoryFiles = jsonDoc.array();
            QTreeWidgetItem *parentItem = static_cast<QTreeWidgetItem*>(reply->property("parentItem").value<void*>());
            buildFolderTree(directoryFiles, parentItem);
        }
    }
}

void AddSharedFileDialog::buildFolderTree(const QJsonArray &files, QTreeWidgetItem *parentItem)
{
    qDebug()<<"建树咯";
    for (const auto &file : files) {
        QJsonObject fileObj = file.toObject();
        int type = fileObj["type"].toInt();
        if (type == 0) { // 假设 type 为 0 表示文件夹
            QIcon icon;
            icon = QIcon(":/FileIcons/source/Dir.png");
            // 缩放图标
            QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(50, 50)));
            pixmap = pixmap.scaled(QSize(50, 50), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            icon = QIcon(pixmap);
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, fileObj["name"].toString());
            item->setIcon(0, icon);
            // 将文件夹的id存储在item的用户数据项中（使用Qt::UserRole）
            int id = fileObj["id"].toInt();
            item->setData(0, Qt::UserRole, id);
            if (parentItem) {
                parentItem->addChild(item);
            } else {
                ui->folderTree->addTopLevelItem(item);
            }
            // 发送请求获取子文件夹
            QString url = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Directory/%1").arg(id);
            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            QNetworkRequest request;
            request.setUrl(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            // 忽略证书验证
            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);
            QNetworkReply *reply = manager->get(request);
            reply->setProperty("parentItem", QVariant::fromValue(static_cast<void*>(item)));
            connect(reply, &QNetworkReply::finished, this, [this, reply]() {
                onDirectoryFilesReceived(reply);
                reply->deleteLater();
            });
        }
    }
}
AddSharedFileDialog::~AddSharedFileDialog()
{
    delete ui;
}

void AddSharedFileDialog::on_newfoldbtn_clicked()
{
    // 获取用户输入的文件夹名称
    bool ok;
    QString folderName = QInputDialog::getText(this, "创建新文件夹", "请输入文件夹名称:", QLineEdit::Normal, "", &ok);
    if (!ok || folderName.isEmpty()) {
        return; // 用户取消输入或输入为空
    }
    // 确定父文件夹 ID
    int parentId;
    QTreeWidgetItem *selectedItem = ui->folderTree->currentItem();
    if (selectedItem) {
        // 如果有选中的项，直接使用选中项的 ID 作为父文件夹 ID
        parentId = selectedItem->data(0, Qt::UserRole).toInt();
    } else {
        // 如果没有选中项，使用 rootId
        parentId = rootId;
    }
    // 构建请求数据
    QJsonObject fileItemJson;
    fileItemJson["name"] = folderName;
    fileItemJson["parentId"] = parentId;
    fileItemJson["type"] = 0; // 0 表示文件夹
    fileItemJson["ownerId"] = UserId;
    fileItemJson["permission"] = 1; // 假设默认权限为 1
    fileItemJson["pin"] = ""; // 假设初始提取码为空
    QJsonDocument jsonDoc(fileItemJson);
    QByteArray requestData = jsonDoc.toJson();
    // 发送请求
    QString url = GlobalData::instance()->getSharedString()+"/api/FileItem/";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    // 创建网络请求
    QNetworkRequest request(QUrl(GlobalData::instance()->getSharedString()+"/api/FileItem/"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QNetworkReply *reply = manager->post(request, requestData);
    connect(reply, &QNetworkReply::finished, [this, reply,sslConfig]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
            if (responseDoc.isObject()) {
                QJsonObject responseObj = responseDoc.object();
                if (responseObj.contains("id")) {
                    // 新文件夹创建成功
                    int newFolderId = responseObj["id"].toInt();
                    qDebug() << "新文件夹创建成功，ID: " << newFolderId;
                    // 刷新 treewidget
                    ui->folderTree->clear(); // 清空现有的 treewidget 内容
                    // 重新获取根文件夹数据
                    QString rootUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Root/%1").arg(UserId);
                    QNetworkAccessManager *rootManager = new QNetworkAccessManager(this);
                    QNetworkRequest rootRequest(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Root/%1").arg(UserId)));
                    rootRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                    rootRequest.setSslConfiguration(sslConfig);
                    QNetworkReply *rootReply = rootManager->get(rootRequest);
                    connect(rootReply, &QNetworkReply::finished, [this, rootReply]() {
                        if (rootReply->error() == QNetworkReply::NoError) {
                            QByteArray rootData = rootReply->readAll();
                            QJsonDocument rootJsonDoc = QJsonDocument::fromJson(rootData);
                            if (rootJsonDoc.isArray()) {
                                QJsonArray rootFiles = rootJsonDoc.array();
                                buildFolderTree(rootFiles);
                            }
                        }
                        rootReply->deleteLater();
                    });
                }
            }
        } else {
            qDebug() << "创建文件夹失败: " << reply->errorString();
        }
        reply->deleteLater();
    });
}


void AddSharedFileDialog::on_cancelbtn_clicked()
{
    this->close();
}


void AddSharedFileDialog::on_confirmbtn_clicked()
{
    // 确定目标文件夹 ID
    int dstId;
    QTreeWidgetItem *selectedItem = ui->folderTree->currentItem();
    if (selectedItem) {
        // 如果有选中的项，使用选中项的 ID 作为目标文件夹 ID
        dstId = selectedItem->data(0, Qt::UserRole).toInt();
    } else {
        // 如果没有选中项，使用 rootId 作为目标文件夹 ID
        dstId = rootId;
    }


    // 发送请求
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Duplicate/%1-%2").arg(fileId).arg(dstId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QNetworkReply *reply = manager->post(request, QByteArray());
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "操作成功", "文件成功保存");
            qDebug() << "复制成功";
        } else {
            qDebug() << "复制失败: " << reply->errorString();
        }
        reply->deleteLater();
    });
}
