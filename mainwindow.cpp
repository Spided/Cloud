#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QFileInfo>
#include <QClipboard>
#include <QGuiApplication>
#include<QFileDialog>
#include<QLayout>
#include<QHttpMultiPart>
#include<QTimer>
#include<QButtonGroup>
#include<QPainter>
#include<QPainterPath>
#include<QOverload>
#include<QBuffer>
#include<QHBoxLayout>
#include<QNetworkRequest>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    LoginDialog dialog(this);
    connect(&dialog, &LoginDialog::sendID, this, &MainWindow::receiveId);
    dialog.exec();
    this->setWindowTitle("轻协云储");
   // this->resize(780,660);
    this->setFixedSize(880,670);
    QIcon icon(":/FileIcons/source/Window_Icon.jpg");
    this->setWindowIcon(icon);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    ui->FileListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->FileListWidget_team->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->TrashList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->FileListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onFileItemDoubleClicked);
    connect(ui->FileListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onFileItemRightClicked);
    connect(ui->FileListWidget_team, &QListWidget::itemDoubleClicked, this, &MainWindow::onFileItemDoubleClicked);
    connect(ui->FileListWidget_team, &QListWidget::customContextMenuRequested, this, &MainWindow::onFileItemRightClicked);
    connect(ui->TrashList, &QListWidget::customContextMenuRequested, this, &MainWindow::onTrashItemRightClicked);
    connect(ui->FileListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::onEmptyAreaRightClicked);
    connect(ui->FileListWidget_team, &QListWidget::customContextMenuRequested, this, &MainWindow::onEmptyAreaRightClicked);

    btng.addButton(ui->toolButton,0);
    btng.addButton(ui->toolButton_2,1);
    btng.addButton(ui->toolButton_3,2);
    btng.addButton(ui->toolButton_4,3);
    btng.addButton(ui->toolButton_5,4);
    btng.addButton(ui->toolButton_6,5);
    btng.addButton(ui->toolButton_7,6);
    btng.addButton(ui->toolButton_8,7);
    ui->stackedWidget->setCurrentWidget(ui->FileListPage);
    UpdateBtnCol(0);
    iconinit();
    fileContextMenu = new QMenu(this);
    QAction *renameAction = fileContextMenu->addAction("重命名");
    QAction *deleteAction = fileContextMenu->addAction("删除");
    QAction *cutAction = fileContextMenu->addAction("剪切");
    QAction *copyAction = fileContextMenu->addAction("复制");
    QAction *loadAction=fileContextMenu->addAction("下载");
    QAction *previewAction=fileContextMenu->addAction("预览");
    QAction *shareAction=fileContextMenu->addAction("分享");
    connect(renameAction, &QAction::triggered, this, &MainWindow::onRenameActionTriggered);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteActionTriggered);
    connect(cutAction, &QAction::triggered, this, &MainWindow::onCutActionTriggered);
    connect(loadAction,&QAction::triggered,this,&MainWindow::onLoadActionTriggered);
    connect(copyAction, &QAction::triggered, this, &MainWindow::onCopyActionTriggered);
    connect(previewAction,&QAction::triggered,this, &MainWindow::PreviewItem);
    connect(shareAction,&QAction::triggered,this,&MainWindow::ShareFile);
    trashContextMenu = new QMenu(this);
    QAction *restoreAction = trashContextMenu->addAction("恢复");
    QAction *deleteFromTrashAction = trashContextMenu->addAction("永久删除");
    connect(restoreAction, &QAction::triggered, this, &MainWindow::onRestoreActionTriggered);
    connect(deleteFromTrashAction, &QAction::triggered, this, &MainWindow::onDeleteFromTrashActionTriggered);
    emptyAreaContextMenu = new QMenu(this);
    QAction *pasteAction = emptyAreaContextMenu->addAction("粘贴");
    QAction *newFolderAction = emptyAreaContextMenu->addAction("新建文件夹");
    pasteAction->setEnabled(false);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::onPasteActionTriggered);
    connect(newFolderAction, &QAction::triggered, this, &MainWindow::onNewFolderTriggered);
    connect(ui->FileListWidget, &CustomListWidget::moveFileOrFolder, this, &MainWindow::moveFileOrFolder);
    connect(ui->FileListWidget_team, &CustomListWidget::moveFileOrFolder, this, &MainWindow::moveFileOrFolder);
    networkManager = new QNetworkAccessManager(this);
    currentParentId = -1; // 初始化当前目录ID
    //上传文件初始化
    uploadinit();
}
void MainWindow::iconinit(){
    QIcon btnicon(":/FileIcons/source/unable_return.png");
    ui->returnBtn->setIcon(btnicon);
    ui->returnBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");
    ui->returnBtn_team->setIcon(btnicon);
    ui->returnBtn_team->setStyleSheet("QPushButton { border: none; background: transparent; }");
    QIcon btnicon1(":/FileIcons/source/upload.png");
    ui->upload->setIcon(btnicon1);
    ui->upload_team->setIcon(btnicon1);
    QIcon btnicon2(":/FileIcons/source/refresh.png");
    ui->refreshBtn->setIcon(btnicon2);
    ui->refreshBtn->setStyleSheet("QPushButton { border: none; background: transparent; }");
    ui->refreshBtn_team->setIcon(btnicon2);
    ui->refreshBtn_team->setStyleSheet("QPushButton { border: none; background: transparent; }");
    QIcon toolicon2(":/FileIcons/source/upload_tool.png");
    ui->toolButton_2->setIcon(toolicon2);
    ui->toolButton_2->setIconSize(QSize(20,20));
    ui->toolButton->setIconSize(QSize(20,20));
    QIcon toolicon3(":/FileIcons/source/load_tool.png");
    ui->toolButton_3->setIcon(toolicon3);
    ui->toolButton_3->setIconSize(QSize(20,20));
    QIcon toolicon4(":/FileIcons/source/delete_tool.png");
    ui->toolButton_4->setIcon(toolicon4);
    ui->toolButton_4->setIconSize(QSize(20,20));
    QIcon toolicon5(":/FileIcons/source/share_tool.png");
    ui->toolButton_5->setIcon(toolicon5);
    ui->toolButton_5->setIconSize(QSize(20,20));
    QIcon toolicon6(":/FileIcons/source/relogin_tool.png");
    ui->toolButton_6->setIcon(toolicon6);
    ui->toolButton_6->setIconSize(QSize(20,20));
    QIcon toolicon7(":/FileIcons/source/team.png");
    ui->toolButton_7->setIcon(toolicon7);
    ui->toolButton_7->setIconSize(QSize(20,20));
    QIcon toolicon8(":/FileIcons/source/MyData.png");
    ui->toolButton_8->setIcon(toolicon8);
    ui->toolButton_8->setIconSize(QSize(20,20));
}

QString MainWindow::decryptLink(const QString& encryptedUrl)
{
    QString baseUrl = GlobalData::instance()->getSharedString()+"/api/FileItem/";
    QString encryptedFileId = encryptedUrl.mid(baseUrl.length());
    QByteArray byteArray = QByteArray::fromBase64(encryptedFileId.toUtf8());
    QString fileIdStr = QString::fromUtf8(byteArray);
    return baseUrl + fileIdStr;
}
void MainWindow::ShareFile(){
    if (ui->FileListWidget->getSelectedItemIds().size() > 1||ui->FileListWidget_team->getSelectedItemIds().size()>1)
    {
        QMessageBox::warning(this, "分享失败", "非法操作！" );
        return;
    }

    if (currentRightClickedItem)
    {
        int fileId = currentRightClickedItem->data(Qt::UserRole).toInt();
        ShareFileDialog dia(this);
        dia.getid(fileId);
        bool isencoded=false;
        QString ShareUrl=dia.returnTheUrl(isencoded);
        dia.exec();
        if(isencoded){
            ShareUrl=decryptLink(ShareUrl);
        }

    }



}
void MainWindow::UpdateBtnCol(int idx){
    QString defaultStyleSheet = ""; // 默认样式表，可根据实际情况修改
    QString selectedStyleSheet = "QToolButton {"
                                 "background-color: rgba(135, 206, 250, 128);"
                                 "}";

    // 遍历按钮组中的所有按钮
    for (int i = 0; i < btng.buttons().size(); ++i) {
        QToolButton* button = qobject_cast<QToolButton*>(btng.button(i));
        if (button) {
            if (i == idx) {
                // 如果是指定索引的按钮，设置为半透明天蓝色
                button->setStyleSheet(selectedStyleSheet);
            } else {
                // 其他按钮恢复默认样式
                button->setStyleSheet(defaultStyleSheet);
            }
        }
    }
}
void MainWindow::uploadinit(){
    // 初始化表格属性
    ui->UploadProcessWidget->setColumnCount(6);
    QStringList headers;
    headers << "文件" << "大小" << "进度"<<"状态"<<"暂停"<<"删除";
    ui->UploadProcessWidget->setHorizontalHeaderLabels(headers);
    ui->UploadProcessWidget->setShowGrid(false);
    ui->UploadProcessWidget->verticalHeader()->setVisible(false);
    ui->UploadProcessWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->UploadProcessWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 获取水平表头指针
    QHeaderView *horizontalHeader = ui->UploadProcessWidget->horizontalHeader();

    // 设置列宽策略为固定宽度
    horizontalHeader->setSectionResizeMode(QHeaderView::Fixed);

    // 计算每列的宽度
    int firstColumnWidth = 100;
    int secondColumnWidth = 150;
    int thirdColumnWidth = 230;
    int fourthColumnWidth=170;
    int fifthColumnWidth=50;
    int sixthColumnWidth=50;

    // 设置每列的宽度
    horizontalHeader->resizeSection(0, firstColumnWidth);
    horizontalHeader->resizeSection(1, secondColumnWidth);
    horizontalHeader->resizeSection(2, thirdColumnWidth);
    horizontalHeader->resizeSection(3,fourthColumnWidth);
    horizontalHeader->resizeSection(4,fifthColumnWidth);
    horizontalHeader->resizeSection(5,sixthColumnWidth);
}
MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::onCopyActionTriggered() {   
    QList<QListWidgetItem*> selectedItems = ui->FileListWidget->selectedItems();
    if(!IsTeam)selectedItems = ui->FileListWidget->selectedItems();
    else selectedItems = ui->FileListWidget_team->selectedItems();
    if (!selectedItems.isEmpty()) {
        copyFileIds.clear();
        for (QListWidgetItem* item : selectedItems) {
            copyFileIds.append(item->data(Qt::UserRole).toInt());
        }
        hasCopied = true;
        QAction *pasteAction = emptyAreaContextMenu->actions().first();
        pasteAction->setEnabled(true);
    }

}




void MainWindow::showFileList(int ownerId, int parentId)
{
    IsTeam=false;
    if(parentId==rootId){
        QIcon btnicon(":/FileIcons/source/unable_return.png");
        ui->returnBtn->setIcon(btnicon);
        ui->returnBtn->setEnabled(false);
    }
    else {
        QIcon btnicon(":/FileIcons/source/return.png");
        ui->returnBtn->setIcon(btnicon);
        ui->returnBtn->setEnabled(true);
    }

    nowId=parentId;
    ui->FileListWidget->clear();
    if(parentId==-1)parentId=rootId;   
    if (directoryStack.empty()||directoryStack.top()!=parentId) {
        directoryStack.push(parentId); // 进入新目录时将当前目录 ID 压入栈中
    }
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Directory/%1").arg(parentId)));
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
            if (!jsonResponse.isNull() && jsonResponse.isArray()) {
                QJsonArray fileList = jsonResponse.array();
                // 设置 QListWidget 为图标模式
                ui->FileListWidget->setViewMode(QListView::IconMode);
                // 设置图标大小为 50x50
                ui->FileListWidget->setIconSize(QSize(50, 50));
                // 设置项之间的间距
                ui->FileListWidget->setSpacing(10);
                for (const auto &fileItem : fileList) {
                    QJsonObject fileObj = fileItem.toObject();
                    QString name = fileObj["name"].toString();
                    int id = fileObj["id"].toInt();
                    int type = fileObj["type"].toInt();                    
                    QIcon icon;
                    if (type == 0) {
                        icon = QIcon(":/FileIcons/source/Dir.png");
                    } else {
                        QFileInfo fileInfo(name);
                        QString suffix = fileInfo.suffix().toLower();
                        icon=getFileIcon(suffix);
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
        }
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::errorOccurred, [](QNetworkReply::NetworkError code) {
        qDebug() << "Network request error:" << code;
    });

    ui->FileListWidget->setCurrentUserId(UserId);
}
void MainWindow::moveFileOrFolder(int ownerId, int sourceId, int destinationId)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Move/%1-%2").arg(sourceId).arg(destinationId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QNetworkReply *reply = manager->put(request, QByteArray());

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "操作成功", "文件或文件夹已成功移动");
            if(!IsTeam)
                showFileList(UserId, directoryStack.top());
            else ShowFileListForTeam(teamData.TeamId,directoryStack_Team.top());
        } else {
            QMessageBox::warning(this, "操作失败", "移动文件或文件夹时发生网络错误: " + reply->errorString());
        }
        reply->deleteLater();
    });
}
void MainWindow::onFileItemDoubleClicked(QListWidgetItem *item)
{
    int fileId = item->data(Qt::UserRole).toInt();
    int type = item->data(Qt::UserRole + 1).toInt();
    if (type == 0) { // 如果是文件夹
        if(!IsTeam)
            showFileList(UserId, fileId);
        else ShowFileListForTeam(teamData.TeamId,fileId);
    } else {
        PreviewFile(fileId);
    }
}

