#include"downloadprogresstablewidget.h"
#include<QHBoxLayout>
#include<QJsonDocument>
#include <QHeaderView>
#include<QJsonObject>
#include<QFileInfo>
#include<QMutex>
#include<QTimer>
#include <QCryptographicHash>
#include <QDir>
#include<QIcon>
#include<QMessageBox>
DownloadProgressTableWidget::DownloadProgressTableWidget(QWidget *parent) : QTableWidget(parent)
{
    networkManager = new QNetworkAccessManager(this);
    setColumnCount(6);

    QStringList headers;
    headers << "文件" << "大小" << "进度"<<"状态"<<"暂停"<<"删除";
    setHorizontalHeaderLabels(headers);
    setShowGrid(false);
    verticalHeader()->setVisible(false);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 获取水平表头指针
    QHeaderView *horizontalHeader = this->horizontalHeader();
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

DownloadProgressTableWidget::~DownloadProgressTableWidget()
{
    for (const auto &task : downloadTasks) {
        if (task.reply) {
            task.reply->abort();
            task.reply->deleteLater();
        }
    }
}

// 计算下载速度和剩余时间
void DownloadProgressTableWidget::updateDownloadSpeed(int row, qint64 chunkStartTime) {
    if (row < 0 || row >= downloadTasks.size()) return;

    DownloadTask &task = downloadTasks[row];
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 timeElapsed = currentTime - task.lastUpdateTime;

    // 每500ms更新一次速度，避免更新过于频繁
    if (timeElapsed < 500) return;

    qint64 bytesDelta = task.bytesReceived - task.lastBytesReceived;

    if (timeElapsed > 0) {
        // 计算下载速度 (bytes/sec)
        task.downloadSpeed = (bytesDelta * 1000) / timeElapsed;

        // 计算剩余时间
        qint64 remainingBytes = task.bytesTotal - task.bytesReceived;
        if (task.downloadSpeed > 0) {
            task.estimatedTime = remainingBytes / task.downloadSpeed;
        } else {
            task.estimatedTime = 0;
        }

        // 更新状态标签
        updateStatusLabel(row, task.bytesTotal);
    }

    // 更新记录
    task.lastUpdateTime = currentTime;
    task.lastBytesReceived = task.bytesReceived;
}

// 更新状态标签显示
void DownloadProgressTableWidget::updateStatusLabel(int row, qint64 totalBytes) {
    if (row < 0 || row >= downloadTasks.size()) return;

    DownloadTask &task = downloadTasks[row];

    if (task.isPaused) {
        task.statusLabel->setText("已暂停");
        return;
    }

    if (task.bytesReceived >= totalBytes) {        
        return;
    }

    if (task.downloadSpeed > 0) {
        QString speedText = QString("%1/s").arg(bytesToHumanReadable(task.downloadSpeed));
        QString timeText;

        if (task.estimatedTime > 0) {
            timeText = QString("剩余: %1").arg(secondsToTimeString(task.estimatedTime));
        } else {
            timeText = "即将完成";
        }

        task.statusLabel->setText(QString("%1 • %2").arg(speedText).arg(timeText));
    } else {
        task.statusLabel->setText("下载中...");
    }
}
// 秒转换为可读时间格式
QString DownloadProgressTableWidget::secondsToTimeString(qint64 seconds) {
    if (seconds < 60) return QString("%1秒").arg(seconds);
    if (seconds < 3600) return QString("%1分%2秒").arg(seconds / 60).arg(seconds % 60);
    return QString("%1时%2分%3秒").arg(seconds / 3600).arg((seconds % 3600) / 60).arg(seconds % 60);
}
void DownloadProgressTableWidget::addDownloadTask(int fileId, const QString &fileName, const QIcon &icon) {
    int row = rowCount();
    insertRow(row);

    // 文件名列
    QTableWidgetItem *item = new QTableWidgetItem(icon, fileName);
    setItem(row, 0, item);

    // 大小列
    QTableWidgetItem *sizeItem = new QTableWidgetItem("0/0");
    setItem(row, 1, sizeItem);

    // 进度列
    QWidget *progressWidget = new QWidget(this);
    QHBoxLayout *progressLayout = new QHBoxLayout(progressWidget);
    QProgressBar *progressBar = new QProgressBar(progressWidget);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressLayout->addWidget(progressBar);
    progressLayout->setContentsMargins(0, 0, 0, 0);
    setCellWidget(row, 2, progressWidget);

    // 状态列
    QWidget *statusWidget = new QWidget(this);
    QHBoxLayout *statusLayout = new QHBoxLayout(statusWidget);
    QLabel *statusLabel = new QLabel("准备下载", statusWidget);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(statusLabel);
    statusLayout->setContentsMargins(0, 0, 0, 0);
    setCellWidget(row, 3, statusWidget);

    // 暂停列
    QWidget *pauseWidget = new QWidget(this);
    QHBoxLayout *pauseLayout = new QHBoxLayout(pauseWidget);
    QPushButton *pauseButton = new QPushButton(pauseWidget);
    pauseButton->setIcon(QIcon(":/FileIcons/source/stop.png"));
    pauseButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
    pauseLayout->addWidget(pauseButton);
    pauseLayout->setContentsMargins(0, 0, 0, 0);
    setCellWidget(row, 4, pauseWidget);

    // 删除列
    QWidget *deleteWidget = new QWidget(this);
    QHBoxLayout *deleteLayout = new QHBoxLayout(deleteWidget);
    QPushButton *deleteButton = new QPushButton(deleteWidget);
    deleteButton->setIcon(QIcon(":/FileIcons/source/delete_tool.png"));
    deleteButton->setStyleSheet("QPushButton { border: none; background: transparent; }");
    deleteLayout->addWidget(deleteButton);
    deleteLayout->setContentsMargins(0, 0, 0, 0);
    setCellWidget(row, 5, deleteWidget);

    // 初始化下载任务
    DownloadTask task;
    task.fileId = fileId;
    task.reply = nullptr;
    task.progressBar = progressBar;
    task.pauseButton = pauseButton;
    task.deleteButton = deleteButton;
    task.statusLabel = statusLabel;
    task.bytesReceived = 0;
    task.bytesTotal = 0;
    task.isPaused = false;
    task.currentChunkId = 0;
    task.fileName = fileName;
    task.filePath=downloadPath;
    task.isFinished = false;
    task.downloadSpeed = 0;
    task.estimatedTime = 0;
    task.lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    task.lastBytesReceived = 0; // 新增：上次接收的字节数

    downloadTasks.append(task);

    // 连接按钮事件
    connect(pauseButton, &QPushButton::clicked, [this, row]() {
        pauseDownload(row,false);
    });

    connect(deleteButton, &QPushButton::clicked, [this, row]() {
        // 确认对话框
        QMessageBox::StandardButton boxreply;
        boxreply = QMessageBox::question(
            this,
            "确认删除此下载任务",
            QString("确认删除此下载任务？此操作不可撤销！"),
            QMessageBox::Yes | QMessageBox::No
        );

        if (boxreply == QMessageBox::No) {
            return;
        }
        if(!downloadTasks[row].isPaused){
            QMessageBox::warning(this, "删除失败", "请先暂停下载任务！" );
        }
        else{

            this->removeRow(row);
            deleteSpecificFiles(downloadTasks[row].filePath);
        }

    });

    // 发起获取文件大小的网络请求
    QNetworkRequest sizeRequest(QUrl(GlobalData::instance()->getSharedString()+QString("/api/FileItem/Size/%1").arg(fileId)));
    QSslConfiguration sslConfig = sizeRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    sizeRequest.setSslConfiguration(sslConfig);

    QNetworkReply *sizeReply = networkManager->get(sizeRequest);
    connect(sizeReply, &QNetworkReply::finished, [this, row, sizeReply, sizeItem, statusLabel]() {
        if (sizeReply->error() == QNetworkReply::NoError) {
            QByteArray responseData = sizeReply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject responseObj = jsonResponse.object();
            qint64 fileSize = responseObj["size"].toVariant().toLongLong();

            // 更新任务的总字节数
            downloadTasks[row].bytesTotal = fileSize;

            // 更新表格中大小列的显示
            sizeItem->setText(QString("0/%1").arg(bytesToHumanReadable(fileSize)));

            // 更新状态显示
            statusLabel->setText("正在下载");

            // 自动开始下载
            startDownload(row);
        } else {
            qDebug() << "获取文件大小出错: " << sizeReply->errorString();
            statusLabel->setText("获取大小失败");
        }
        sizeReply->deleteLater();
    });
}

void DownloadProgressTableWidget::downloadNextChunk(int row, QString saveDir, QString finalPath) {
    if (row < 0 || row >= downloadTasks.size()) return;

    DownloadTask &task = downloadTasks[row];
    if (task.isPaused) {
        return;
    }

    qint64 fileSize = task.bytesTotal;
    int num_chunks = (fileSize + CHUNK_SIZE - 1) / CHUNK_SIZE;
    int chunk_id = task.currentChunkId;

    if (chunk_id >= num_chunks) {
        // 所有块都已下载完成，检查是否可以合并
        QString saveDir = downloadPath;
        QString finalPath = saveDir + "/" + item(row, 0)->text();
        bool all_chunks_downloaded = true;
        for (int i = 0; i < num_chunks; ++i) {
            QString chunk_path = saveDir + "/" + QString::number(i) + ".part";
            if (!QFile::exists(chunk_path)) {
                all_chunks_downloaded = false;
                break;
            }
        }

        if (all_chunks_downloaded) {
            // 合并所有块
            QFile final_file(finalPath);
            if (final_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                for (int i = 0; i < num_chunks; ++i) {
                    QString chunk_path = saveDir + "/" + QString::number(i) + ".part";
                    QFile chunk_file(chunk_path);
                    if (chunk_file.open(QIODevice::ReadOnly)) {
                        QByteArray chunk_data = chunk_file.readAll();
                        final_file.write(chunk_data);
                        chunk_file.close();
                        QFile::remove(chunk_path);
                    } else {
                        qDebug() << "Error opening chunk file for reading in mergeChunks: " << chunk_file.errorString();
                    }
                }
                final_file.close();
                qDebug() << "文件合并完成: " << finalPath;
                task.statusLabel->setText("已完成");

            } else {
                qDebug() << "Error opening final file for writing in mergeChunks: " << final_file.errorString();
            }
            task.progressBar->setValue(task.progressBar->maximum());
            task.progressBar->setFormat("完成");

            task.pauseButton->setVisible(false);
            task.deleteButton->setVisible(false);
            task.isFinished=true;
        }
        deleteSpecificFiles(saveDir);
        return;
    }

    qint64 start = chunk_id * CHUNK_SIZE;
    qint64 end = qMin(start + CHUNK_SIZE, fileSize);
    QNetworkRequest downloadRequest(QUrl(QString(GlobalData::instance()->getSharedString()+"/api/FileItem/Download/%1").arg(task.fileId)));
    QSslConfiguration downloadSslConfig = downloadRequest.sslConfiguration();
    downloadSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    downloadRequest.setSslConfiguration(downloadSslConfig);
    downloadRequest.setRawHeader("Range", QString("bytes=%1-%2").arg(start).arg(end).toUtf8());

    // 记录下载开始时间
    qint64 chunkStartTime = QDateTime::currentMSecsSinceEpoch();

    QNetworkReply *downloadReply = networkManager->get(downloadRequest);
    task.reply = downloadReply; // 保存当前的reply指针

    connect(downloadReply, &QNetworkReply::finished, [this, row, chunk_id, saveDir, downloadReply, finalPath, fileSize, num_chunks, &task, chunkStartTime]() {
        if (downloadReply->error() == QNetworkReply::NoError) {
            QByteArray data = downloadReply->readAll();
            if (task.isPaused) {
                // 如果任务已暂停，记录当前块编号
                task.currentChunkId = chunk_id;
                qDebug() << "暂停的块为：" << task.currentChunkId;
            } else {
                QString chunk_path = saveDir + "/" + QString::number(chunk_id) + ".part";
                QFile chunk_file(chunk_path);
                if (chunk_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                    chunk_file.write(data);
                    chunk_file.close();

                    qint64 actualReceived = (chunk_id < num_chunks - 1) ?
                                CHUNK_SIZE :
                                (fileSize - (chunk_id * CHUNK_SIZE));
                    task.bytesReceived = task.currentChunkId * CHUNK_SIZE + actualReceived;

                    int progress = (task.bytesTotal > 0) ?
                                qMin(100, static_cast<int>(100 * task.bytesReceived / task.bytesTotal)) :
                                0;
                    task.progressBar->setValue(progress);

                    // 更新状态栏大小显示
                    QTableWidgetItem *sizeItem = this->item(row, 1);
                    if (sizeItem) {
                        int received = qMin(task.bytesReceived, task.bytesTotal);
                        sizeItem->setText(QString("%1/%2").arg(bytesToHumanReadable(received)).arg(bytesToHumanReadable(fileSize)));
                    }

                    // 计算下载速度和剩余时间
                    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
                    qint64 timeElapsed = currentTime - chunkStartTime;

                    if (timeElapsed > 0) { // 避免除零错误
                        // 计算下载速度 (bytes/sec)
                        qint64 bytesDelta = actualReceived;
                        task.downloadSpeed = (bytesDelta * 1000) / timeElapsed;

                        // 计算剩余时间
                        qint64 remainingBytes = task.bytesTotal - task.bytesReceived;
                        if (task.downloadSpeed > 0) {
                            task.estimatedTime = remainingBytes / task.downloadSpeed;
                        } else {
                            task.estimatedTime = 0;
                        }

                        // 更新状态标签
                        updateStatusLabel(row, task.bytesTotal);
                    }

                    // 更新时间记录
                    task.lastUpdateTime = currentTime;
                    task.lastBytesReceived = task.bytesReceived;

                    // 下载下一个块
                    task.currentChunkId = chunk_id + 1;
                    downloadNextChunk(row, saveDir, finalPath);
                } else {
                    qDebug() << "Error opening chunk file for writing: " << chunk_file.errorString();
                }
            }
        } else {
            qDebug() << "Error downloading chunk " << chunk_id << ": " << downloadReply->errorString();
        }
        downloadReply->deleteLater();
    });
}
void DownloadProgressTableWidget::deleteSpecificFiles(const QString &folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        qDebug() << "文件夹不存在:" << folderPath;
        return;
    }

    // 设置文件过滤器，包括隐藏文件
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
    dir.setNameFilters(QStringList() << "*.part");

    // 获取所有.part文件
    QFileInfoList fileList = dir.entryInfoList();
    // 精确匹配纯数字文件名后接.part
    QRegularExpression regex("^\\d+\\.part$",
                             QRegularExpression::CaseInsensitiveOption);

    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.fileName();
        QString filePath = fileInfo.absoluteFilePath();

        // 检查是否匹配纯数字文件名
        if (regex.match(fileName).hasMatch()) {
            QFile file(filePath);

            // 检查文件状态
            bool isOpen = false;
            if (!file.isOpen()) {
                if (file.open(QIODevice::ReadWrite | QIODevice::ExistingOnly)) {
                    file.close(); // 关闭文件以确保可删除
                } else {
                    isOpen = true;
                }
            }

            if (!isOpen) {
                // 设置可写权限
                if (!fileInfo.isWritable()) {
                    file.setPermissions(file.permissions() |
                                        QFileDevice::WriteUser |
                                        QFileDevice::WriteGroup |
                                        QFileDevice::WriteOther);
                }
                file.remove();

            }}
    }
}
void DownloadProgressTableWidget::startDownload(int row) {

    if (row < 0 || row >= downloadTasks.size()) return;
    DownloadTask &task = downloadTasks[row];

    if (task.isPaused) {
        return;
    } else {
        // 获取文件大小
        QNetworkRequest sizeRequest(QUrl(GlobalData::instance()->getSharedString()+QString("/api/FileItem/Size/%1").arg(task.fileId)));
        QSslConfiguration sslConfig = sizeRequest.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        sizeRequest.setSslConfiguration(sslConfig);
        QNetworkReply *sizeReply = networkManager->get(sizeRequest);
        qDebug() << "Size request URL:" << sizeRequest.url();
        qDebug() << "Size reply:" << sizeReply;
        connect(sizeReply, &QNetworkReply::finished, [this, row, sizeReply]() {
            if (sizeReply->error() == QNetworkReply::NoError) {
                QByteArray responseData = sizeReply->readAll();
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                QJsonObject responseObj = jsonResponse.object();
                qint64 fileSize = responseObj["size"].toVariant().toLongLong();
                DownloadTask &task = downloadTasks[row];
                task.bytesTotal = fileSize;
                QString saveDir = downloadPath;
                QString finalPath = saveDir + "/" + item(row, 0)->text();

                // 创建保存块的临时目录
                QDir dir(saveDir);
                if (!dir.exists()) {
                    dir.mkpath(saveDir);
                }
                // 开始下载第一个块
                downloadNextChunk(row,saveDir,finalPath);
            } else {
                qDebug() << "Size request error:" << sizeReply->errorString();
                qDebug() << "Size response data:" << sizeReply->readAll();
            }
            sizeReply->deleteLater();
        });
    }
}

void DownloadProgressTableWidget::pauseDownload(int row,bool isallstop) {
    if (row < 0 || row >= downloadTasks.size()) return;
    DownloadTask &task = downloadTasks[row];

    task.statusLabel->setText("已暂停");
    if(isallstop)return;
    if(task.isPaused){resumeDownload(row);return;}
    if (task.reply) {
        task.reply->abort();
        task.reply->deleteLater();
        task.reply = nullptr;
    }
    task.isPaused = true;
    QIcon btnicon(":/FileIcons/source/resume.png");
    task.pauseButton->setIcon(btnicon);

}

void DownloadProgressTableWidget::resumeDownload(int row) {    
    if (row < 0 || row >= downloadTasks.size()) return;
    DownloadTask &task = downloadTasks[row];
    if(!task.isPaused)return;
    task.isPaused = false;
    QIcon btnicon(":/FileIcons/source/stop.png");
    task.pauseButton->setIcon(btnicon);
    task.statusLabel->setText("正在下载");
    startDownload(row);
}




