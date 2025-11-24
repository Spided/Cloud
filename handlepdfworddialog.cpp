#include "handlepdfworddialog.h"
#include "ui_handlepdfworddialog.h"
#include <QTemporaryFile>
#include <QDebug>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QThread>
#include <QSurfaceFormat>
#include <QPdfPageNavigator>
#include <QPointer>
#include <QStyle>
#include <QScrollArea>
#include <QToolButton>
#include <QBoxLayout>
#include <QResizeEvent>
#include <QScreen>
#include <QPushButton>
#include <QStackedLayout>
#include <QApplication>
#include <QTextDocument>
#include <QTextDocumentWriter>

HandlePdfWordDialog::HandlePdfWordDialog(const QUrl& url, QWidget* parent, bool isw) :
    QDialog(parent),
    ui(new Ui::HandlePdfWordDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_NativeWindow, true);
    setStyleSheet("background-color: white;");
    resize(800, 600);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);
    // 创建主布局 - 只包含滚动区域和导航栏
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建并设置滚动区域 - 占据主要空间
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(scrollArea, 1); // 伸缩因子为1，占据所有可用空间

    // 创建PDF视图容器
    QWidget *pdfContainer = new QWidget();
    pdfContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // PDF视图 - 放在容器内
    pdfView = new QPdfView(pdfContainer);
    pdfView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // PDF容器布局 - 确保PDF视图填充容器
    QVBoxLayout *containerLayout = new QVBoxLayout(pdfContainer);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(pdfView);

    // 将PDF容器设置为滚动区域的内容
    scrollArea->setWidget(pdfContainer);

    // 设置PDF文档
    pdfDocument = new QPdfDocument(this);
    pdfView->setDocument(pdfDocument);

    // 设置网络管理器和临时文件
    manager = new QNetworkAccessManager(this);
    isword = isw;
    connect(manager, &QNetworkAccessManager::finished, this, &HandlePdfWordDialog::downloadFinished);
    tempFile = new QTemporaryFile(this);

    // 创建导航栏布局
    navLayout = new QHBoxLayout();
    navLayout->setContentsMargins(10, 5, 10, 5);

    // 页码标签
    pageLabel = new QLabel("准备中...", this);

    // 上一页按钮
    prevButton = new QPushButton("←", this);
    prevButton->setFixedSize(30, 30);
    prevButton->setEnabled(false);

    // 下一页按钮
    nextButton = new QPushButton("→", this);
    nextButton->setFixedSize(30, 30);
    nextButton->setEnabled(false);

    // 放大按钮
    zoomInButton = new QToolButton(this);
    zoomInButton->setText("+");
    zoomInButton->setToolTip("放大");
    zoomInButton->setFixedSize(30, 30);
    zoomInButton->setEnabled(false);

    // 缩小按钮
    zoomOutButton = new QToolButton(this);
    zoomOutButton->setText("-");
    zoomOutButton->setToolTip("缩小");
    zoomOutButton->setFixedSize(30, 30);
    zoomOutButton->setEnabled(false);

    // 适应宽度按钮
    fitWidthButton = new QToolButton(this);
    fitWidthButton->setText("⤢");
    fitWidthButton->setToolTip("适应宽度");
    fitWidthButton->setFixedSize(30, 30);
    fitWidthButton->setEnabled(false);

    // 连接信号槽
    connect(prevButton, &QPushButton::clicked, [this](){
        if(currentPage > 0) {
            pdfView->pageNavigator()->jump(currentPage - 1, {}, {});
            currentPage--;
            pageLabel->setText(QString("第 %1 页 / 共 %2 页")
                               .arg(currentPage + 1)
                               .arg(pdfDocument->pageCount()));
        }
    });

    connect(nextButton, &QPushButton::clicked, [this](){
        if(currentPage < pdfDocument->pageCount() - 1) {
            pdfView->pageNavigator()->jump(currentPage + 1, {}, {});
            currentPage++;
            pageLabel->setText(QString("第 %1 页 / 共 %2 页")
                               .arg(currentPage + 1)
                               .arg(pdfDocument->pageCount()));
        }
    });

    connect(zoomInButton, &QToolButton::clicked, this, &HandlePdfWordDialog::zoomIn);
    connect(zoomOutButton, &QToolButton::clicked, this, &HandlePdfWordDialog::zoomOut);
    connect(fitWidthButton, &QToolButton::clicked, this, &HandlePdfWordDialog::fitToWidth);

    // 将导航元素添加到布局
    navLayout->addWidget(prevButton);
    navLayout->addWidget(nextButton);
    navLayout->addWidget(pageLabel);
    navLayout->addStretch();
    navLayout->addWidget(fitWidthButton);
    navLayout->addWidget(zoomOutButton);
    navLayout->addWidget(zoomInButton);

    // 将导航栏添加到主布局底部
    mainLayout->addLayout(navLayout);

    // 设置主布局
    setLayout(mainLayout);

    // 开始预览
    preview(url);
}