void MainWindow::on_returnBtn_clicked()
{

    if (directoryStack.size() > 1) { // 栈中至少有两个元素（当前目录和上一级目录）
        directoryStack.pop(); // 弹出当前目录的 ID
        int parentId = directoryStack.top(); // 获取上一级目录的 ID

        showFileList(UserId, parentId);

    } else if (directoryStack.size() == 1) { // 回到根目录
        directoryStack.pop();

        showFileList(UserId, rootId); // 使用 rootId 作为根目录
    } else {
        qDebug() << "已经是根目录，无法再返回。";
    }

}
void MainWindow::onFileItemRightClicked(const QPoint &pos)
{
    if(!IsTeam){
        QListWidgetItem *item = ui->FileListWidget->itemAt(pos);
        if (item) {
            currentRightClickedItem = item;
            fileContextMenu->popup(ui->FileListWidget->viewport()->mapToGlobal(pos));
        }
    }
    else{
        QListWidgetItem *item = ui->FileListWidget_team->itemAt(pos);
        if (item) {
            currentRightClickedItem = item;
            fileContextMenu->popup(ui->FileListWidget_team->viewport()->mapToGlobal(pos));
        }
    }
}
void MainWindow::onCutActionTriggered()
{

    QList<QListWidgetItem*> selectedItems = ui->FileListWidget->selectedItems();
    if(!IsTeam){
        selectedItems = ui->FileListWidget->selectedItems();
    }
    else{
        selectedItems = ui->FileListWidget_team->selectedItems();
    }
    if (!selectedItems.isEmpty()) {
        cutFileIds.clear();
        for (QListWidgetItem* item : selectedItems) {
            cutFileIds.append(item->data(Qt::UserRole).toInt());
        }
        hasCut = true;
        QAction *pasteAction = emptyAreaContextMenu->actions().first();
        pasteAction->setEnabled(true);
    }
}
void MainWindow::onEmptyAreaRightClicked(const QPoint &pos)
{
    if(!IsTeam){
        QListWidgetItem *item = ui->FileListWidget->itemAt(pos);
        if (!item) {
            QAction *pasteAction = emptyAreaContextMenu->actions().first();
            if(hasCut)
                pasteAction->setEnabled(hasCut);
            if(hasCopied)pasteAction->setEnabled(hasCopied);
            emptyAreaContextMenu->popup(ui->FileListWidget->viewport()->mapToGlobal(pos));
        }
    }
    else{
        QListWidgetItem *item = ui->FileListWidget_team->itemAt(pos);
        if (!item) {
            QAction *pasteAction = emptyAreaContextMenu->actions().first();
            if(hasCut)
                pasteAction->setEnabled(hasCut);
            if(hasCopied)pasteAction->setEnabled(hasCopied);
            emptyAreaContextMenu->popup(ui->FileListWidget_team->viewport()->mapToGlobal(pos));
        }
    }

}
void MainWindow::onPasteActionTriggered() {
    if (hasCopied) {
        int destinationId;
        if(!IsTeam){
            if (!directoryStack.isEmpty()) {
                destinationId = directoryStack.top(); // 获取当前目录 ID
            } else {
                destinationId = rootId; // 默认根目录
            }
        }
        else{
            if (!directoryStack_Team.isEmpty()) {
                destinationId = directoryStack_Team.top(); // 获取当前目录 ID
            } else {
                destinationId = teamData.RootId; // 默认根目录
            }
        }

        isallPasted = true;
        for (int copyFileId : copyFileIds) {
            QNetworkRequest request;
            request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Duplicate/%1-%2").arg(copyFileId).arg(destinationId)));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);

            QNetworkReply *reply = networkManager->post(request, QByteArray());

            connect(reply, &QNetworkReply::finished, [this, reply]() {
                if (reply->error() != QNetworkReply::NoError) {
                    QMessageBox::warning(this, "操作失败", "复制文件或文件夹时发生网络错误: " + reply->errorString());
                    isallPasted = false;
                }
                reply->deleteLater();
            });
        }
        if (isallPasted) {
            QMessageBox::information(this, "操作成功", "文件或文件夹已成功复制");
        }
        if(!IsTeam){
            showFileList(UserId, directoryStack.top());
        }
        else ShowFileListForTeam(teamData.TeamId,directoryStack_Team.top());
        hasCopied = false;
        QAction *pasteAction = emptyAreaContextMenu->actions().first();
        pasteAction->setEnabled(false);
    } else if (hasCut) {
        // 原有的剪切粘贴逻辑
        int destinationId;
        if(!IsTeam){
            if (!directoryStack.isEmpty()) {
                destinationId = directoryStack.top(); // 获取当前目录 ID
            } else {
                destinationId = rootId; // 默认根目录
            }
        }
        else{
            if (!directoryStack_Team.isEmpty()) {
                destinationId = directoryStack_Team.top(); // 获取当前目录 ID
            } else {
                destinationId = teamData.RootId; // 默认根目录
            }
        }
        isallPasted = true;
        for (int cutFileId : cutFileIds) {
            QNetworkRequest request;
            request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Move/%1-%2").arg(cutFileId).arg(destinationId)));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);

            QNetworkReply *reply = networkManager->put(request, QByteArray());

            connect(reply, &QNetworkReply::finished, [this, reply]() {
                if (reply->error() != QNetworkReply::NoError) {
                    QMessageBox::warning(this, "操作失败", "移动文件或文件夹时发生网络错误: " + reply->errorString());
                    isallPasted = false;
                }
                reply->deleteLater();
            });
        }
        if (isallPasted) {
            if (!isRestore) {
                QMessageBox::information(this, "操作成功", "文件或文件夹已成功移动");
            } else {
                QMessageBox::information(this, "操作成功", "文件或文件夹已成功恢复");
                isRestore = false;
            }
        }
        if(!IsTeam){
            showFileList(UserId, directoryStack.top());
        }
        else ShowFileListForTeam(teamData.TeamId,directoryStack_Team.top());
        hasCut = false;
        QAction *pasteAction = emptyAreaContextMenu->actions().first();
        pasteAction->setEnabled(false);
    }
}
void MainWindow::onRenameActionTriggered()
{
    if(ui->FileListWidget->getSelectedItemIds().size()>1||ui->FileListWidget_team->getSelectedItemIds().size()>1)
        QMessageBox::warning(this, "重命名失败", "一次只能重命名一个项目！");
    else if (currentRightClickedItem) {
        // 获取当前选中项的完整名称
        QString currentName = currentRightClickedItem->text();
        QString originalName = currentName;

        // 提取文件名和后缀
        QString baseName;
        QString extension;

        // 检查是否有后缀（包含点且不在第一个字符）
        int dotIndex = currentName.lastIndexOf('.');
        if (dotIndex > 0 && dotIndex < currentName.length() - 1) {
            baseName = currentName.left(dotIndex);
            extension = currentName.mid(dotIndex); // 包含点的后缀
        } else {
            baseName = currentName;
            extension = "";
        }

        // 显示输入对话框，预填当前文件名（不含后缀）
        QString newBaseName = QInputDialog::getText(
                    this, "重命名",
                    "请输入新的名称：",
                    QLineEdit::Normal,
                    baseName
                    );

        if (!newBaseName.isEmpty()) {
            // 构建新的完整名称（保留原后缀）
            QString newName = newBaseName + extension;

            // 如果新名称与原名称相同，不执行重命名
            if (newName == originalName) {
                return;
            }

            int fileId = currentRightClickedItem->data(Qt::UserRole).toInt();

            // 发送重命名请求
            QNetworkRequest request;
            request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(fileId)));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            // 忽略证书验证
            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);

            // 构建请求JSON
            QJsonObject json;
            json["name"] = newName;

            QByteArray data = QJsonDocument(json).toJson();
            QNetworkReply *reply = manager->put(request, data);

            connect(reply, &QNetworkReply::finished, [this, reply, newName]() {
                if (reply->error() == QNetworkReply::NoError) {
                    // 更新UI显示
                    currentRightClickedItem->setText(newName);
                    QMessageBox::information(this, "成功", "文件已成功重命名为：" + newName);

                    // 刷新文件列表
                    if(!IsTeam)
                        showFileList(UserId, directoryStack.top());
                    else
                        ShowFileListForTeam(teamData.TeamId, directoryStack_Team.top());
                } else {
                    QMessageBox::warning(this, "失败", "重命名失败: " + reply->errorString());
                }

                reply->deleteLater();
            });
        }
    }
}

void MainWindow::onDeleteActionTriggered()
{
    if(!IsTeam){
        QList<QListWidgetItem*> selectedItems = ui->FileListWidget->selectedItems();
        isallDeleted=true;
        if (!selectedItems.isEmpty()) {
            for (QListWidgetItem* item : selectedItems) {
                int fileId = item->data(Qt::UserRole).toInt();
                QNetworkRequest request;
                request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/MoveToTrashBin/%1-%2").arg(UserId).arg(fileId)));
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QSslConfiguration sslConfig = request.sslConfiguration();
                sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
                request.setSslConfiguration(sslConfig);

                QNetworkReply *reply = manager->put(request, QByteArray());

                connect(reply, &QNetworkReply::finished, [this, reply, item]() {

                    if (reply->error() == QNetworkReply::NoError) {
                        // 从列表中移除该项
                        ui->FileListWidget->takeItem(ui->FileListWidget->row(item));
                        delete item;

                    } else {
                        isallDeleted=false;
                        QMessageBox::warning(this, "删除失败", "网络错误：" + reply->errorString());
                    }
                    reply->deleteLater();
                });
            }
        }
        if(isallDeleted){
            QMessageBox::information(this, "删除成功", "文件或文件夹已全部成功移至回收站。");
        }
    }
    else{
        QList<QListWidgetItem*> selectedItems = ui->FileListWidget_team->selectedItems();

        if (!selectedItems.isEmpty()) {
            // 确认对话框
            QMessageBox::StandardButton boxreply;
            boxreply = QMessageBox::question(
                        this,
                        "确认删除选中的文件",
                        QString("确定要删除文件吗？此操作不可撤销！"),
                        QMessageBox::Yes | QMessageBox::No
                        );

            if (boxreply == QMessageBox::No) {
                return;
            }
            for (QListWidgetItem* item : selectedItems) {
                int fileId = item->data(Qt::UserRole).toInt();
                QNetworkRequest request;
                request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(fileId)));
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QSslConfiguration sslConfig = request.sslConfiguration();
                sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
                request.setSslConfiguration(sslConfig);

                QNetworkReply *reply = manager->deleteResource(request);

                connect(reply, &QNetworkReply::finished, [this, reply, item]() {
                    if (reply->error() == QNetworkReply::NoError) {

                        // 从列表中移除该项
                        ui->FileListWidget_team->takeItem(ui->FileListWidget_team->row(item));
                        delete item;
                    } else {
                        isalltruDeleted=false;
                        QMessageBox::warning(this, "操作失败", "删除文件或文件夹时发生网络错误: " + reply->errorString());
                    }
                    reply->deleteLater();
                });
            }
        }
        if(isalltruDeleted){
            QMessageBox::information(this, "操作成功", "文件或文件夹已全部成功删除");
        }
    }

}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->FileListPage);

    showFileList(UserId,nowId);
}



void MainWindow::showTrashFileList(int ownerId)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/TrashBin/%1").arg(ownerId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            if (!jsonResponse.isNull() && jsonResponse.isArray()) {
                QJsonArray fileList = jsonResponse.array();
                ui->TrashList->clear(); // 清空回收站列表

                // 设置 QListWidget 为图标模式
                ui->TrashList->setViewMode(QListView::IconMode);
                // 设置图标大小为 50x50
                ui->TrashList->setIconSize(QSize(50, 50));
                // 设置项之间的间距
                ui->TrashList->setSpacing(10);

                for (const auto &fileItem : fileList) {
                    QJsonObject fileObj = fileItem.toObject();
                    QString name = fileObj["name"].toString();
                    int id = fileObj["id"].toInt();
                    int type = fileObj["type"].toInt();

                    QIcon icon;
                    if (type == 0) { // 假设 type 为 0 表示文件夹
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
                        } else if(suffix=="ppt"||suffix=="pptx"){
                            icon=QIcon(":/FileIcons/source/PPT.png");
                        }else if(suffix=="jpg"){
                            icon=QIcon(":/FileIcons/source/Jpg.png");
                        }else if(suffix=="rar"){
                            icon=QIcon(":/FileIcons/source/rar.png");
                        }else if(suffix=="xlsx"||suffix=="csv"||suffix=="xls"){
                            icon=QIcon(":/FileIcons/source/EXCEL.png");
                        } else if(suffix=="md"){
                            icon=QIcon(":/FileIcons/source/MD.png");
                        }

                        else {
                            icon = QIcon(":/FileIcons/source/UnNamedFile.png");
                        }
                    }
                    // 缩放图标
                    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(50, 50)));
                    pixmap = pixmap.scaled(QSize(50, 50), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    icon = QIcon(pixmap);

                    QListWidgetItem *item = new QListWidgetItem(icon, name);
                    item->setData(Qt::UserRole, id);
                    item->setData(Qt::UserRole + 1, type);
                    item->setSizeHint(QSize(100, 70));
                    // 设置文件名换行显示
                    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignTop);
                    ui->TrashList->addItem(item);
                }
            }
        } else {
            qDebug() << "Network error while getting trash file list:" << reply->errorString();
        }
        reply->deleteLater();
    });
}
void MainWindow::onTrashItemRightClicked(const QPoint &pos)
{
    QListWidgetItem *item = ui->TrashList->itemAt(pos);
    if (item) {
        currentRightClickedItem = item;
        trashContextMenu->popup(ui->TrashList->viewport()->mapToGlobal(pos));
    }
}
void MainWindow::onRestoreActionTriggered()
{
    isRestore=true;
    QList<QListWidgetItem*> selectedItems = ui->TrashList->selectedItems();
    if (!selectedItems.isEmpty()) {
        cutFileIds.clear();
        for (QListWidgetItem* item : selectedItems) {
            cutFileIds.append(item->data(Qt::UserRole).toInt());
        }
        hasCut = true;
        QAction *pasteAction = emptyAreaContextMenu->actions().first();
        pasteAction->setEnabled(true);
    }
}

