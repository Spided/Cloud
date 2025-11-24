#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"globaldata.h"
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QListWidget>
#include<QJsonArray>
#include<QStack>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include<QDialog>
#include<QInputDialog>
#include<QProgressDialog>
#include<functional>
#include<QTimer>
#include<QButtonGroup>
#include "customlistwidget.h"
#include"downloadprogresstablewidget.h"
#include"handlevideodialog.h"
#include"handleimagedialog.h"
#include"handlepdfworddialog.h"
#include"handleworddialog.h"
#include"handletxtdialog.h"
#include"logindialog.h"
#include"sharefiledialog.h"
#include"handleexceldialog.h"
#include"sharelistdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct UploadTask {
    QString filePath;
    int fileId;
    qint64 uploadedBytes;
    qint64 fileSize;
    QNetworkReply *reply;
    QProgressBar *progressBar;
    QPushButton *pauseButton;
    QPushButton *deleteButton;
    bool isPaused;
    int taskIndex;
    bool isallpaused;
    QLabel *statusLabel;          // 状态标签（显示速度和时间）
    QTableWidgetItem *statusItem;    // 状态显示项，用于更新速度和预估时间
    qint64 uploadSpeed;              // 当前上传速度（字节/秒）
    qint64 estimatedTime;            // 预估剩余时间（秒）
    qint64 lastUpdateTime;           // 上次更新时间戳（毫秒）
    int chunksUploaded;              // 已上传的数据块数量
    enum class UploadStatus {
        None,
        InProgress,
        Paused,
        Completed,
        Error
    } status = UploadStatus::None; //状态标识
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void onFileItemDoubleClicked(QListWidgetItem *item);
    void showFileList(int ownerId, int parentId = -1);//展示文件列表
    void onFileItemRightClicked(const QPoint &pos);//右键文件或文件夹
    void onRenameActionTriggered();//重命名
    void onDeleteActionTriggered();//移至回收站
    void showTrashFileList(int ownerId);//展示回收站内容
    void onTrashItemRightClicked(const QPoint &pos);//右键回收站文件或文件夹
    void onRestoreActionTriggered();//恢复
    void onDeleteFromTrashActionTriggered();//永久删除
    void moveFileOrFolder(int ownerId, int sourceId, int destinationId);//移动
    void onCopyActionTriggered();//复制
    void on_returnBtn_clicked();

    void on_pushButton_clicked();



    void on_pushButton_2_clicked();
    void onCutActionTriggered();
    void onPasteActionTriggered();
    void onEmptyAreaRightClicked(const QPoint &pos);

    void on_upload_clicked();
    void onLoadActionTriggered();//下载


    void onNewFolderTriggered(); // 处理新建文件夹的槽函数


    void uploadinit();
    void on_stopallprocess_clicked();

    void on_startallprocess_clicked();

    void on_stopallprocess_2_clicked();

    void on_startallprocess_2_clicked();

    void handlePauseButtonClick(int i);
    void stopthetask(int i);
    void resumethetask(int i);
    void PreviewItem();
    void PreviewFile(int fileId);
    void onStackedWidgetCurrentChanged(int index);
    void receiveId(int UserId, int rootId, int trashBinId,TeamData td);
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();
    void UpdateBtnCol(int idx);
    void ShareFile();
    QString decryptLink(const QString& encryptedUrl);//解码
    void on_toolButton_5_clicked();

    void on_toolButton_6_clicked();



    void on_deleteallprocess_2_clicked();
    void updateUploadProcessWidget();
    void on_refreshBtn_clicked();
    void iconinit();
    void on_toolButton_7_clicked();
    void refreshTeamPage();

    void on_createTeam_btn_clicked();


    void on_toolButton_8_clicked();

    void on_upload_btn_clicked();
    QPixmap createRoundedPixmap(const QPixmap &source);
    QString getUserName(int UserId);
    void getAvatarAndSet(int userId ,QLabel *targetLabel);

    void on_changeDate_btn_clicked();

    void on_Finish_btn_clicked();
    bool isValidEmail(const QString &email);
    void on_Add_btn_clicked();
    void addMemberToTeam(QString email);
    void refreshTeamMembers();
    QString getEmail(int userId);
    void on_Changename_btn_clicked();

    void on_Remove_btn_clicked();

    void on_Dismiss_btn_clicked();
    void ShowFileListForTeam(int teamId, int parentId);
    QIcon getFileIcon(QString suffix);
    void on_upload_team_clicked();

    void on_returnBtn_team_clicked();

    void on_refreshBtn_team_clicked();

    void on_deleteallprocess_clicked();
    QString bytesToHumanReadable(qint64 bytes);
    QString secondsToTimeString(qint64 seconds);
    void handleDeleteButtonClick(int row);
    void resumeUpload(int i);
    void updateTaskStatus(int i, qint64 uploadedBytes, qint64 totalBytes, qint64 speed);

    void verifyCurrentPassword(const QString &password, std::function<void(bool)> callback);
    void showNewPasswordDialog();
    void changePassword(const QString &newPassword, std::function<void(bool)> callback);
    void changePassward_clicked();

    void on_changePassword_btn_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QStack<int> directoryStack;
    QStack<int>directoryStack_Team;
    int UserId;
    int cutFileId;
    bool hasCut;
    int fileType;
    QNetworkAccessManager *manager;
    QMenu *fileContextMenu;//文件目录菜单
    QListWidgetItem *currentRightClickedItem;
    QMenu *trashContextMenu;//回收站菜单
    QMenu *emptyAreaContextMenu;//空白区域菜单
    QString s; // 选中的文件路径
    QNetworkReply *uploadReply=nullptr; // 上传文件的网络回复
    QProgressDialog *progressDialog=nullptr; // 上传进度对话框
    QNetworkAccessManager *networkManager;
    QList<int> copyFileIds;
    int currentParentId; // 当前目录的 ID
    int rootId;  // 新增成员变量，用于保存 rootId
    int trashBinId;  // 新增成员变量，用于保存 trashBinId
    bool isRestore;//判断是否恢复
    bool hasCopied = false;
    bool isallPasted;//是否全部粘贴
    bool isallDeleted;//是否全部删除
    bool isalltruDeleted;//是否全部被永久删除
    bool isstopallload=false;
    bool isresumeallload=false;
    QString UserEmail="";
    int nowId;
    int nowId_team;

    bool IsTeam=false;
    QList<int> cutFileIds;  // 存储要剪切的文件 ID 列表
    qint64 totalBytes;
    // 分块大小，这里设置为0.5MB
    const qint64 chunkSize = 0.5*1024 * 1024;
    QButtonGroup btng;
    QList<UploadTask> uploadTasks;
    TeamData teamData;
};