void HandlePdfWordDialog::resizeEvent(QResizeEvent *event) {
    QDialog::resizeEvent(event);
    updatePdfSize(); // 窗口大小变化时更新PDF大小
}

void HandlePdfWordDialog::updatePdfSize() {
    // 强制更新布局
    QApplication::processEvents();

    // 设置滚动区域大小
    if (scrollArea && scrollArea->widget()) {
        scrollArea->widget()->setMinimumSize(scrollArea->viewport()->size());
    }

    // 更新缩放
    fitToWidth();
}

HandlePdfWordDialog::~HandlePdfWordDialog()
{
    QFileInfo fi(tempFilePath);
    if (fi.exists()) {
        QFile::remove(tempFilePath);
        if(isword) QFile::remove(tempFilePath + ".pdf");
    }

    if(pdfDocument) {
        pdfDocument->close();
    }

    delete ui;
}

void HandlePdfWordDialog::preview(const QUrl& url)
{
    QNetworkRequest request(url);
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);
    manager->get(request);
}

void HandlePdfWordDialog::downloadFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "下载错误:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QString finalFilePath;

    if (isword) {
        setWindowTitle("Word预览");
        QTemporaryFile wordTempFile(QDir::tempPath() + "/XXXXXX.docx");
        wordTempFile.setAutoRemove(false);
        if (!wordTempFile.open()) {
            qDebug() << "无法创建Word临时文件";
            reply->deleteLater();
            return;
        }
        wordTempFile.write(data);
        wordTempFile.close();
        tempFilePath = wordTempFile.fileName();
        QString pdfFilePath = tempFilePath + ".pdf";
        if (convertWordToPdf(tempFilePath, pdfFilePath)) {
            finalFilePath = pdfFilePath;
        } else {
            qDebug() << "Word转PDF失败";
            reply->deleteLater();
            return;
        }
    } else {
        setWindowTitle("PDF预览");
        tempFile->setFileTemplate(QDir::tempPath() + "/XXXXXX");
        if (!tempFile->open() || !tempFile->write(data)) {
            qDebug() << "无法写入临时文件";
            reply->deleteLater();
            return;
        }
        tempFile->close();
        tempFilePath = tempFile->fileName();
        finalFilePath = tempFilePath;
    }

    QTimer::singleShot(100, this, [this, finalFilePath]() {
        if (!QFile::exists(finalFilePath)) {
            qDebug() << "文件不存在:" << finalFilePath;
            return;
        }

        // 加载PDF文档
        pdfDocument->load(finalFilePath);

        // 连接文档状态变化信号
        if(pdfDocument->status() == QPdfDocument::Status::Ready) {
            prevButton->setEnabled(true);
            nextButton->setEnabled(true);
            zoomInButton->setEnabled(true);
            zoomOutButton->setEnabled(true);
            fitWidthButton->setEnabled(true);

            currentPage = 0;
            if(pdfDocument->pageCount() > 0) {
                pageLabel->setText(QString("第 %1 页 / 共 %2 页")
                                  .arg(currentPage + 1)
                                  .arg(pdfDocument->pageCount()));
                pdfView->pageNavigator()->jump(0, {}, {});
                updatePdfSize(); // 文档加载后更新大小
            } else {
                pageLabel->setText("文档无内容");
            }
        }
        else if(pdfDocument->status() == QPdfDocument::Status::Loading) {
            // 如果仍在加载，设置定时器检查
            QTimer::singleShot(100, this, [this]() {
                if(pdfDocument->status() == QPdfDocument::Status::Ready) {
                    prevButton->setEnabled(true);
                    nextButton->setEnabled(true);
                    zoomInButton->setEnabled(true);
                    zoomOutButton->setEnabled(true);
                    fitWidthButton->setEnabled(true);

                    currentPage = 0;
                    if(pdfDocument->pageCount() > 0) {
                        pageLabel->setText(QString("第 %1 页 / 共 %2 页")
                                          .arg(currentPage + 1)
                                          .arg(pdfDocument->pageCount()));
                        pdfView->pageNavigator()->jump(0, {}, {});
                        updatePdfSize(); // 文档加载后更新大小
                    }
                }
            });
        }
        else if(pdfDocument->status() == QPdfDocument::Status::Error) {
            qDebug() << "文档加载错误";
        }
    });

    reply->deleteLater();
}