void MainWindow::onDeleteFromTrashActionTriggered()
{
    QList<QListWidgetItem*> selectedItems = ui->TrashList->selectedItems();
    isalltruDeleted=true;
    if (!selectedItems.isEmpty()) {
        for (QListWidgetItem* item : selectedItems) {
            int fileId = item->data(Qt::UserRole).toInt();
            QNetworkRequest request;
            request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(fileId)));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);

            QNetworkReply *reply = manager->deleteResource(request);

            connect(reply, &QNetworkReply::finished, [this, reply, item]() {
                if (reply->error() == QNetworkReply::NoError) {

                    // 从列表中移除该项
                    ui->TrashList->takeItem(ui->TrashList->row(item));
                    delete item;
                } else {
                    isalltruDeleted=false;
                    QMessageBox::warning(this, "操作失败", "从回收站删除文件或文件夹时发生网络错误: " + reply->errorString());
                }
                reply->deleteLater();
            });
        }
    }
    if(isalltruDeleted){
        QMessageBox::information(this, "操作成功", "文件或文件夹已全部成功从回收站删除");
    }
}



void MainWindow::on_pushButton_2_clicked()//清空回收站
{
    // 弹出确认对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", "你确定要删除回收站中的所有文件和文件夹吗？",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return; // 用户选择不删除，直接返回
    }

    // 获取回收站列表中的所有项
    QList<QListWidgetItem*> allItems;
    for (int i = 0; i < ui->TrashList->count(); ++i) {
        allItems.append(ui->TrashList->item(i));
    }

    isalltruDeleted = true;
    int totalItems = allItems.size();
    int completedItems = 0;

    if (!allItems.isEmpty()) {
        for (QListWidgetItem* item : allItems) {
            int fileId = item->data(Qt::UserRole).toInt();
            QNetworkRequest request;
            request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(fileId)));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);

            QNetworkReply *reply = manager->deleteResource(request);

            connect(reply, &QNetworkReply::finished, [this, reply, item, &completedItems, totalItems]() {
                if (reply->error() == QNetworkReply::NoError) {
                    // 从列表中移除该项
                    ui->TrashList->takeItem(ui->TrashList->row(item));
                    completedItems++;
                    delete item;
                } else {
                    isalltruDeleted = false;
                    QMessageBox::warning(this, "操作失败", "从回收站删除文件或文件夹时发生网络错误: " + reply->errorString());
                }
                reply->deleteLater();
            });
        }
    }
    if (completedItems==totalItems) {
        if (isalltruDeleted) {
            QMessageBox::information(this, "操作成功", "文件或文件夹已全部成功从回收站删除");
        }
    }
}


void MainWindow::on_stopallprocess_2_clicked() {
    for (int i = 0; i < uploadTasks.size(); ++i) {
        if(uploadTasks[i].status==UploadTask::UploadStatus::InProgress)
            qDebug()<<"inprogress";
        else if(uploadTasks[i].status==UploadTask::UploadStatus::Completed)
            qDebug()<<"completed";
        if(uploadTasks[i].status==UploadTask::UploadStatus::InProgress)
            stopthetask(i);
    }
}
// 辅助函数：将字节数转换为合适的单位
QString bytesToHumanReadable(qint64 bytes) {
    const qint64 kb = 1024;
    const qint64 mb = kb * 1024;
    const qint64 gb = mb * 1024;

    if (bytes >= gb) {
        return QString("%1 GB").arg(static_cast<double>(bytes) / gb, 0, 'f', 2);
    } else if (bytes >= mb) {
        return QString("%1 MB").arg(static_cast<double>(bytes) / mb, 0, 'f', 2);
    } else if (bytes >= kb) {
        return QString("%1 KB").arg(static_cast<double>(bytes) / kb, 0, 'f', 2);
    } else {
        return QString("%1 B").arg(bytes);
    }
}

void MainWindow::on_startallprocess_2_clicked() {
    for (int i = 0; i < uploadTasks.size(); ++i) {
        if(uploadTasks[i].status==UploadTask::UploadStatus::Paused)
            resumethetask(i);
    }

}


void MainWindow::on_upload_clicked() {
    QString selectedFilePath = QFileDialog::getOpenFileName(nullptr, tr("Select a File to Upload"), "", tr("All Files (*.*)"));
    ui->stackedWidget->setCurrentWidget(ui->UploadPage);
    if (!selectedFilePath.isEmpty()) {
        QFileInfo fileInfo(selectedFilePath);
        QString fileName = fileInfo.fileName();
        int fileType = 1;
        int currentParentId = (directoryStack.isEmpty())? rootId : directoryStack.top();

        if (ui->UploadProcessWidget) {
            int row = ui->UploadProcessWidget->rowCount();
            ui->UploadProcessWidget->insertRow(row);

            // 文件名列
            QTableWidgetItem *fileItem = new QTableWidgetItem(fileName);
            ui->UploadProcessWidget->setItem(row, 0, fileItem);

            // 大小列
            qint64 fileSize = fileInfo.size();
            QTableWidgetItem *sizeItem = new QTableWidgetItem(QString("0/%1").arg(bytesToHumanReadable(fileSize)));
            ui->UploadProcessWidget->setItem(row, 1, sizeItem);

            // 进度列 - 添加进度条
            QWidget *progressWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
            QProgressBar *progressBar = new QProgressBar(progressWidget);
            progressBar->setRange(0, 100);
            progressBar->setValue(0);
            progressLayout->addWidget(progressBar);
            progressLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 2, progressWidget);

            // 状态列 - 添加标签
            QWidget *statusWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *statusLayout = new QHBoxLayout(statusWidget);
            QLabel *statusLabel = new QLabel("准备上传", statusWidget);
            statusLabel->setAlignment(Qt::AlignCenter);
            statusLayout->addWidget(statusLabel);
            statusLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 3, statusWidget);

            // 暂停列 - 添加暂停按钮
            QWidget *pauseWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *pauseLayout = new QHBoxLayout(pauseWidget);
            QPushButton *pauseButton = new QPushButton(pauseWidget);
            pauseButton->setIcon(QIcon(":/FileIcons/source/stop.png"));
            pauseButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
            pauseLayout->addWidget(pauseButton);
            pauseLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 4, pauseWidget);

            // 删除列 - 添加删除按钮
            QWidget *deleteWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *deleteLayout = new QHBoxLayout(deleteWidget);
            QPushButton *deleteButton = new QPushButton(deleteWidget);
            deleteButton->setIcon(QIcon(":/FileIcons/source/delete_tool.png"));
            deleteButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
            deleteLayout->addWidget(deleteButton);
            deleteLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 5, deleteWidget);

            QJsonObject fileObj;
            fileObj["name"] = fileName;
            fileObj["parentid"] = currentParentId;
            fileObj["type"] = fileType;
            fileObj["ownerid"] = UserId;
            fileObj["permission"] = 2;

            QJsonDocument jsonDoc(fileObj);
            QByteArray requestData = jsonDoc.toJson();

            QNetworkRequest createRequest(QUrl(GlobalData::instance()->getSharedString()+"/api/FileItem/"));
            createRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QSslConfiguration createSslConfig = createRequest.sslConfiguration();
            createSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            createRequest.setSslConfiguration(createSslConfig);

            QNetworkReply *createReply = networkManager->post(createRequest, requestData);
            connect(createReply, &QNetworkReply::finished, [this, createReply, selectedFilePath, row, fileSize, statusLabel, pauseButton,progressBar, deleteButton]() {
                if (createReply->error() == QNetworkReply::NoError) {
                    QByteArray responseData = createReply->readAll();
                    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                    QJsonObject responseObj = jsonResponse.object();
                    int fileId = responseObj["id"].toInt();

                    UploadTask task;
                    task.filePath = selectedFilePath;
                    task.fileId = fileId;
                    task.uploadedBytes = 0;
                    task.fileSize = fileSize;
                    task.reply = nullptr;
                    task.isPaused = false;
                    task.status = UploadTask::UploadStatus::InProgress;
                    task.pauseButton = pauseButton;
                    task.progressBar = progressBar;
                    task.statusLabel = statusLabel;
                    task.deleteButton = deleteButton;
                    task.taskIndex = row;

                    QIcon btnicon(":/FileIcons/source/stop.png");
                    task.pauseButton->setIcon(btnicon);
                    task.pauseButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
                    task.pauseButton->setVisible(true);

                    uploadTasks.append(task);

                    // 连接按钮事件
                    connect(task.pauseButton, &QPushButton::clicked, [this, row]() {
                        handlePauseButtonClick(row);
                    });

                    connect(task.deleteButton, &QPushButton::clicked, [this, row]() {
                        handleDeleteButtonClick(row);
                    });

                    QFile file(task.filePath);
                    if (file.open(QIODevice::ReadOnly)) {
                        qint64 uploadedBytes = 0;
                        const int maxRetries = 3; // 最大重试次数
                        qint64 startTime = QDateTime::currentMSecsSinceEpoch(); // 记录开始时间用于计算速度

                        while (uploadedBytes < fileSize &&!task.isPaused && task.status == UploadTask::UploadStatus::InProgress) {
                            qint64 start = uploadedBytes;
                            qint64 end = qMin(start + chunkSize - 1, fileSize - 1);
                            qint64 chunkLength = end - start + 1;
                            file.seek(start);
                            QByteArray chunkData = file.read(chunkLength);

                            QNetworkRequest appendRequest(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/AppendContent/%1").arg(fileId)));
                            appendRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
                            appendRequest.setRawHeader("Content-Range", QString("bytes %1-%2/%3").arg(start).arg(end).arg(fileSize).toUtf8());
                            QSslConfiguration appendSslConfig = appendRequest.sslConfiguration();
                            appendSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
                            appendRequest.setSslConfiguration(appendSslConfig);

                            int retryCount = 0;
                            bool uploadSuccess = false;
                            qint64 chunkStartTime = QDateTime::currentMSecsSinceEpoch();

                            while (retryCount < maxRetries &&!uploadSuccess) {
                                task.reply = networkManager->put(appendRequest, chunkData);
                                int taskIndex = uploadTasks.size() - 1;
                                uploadTasks[taskIndex].reply = task.reply;

                                QEventLoop loop;
                                connect(task.reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                                loop.exec();

                                if (task.reply->error() == QNetworkReply::NoError) {
                                    qint64 chunkEndTime = QDateTime::currentMSecsSinceEpoch();
                                    qint64 chunkDuration = chunkEndTime - chunkStartTime;

                                    // 计算上传速度 (bytes/sec)
                                    if (chunkDuration > 0) {
                                        qint64 speed = (chunkLength * 1000) / chunkDuration;
                                        task.uploadSpeed = speed;

                                        // 计算剩余时间
                                        qint64 remainingBytes = fileSize - uploadedBytes;
                                        qint64 estimatedTime = remainingBytes / speed;

                                        // 更新状态显示
                                        QString speedText = QString("%1/s").arg(bytesToHumanReadable(speed));
                                        QString timeText = estimatedTime > 0 ?
                                                    QString("剩余: %1").arg(secondsToTimeString(estimatedTime)) : "即将完成";
                                        task.statusLabel->setText(QString("%1 • %2").arg(speedText).arg(timeText));
                                    }

                                    uploadedBytes += chunkLength;
                                    task.uploadedBytes = uploadedBytes;
                                    uploadTasks[uploadTasks.size()-1] = task;

                                    // 使用转换后的单位显示上传进度
                                    QTableWidgetItem *sizeItem = ui->UploadProcessWidget->item(row, 1);
                                    sizeItem->setText(QString("%1/%2").arg(bytesToHumanReadable(uploadedBytes)).arg(bytesToHumanReadable(fileSize)));
                                    task.progressBar->setValue(qMax((100*uploadedBytes/fileSize)-1, 0));

                                    uploadSuccess = true;
                                } else {
                                    qDebug() << "Failed to upload chunk (attempt " << retryCount + 1 << "): " << task.reply->errorString();
                                    retryCount++;
                                    QThread::msleep(500); // 每次重试之间延迟 500 毫秒
                                }
                            }

                            if (!uploadSuccess) {
                                qDebug() << "Failed to upload chunk after " << maxRetries << " attempts.";
                                task.status = UploadTask::UploadStatus::Error;
                                task.statusLabel->setText("上传错误");
                                break;
                            }
                        }

                        if(task.uploadedBytes == task.fileSize){
                            task.progressBar->setValue(100);
                            task.progressBar->setFormat("完成");
                            task.statusLabel->setText("已完成");
                            task.pauseButton->setVisible(false);
                            task.deleteButton->setVisible(false);
                            file.close();
                        }
                    } else {
                        qDebug() << "Failed to open file";
                        task.status = UploadTask::UploadStatus::Error;
                        task.statusLabel->setText("文件打开错误");
                    }
                } else {
                    qDebug() << "Error creating file: " << createReply->errorString();
                    qDebug() << "Server response:" << createReply->readAll();
                    UploadTask task;
                    task.filePath = selectedFilePath;
                    task.fileId = -1;
                    task.uploadedBytes = 0;
                    task.fileSize = 0;
                    task.reply = nullptr;
                    task.isPaused = false;
                    task.status = UploadTask::UploadStatus::Error;
                    task.statusLabel = statusLabel;
                    task.pauseButton = pauseButton;
                    task.deleteButton = deleteButton;
                    uploadTasks.append(task);
                    task.statusLabel->setText("创建文件错误");
                }
                createReply->deleteLater();
            });
        }
    }
}

