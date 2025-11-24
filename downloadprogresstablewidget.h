#ifndef DOWNLOADPROGRESSTABLEWIDGET_H
#define DOWNLOADPROGRESSTABLEWIDGET_H

#include <QTableWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProgressBar>
#include <QPushButton>
#include <QMouseEvent>
#include <QToolTip>
#include <QHeaderView>
#include <QThread>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThreadPool>
#include <QRunnable>
#include<QEventLoop>
#include<QMutex>
#include<QLabel>
#include <QObject>
#include"globaldata.h"

// 下载任务结构体
struct DownloadTask {
    int fileId;
    QString fileName;
    QNetworkReply *reply;
    QProgressBar *progressBar;
    QPushButton *pauseButton;
    QLabel *statusLabel;
    QPushButton *deleteButton;
    qint64 bytesReceived;
    qint64 bytesTotal;
    bool isPaused;
    QString filePath; // 添加文件路径，用于打开源文件
    int currentChunkId;
    bool isFinished=false;
    qint64 downloadSpeed;     // 新增：下载速度
    qint64 estimatedTime;     // 新增：估计剩余时间
    qint64 lastUpdateTime;    // 新增：上次更新时间
    qint64 lastBytesReceived;
};

class DownloadChunkTask : public QRunnable {
public:
    DownloadChunkTask(const QUrl& url, qint64 start, qint64 end, int index, const QString& dir, QMutex* mutex)
        : url(url), start(start), end(end), index(index), dir(dir), mutex(mutex) {}

    void run() override {
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);

        // 设置 Range 头
        request.setRawHeader("Range", QString("bytes=%1-%2").arg(start).arg(end).toUtf8());

        QNetworkReply* reply = manager.get(request);
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            mutex->lock();
            QFile file(dir + "/" + QString::number(index));
            if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
                file.write(data);
                file.close();
            } else {
                qDebug() << "Error opening file for writing in DownloadChunkTask: " << file.errorString();
            }
            mutex->unlock();
        } else {
            qDebug() << "Error in network request in DownloadChunkTask: " << reply->errorString();
        }
        reply->deleteLater();
    }

private:
    QUrl url;
    qint64 start;
    qint64 end;
    int index;
    QString dir;
    QMutex* mutex;
};


// 自定义表格控件类
class DownloadProgressTableWidget : public QTableWidget {
    Q_OBJECT
public:
    DownloadProgressTableWidget(QWidget *parent = nullptr);
    ~DownloadProgressTableWidget();

    void addDownloadTask(int fileId, const QString &fileName, const QIcon &icon);
    void startDownload(int row);
    void pauseDownload(int row,bool isallstop);
    void resumeDownload(int row);
    void deleteSpecificFiles(const QString &folderPath);
    QString downloadPath;
    QList<DownloadTask> downloadTasks;

private slots:    

    void downloadNextChunk(int row,QString saveDir,QString finalPath);
    QString secondsToTimeString(qint64 seconds);
    QString bytesToHumanReadable(qint64 bytes) {
        if (bytes < 1024) {
            return QString("%1 B").arg(bytes);
        } else if (bytes < 1024 * 1024) {
            return QString("%1 KB").arg(static_cast<double>(bytes) / 1024, 0, 'f', 2);
        } else if (bytes < 1024 * 1024 * 1024) {
            return QString("%1 MB").arg(static_cast<double>(bytes) / (1024 * 1024), 0, 'f', 2);
        } else {
            return QString("%1 GB").arg(static_cast<double>(bytes) / (1024 * 1024 * 1024), 0, 'f', 2);
        }
    }
    void updateDownloadSpeed(int row, qint64 chunkStartTime);
    void updateStatusLabel(int row, qint64 totalBytes);
private:

    QNetworkAccessManager *networkManager;
    const qint64 CHUNK_SIZE = 0.5 * 1024 * 1024;
};



#endif // DOWNLOADPROGRESSTABLEWIDGET_H
