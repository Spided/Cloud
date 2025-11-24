#ifndef HANDLEIMAGEDIALOG_H
#define HANDLEIMAGEDIALOG_H
#include"globaldata.h"
#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include <QUrl>
#include<QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include<QDir>
#include <QSizePolicy>

namespace Ui {
class HandleImageDialog;
}

class HandleImageDialog : public QDialog
{
    Q_OBJECT

public:
    HandleImageDialog(const QUrl& imageUrl, QWidget* parent = nullptr) : QDialog(parent)
    {
        // 设置对话框属性
        setWindowTitle("图片预览");
        setMinimumSize(400, 300); // 设置最小尺寸，允许窗口缩小到合理范围
        setParent(nullptr);
        Qt::WindowFlags windowFlag = Qt::Dialog;
        windowFlag |= Qt::WindowMinimizeButtonHint;
        windowFlag |= Qt::WindowMaximizeButtonHint;
        windowFlag |= Qt::WindowCloseButtonHint;
        setWindowFlags(windowFlag);

        // 创建网络访问管理器
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        QNetworkRequest request(imageUrl);

        // 设置 SSL 配置
        QSslConfiguration sslConfig = request.sslConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslConfig);

        // 发送请求
        QNetworkReply* reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                // 保存临时文件
                QFile tempFile(QDir::tempPath() + "/" + reply->url().fileName());
                if (tempFile.open(QIODevice::WriteOnly)) {
                    tempFile.write(reply->readAll());
                    tempFile.close();
                    QString tempFileName = tempFile.fileName();

                    // 创建图片标签并设置尺寸策略
                    imageLabel = new QLabel(this);
                    //首先要确定imageLabel的大小，不然默认0*0导致图片很糊
                    imageLabel->resize(400,300);
                    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // 关键：允许标签扩展
                    imageLabel->setScaledContents(true); // 自动缩放图片适应标签

                    originalPixmap = QPixmap(tempFileName);
                    if (!originalPixmap.isNull()) {
                        updateImageDisplay();
                    } else {
                        imageLabel->setText("无法加载图片");
                    }

                    // 创建布局并添加标签（移除固定尺寸）
                    QVBoxLayout* layout = new QVBoxLayout(this);
                    layout->setContentsMargins(0, 0, 0, 0);
                    layout->addWidget(imageLabel);
                    setLayout(layout);

                    // 预览结束后删除临时文件
                    connect(this, &QDialog::finished, this, [tempFileName]() {
                        QFile::remove(tempFileName);
                    });
                }
            } else {
                QLabel* errorLabel = new QLabel(this);
                errorLabel->setText("无法下载图片");
                QVBoxLayout* layout = new QVBoxLayout(this);
                layout->addWidget(errorLabel);
                setLayout(layout);
            }
            reply->deleteLater();
        });
    }

    void resizeEvent(QResizeEvent *event) override
    {
        QDialog::resizeEvent(event);
        if (!originalPixmap.isNull()) {
            updateImageDisplay();
        }
    }

    ~HandleImageDialog(){};

    void updateImageDisplay()
    {
        if (imageLabel) {
            QSize labelSize = imageLabel->size();
            // 无比例缩放图片
            QPixmap scaledPixmap = originalPixmap.scaled(
                labelSize,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation
            );
            imageLabel->setPixmap(scaledPixmap);
        }
    }

private:

    QLabel* imageLabel;
    QPixmap originalPixmap;
};

#endif // HANDLEIMAGEDIALOG_H