// 处理暂停按钮点击事件（需在类中实现）
void MainWindow::handlePauseButtonClick(int row) {
    if (row < 0 || row >= uploadTasks.size()) return;

    UploadTask &task = uploadTasks[row];
    if (task.isPaused) {

        task.statusLabel->setText("恢复上传中...");
        resumethetask(row);
    } else {

        stopthetask(row);

    }
}

// 处理删除按钮点击事件（需在类中实现）
void MainWindow::handleDeleteButtonClick(int row) {
    if (row < 0 || row >= uploadTasks.size()) return;

    UploadTask &task = uploadTasks[row];
    if (QMessageBox::Yes != QMessageBox::question(this, "确认删除", "确定要删除此上传任务吗？",
                                                  QMessageBox::Yes | QMessageBox::No)) {
        return;
    }
    if(task.status==UploadTask::UploadStatus::Completed){
        // 从表格和任务列表中移除
        ui->UploadProcessWidget->removeRow(row);
        uploadTasks.removeAt(row);
        return;
    }
    if(task.status==UploadTask::UploadStatus::InProgress){
        QMessageBox::warning(this, "操作失败", "请先暂停上传任务!" );
    }
    if (task.status == UploadTask::UploadStatus::Paused) {
        // 删除已创建的文件
        int fileId = task.fileId;
        QNetworkRequest request;
        request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(fileId)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);
        QNetworkReply *reply = manager->deleteResource(request);
        connect(reply, &QNetworkReply::finished, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
            } else {
                QMessageBox::warning(this, "删除失败", "网络错误：" + reply->errorString());
            }
            reply->deleteLater();
        });

        // 从表格和任务列表中移除
        ui->UploadProcessWidget->removeRow(row);
        uploadTasks.removeAt(row);

        // 更新剩余任务索引
        for (int i = row; i < uploadTasks.size(); i++) {
            uploadTasks[i].taskIndex = i;
        }
    }
}

// 字节转换为可读格式（需在类中实现）
QString MainWindow::bytesToHumanReadable(qint64 bytes) {
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024), 0, 'f', 1);
    return QString("%1 GB").arg(bytes / (1024.0 * 1024 * 1024), 0, 'f', 1);
}

// 秒转换为时分秒格式（需在类中实现）
QString MainWindow::secondsToTimeString(qint64 seconds) {
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;
    if (h > 0) return QString("%1h%2m%3s").arg(h).arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
    if (m > 0) return QString("%1m%2s").arg(m).arg(s, 2, 10, QChar('0'));
    return QString("%1s").arg(s);
}
void MainWindow::stopthetask(int i){
    if (uploadTasks[i].reply) {
        if (!uploadTasks[i].reply->isFinished()) {
            qDebug()<<"正确转换信号";
            // 正确转换 finished 信号
            disconnect(uploadTasks[i].reply, static_cast<void (QNetworkReply::*)()>(&QNetworkReply::finished), nullptr, nullptr);
            // 正确转换 error 信号
            disconnect(uploadTasks[i].reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::errorOccurred), nullptr, nullptr);
        }
        uploadTasks[i].reply->abort();
        uploadTasks[i].reply->deleteLater();
        uploadTasks[i].reply = nullptr;
    }
    QIcon btnicon(":/FileIcons/source/resume.png");
    uploadTasks[i].pauseButton->setIcon(btnicon);
    uploadTasks[i].isPaused=true;
    uploadTasks[i].status=UploadTask::UploadStatus::Paused;
    uploadTasks[i].statusLabel->setText("已暂停");
}
void MainWindow::resumethetask(int i)
{
    if (i < 0 || i >= uploadTasks.size()) return;

    UploadTask &task = uploadTasks[i];

    // 检查任务是否存在且处于暂停状态
    if (task.status != UploadTask::UploadStatus::Paused) {
        qDebug() << "Task is not paused, cannot resume";
        return;
    }

    task.isPaused = false;
    task.status = UploadTask::UploadStatus::InProgress;
    QIcon btnicon(":/FileIcons/source/stop.png");
    task.pauseButton->setIcon(btnicon);
    task.statusLabel->setText("恢复上传...");

    int fileId = task.fileId;
    qint64 initialUploadedBytes = task.uploadedBytes;

    // 发起获取文件大小的网络请求
    QNetworkRequest sizeRequest(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Size/%1").arg(fileId)));
    QSslConfiguration sslConfig = sizeRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sizeRequest.setSslConfiguration(sslConfig);

    QNetworkReply *sizeReply = networkManager->get(sizeRequest);
    connect(sizeReply, &QNetworkReply::finished, [this, i, sizeReply, initialUploadedBytes]() {
        if (sizeReply->error() == QNetworkReply::NoError) {
            QByteArray responseData = sizeReply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObj = jsonResponse.object();
            qint64 serverSize = responseObj["size"].toVariant().toLongLong();

            // 更新已上传字节数
            uploadTasks[i].uploadedBytes = serverSize;
            qDebug() << "已接收字节" << serverSize;

            // 开始恢复上传
            resumeUpload(i);
        } else {
            qDebug() << "获取文件大小出错: " << sizeReply->errorString();
            uploadTasks[i].status = UploadTask::UploadStatus::Error;
            uploadTasks[i].statusLabel->setText("获取文件大小失败");
        }
        sizeReply->deleteLater();
    });
}

// 分离的上传逻辑，便于更新状态
void MainWindow::resumeUpload(int i)
{
    UploadTask &task = uploadTasks[i];

    QFile file(task.filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件:" << task.filePath;
        task.status = UploadTask::UploadStatus::Error;
        task.statusLabel->setText("文件打开失败");
        return;
    }

    qint64 fileSize = task.fileSize;
    qint64 uploadedBytes = task.uploadedBytes;
    const int maxRetries = 3;

    // 记录开始时间，用于计算平均速度
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    while (uploadedBytes < fileSize && !task.isPaused && task.status == UploadTask::UploadStatus::InProgress) {
        qint64 start = uploadedBytes;
        qint64 end = qMin(start + chunkSize - 1, fileSize - 1);
        qint64 chunkLength = end - start + 1;

        file.seek(start);
        QByteArray chunkData = file.read(chunkLength);

        QNetworkRequest appendRequest(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/AppendContent/%1").arg(task.fileId)));
        appendRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        appendRequest.setRawHeader("Content-Range", QString("bytes %1-%2/%3").arg(start).arg(end).arg(fileSize).toUtf8());
        QSslConfiguration appendSslConfig = appendRequest.sslConfiguration();
        appendSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        appendRequest.setSslConfiguration(appendSslConfig);

        int retryCount = 0;
        bool uploadSuccess = false;
        qint64 chunkStartTime = QDateTime::currentMSecsSinceEpoch();

        while (retryCount < maxRetries && !uploadSuccess) {
            task.reply = networkManager->put(appendRequest, chunkData);

            QEventLoop loop;
            connect(task.reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            if (task.reply->error() == QNetworkReply::NoError) {
                qint64 chunkEndTime = QDateTime::currentMSecsSinceEpoch();
                qint64 chunkDuration = chunkEndTime - chunkStartTime;

                // 计算当前块的上传速度 (bytes/sec)
                if (chunkDuration > 0) {
                    task.uploadSpeed = (chunkLength * 1000) / chunkDuration;
                }

                // 更新已上传字节数
                uploadedBytes += chunkLength;
                task.uploadedBytes = uploadedBytes;

                // 计算总上传时间和平均速度
                qint64 totalTime = chunkEndTime - startTime;
                qint64 averageSpeed = totalTime > 0 ? (uploadedBytes * 1000) / totalTime : 0;

                // 更新UI
                updateTaskStatus(i, uploadedBytes, fileSize, task.uploadSpeed);

                uploadSuccess = true;
            } else {
                qDebug() << "Failed to upload chunk (attempt" << retryCount + 1 << "):" << task.reply->errorString();
                retryCount++;
                QThread::msleep(500);
            }

            task.reply->deleteLater();
        }

        if (!uploadSuccess) {
            qDebug() << "Failed to upload chunk after" << maxRetries << "attempts.";
            task.status = UploadTask::UploadStatus::Error;
            task.statusLabel->setText("上传失败");
            break;
        }
    }

    if (task.uploadedBytes == task.fileSize) {
        task.progressBar->setValue(100);
        task.progressBar->setFormat("完成");
        task.statusLabel->setText("已完成");
        task.pauseButton->setVisible(false);
        task.status = UploadTask::UploadStatus::Completed;
        file.close();
    }
}

// 更新任务状态的辅助函数
void MainWindow::updateTaskStatus(int i, qint64 uploadedBytes, qint64 totalBytes, qint64 speed)
{
    if (i < 0 || i >= uploadTasks.size()) return;

    UploadTask &task = uploadTasks[i];

    // 更新进度条
    int progress = qMin((100 * uploadedBytes / totalBytes), 100);
    task.progressBar->setValue(progress);

    // 更新大小显示
    QTableWidgetItem *sizeItem = ui->UploadProcessWidget->item(i, 1);
    if (sizeItem) {
        sizeItem->setText(QString("%1/%2").arg(
                              bytesToHumanReadable(uploadedBytes),
                              bytesToHumanReadable(totalBytes)));
    }

    // 更新状态标签（显示速度和剩余时间）
    if (speed > 0 && uploadedBytes < totalBytes) {
        qint64 remainingBytes = totalBytes - uploadedBytes;
        qint64 estimatedTime = remainingBytes / speed;

        QString speedText = QString("%1/s").arg(bytesToHumanReadable(speed));
        QString timeText = estimatedTime > 0 ?
                    QString("剩余: %1").arg(secondsToTimeString(estimatedTime)) : "即将完成";

        task.statusLabel->setText(QString("%1 • %2").arg(speedText).arg(timeText));
    } else {
        task.statusLabel->setText("上传中...");
    }
}

void MainWindow::onLoadActionTriggered(){
    ui->stackedWidget->setCurrentWidget(ui->ProgressPage);
    // 获取选中的文件

    QList<QListWidgetItem *> selectedItems = ui->FileListWidget->selectedItems(); // 假设 FileListWidget 是显示文件列表的控件
    if(!IsTeam){
        selectedItems = ui->FileListWidget->selectedItems();
    }
    else {
        selectedItems = ui->FileListWidget_team->selectedItems();
    }
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
void MainWindow::onNewFolderTriggered()
{
    // 弹出输入对话框让用户输入文件夹名称
    bool ok;
    QString folderName = QInputDialog::getText(this, tr("新建文件夹"), tr("请输入文件夹名称:"), QLineEdit::Normal, "", &ok);

    if (ok && !folderName.isEmpty()) {
        // 获取当前父文件夹的 ID
        int currentParentId;
        if(!IsTeam){
            if (directoryStack.empty()) {
                currentParentId = rootId;
            } else {
                currentParentId = directoryStack.top();
            }
        }
        else{
            if (directoryStack_Team.empty()) {
                currentParentId = rootId;
            } else {
                currentParentId = directoryStack_Team.top();
            }
        }

        // 构造创建文件夹的请求数据
        QJsonObject folderObj;
        folderObj["name"] = folderName;
        folderObj["parentid"] = currentParentId;
        folderObj["type"] = 0; // 0 表示文件夹
        folderObj["ownerid"] = UserId;
        folderObj["permission"] = 2; // 默认权限为 2

        QJsonDocument jsonDoc(folderObj);
        QByteArray requestData = jsonDoc.toJson();

        // 创建网络请求
        QNetworkRequest request(QUrl(GlobalData::instance()->getSharedString()+"/api/FileItem/"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // 忽略证书验证
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);

        // 发送 POST 请求创建文件夹
        QNetworkReply *reply = networkManager->post(request, requestData);
        connect(reply, &QNetworkReply::finished, [this, reply, folderName]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                QJsonObject responseObj = jsonResponse.object();
                int folderId = responseObj["id"].toInt();
                qDebug() << "文件夹创建成功，ID: " << folderId;
                // 在 ui->FileListWidget 中添加新建的文件夹
                QListWidgetItem *newFolderItem = new QListWidgetItem(QIcon(":/FileIcons/source/Dir.png"), folderName);
                newFolderItem->setData(Qt::UserRole, folderId); // 存储文件夹 ID 作为用户数据
                if(!IsTeam)
                    ui->FileListWidget->addItem(newFolderItem);
                else ui->FileListWidget_team->addItem(newFolderItem);

            } else {
                qDebug() << "创建文件夹时出错: " << reply->errorString();
                qDebug() << "服务器响应: " << reply->readAll();
            }
            reply->deleteLater();
        });
    }
}







void MainWindow::on_stopallprocess_clicked()
{

    for (int i = 0; i < ui->ProgessWidget->rowCount(); ++i) {
        if(!ui->ProgessWidget->downloadTasks[i].isFinished)
            ui->ProgessWidget->pauseDownload(i,isstopallload);
    }
    isstopallload=true;
}

void MainWindow::on_startallprocess_clicked()
{
    for (int i = 0; i < ui->ProgessWidget->rowCount(); ++i) {
        if(ui->ProgessWidget->downloadTasks[i].isPaused)
            ui->ProgessWidget->resumeDownload(i);
    }
    isstopallload=false;
}
void MainWindow::PreviewFile(int fileId){
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
                HandleVideoDialog* dialog=new HandleVideoDialog(videoUrl, this,type);
                dialog->show();
                dialog->setAttribute(Qt::WA_DeleteOnClose);
            }
            else if (fileName.endsWith(".png", Qt::CaseInsensitive) || fileName.endsWith(".jpg", Qt::CaseInsensitive))
            {
                qDebug()<<"图片为"<<fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl imageUrl = QUrl(viewUrl);
                // 创建并显示图片预览对话框
                HandleImageDialog* dialog=new HandleImageDialog(imageUrl, this);
                dialog->show();
                dialog->setAttribute(Qt::WA_DeleteOnClose);

            }else if (fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
                qDebug() << "PDF 文件为" << fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl fileUrl = QUrl(viewUrl);
                // 创建并显示文件预览对话框
                HandlePdfWordDialog* dialog=new HandlePdfWordDialog(fileUrl, this);
                dialog->show();
                dialog->setAttribute(Qt::WA_DeleteOnClose);
            } else if (fileName.endsWith(".doc", Qt::CaseInsensitive) || fileName.endsWith(".docx", Qt::CaseInsensitive)) {
                qDebug() << "Word 文件为" << fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl fileUrl = QUrl(viewUrl);
                // 创建并显示文件预览对话框
                HandlePdfWordDialog* dialog=new HandlePdfWordDialog(fileUrl, this,1);
                dialog->show();
                dialog->setAttribute(Qt::WA_DeleteOnClose);
            }else if (fileName.endsWith(".txt", Qt::CaseInsensitive)) {
                qDebug() << "txt 文件为" << fileName;
                // 构建在线预览的请求 URL
                QString viewUrl = QString(GlobalData::instance()->getSharedString()+"/api/FileItem/View/%1").arg(fileId);
                QUrl fileUrl = QUrl(viewUrl);
                // 创建并显示文件预览对话框
                HandleTxtDialog* dialog=new HandleTxtDialog(fileUrl, this);
                dialog->show();
                dialog->setAttribute(Qt::WA_DeleteOnClose);
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
    if (!connected) {
        qDebug() << "Signal-slot connection failed!";
    }

    connect(reply, &QNetworkReply::errorOccurred, [](QNetworkReply::NetworkError code) {
        qDebug() << "Network request error:" << code;
    });
}
void MainWindow::PreviewItem()
{
    if (ui->FileListWidget->getSelectedItemIds().size() > 1||ui->FileListWidget_team->getSelectedItemIds().size()>1)
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

void MainWindow::onStackedWidgetCurrentChanged(int index)
{
    // 清除所有按钮的选中样式
    ui->toolButton->setStyleSheet("");
    ui->toolButton_2->setStyleSheet("");
    ui->toolButton_3->setStyleSheet("");
    ui->toolButton_4->setStyleSheet("");
    ui->toolButton_5->setStyleSheet("");
    ui->toolButton_6->setStyleSheet("");
    ui->toolButton_7->setStyleSheet("");
    ui->toolButton_8->setStyleSheet("");
    // 根据当前页面设置对应的按钮样式
    switch (index) {
    case 0: // FileListPage
        ui->toolButton->setStyleSheet("background-color: blue; color: white;");
        break;
    case 1: // UploadPage
        ui->toolButton_2->setStyleSheet("background-color: blue; color: white;");
        break;
    case 2: // ProgressPage
        ui->toolButton_4->setStyleSheet("background-color: blue; color: white;");
        break;
    case 3: // TrashPage
        ui->toolButton_3->setStyleSheet("background-color: blue; color: white;");
        break;
    case 4: // TrashPage
        ui->toolButton_5->setStyleSheet("background-color: blue; color: white;");
        break;
    case 5: // TrashPage
        ui->toolButton_6->setStyleSheet("background-color: blue; color: white;");
        break;
    case 6: // TrashPage
        ui->toolButton_7->setStyleSheet("background-color: blue; color: white;");
        break;
    case 7:
        ui->toolButton_8->setStyleSheet("background-color: blus;color:white;");
    default:
        break;
    }
}
QString MainWindow::getUserName(int userId)
{
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/Account/%1").arg(userId);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QNetworkReply *reply = manager->get(request);

    // 使用事件循环等待请求完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString userName;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if (jsonDoc.isObject()) {
            QJsonObject obj = jsonDoc.object();
            if (obj.contains("userName")) {
                userName = obj["userName"].toString();
                UserEmail=obj["email"].toString();
            }

        }
    } else {
        QMessageBox::warning(this, "网络错误", reply->errorString());
    }

    reply->deleteLater();
    return userName;
}

void MainWindow::getAvatarAndSet(int userId, QLabel *targetLabel)
{
    // 构建请求URL（确保与后端路由一致）
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/Account/Avatar/%1").arg(userId);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Accept", "image/*");

    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QNetworkReply* reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, targetLabel, userId]() {
        // 打印响应状态码
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


        // 打印响应头Content-Type
        QByteArray contentType = reply->header(QNetworkRequest::ContentTypeHeader).toByteArray();


        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            // 保存数据用于调试
            QFile debugFile(QString("avatar_debug_%1.dat").arg(userId));
            if (debugFile.open(QIODevice::WriteOnly)) {
                debugFile.write(imageData);
                debugFile.close();
            }
            QImage image;
            if (image.loadFromData(imageData)) {
                QPixmap pixmap = QPixmap::fromImage(image);
                targetLabel->setPixmap(pixmap.scaled(
                                           targetLabel->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation
                                           ));
            } else {
                // 设置默认头像
                QPixmap defaultAvatar(":/FileIcons/source/default.jpg"); // 假设存在默认头像资源
                if (defaultAvatar.isNull()) {
                    // 如果没有默认头像资源，显示占位符
                    targetLabel->setText("A");
                    targetLabel->setAlignment(Qt::AlignCenter);
                    targetLabel->setStyleSheet("border-radius: 20px; background-color: #cccccc; color: white;");
                } else {
                    targetLabel->setPixmap(defaultAvatar.scaled(
                                               targetLabel->size(),
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation
                                               ));
                }

            }
        } else {
            // 设置默认头像
            QPixmap defaultAvatar(":/FileIcons/source/default.jpg");
            if (defaultAvatar.isNull()) {
                targetLabel->setText("A");
                targetLabel->setAlignment(Qt::AlignCenter);
                targetLabel->setStyleSheet("border-radius: 20px; background-color: #cccccc; color: white;");
            } else {
                targetLabel->setPixmap(defaultAvatar.scaled(
                                           targetLabel->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation
                                           ));
            }

        }
        QFile::remove(QString("avatar_debug_%1.dat").arg(userId));
        reply->deleteLater();
    });
}
void MainWindow::receiveId(int UserId, int rootId, int trashBinId,TeamData td)
{
    this->UserId = UserId;
    this->rootId = rootId;
    this->trashBinId = trashBinId;
    this->teamData=td;
    ui->stackedWidget->setCurrentWidget(ui->FileListPage);
    QString Username=getUserName(UserId);
    ui->UserNameLabel->setText(Username);
    ui->UsernameLine->setText(Username);
    ui->EmailLine->setText(UserEmail);
    getAvatarAndSet(UserId,ui->png_label);
    getAvatarAndSet(UserId,ui->png_label_2);
    showFileList(UserId, rootId);
}