class UploadWorker : public QObject {
    Q_OBJECT

public:
    UploadWorker(const QString& filePath, int fileId, qint64 fileSize, QObject* parent = nullptr)
        : QObject(parent), filePath(filePath), fileId(fileId), fileSize(fileSize) {}

signals:
    void progressUpdated(qint64 uploadedBytes);
    void uploadFinished(bool success);

public slots:
    void startUpload() {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            emit uploadFinished(false);
            return;
        }

        QDataStream in(&file);
        qint64 uploadedBytes = 0;
        const int maxRetries = 3;
        const int batchSize = 1 * 1024 * 1024;

        while (uploadedBytes < fileSize) {
            qint64 readBytes = qMin(batchSize, fileSize - uploadedBytes);
            QByteArray chunkData;
            chunkData.resize(readBytes);
            in.readRawData(chunkData.data(), readBytes);
             QString Net_str = GlobalData::instance()->getSharedString();
            QNetworkRequest appendRequest(QUrl(QString(Net_str+"/api/FileItem/AppendContent/%1").arg(fileId)));
            appendRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
            appendRequest.setRawHeader("Content-Range", QString("bytes %1-%2/%3").arg(uploadedBytes).arg(uploadedBytes + readBytes - 1).arg(fileSize).toUtf8());
            QSslConfiguration appendSslConfig = appendRequest.sslConfiguration();
            appendSslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            appendRequest.setSslConfiguration(appendSslConfig);

            int timeout = 300 * 1000; // 设置超时时间为 300 秒（5 分钟）
            QTimer *timer = new QTimer(this);
            timer->setSingleShot(true);
            connect(timer, &QTimer::timeout, [this, timer]() {
                qDebug() << "Upload request timed out";
                timer->deleteLater();
                emit uploadFinished(false);
            });
            timer->start(timeout);

            int retryCount = 0;
            bool uploadSuccess = false;
            QNetworkReply *reply = networkManager->put(appendRequest, chunkData);

            connect(reply, &QNetworkReply::finished, [this, reply, &uploadSuccess, &uploadedBytes, &readBytes, timer]() {
                if (reply->error() == QNetworkReply::NoError) {
                    uploadedBytes += readBytes;
                    emit progressUpdated(uploadedBytes);
                    uploadSuccess = true;
                } else {
                    qDebug() << "Failed to upload chunk: " << reply->errorString();
                }
                timer->stop();
                timer->deleteLater();
                reply->deleteLater();
            });

            while (retryCount < maxRetries &&!uploadSuccess) {
                QEventLoop loop;
                connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                loop.exec();

                if (uploadSuccess) {
                    break;
                } else {
                    retryCount++;
                    QThread::msleep(500);
                }
            }

            if (!uploadSuccess) {
                qDebug() << "Failed to upload chunk after " << maxRetries << " attempts.";
                emit uploadFinished(false);
                return;
            }
        }

        emit uploadFinished(true);
        file.close();
    }

private:
    QString filePath;
    int fileId;
    qint64 fileSize;
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();
};
#endif // MAINWINDOW_H
