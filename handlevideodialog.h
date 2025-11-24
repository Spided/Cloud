#ifndef HANDLEVIDEODIALOG_H
#define HANDLEVIDEODIALOG_H

#include "globaldata.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QUrl>
#include <QCloseEvent>
#include <QAudioOutput>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QIcon>
#include <QTime>
#include <QMediaMetaData>
#include <QStyle>

class HandleVideoDialog : public QDialog
{
    Q_OBJECT

public:
    HandleVideoDialog(const QUrl& videoUrl, QWidget* parent = nullptr, int type = 0)
        : QDialog(parent),
          type(type)
    {
        resize(800, 600);
        setWindowTitle(type == 1 ? "MP3音频预览" : "MP4视频预览");
        setParent(nullptr);
        Qt::WindowFlags windowFlag = Qt::Dialog;
        windowFlag |= Qt::WindowMinimizeButtonHint;
        windowFlag |= Qt::WindowMaximizeButtonHint;
        windowFlag |= Qt::WindowCloseButtonHint;
        setWindowFlags(windowFlag);
        // 初始化媒体相关组件
        mediaPlayer = new QMediaPlayer(this);
        audioOutput = new QAudioOutput(this);
        mediaPlayer->setAudioOutput(audioOutput);
        mediaPlayer->setSource(videoUrl);

        // 构建控制栏
        buildControlWidgets();

        // 构建主布局
        QVBoxLayout* mainLayout = new QVBoxLayout(this);
        if (type == 1) {
            // 音频模式：添加背景
            setupAudioModeUI(mainLayout);
        } else {
            // 视频模式：添加视频渲染部件
            setupVideoModeUI(mainLayout);
        }

        // 连接媒体状态变更信号
        connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &HandleVideoDialog::updatePosition);
        connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &HandleVideoDialog::updateDuration);
        connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &HandleVideoDialog::updatePlayButton);
        connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &HandleVideoDialog::handleMediaStatus);

        // 启动播放
        mediaPlayer->play();
    }

    ~HandleVideoDialog()
    {
        mediaPlayer->stop();
    }

private slots:
    void togglePlay()
    {
        if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
            mediaPlayer->pause();
        } else {
            mediaPlayer->play();
        }
    }

    void seekToPosition(int position)
    {
        mediaPlayer->setPosition(position);
    }

    void adjustVolume(int volume)
    {
        audioOutput->setVolume(volume / 100.0);
    }

    void updatePosition(qint64 pos)
    {
        if (!positionSlider->isSliderDown()) {
            positionSlider->setValue(pos);
        }
        updateTimeLabel(pos, mediaPlayer->duration());
    }

    void updateDuration(qint64 duration)
    {
        positionSlider->setRange(0, duration);
        updateTimeLabel(mediaPlayer->position(), duration);
    }

    void updatePlayButton()
    {
        if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        } else {
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }
    }

    void handleMediaStatus(QMediaPlayer::MediaStatus status)
    {
        // 修复音频模式下不自动播放的问题
        if (type == 1 && status == QMediaPlayer::LoadedMedia) {
            // 音频模式下确保播放器开始播放
            if (mediaPlayer->playbackState() != QMediaPlayer::PlayingState) {
                mediaPlayer->play();
            }
        }
    }

private:
    int type;
    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;
    QSlider* positionSlider;
    QPushButton* playButton;
    QSlider* volumeSlider;
    QLabel* timeLabel;
    QVideoWidget* videoWidget = nullptr;

    // 构建控制栏
    void buildControlWidgets()
    {
        playButton = new QPushButton(this);
        playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        playButton->setFixedSize(32, 32);
        connect(playButton, &QPushButton::clicked, this, &HandleVideoDialog::togglePlay);

        positionSlider = new QSlider(Qt::Horizontal, this);
        positionSlider->setRange(0, 0);
        connect(positionSlider, &QSlider::sliderMoved, this, &HandleVideoDialog::seekToPosition);

        volumeSlider = new QSlider(Qt::Horizontal, this);
        volumeSlider->setRange(0, 100);
        volumeSlider->setValue(70);
        connect(volumeSlider, &QSlider::valueChanged, this, &HandleVideoDialog::adjustVolume);

        timeLabel = new QLabel("00:00/00:00", this);
    }

    // 视频模式UI：包含视频渲染部件
    void setupVideoModeUI(QVBoxLayout* mainLayout)
    {
        videoWidget = new QVideoWidget(this);
        mediaPlayer->setVideoOutput(videoWidget); // 关联视频输出

        QVBoxLayout* videoLayout = new QVBoxLayout();
        videoLayout->addWidget(videoWidget);

        // 控制栏布局
        QHBoxLayout* controlLayout = new QHBoxLayout();
        controlLayout->addWidget(playButton);
        controlLayout->addWidget(positionSlider);
        controlLayout->addWidget(timeLabel);
        controlLayout->addWidget(volumeSlider);

        mainLayout->addLayout(videoLayout);
        mainLayout->addLayout(controlLayout);
    }

    // 音频模式UI：确保媒体播放器正确初始化
    void setupAudioModeUI(QVBoxLayout* mainLayout)
    {
        // 背景图
        QLabel* backgroundLabel = new QLabel(this);
        backgroundLabel->setPixmap(QPixmap(":/FileIcons/source/background.png"));
        backgroundLabel->setScaledContents(true);

        // 关键修复：音频模式下设置空视频输出
        mediaPlayer->setVideoOutput(nullptr); // 明确设置视频输出为nullptr

        // 控制栏布局
        QHBoxLayout* controlLayout = new QHBoxLayout();
        controlLayout->addWidget(playButton);
        controlLayout->addWidget(positionSlider);
        controlLayout->addWidget(timeLabel);
        controlLayout->addWidget(volumeSlider);

        mainLayout->addWidget(backgroundLabel);
        mainLayout->addLayout(controlLayout);
    }

    // 更新时间显示
    void updateTimeLabel(qint64 current, qint64 total)
    {
        QTime currentTime = QTime::fromMSecsSinceStartOfDay(current);
        QTime totalTime = QTime::fromMSecsSinceStartOfDay(total);
        timeLabel->setText(
            currentTime.toString("mm:ss") + "/" +
            totalTime.toString("mm:ss")
        );
    }

protected:
    void closeEvent(QCloseEvent* event) override
    {
        mediaPlayer->stop();
        event->accept();
    }
};

#endif // HANDLEVIDEODIALOG_H