void MainWindow::on_toolButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->FileListPage);
    UpdateBtnCol(0);
    showFileList(UserId,nowId);
}


void MainWindow::on_toolButton_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->UploadPage);
    UpdateBtnCol(1);
}


void MainWindow::on_toolButton_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ProgressPage);
    UpdateBtnCol(2);
}


void MainWindow::on_toolButton_4_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->TrashPage);
    showTrashFileList(UserId);
    UpdateBtnCol(3);
}


void MainWindow::on_toolButton_5_clicked()
{
    ShareListDialog* dia=new ShareListDialog(this);
    QString link;
    QString code;
    bool iszidong=false;
    // 获取系统剪贴板对象
    QClipboard *clipboard = QGuiApplication::clipboard();
    // 获取剪贴板中的文本内容
    QString clipboardText = clipboard->text();
    // 检查剪贴板内容是否符合特定格式
    if (clipboardText.startsWith("通过轻协云储分享的文件：  链接：") && clipboardText.contains(" 提取码：")) {
        // 提取链接部分
        int linkStart = clipboardText.indexOf("链接：") + 3;
        int linkEnd = clipboardText.indexOf("?code=");
        if (linkEnd != -1) {
            iszidong=true;
            link = clipboardText.mid(linkStart, linkEnd - linkStart);
        }
        // 提取提取码部分
        int codeStart = clipboardText.indexOf("提取码：") + 4;
        code= clipboardText.mid(codeStart);
    }
    if(iszidong){
        QMessageBox::information(this, "自动填充", "自动填充链接成功");
        dia->getlink(link, code);
    }
    dia->getUserId(UserId);
    dia->show();
    dia->setAttribute(Qt::WA_DeleteOnClose);
    UpdateBtnCol(4);

}


void MainWindow::on_toolButton_6_clicked()
{
    LoginDialog dialog(this);
    connect(&dialog, &LoginDialog::sendID, this, &MainWindow::receiveId);
    dialog.exec();
    UpdateBtnCol(5);
}


void MainWindow::updateUploadProcessWidget()
{
    ui->UploadProcessWidget->clearContents();
    ui->UploadProcessWidget->setRowCount(uploadTasks.size());
    for (int i = 0; i < uploadTasks.size(); ++i) {
        UploadTask& task = uploadTasks[i];
        QTableWidgetItem *fileItem = new QTableWidgetItem(task.filePath.split('/').last());
        ui->UploadProcessWidget->setItem(i, 0, fileItem);

        QTableWidgetItem *sizeItem = new QTableWidgetItem(QString("%1/%2").arg(bytesToHumanReadable(task.uploadedBytes)).arg(bytesToHumanReadable(task.fileSize)));
        ui->UploadProcessWidget->setItem(i, 1, sizeItem);

        ui->UploadProcessWidget->setCellWidget(i, 2, task.progressBar);
        ui->UploadProcessWidget->setCellWidget(i, 3, task.pauseButton);
    }
}

void MainWindow::on_deleteallprocess_2_clicked()
{
    // 确认对话框
    QMessageBox::StandardButton boxreply;
    boxreply = QMessageBox::question(
                this,
                "确认删除所有上传任务",
                QString("确认删除所有上传任务？此操作不可撤销！"),
                QMessageBox::Yes | QMessageBox::No
                );

    if (boxreply == QMessageBox::No) {
        return;
    }
    for(int i=0;i<uploadTasks.size();++i){
        UploadTask& task = uploadTasks[i];
        if(task.status==UploadTask::UploadStatus::InProgress){
            QMessageBox::warning(this, "删除失败", "请先停止上传任务！" );
            return;
        }
    }
    for (int i = 0; i < uploadTasks.size(); ++i) {
        UploadTask& task = uploadTasks[i];
        if (task.status == UploadTask::UploadStatus::Paused) {

            // 删除已创建的文件
            int fileId = task.fileId;            
            QNetworkRequest request;
            request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/%1").arg(fileId)));
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            QSslConfiguration sslConfig = request.sslConfiguration();
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslConfig);
            QNetworkReply *reply = manager->deleteResource(request);


            connect(reply, &QNetworkReply::finished, [this, reply, i]() {
                if (reply->error() == QNetworkReply::NoError) {
                } else {
                    QMessageBox::warning(this, "删除失败", "网络错误：" + reply->errorString());
                }
                reply->deleteLater();
            });
        }
    }
    for(int i=0;i<uploadTasks.size();i++){

        ui->UploadProcessWidget->removeRow(i);
    }
    uploadTasks.clear();

}


void MainWindow::on_refreshBtn_clicked()
{
    showFileList(UserId,nowId);
}

