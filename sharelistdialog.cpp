#include "sharelistdialog.h"
#include "ui_sharelistdialog.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include<QJsonArray>
#include<QFileInfo>
#include <QRandomGenerator>
#include <QRegularExpression>
#include<QMessageBox>
#include<QUrlQuery>
#include<QMenu>
#include<QFileDialog>
ShareListDialog::ShareListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShareListDialog)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page);
    fileContextMenu = new QMenu(this);
    this->setFixedSize(800,660);
    setParent(nullptr);
    ui->FileListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->FileListWidget->setViewMode(QListView::IconMode);
    ui->FileListWidget->setIconSize(QSize(50, 50));
    ui->FileListWidget->setSpacing(10);
    QIcon btnicon(":/FileIcons/source/unable_return.png");
    ui->lastBtn->setIcon(btnicon);
    ui->lastBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");
    QAction *loadAction=fileContextMenu->addAction("下载");
    QAction *saveAction=fileContextMenu->addAction("保存");
    QAction *prewAction=fileContextMenu->addAction("预览");
    connect(loadAction,&QAction::triggered,this,&ShareListDialog::downloadFile);
    connect(prewAction,&QAction::triggered,this,&ShareListDialog::PreviewItem);
    connect(ui->FileListWidget, &QListWidget::customContextMenuRequested, this, &ShareListDialog::FileItemClicked);
    connect(saveAction,&QAction::triggered,this,&ShareListDialog::SaveFile);
    connect(ui->FileListWidget, &QListWidget::itemDoubleClicked, this, &ShareListDialog::doubleFileItem);
}
void ShareListDialog::getlink(QString link,QString code){
    ui->UrlLine->setText(link);
    ui->codeLine->setText(code);
}
void ShareListDialog::FileItemClicked(const QPoint &pos)
{
    qDebug()<<"life";
    QListWidgetItem *item = ui->FileListWidget->itemAt(pos);
    if (item) {
        qDebug()<<"is beautiful";
        currentRightClickedItem = item;
        fileContextMenu->popup(ui->FileListWidget->viewport()->mapToGlobal(pos));
    }
}
void ShareListDialog::PreviewItem(){
    if (ui->FileListWidget->getSelectedItemIds().size() > 1)
    {
        QMessageBox::warning(this, "预览失败", "非法操作！" );
        return;
    }

    if (currentRightClickedItem)
    {
        int fileId = currentRightClickedItem->data(Qt::UserRole).toInt();
        PreviewFile(fileId);
    }
}
void ShareListDialog::PreviewFile(int fileId){
    // 创建网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    // 构建获取文件信息的请求 URL
    QString apiUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(fileId);
    request.setUrl(QUrl(apiUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    // 发送 GET 请求
    QNetworkReply* reply = manager->get(request);
    bool connected = connect(reply, &QNetworkReply::finished, [this, fileId,manager,reply]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            // 解析 JSON 响应
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObj = jsonDoc.object();
            // 获取文件名
            QString fileName = jsonObj["name"].toString();
            // 判断文件类型是否为 MP4
            if (fileName.endsWith(".mp4", Qt::CaseInsensitive)||fileName.endsWith(".mp3", Qt::CaseInsensitive))
            {
                qDebug()<<"视频为"<<fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl videoUrl = QUrl(viewUrl);
                int type=0;//0表示视频，1表示音频
                if(fileName.endsWith(".mp3", Qt::CaseInsensitive))type=1;
                // 创建并显示视频预览对话框
                HandleVideoDialog dialog(videoUrl, this,type);
                dialog.exec();
            }
            else if (fileName.endsWith(".png", Qt::CaseInsensitive) || fileName.endsWith(".jpg", Qt::CaseInsensitive))
            {
                qDebug()<<"图片为"<<fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl imageUrl = QUrl(viewUrl);
                // 创建并显示图片预览对话框
                HandleImageDialog dialog(imageUrl, this);
                dialog.exec();
            }else if (fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
                qDebug() << "PDF 文件为" << fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl fileUrl = QUrl(viewUrl);
                // 创建并显示文件预览对话框
                HandlePdfWordDialog dialog(fileUrl, this);
                dialog.exec();
            } else if (fileName.endsWith(".doc", Qt::CaseInsensitive) || fileName.endsWith(".docx", Qt::CaseInsensitive)) {
                qDebug() << "Word 文件为" << fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl fileUrl = QUrl(viewUrl);
                // 创建并显示文件预览对话框
                HandlePdfWordDialog dialog(fileUrl, this,1);
                dialog.exec();
            }else if (fileName.endsWith(".txt", Qt::CaseInsensitive)) {
                qDebug() << "txt 文件为" << fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl fileUrl = QUrl(viewUrl);
                // 创建并显示文件预览对话框
                HandleTxtDialog dialog(fileUrl, this);
                dialog.exec();
            }
            else
            {
                QMessageBox::warning(this, "预览失败", "不支持的文件类型！");
            }
        }
        else
        {
            QMessageBox::warning(this, "预览失败", "获取文件信息失败！");
        }
        reply->deleteLater();
        manager->deleteLater();
    });

}
void ShareListDialog::doubleFileItem(QListWidgetItem *item){
    int fileId = item->data(Qt::UserRole).toInt();
    int type = item->data(Qt::UserRole + 1).toInt();
    if (type == 0) { // 如果是文件夹
        showFileList(fileId);
    }
    else PreviewFile(fileId);
}
void ShareListDialog::SaveFile(){
    AddSharedFileDialog dig(this,UserId,currentRightClickedItem->data(Qt::UserRole).toInt());
    dig.exec();
}
void ShareListDialog::downloadFile(){
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    // 获取选中的文件
    QList<QListWidgetItem *> selectedItems = ui->FileListWidget->selectedItems(); // 假设 FileListWidget 是显示文件列表的控件
    if (selectedItems.isEmpty()) return;    
    // 选择下载路径
    QString downloadPath = QFileDialog::getExistingDirectory(this, tr("选择下载路径"));
    if (downloadPath.isEmpty()) return;
    ui->ProgessWidget->downloadPath = downloadPath;
    for (QListWidgetItem *item : selectedItems) {
        int fileId = item->data(Qt::UserRole).toInt();
        QString fileName = item->text();
        QIcon icon = item->icon();
        ui->ProgessWidget->addDownloadTask(fileId, fileName, icon);
        int row = ui->ProgessWidget->rowCount() - 1;
        ui->ProgessWidget->startDownload(row);
    }
}
ShareListDialog::~ShareListDialog()
{
    delete ui;
}
QString ShareListDialog::decryptLink(const QString& encryptedUrl)
{
    QString encryptedFileId = encryptedUrl.mid(baseUrl.length());
    int index = encryptedFileId.indexOf("?");
    if (index != -1) {
        encryptedFileId = encryptedFileId.left(index);
    }
    QByteArray byteArray = QByteArray::fromBase64(encryptedFileId.toUtf8());
    QString fileIdStr = QString::fromUtf8(byteArray);
    rootId=fileIdStr.toInt();
    return baseUrl + fileIdStr;
}
void ShareListDialog::showFileList(int parentId)
{
    ui->FileListWidget->clear();
    if(parentId==rootId){
        QIcon btnicon(":/FileIcons/source/unable_return.png");
        ui->lastBtn->setIcon(btnicon);
        ui->lastBtn->setEnabled(false);
    }
    else {
        QIcon btnicon(":/FileIcons/source/return.png");
        ui->lastBtn->setIcon(btnicon);
        ui->lastBtn->setEnabled(true);
    }
    if (directoryStack.empty()||directoryStack.top()!=parentId) {
        directoryStack.push(parentId); // 进入新目录时将当前目录 ID 压入栈中
    }
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    if(inittype==0)
        request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Directory/%1").arg(parentId)));
    else request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(parentId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QNetworkReply *reply = manager->get(request);
    bool connected = connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonArray fileList;
            if(inittype==0)fileList = jsonResponse.array();
            else fileList.append(jsonResponse.object());
            for (const auto &fileItem : fileList) {
                QJsonObject fileObj = fileItem.toObject();
                QString name = fileObj["name"].toString();
                int id = fileObj["id"].toInt();
                int type = fileObj["type"].toInt();
                qDebug()<<name;
                QIcon icon;
                if (type == 0) {
                    icon = QIcon(":/FileIcons/source/Dir.png");
                } else {
                    QFileInfo fileInfo(name);
                    QString suffix = fileInfo.suffix().toLower();
                    if (suffix == "mp3") {
                        icon = QIcon(":/FileIcons/source/mp3.png");
                    } else if (suffix == "mp4") {
                        icon = QIcon(":/FileIcons/source/mp4.png");
                    } else if (suffix == "docx" || suffix == "doc") {
                        icon = QIcon(":/FileIcons/source/word.png");
                    } else if (suffix == "pdf") {
                        icon = QIcon(":/FileIcons/source/PDF.png");
                    } else if (suffix == "png") {
                        icon = QIcon(":/FileIcons/source/png.png");
                    } else if (suffix == "txt") {
                        icon = QIcon(":/FileIcons/source/TXT.png");
                    } else if (suffix == "zip") {
                        icon = QIcon(":/FileIcons/source/zip.png");
                    } else if(suffix=="jpg"){
                        icon = QIcon(":/FileIcons/source/Jpg.png");
                    }else if(suffix=="rar"){
                        icon = QIcon(":/FileIcons/source/rar.png");
                    }else if(suffix=="ppt"||suffix=="pptx"){
                        icon=QIcon(":/FileIcons/source/PPT.png");
                    }
                    else {
                        icon = QIcon(":/FileIcons/source/UnNamedFile.png");
                    }
                }
                QListWidgetItem *item = new QListWidgetItem(icon, name);
                item->setData(Qt::UserRole, id);
                item->setData(Qt::UserRole + 1, type);
                item->setSizeHint(QSize(100, 70));
                // 设置文件名换行显示
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignTop);
                ui->FileListWidget->addItem(item);
            }

        }
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::errorOccurred, [](QNetworkReply::NetworkError code) {
        qDebug() << "Network request error:" << code;
    });


}
void ShareListDialog::on_pushButton_clicked()
{
    QString link = ui->UrlLine->text();
    QString code = ui->codeLine->text();

    // 处理链接，确保参数部分以 ? 开头
    int questionMarkIndex = link.indexOf('?');
    if (questionMarkIndex == -1) {
        int firstAmpersandIndex = link.indexOf('&');
        if (firstAmpersandIndex != -1) {
            link.insert(firstAmpersandIndex, '?');
        }
    }

    QUrl url(link);
    QUrlQuery query(url);
    QString encryptedFlag = query.queryItemValue("encrypted");
    if (encryptedFlag == "1") {
        link = decryptLink(link);
    }
    else {
        if (link.startsWith(baseUrl)) {
            QString fileIdStr = link.mid(baseUrl.length());
            int questionMarkIndex = fileIdStr.indexOf('?');
            if (questionMarkIndex != -1) {
                fileIdStr = fileIdStr.left(questionMarkIndex);
            }
            rootId = fileIdStr.toInt();
        }
    }
    qDebug() << link;

    // 发送请求更新文件信息
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(link));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply, manager, link, code]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
            if (!jsonDoc.isNull() && jsonDoc.isObject()) {
                QJsonObject jsonObj = jsonDoc.object();
                if (jsonObj.contains("pin")) {
                    QString pin = jsonObj["pin"].toString();
                    inittype=jsonObj["type"].toInt();
                    if (pin != code) {
                        QMessageBox::warning(this, "提取码错误", "你输入的提取码不正确，请重新输入。");
                    } else {
                        // 提取码正确，可进行后续操作
                        ui->stackedWidget->setCurrentWidget(ui->page_2);
                        showFileList(rootId);

                        qDebug() << "提取码正确，可继续处理文件信息";
                    }
                }
            }
        }
        else{qDebug()<<reply->error();}
        reply->deleteLater();
    });
}

void ShareListDialog::on_lastBtn_clicked()
{
    if (directoryStack.size() > 1) { // 栈中至少有两个元素（当前目录和上一级目录）
        directoryStack.pop(); // 弹出当前目录的 ID
        int parentId = directoryStack.top(); // 获取上一级目录的 ID
        showFileList(parentId);
    } else if (directoryStack.size() == 1) { // 回到根目录
        directoryStack.pop();
        showFileList(rootId); // 使用 rootId 作为根目录
    } else {
        qDebug() << "已经是根目录，无法再返回。";
    }

}


void ShareListDialog::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