void HandlePdfWordDialog::zoomIn() {
    zoomFactor *= 1.2;
    if (zoomFactor > 5.0) zoomFactor = 5.0;

    pdfView->setZoomFactor(zoomFactor);

}

void HandlePdfWordDialog::zoomOut() {
    zoomFactor /= 1.2;
    if (zoomFactor < 0.1) zoomFactor = 0.1;

    pdfView->setZoomFactor(zoomFactor);

}

void HandlePdfWordDialog::fitToWidth() {
    // 确保滚动区域和视图可用
    if (!scrollArea || !scrollArea->viewport()) {
        return;
    }

    // 计算视口宽度
    int viewportWidth = scrollArea->viewport()->width() - 20; // 减去滚动条宽度
    int pageWidth = 800;

    // 计算新的缩放因子
    zoomFactor = qBound(0.1, static_cast<double>(viewportWidth) / pageWidth, 5.0);

    // 应用缩放
    pdfView->setZoomFactor(zoomFactor);
    //scrollArea->setWidgetResizable(false);

    // 确保内容正确填充
    if (scrollArea->widget()) {
        scrollArea->widget()->setMinimumSize(scrollArea->viewport()->size());
    }
}

bool HandlePdfWordDialog::convertWordToPdf(const QString& wordFilePath, const QString& pdfFilePath)
{
    QString appName = getOfficeAppName(); // 动态获取可用办公软件

    if (appName.isEmpty()) {
        qWarning() << "No compatible office application found (WPS or Microsoft Office)";
        return false;
    }

    QScopedPointer<QAxObject> officeApp(new QAxObject(appName));
    if (!officeApp->isNull()) {
        try {
            // 设置应用不可见
            officeApp->dynamicCall("SetVisible(bool)", false);

            // 打开文档
            QScopedPointer<QAxObject> documents(officeApp->querySubObject("Documents"));
            if (documents.isNull()) {
                qWarning() << "Failed to access Documents interface";
                return false;
            }

            QScopedPointer<QAxObject> document(documents->querySubObject("Open(const QString&)", wordFilePath));
            if (document.isNull()) {
                qWarning() << "Failed to open document:" << wordFilePath;
                return false;
            }

            // PDF格式代码（Word和WPS使用相同的值17）
            const int wdFormatPDF = 17;

            // 使用事件循环确保操作完成
            QEventLoop loop;
            QTimer::singleShot(1000, &loop, &QEventLoop::quit);

            // 保存为PDF
            QVariant result = document->dynamicCall("SaveAs(const QString&, int)", pdfFilePath, wdFormatPDF);

            // 关闭文档和退出应用
            document->dynamicCall("Close(bool)", false); // 不保存修改
            officeApp->dynamicCall("Quit()");

            loop.exec();

            return QFile::exists(pdfFilePath);
        } catch (...) {
            qCritical() << "Exception occurred during conversion";
            return false;
        }
    }

    qWarning() << "Failed to initialize office application:" << appName;
    return false;
}

// 检测系统中可用的办公软件
QString HandlePdfWordDialog::getOfficeAppName()
{
    // 尝试创建各种可能的办公应用对象
    const QStringList possibleApps = {
        "KWPS.Application",         // WPS Office
        "Ket.Application",          // Kingsoft Office（兼容模式）
        "Word.Application",         // Microsoft Word
        "Microsoft.Office.Word"     // 新版Microsoft Office
    };

    foreach (const QString &appName, possibleApps) {
        QAxObject app(appName);
        if (!app.isNull()) {
            qDebug() << "Found office application:" << appName;
            return appName;
        }
    }

    return QString();
}