void MainWindow::refreshTeamPage(){
    if(teamData.TeamId == -1) {
        // 没有进团队，显示团队选择页面
        ui->stackedWidget->setCurrentWidget(ui->page_noteam);
        return;
    }
    ui->stackedWidget->setCurrentWidget(ui->page_team);
    ui->TeamNameLabel->setText(teamData.name);
    // 有团队，获取团队成员列表
    QNetworkRequest request;
    request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/TeamInfo/Members/%1").arg(teamData.TeamId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证（开发环境）
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // 发送请求
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        // 检查网络错误
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

            // 检查JSON格式是否正确
            if (jsonResponse.isArray()) {
                QJsonArray membersArray = jsonResponse.array();

                // 清空现有列表
                ui->TeamListWidget->clear();
                ui->TeamListWidget->setViewMode(QListView::ListMode); // 设置为列表模式
                ui->TeamListWidget->setSpacing(5); // 设置项间距
                ui->TeamListWidget->setStyleSheet("QListWidget { border: none; }");

                // 遍历成员列表并添加到UI
                for (const auto &memberValue : membersArray) {
                    QJsonObject memberObj = memberValue.toObject();
                    // 提取成员信息
                    int accountId = memberObj["accountId"].toInt();
                    QString userName = memberObj["userName"].toString();
                    int role = memberObj["role"].toInt(); // 获取角色值

                    // 创建自定义项部件
                    QWidget *itemWidget = new QWidget();
                    QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
                    itemLayout->setContentsMargins(0, 0, 0, 0);
                    itemLayout->setSpacing(10);

                    // 创建头像标签
                    QLabel *avatarLabel = new QLabel();
                    avatarLabel->setFixedSize(40, 40);
                    avatarLabel->setStyleSheet("border-radius: 20px; background-color: #f0f0f0;");
                    itemLayout->addWidget(avatarLabel);

                    // 创建用户名标签
                    QLabel *nameLabel = new QLabel(userName);
                    QFont font = nameLabel->font();

                    if (role == 0) { // role为0表示队长
                        font.setBold(true);
                        nameLabel->setText(nameLabel->text());
                    }

                    if (accountId == UserId) {
                        nameLabel->setText(nameLabel->text() + " (我)");
                    }

                    nameLabel->setFont(font);
                    itemLayout->addWidget(nameLabel);

                    // 扩展布局以填充空间
                    itemLayout->addStretch();

                    // 为队长添加角色标识
                    if (role == 0) {
                        QLabel *roleLabel = new QLabel("队长");
                        roleLabel->setAlignment(Qt::AlignCenter);
                        roleLabel->setStyleSheet("background-color: #add8e6; color: #00008b; border-radius: 4px; padding: 2px 8px;");
                        roleLabel->setFixedHeight(24);
                        itemLayout->addWidget(roleLabel);
                    }

                    // 创建列表项并设置大小
                    QListWidgetItem *listItem = new QListWidgetItem();
                    listItem->setSizeHint(QSize(ui->TeamListWidget->width(), 50));
                    listItem->setData(Qt::UserRole, accountId);

                    // 将部件添加到列表项
                    ui->TeamListWidget->addItem(listItem);
                    ui->TeamListWidget->setItemWidget(listItem, itemWidget);

                    // 获取头像并显示
                    getAvatarAndSet(accountId, avatarLabel);
                }
            } else {
                QMessageBox::warning(this, "获取失败", "服务器返回格式错误");
            }
        } else {
            QMessageBox::warning(this, "获取失败", "网络错误: " + reply->errorString());
        }

        // 释放资源
        reply->deleteLater();
    });

    // 更新按钮颜色
    UpdateBtnCol(6);
}
void MainWindow::on_toolButton_7_clicked()
{
    if(teamData.role==1){
        ui->Add_btn->setEnabled(false);
        ui->Dismiss_btn->setEnabled(false);
        ui->Remove_btn->setEnabled(false);
        ui->Changename_btn->setEnabled(false);
        ui->groupBox_2->setVisible(false);
    }
    refreshTeamPage();
    ShowFileListForTeam(teamData.TeamId,teamData.RootId);
}


void MainWindow::on_createTeam_btn_clicked()
{
    // 获取团队名称输入
    QString teamName = QInputDialog::getText(this, "创建团队", "请输入团队名称:", QLineEdit::Normal, "新团队");
    if (teamName.isEmpty()) {
        QMessageBox::warning(this, "创建失败", "团队名称不能为空");
        return;
    }
    // 构建请求
    QNetworkRequest request;
    request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/TeamInfo/Account/%1").arg(UserId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    // 构建请求参数
    QJsonObject json;
    json["name"] = teamName;
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkReply *reply = manager->post(request, data);
    connect(reply, &QNetworkReply::finished, [this, reply, teamName]() {

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject.contains("teamId") && jsonObject.contains("rootId") &&
                    jsonObject.contains("name") && jsonObject.contains("role")) {
                // 解析创建的团队信息
                int teamId = jsonObject["teamId"].toInt();
                int rootId = jsonObject["rootId"].toInt();
                QString name = jsonObject["name"].toString();
                int role = jsonObject["role"].toInt();

                // 更新团队信息结构体
                teamData.TeamId = teamId;
                teamData.RootId = rootId;
                teamData.name = name;
                teamData.role = 0; // 所有者角色

                // 显示成功消息
                QMessageBox::information(this, "创建成功",
                                         QString("团队 '%1' 创建成功！\n团队ID: %2\n根目录ID: %3")
                                         .arg(name)
                                         .arg(teamId)
                                         .arg(rootId));

                // 刷新文件列表或执行其他需要团队信息的操作
                showFileList(UserId, rootId);
            } else if (jsonObject.contains("message")) {
                QString message = jsonObject["message"].toString();
                QMessageBox::warning(this, "创建失败", message);
            } else {
                qDebug() << "Unexpected JSON data:" << jsonObject;
                QMessageBox::warning(this, "创建失败", "服务器返回未知数据");
            }
        } else {
            QMessageBox::warning(this, "创建失败", "网络错误: " + reply->errorString());
        }
        reply->deleteLater();
    });
    refreshTeamPage();
}



void MainWindow::on_toolButton_8_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->My_Data_page);
    UpdateBtnCol(7);
}


void MainWindow::on_upload_btn_clicked()
{
    // 打开文件选择对话框
    QString filePath = QFileDialog::getOpenFileName(
                this,
                "选择头像图片",
                "",
                "图片文件 (*.png *.jpg *.jpeg *.bmp);;所有文件 (*)"
                );

    if (filePath.isEmpty()) {
        return; // 用户取消选择
    }

    // 加载并裁剪图片为正方形
    QImage originalImage(filePath);
    if (originalImage.isNull()) {
        QMessageBox::warning(this, "错误", "无法加载所选图片");
        return;
    }

    // 裁剪为正方形（取最短边为边长）
    int size = qMin(originalImage.width(), originalImage.height());
    QImage croppedImage = originalImage.copy(
                (originalImage.width() - size) / 2,
                (originalImage.height() - size) / 2,
                size,
                size
                );

    // 调整为合适大小（例如200x200像素）
    QImage scaledImage = croppedImage.scaled(
                200, 200,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation
                );

    // 确定文件格式和MIME类型
    QString fileFormat;
    QString mimeType;
    if (filePath.endsWith(".jpg", Qt::CaseInsensitive) || filePath.endsWith(".jpeg", Qt::CaseInsensitive)) {
        fileFormat = "JPG";
        mimeType = "image/jpeg";
    } else if (filePath.endsWith(".png", Qt::CaseInsensitive)) {
        fileFormat = "PNG";
        mimeType = "image/png";
    } else if (filePath.endsWith(".bmp", Qt::CaseInsensitive)) {
        fileFormat = "BMP";
        mimeType = "image/bmp";
    } else {
        // 默认使用JPEG格式
        fileFormat = "JPG";
        mimeType = "image/jpeg";
    }

    // 直接将图片数据写入字节数组
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    if (!scaledImage.save(&buffer, fileFormat.toUtf8())) {
        QMessageBox::warning(this, "错误", "无法转换图片格式");
        return;
    }
    buffer.close();

    // 准备HTTP请求
    QNetworkRequest request;
    request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/Account/Avatar/%1").arg(UserId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, mimeType);

    // 忽略证书验证（开发环境）
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // 发送请求
    QNetworkReply *reply = manager->put(request, imageData);
    connect(reply, &QNetworkReply::finished, [this, reply, scaledImage]() {
        // 检查网络错误
        if (reply->error() == QNetworkReply::NoError) {
            QMessageBox::information(this, "成功", "头像上传成功");

            // 显示为圆形
            QPixmap pixmap = QPixmap::fromImage(scaledImage);
            QPixmap roundedPixmap = pixmap;
            // 在QLabel中显示
            ui->png_label->setPixmap(roundedPixmap.scaled(
                                         ui->png_label->size(),
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation
                                         ));
            ui->png_label_2->setPixmap(roundedPixmap.scaled(
                                           ui->png_label_2->size(),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation
                                           ));
        } else {
            QMessageBox::warning(this, "失败", "头像上传失败: " + reply->errorString());
        }

        // 释放资源
        reply->deleteLater();
    });
}
// 创建圆形头像的辅助函数
QPixmap MainWindow::createRoundedPixmap(const QPixmap &source)
{
    int size = qMin(source.width(), source.height());
    QPixmap target(size, size);
    target.fill(Qt::transparent);

    QPainter painter(&target);
    painter.setRenderHint(QPainter::Antialiasing);

    // 创建圆形裁剪区域
    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);

    // 绘制图像
    painter.drawPixmap(0, 0, size, size, source);
    painter.end();

    return target;
}

void MainWindow::on_changeDate_btn_clicked()
{
    ui->EmailLine->setEnabled(true);
    ui->UsernameLine->setEnabled(true);
    ui->Finish_btn->setEnabled(true);
}

void MainWindow::on_Finish_btn_clicked()
{
    // 获取输入的新用户名和邮箱
    QString newUserName = ui->UsernameLine->text();
    QString newEmail = ui->EmailLine->text();

    // 简单验证输入
    if (newUserName.isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名不能为空");
        return;
    }

    if (!newEmail.isEmpty() && !isValidEmail(newEmail)) {
        QMessageBox::warning(this, "错误", "请输入有效的邮箱地址");
        return;
    }

    // 构建请求JSON数据
    QJsonObject requestData;
    requestData["userName"] = newUserName;
    if (!newEmail.isEmpty()) {
        requestData["email"] = newEmail;
    }

    // 转换为JSON字节数组
    QJsonDocument jsonDoc(requestData);
    QByteArray requestBody = jsonDoc.toJson();

    // 构建请求URL（假设UserId是当前登录用户ID）
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/Account/%1").arg(UserId);
    qDebug() << "请求URL:" << url;

    // 准备HTTP请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证（开发环境）
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // 发送PUT请求
    QNetworkReply *reply = manager->put(request, requestBody);
    connect(reply, &QNetworkReply::finished, [this, reply, newUserName]() {
        // 检查网络错误
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

            // 解析响应状态
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                QMessageBox::information(this, "成功", "账户信息修改成功");
                ui->UserNameLabel->setText(newUserName);
                ui->UsernameLine->setEnabled(false);
                ui->EmailLine->setEnabled(false);
                ui->Finish_btn->setEnabled(false);
            } else {
                QString errorMsg = "修改失败，状态码: " + QString::number(statusCode);
                if (jsonResponse.isObject()) {
                    QJsonObject responseObj = jsonResponse.object();
                    if (responseObj.contains("error")) {
                        errorMsg += "\n错误信息: " + responseObj["error"].toString();
                    }
                }
                QMessageBox::warning(this, "失败", errorMsg);
            }
        } else {
            QMessageBox::warning(this, "错误", "网络请求失败: " + reply->errorString());
        }

        // 释放资源
        reply->deleteLater();
    });
}

// 邮箱格式验证辅助函数
bool MainWindow::isValidEmail(const QString &email)
{
    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    return emailRegex.match(email).hasMatch();
}

void MainWindow::on_Add_btn_clicked()
{
    // 检查是否已加入团队
    if(teamData.TeamId == -1) {
        QMessageBox::warning(this, "错误", "请先加入一个团队");
        return;
    }

    // 创建输入对话框
    bool ok;
    QString accountEmailText = QInputDialog::getText(
                this,
                "添加团队成员",
                "请输入要添加的用户邮箱:",
                QLineEdit::Normal,
                "",
                &ok
                );

    if(!ok)return;
    addMemberToTeam(accountEmailText);

}
QString MainWindow::getEmail(int userId){
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/Account/%1").arg(userId);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QNetworkReply *reply = manager->get(request);

    // 使用事件循环等待请求完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString email;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if (jsonDoc.isObject()) {
            QJsonObject obj = jsonDoc.object();
            if (obj.contains("email")) {
                email = obj["email"].toString();
                UserEmail=obj["email"].toString();
            }

        }
    } else {
        QMessageBox::warning(this, "网络错误", reply->errorString());
    }

    reply->deleteLater();
    return email;
}
void MainWindow::addMemberToTeam(QString email)
{
    // 构建请求URL
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/TeamInfo/AddMember/%1").arg(teamData.TeamId);
    qDebug() << "请求URL:" << url;

    // 构建请求JSON数据
    QJsonObject requestData;
    requestData["email"] = email; // 由于后端需要邮箱字段，但我们使用ID，可以传空或其他标识


    // 转换为JSON字节数组
    QJsonDocument jsonDoc(requestData);
    QByteArray requestBody = jsonDoc.toJson();

    // 准备HTTP请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证（开发环境）
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // 发送POST请求
    QNetworkReply *reply = manager->post(request, requestBody);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        // 检查网络错误
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

            // 解析响应状态
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                QMessageBox::information(this, "成功", "成员添加成功");

                // 刷新团队成员列表
                refreshTeamMembers();
            } else {
                QString errorMsg = "添加失败，状态码: " + QString::number(statusCode);
                if (jsonResponse.isObject()) {
                    QJsonObject responseObj = jsonResponse.object();
                    if (responseObj.contains("message")) {
                        errorMsg += "\n错误信息: " + responseObj["message"].toString();
                    }
                }
                QMessageBox::warning(this, "失败", errorMsg);
            }
        } else {
            QMessageBox::warning(this, "错误", "网络请求失败: " + reply->errorString());
        }

        // 释放资源
        reply->deleteLater();
    });
}

void MainWindow::refreshTeamMembers()
{
    // 模拟点击团队成员按钮来刷新列表
    refreshTeamPage();

}

void MainWindow::on_Changename_btn_clicked()
{
    // 检查是否已加入团队
    if (teamData.TeamId == -1) {
        QMessageBox::warning(this, "错误", "请先加入一个团队");
        return;
    }

    // 获取当前团队名称（用于输入框默认值）
    QString currentTeamName = teamData.name;

    // 创建输入对话框
    bool ok;
    QString newTeamName = QInputDialog::getText(
                this,
                "重命名团队",
                "请输入新的团队名称:",
                QLineEdit::Normal,
                currentTeamName,
                &ok
                );

    // 处理用户输入
    if (ok && !newTeamName.isEmpty()) {
        // 构建请求URL
        QString url = QString(GlobalData::instance()->getSharedString()+"/api/TeamInfo/%1").arg(teamData.TeamId);
        qDebug() << "请求URL:" << url;

        // 构建请求JSON数据
        QJsonObject requestData;
        requestData["name"] = newTeamName;

        // 转换为JSON字节数组
        QJsonDocument jsonDoc(requestData);
        QByteArray requestBody = jsonDoc.toJson();

        // 准备HTTP请求
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // 忽略证书验证（开发环境）
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);

        // 发送PUT请求
        QNetworkReply *reply = manager->put(request, requestBody);
        connect(reply, &QNetworkReply::finished, [this, reply, newTeamName]() {
            // 检查网络错误
            if (reply->error() == QNetworkReply::NoError) {
                // 解析响应状态
                int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                if (statusCode == 200) {
                    QMessageBox::information(this, "成功", "团队重命名成功");

                    // 更新本地团队名称数据
                    teamData.name = newTeamName;

                    // 更新UI上的团队名称显示
                    if (ui->TeamNameLabel) {
                        ui->TeamNameLabel->setText(newTeamName);
                    }
                } else {
                    QString errorMsg = "重命名失败，状态码: " + QString::number(statusCode);
                    QMessageBox::warning(this, "失败", errorMsg);
                }
            } else {
                QMessageBox::warning(this, "错误", "网络请求失败: " + reply->errorString());
            }

            // 释放资源
            reply->deleteLater();
        });
    }
}

void MainWindow::on_Remove_btn_clicked()
{
    // 检查是否已加入团队
    if (teamData.TeamId == -1) {
        QMessageBox::warning(this, "错误", "请先加入一个团队");
        return;
    }

    // 检查是否有选中的队员
    QList<QListWidgetItem*> selectedItems = ui->TeamListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "错误", "请先选择要移除的队员");
        return;
    }

    // 由于只支持单选，取第一个选中项
    QListWidgetItem *selectedItem = selectedItems[0];
    QWidget *itemWidget = ui->TeamListWidget->itemWidget(selectedItem);

    if (!itemWidget) {
        QMessageBox::warning(this, "错误", "无法获取队员信息");
        return;
    }

    // 获取队员ID（假设在创建项时已设置为数据）
    int accountId = selectedItem->data(Qt::UserRole).toInt();
    if (accountId <= 0) {
        QMessageBox::warning(this, "错误", "队员ID无效");
        return;
    }

    // 获取队员名称用于提示
    QLabel *nameLabel = itemWidget->findChild<QLabel*>();
    QString memberName = nameLabel ? nameLabel->text() : "该队员";

    // 确认是否移除
    QMessageBox::StandardButton Reply;
    QString MessString="确定要移除"+memberName+"吗？";
    Reply = QMessageBox::question(this, "确认移除",
                                  QString(MessString),
                                  QMessageBox::Yes | QMessageBox::No);
    if (Reply == QMessageBox::No) {
        return;
    }
    if(accountId==UserId){
        QMessageBox::warning(this, "错误", "无法移除队长！" );
        return;
    }

    // 构建请求URL
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/TeamInfo/RemoveMember/%1-%2")
            .arg(teamData.TeamId).arg(accountId);
    qDebug() << "请求URL:" << url;

    // 准备HTTP请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证（开发环境）
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // 发送DELETE请求
    QNetworkReply *reply = manager->deleteResource(request);
    connect(reply, &QNetworkReply::finished, [this, reply, accountId, memberName]() {
        // 检查网络错误
        if (reply->error() == QNetworkReply::NoError) {
            // 解析响应状态
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                QMessageBox::information(this, "成功", QString("%1 移除成功").arg(memberName));
                refreshTeamPage();

            } else {
                QByteArray responseData = reply->readAll();
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                QString errorMsg = "移除失败，状态码: " + QString::number(statusCode);

                if (jsonResponse.isObject()) {
                    QJsonObject responseObj = jsonResponse.object();
                    if (responseObj.contains("message")) {
                        errorMsg += "\n错误信息: " + responseObj["message"].toString();
                    }
                }
                QMessageBox::warning(this, "失败", errorMsg);
            }
        } else {
            QMessageBox::warning(this, "错误", "网络请求失败: " + reply->errorString());
        }

        // 释放资源
        reply->deleteLater();
    });
}

void MainWindow::on_Dismiss_btn_clicked()
{
    // 检查是否已加入团队
    if (teamData.TeamId == -1) {
        QMessageBox::warning(this, "错误", "请先加入一个团队");
        return;
    }

    // 确认对话框
    QMessageBox::StandardButton boxreply;
    boxreply = QMessageBox::question(
                this,
                "确认解散团队",
                QString("确定要解散团队 '%1' 吗？此操作不可撤销！").arg(teamData.name),
                QMessageBox::Yes | QMessageBox::No
                );

    if (boxreply == QMessageBox::No) {
        return;
    }

    // 构建请求URL
    QString url = QString(GlobalData::instance()->getSharedString()+"/api/TeamInfo/%1-%2")
            .arg(teamData.TeamId)
            .arg(UserId);
    qDebug() << "请求URL:" << url;

    // 准备HTTP请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证（开发环境）
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // 发送DELETE请求
    QNetworkReply *reply = manager->deleteResource(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        // 检查网络错误
        if (reply->error() == QNetworkReply::NoError) {
            // 解析响应状态
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                QMessageBox::information(this, "成功", "团队已成功解散");

                // 重置团队数据
                teamData.TeamId = -1;
                teamData.name = "";

                // 更新UI显示
                ui->stackedWidget->setCurrentWidget(ui->page_noteam);
                ui->TeamNameLabel->setText("未加入团队");

                // 清空团队成员列表
                ui->TeamListWidget->clear();
            } else {
                QMessageBox::warning(this, "失败",
                                     QString("解散团队失败，状态码: %1").arg(statusCode));
            }
        } else {
            QMessageBox::warning(this, "错误", "网络请求失败: " + reply->errorString());
        }

        // 释放资源
        reply->deleteLater();
    });
}
void MainWindow::ShowFileListForTeam(int teamId,int parentId){
    IsTeam=true;
    if(parentId==teamData.RootId){
        QIcon btnicon(":/FileIcons/source/unable_return.png");
        ui->returnBtn_team->setIcon(btnicon);
        ui->returnBtn_team->setEnabled(false);
    }
    else {
        QIcon btnicon(":/FileIcons/source/return.png");
        ui->returnBtn_team->setIcon(btnicon);
        ui->returnBtn_team->setEnabled(true);
    }

    nowId_team=parentId;
    ui->FileListWidget_team->clear();
    if(parentId==-1)parentId=teamData.RootId;
    if (directoryStack_Team.empty()||directoryStack_Team.top()!=parentId) {
        directoryStack_Team.push(parentId); // 进入新目录时将当前目录 ID 压入栈中
    }
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Directory/%1").arg(parentId)));
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
            if (!jsonResponse.isNull() && jsonResponse.isArray()) {
                QJsonArray fileList = jsonResponse.array();
                // 设置 QListWidget 为图标模式
                ui->FileListWidget_team->setViewMode(QListView::IconMode);
                // 设置图标大小为 50x50
                ui->FileListWidget_team->setIconSize(QSize(50, 50));
                // 设置项之间的间距
                ui->FileListWidget_team->setSpacing(10);
                for (const auto &fileItem : fileList) {
                    QJsonObject fileObj = fileItem.toObject();
                    QString name = fileObj["name"].toString();
                    int id = fileObj["id"].toInt();
                    int type = fileObj["type"].toInt();
                    QIcon icon;
                    if (type == 0) {
                        icon = QIcon(":/FileIcons/source/Dir.png");
                    } else {
                        QFileInfo fileInfo(name);
                        QString suffix = fileInfo.suffix().toLower();
                        icon=getFileIcon(suffix);
                    }
                    QListWidgetItem *item = new QListWidgetItem(icon, name);
                    item->setData(Qt::UserRole, id);
                    item->setData(Qt::UserRole + 1, type);
                    item->setSizeHint(QSize(100, 70));
                    // 设置文件名换行显示
                    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignTop);
                    ui->FileListWidget_team->addItem(item);
                }
            }
        }
        reply->deleteLater();
    });
    connect(reply, &QNetworkReply::errorOccurred, [](QNetworkReply::NetworkError code) {
        qDebug() << "Network request error:" << code;
    });

    ui->FileListWidget_team->setCurrentUserId(teamId);
}
QIcon MainWindow::getFileIcon(QString suffix){
    QIcon icon;
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
    }else if(suffix=="xlsx"||suffix=="csv"||suffix=="xls"){
        icon=QIcon(":/FileIcons/source/EXCEL.png");
    } else if(suffix=="md"){
        icon=QIcon(":/FileIcons/source/MD.png");
    }
    else {
        icon = QIcon(":/FileIcons/source/UnNamedFile.png");
    }
    return icon;
}





// 上传按钮点击事件处理
void MainWindow::on_upload_team_clicked()
{
    QString selectedFilePath = QFileDialog::getOpenFileName(nullptr, tr("Select a File to Upload"), "", tr("All Files (*.*)"));
    ui->stackedWidget->setCurrentWidget(ui->UploadPage);

    if (!selectedFilePath.isEmpty()) {
        QFileInfo fileInfo(selectedFilePath);
        QString fileName = fileInfo.fileName();
        int fileType = 1;
        int currentParentId = (directoryStack_Team.isEmpty()) ? rootId : directoryStack_Team.top();

        if (ui->UploadProcessWidget) {
            int row = ui->UploadProcessWidget->rowCount();
            ui->UploadProcessWidget->insertRow(row);

            // 文件名列
            QTableWidgetItem *fileItem = new QTableWidgetItem(fileName);
            ui->UploadProcessWidget->setItem(row, 0, fileItem);

            // 大小列
            qint64 fileSize = fileInfo.size();
            QTableWidgetItem *sizeItem = new QTableWidgetItem(QString("0/%1").arg(bytesToHumanReadable(fileSize)));
            ui->UploadProcessWidget->setItem(row, 1, sizeItem);

            // 进度列 - 添加进度条
            QWidget *progressWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
            QProgressBar *progressBar = new QProgressBar(progressWidget);
            progressBar->setRange(0, 100);
            progressBar->setValue(0);
            progressLayout->addWidget(progressBar);
            progressLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 2, progressWidget);

            // 状态列 - 添加标签
            QWidget *statusWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *statusLayout = new QHBoxLayout(statusWidget);
            QLabel *statusLabel = new QLabel("准备上传", statusWidget);
            statusLabel->setAlignment(Qt::AlignCenter);
            statusLayout->addWidget(statusLabel);
            statusLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 3, statusWidget);

            // 暂停列 - 添加暂停按钮
            QWidget *pauseWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *pauseLayout = new QHBoxLayout(pauseWidget);
            QPushButton *pauseButton = new QPushButton(pauseWidget);
            pauseButton->setIcon(QIcon(":/FileIcons/source/stop.png"));
            pauseButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
            pauseLayout->addWidget(pauseButton);
            pauseLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 4, pauseWidget);

            // 删除列 - 添加删除按钮
            QWidget *deleteWidget = new QWidget(ui->UploadProcessWidget);
            QHBoxLayout *deleteLayout = new QHBoxLayout(deleteWidget);
            QPushButton *deleteButton = new QPushButton(deleteWidget);
            deleteButton->setIcon(QIcon(":/FileIcons/source/delete.png"));
            deleteButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
            deleteLayout->addWidget(deleteButton);
            deleteLayout->setContentsMargins(0, 0, 0, 0);
            ui->UploadProcessWidget->setCellWidget(row, 5, deleteWidget);

            // 初始化上传任务
            UploadTask task;
            task.filePath = selectedFilePath;
            task.fileId = -1;
            task.uploadedBytes = 0;
            task.fileSize = fileSize;
            task.reply = nullptr;
            task.isPaused = false;
            task.status = UploadTask::UploadStatus::InProgress;
            task.pauseButton = pauseButton;
            task.progressBar = progressBar;
            task.statusLabel = statusLabel;
            task.deleteButton = deleteButton;
            task.uploadSpeed = 0;
            task.estimatedTime = 0;
            task.lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
            task.chunksUploaded = 0;
            task.taskIndex = row;

            uploadTasks.append(task);

            // 连接按钮事件
            connect(pauseButton, &QPushButton::clicked, [this, taskIndex = row]() {
                if (taskIndex >= 0 && taskIndex < uploadTasks.size()) {
                    handlePauseButtonClick(taskIndex);
                }
            });

            connect(deleteButton, &QPushButton::clicked, [this, taskIndex = row]() {
                if (taskIndex >= 0 && taskIndex < uploadTasks.size()) {
                    handleDeleteButtonClick(taskIndex);
                }
            });

            // 创建文件请求
            QJsonObject fileObj;
            fileObj["name"] = fileName;
            fileObj["parentid"] = currentParentId;
            fileObj["type"] = fileType;
            fileObj["ownerid"] = UserId;
            fileObj["permission"] = 2;
            QJsonDocument jsonDoc(fileObj);
            QByteArray requestData = jsonDoc.toJson();

            QNetworkRequest createRequest(QUrl(GlobalData::instance()->getSharedString()+"/api/FileItem/"));
            createRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QSslConfiguration createSslConfig = createRequest.sslConfiguration();
            createSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            createRequest.setSslConfiguration(createSslConfig);

            QNetworkReply *createReply = networkManager->post(createRequest, requestData);
            connect(createReply, &QNetworkReply::finished, [this, createReply, taskIndex = row]() {
                if (createReply->error() == QNetworkReply::NoError) {
                    QByteArray responseData = createReply->readAll();
                    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                    QJsonObject responseObj = jsonResponse.object();

                    if (taskIndex >= 0 && taskIndex < uploadTasks.size()) {
                        UploadTask &task = uploadTasks[taskIndex];
                        task.fileId = responseObj["id"].toInt();

                        QFile file(task.filePath);
                        if (file.open(QIODevice::ReadOnly)) {
                            qint64 uploadedBytes = 0;
                            const int maxRetries = 3;

                            while (uploadedBytes < task.fileSize && !task.isPaused && task.status == UploadTask::UploadStatus::InProgress) {
                                qint64 start = uploadedBytes;
                                qint64 end = qMin(start + chunkSize - 1, task.fileSize - 1);
                                qint64 chunkLength = end - start + 1;
                                file.seek(start);
                                QByteArray chunkData = file.read(chunkLength);

                                QNetworkRequest appendRequest(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/AppendContent/%1").arg(task.fileId)));
                                appendRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
                                appendRequest.setRawHeader("Content-Range", QString("bytes %1-%2/%3").arg(start).arg(end).arg(task.fileSize).toUtf8());
                                QSslConfiguration appendSslConfig = appendRequest.sslConfiguration();
                                appendSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
                                appendRequest.setSslConfiguration(appendSslConfig);

                                int retryCount = 0;
                                bool uploadSuccess = false;
                                qint64 chunkStartTime = QDateTime::currentMSecsSinceEpoch();

                                while (retryCount < maxRetries && !uploadSuccess) {
                                    task.reply = networkManager->put(appendRequest, chunkData);
                                    // 使用局部变量存储任务索引，避免闭包问题
                                    int currentTaskIndex = taskIndex;
                                    QEventLoop loop;
                                    connect(task.reply, &QNetworkReply::finished, [this, &loop, currentTaskIndex, chunkLength, chunkStartTime]() {
                                        if (currentTaskIndex >= 0 && currentTaskIndex < uploadTasks.size()) {
                                            UploadTask &currentTask = uploadTasks[currentTaskIndex];
                                            if (currentTask.reply->error() == QNetworkReply::NoError) {
                                                qint64 chunkEndTime = QDateTime::currentMSecsSinceEpoch();
                                                qint64 chunkDuration = chunkEndTime - chunkStartTime;

                                                // 计算上传速度
                                                if (chunkDuration > 0) {
                                                    currentTask.uploadSpeed = (chunkLength * 1000) / chunkDuration;
                                                }

                                                // 更新上传进度
                                                currentTask.uploadedBytes += chunkLength;
                                                currentTask.chunksUploaded++;

                                                // 更新UI（添加有效性检查）
                                                if (currentTaskIndex < ui->UploadProcessWidget->rowCount()) {
                                                    QTableWidgetItem *sizeItem = ui->UploadProcessWidget->item(currentTaskIndex, 1);
                                                    if (sizeItem) {
                                                        sizeItem->setText(QString("%1/%2").arg(
                                                                              bytesToHumanReadable(currentTask.uploadedBytes),
                                                                              bytesToHumanReadable(currentTask.fileSize)));
                                                    }

                                                    int progress = qMin((100 * currentTask.uploadedBytes / currentTask.fileSize), 100);
                                                    currentTask.progressBar->setValue(progress);

                                                    if (currentTask.uploadSpeed > 0 && currentTask.uploadedBytes < currentTask.fileSize) {
                                                        qint64 remainingBytes = currentTask.fileSize - currentTask.uploadedBytes;
                                                        currentTask.estimatedTime = remainingBytes / currentTask.uploadSpeed;
                                                        QString speedText = QString("%1/s").arg(bytesToHumanReadable(currentTask.uploadSpeed));
                                                        QString timeText = currentTask.estimatedTime > 0 ?
                                                                    QString("剩余: %1").arg(secondsToTimeString(currentTask.estimatedTime)) : "即将完成";
                                                        currentTask.statusLabel->setText(QString("%1 • %2").arg(speedText).arg(timeText));
                                                    } else {
                                                        currentTask.statusLabel->setText("上传中...");
                                                    }
                                                }
                                            }
                                        }
                                        loop.quit();
                                    });

                                    loop.exec();

                                    if (task.reply->error() == QNetworkReply::NoError) {
                                        uploadedBytes += chunkLength;
                                        task.uploadedBytes = uploadedBytes;
                                        uploadSuccess = true;
                                    } else {
                                        qDebug() << "上传块失败 (尝试" << retryCount + 1 << "):" << task.reply->errorString();
                                        retryCount++;
                                        QThread::msleep(500);
                                    }

                                    task.reply->deleteLater();
                                }

                                if (!uploadSuccess) {

                                    if (taskIndex < ui->UploadProcessWidget->rowCount()) {
                                        task.statusLabel->setText("上传错误");
                                        task.progressBar->setValue(0);
                                    }
                                    break;
                                }
                            }

                            if (task.uploadedBytes == task.fileSize) {
                                if (taskIndex < ui->UploadProcessWidget->rowCount()) {
                                    task.progressBar->setValue(100);
                                    task.progressBar->setFormat("完成");
                                    task.statusLabel->setText("已完成");
                                    task.pauseButton->setVisible(false);
                                    task.pauseButton->setEnabled(false);
                                    task.status=UploadTask::UploadStatus::Completed;
                                }
                                file.close();
                            }
                        } else {
                            if (taskIndex < ui->UploadProcessWidget->rowCount()) {

                                task.statusLabel->setText("文件打开错误");
                                task.progressBar->setValue(0);
                            }
                        }
                    }
                } else {
                    qDebug() << "创建文件错误:" << createReply->errorString();
                    if (taskIndex >= 0 && taskIndex < uploadTasks.size() && taskIndex < ui->UploadProcessWidget->rowCount()) {
                        UploadTask &task = uploadTasks[taskIndex];
                        task.statusLabel->setText("创建文件错误");
                        task.progressBar->setValue(0);
                        task.deleteButton->setVisible(false);
                        task.pauseButton->setVisible(false);
                        task.deleteButton->setEnabled(false);
                        task.pauseButton->setEnabled(false);
                    }
                }
                createReply->deleteLater();
            });
        }
    }
}







void MainWindow::on_returnBtn_team_clicked()
{
    if (directoryStack_Team.size() > 1) { // 栈中至少有两个元素（当前目录和上一级目录）
        directoryStack_Team.pop(); // 弹出当前目录的 ID
        int parentId = directoryStack_Team.top(); // 获取上一级目录的 ID
        ShowFileListForTeam(teamData.TeamId,parentId);


    } else if (directoryStack_Team.size() == 1) { // 回到根目录
        directoryStack_Team.pop();

        ShowFileListForTeam(teamData.TeamId,teamData.RootId);
    } else {
        qDebug() << "已经是根目录，无法再返回。";
    }
}


void MainWindow::on_refreshBtn_team_clicked()
{
    ShowFileListForTeam(teamData.TeamId,nowId_team);
}


void MainWindow::on_deleteallprocess_clicked()
{
    // 确认对话框
    QMessageBox::StandardButton boxreply;
    boxreply = QMessageBox::question(
                this,
                "确认删除所有下载任务",
                QString("确认删除所有下载任务？此操作不可撤销！"),
                QMessageBox::Yes | QMessageBox::No
                );

    if (boxreply == QMessageBox::No) {
        return;
    }

    // 清理所有下载任务
    for (int i = 0; i < ui->ProgessWidget->rowCount(); ++i) {
        // 暂停下载任务
        if (!ui->ProgessWidget->downloadTasks[i].isFinished) {
            ui->ProgessWidget->pauseDownload(i, isstopallload);
        }
    }

    isstopallload = true;
    for (int i = 0; i < ui->ProgessWidget->rowCount(); ++i) {
        //移除磁盘中还未下载完的部分
        ui->ProgessWidget->deleteSpecificFiles(ui->ProgessWidget->downloadTasks[i].filePath);
        ui->ProgessWidget->removeRow(i);
    }
    ui->ProgessWidget->downloadTasks.clear();
}


void MainWindow::changePassward_clicked()
{
    // 第一步：验证当前密码
    QDialog *verifyDialog = new QDialog(this);
    verifyDialog->setWindowTitle("验证当前密码");
    verifyDialog->setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(verifyDialog);

    QLabel *label = new QLabel("请输入当前密码:", verifyDialog);
    QLineEdit *currentPasswordEdit = new QLineEdit(verifyDialog);
    currentPasswordEdit->setEchoMode(QLineEdit::Password);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("确定", verifyDialog);
    QPushButton *cancelButton = new QPushButton("取消", verifyDialog);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(label);
    layout->addWidget(currentPasswordEdit);
    layout->addLayout(buttonLayout);

    // 连接确定按钮
    connect(okButton, &QPushButton::clicked, [this, verifyDialog, currentPasswordEdit]() {
        QString currentPassword = currentPasswordEdit->text();
        if (currentPassword.isEmpty()) {
            QMessageBox::warning(verifyDialog, "错误", "密码不能为空");
            return;
        }

        // 验证当前密码
        verifyCurrentPassword(currentPassword, [this, verifyDialog](bool verified) {
            if (verified) {
                verifyDialog->close();
                // 密码验证成功，显示新密码设置对话框
                showNewPasswordDialog();
            } else {
                QMessageBox::warning(verifyDialog, "错误", "当前密码不正确，请重新输入");
            }
        });
    });

    // 连接取消按钮
    connect(cancelButton, &QPushButton::clicked, verifyDialog, &QDialog::close);

    verifyDialog->exec();
}

// 临时解决方案：使用登录API验证密码
void MainWindow::verifyCurrentPassword(const QString &password, std::function<void(bool)> callback)
{
    // 构建登录请求
    QJsonObject loginData;
    loginData["Email"] = UserEmail; // 假设已保存当前用户邮箱
    loginData["Password"] = password;

    QJsonDocument jsonDoc(loginData);
    QByteArray requestBody = jsonDoc.toJson();

    QString url = GlobalData::instance()->getSharedString() + "/api/Account/Login"; // 假设登录API

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QNetworkReply *reply = manager->post(request, requestBody);
    connect(reply, &QNetworkReply::finished, [reply, callback]() {
        bool verified = (reply->error() == QNetworkReply::NoError);
        reply->deleteLater();
        callback(verified);
    });
}

// 显示新密码设置对话框
void MainWindow::showNewPasswordDialog()
{
    QDialog *newPasswordDialog = new QDialog(this);
    newPasswordDialog->setWindowTitle("设置新密码");
    newPasswordDialog->setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(newPasswordDialog);

    QLabel *newPasswordLabel = new QLabel("新密码:", newPasswordDialog);
    QLineEdit *newPasswordEdit = new QLineEdit(newPasswordDialog);
    newPasswordEdit->setEchoMode(QLineEdit::Password);

    QLabel *confirmPasswordLabel = new QLabel("确认新密码:", newPasswordDialog);
    QLineEdit *confirmPasswordEdit = new QLineEdit(newPasswordDialog);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("确定", newPasswordDialog);
    QPushButton *cancelButton = new QPushButton("取消", newPasswordDialog);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    layout->addWidget(newPasswordLabel);
    layout->addWidget(newPasswordEdit);
    layout->addWidget(confirmPasswordLabel);
    layout->addWidget(confirmPasswordEdit);
    layout->addLayout(buttonLayout);

    // 连接确定按钮
    connect(okButton, &QPushButton::clicked, [this, newPasswordDialog, newPasswordEdit, confirmPasswordEdit]() {
        QString newPassword = newPasswordEdit->text();
        QString confirmPassword = confirmPasswordEdit->text();

        // 验证新密码
        if (newPassword.isEmpty()) {
            QMessageBox::warning(newPasswordDialog, "错误", "新密码不能为空");
            return;
        }



        if (newPassword != confirmPassword) {
            QMessageBox::warning(newPasswordDialog, "错误", "两次输入的密码不一致");
            return;
        }

        // 修改密码（异步方式）
            changePassword(newPassword, [this, newPasswordDialog](bool success) {
                newPasswordDialog->close(); // 关闭对话框

                if (success) {
                    QMessageBox::information(this, "成功", "密码修改成功");
                } else {
                    QMessageBox::warning(this, "错误", "密码修改失败");
                }
            });
    });

    // 连接取消按钮
    connect(cancelButton, &QPushButton::clicked, newPasswordDialog, &QDialog::close);

    newPasswordDialog->exec();
}

// 修改密码的函数（异步版本）
void MainWindow::changePassword(const QString &newPassword, std::function<void(bool)> callback)
{
    QJsonObject requestData;
    requestData["Password"] = newPassword;

    QJsonDocument jsonDoc(requestData);
    QByteArray requestBody = jsonDoc.toJson();

    QString url = QString(GlobalData::instance()->getSharedString()+"/api/Account/%1").arg(UserId);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 忽略证书验证
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    QNetworkReply *reply = manager->put(request, requestBody);
    connect(reply, &QNetworkReply::finished, [reply, callback]() {
        bool success = false;

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            success = true; // 假设成功，实际应检查服务器返回的状态
        }
        reply->deleteLater();
        callback(success); // 通过回调返回结果
    });
}


void MainWindow::on_changePassword_btn_clicked()
{
    changePassward_clicked();
}

