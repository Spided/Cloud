#include "handleexceldialog.h"
#include "ui_handleexceldialog.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QProcess>
#include <QGuiApplication>
#include <QScreen>
#include <QAxObject>
#include <QFileIconProvider>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include<QLabel>
#include<QProgressBar>
#include<QDesktopServices>
HandleExcelDialog::HandleExcelDialog(const QUrl& excelUrl, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::HandleExcelDialog),
      excelControl(nullptr)
{
    ui->setupUi(this);

    // 设置窗口属性
    initUi();

    // 创建临时文件
    QDir tempDir(QDir::temp());
    tempFile = new QTemporaryFile(tempDir.absoluteFilePath("XXXXXX.xlsx"), this);

    // 准备下载
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &HandleExcelDialog::downloadFinished);

    // 开始下载
    startDownload(excelUrl);
}

void HandleExcelDialog::initUi()
{
    setAttribute(Qt::WA_NativeWindow, true);
    setStyleSheet("background-color: white;");
    setWindowTitle("Excel 预览 - 加载中...");

    // 获取屏幕尺寸并设置窗口大小
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int width = screenGeometry.width() * 0.7;
    int height = screenGeometry.height() * 0.7;
    resize(width, height);

    // 设置布局
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    // 添加标题和图标
    QHBoxLayout* headerLayout = new QHBoxLayout;
    QLabel* iconLabel = new QLabel(this);
    QFileIconProvider iconProvider;
    QIcon excelIcon = QIcon(":/FileIcons/source/EXCEL.png");
    iconLabel->setPixmap(excelIcon.pixmap(32, 32));

    QLabel* titleLabel = new QLabel("Excel 文件预览", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    headerLayout->addWidget(iconLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    layout->addLayout(headerLayout);

    // 添加状态信息
    QLabel* statusLabel = new QLabel("正在加载 Excel 文件，请稍候...", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #555; font-style: italic;");
    layout->addWidget(statusLabel);

    // 添加进度条
    QProgressBar* progressBar = new QProgressBar(this);
    progressBar->setRange(0, 0); // 不确定进度
    progressBar->setTextVisible(false);
    layout->addWidget(progressBar);

    // 创建预览区域
    excelWidget = new QWidget(this);
    excelWidget->setStyleSheet("background-color: #f0f0f0; border: 1px solid #ddd;");
    excelWidget->setMinimumSize(600, 400);
    layout->addWidget(excelWidget, 1); // 添加拉伸因子
}

void HandleExcelDialog::startDownload(const QUrl& excelUrl)
{
    QNetworkRequest request(excelUrl);
    manager->get(request);
}

HandleExcelDialog::~HandleExcelDialog()
{
    cleanUpResources();
}

void HandleExcelDialog::cleanUpResources()
{
    // 关闭Excel控件
    if (excelControl) {
        excelControl->clear();
        excelControl->close();
        excelControl->deleteLater();
        excelControl = nullptr;
    }

    // 删除临时文件
    if (!tempFilePath.isEmpty() && QFile::exists(tempFilePath)) {
        QFile file(tempFilePath);
        if (!file.remove()) {
            qDebug() << "未能删除临时文件:" << tempFilePath;
        }
        tempFilePath.clear();
    }
}

void HandleExcelDialog::downloadFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "下载错误", "下载Excel文件失败: " + reply->errorString());
        reply->deleteLater();
        close();
        return;
    }

    // 保存内容到临时文件
    if (!tempFile->open()) {
        QMessageBox::critical(this, "文件错误", "无法创建临时文件");
        reply->deleteLater();
        close();
        return;
    }

    tempFile->write(reply->readAll());
    tempFile->close();
    tempFilePath = tempFile->fileName();

    // 验证文件
    QFileInfo fileInfo(tempFilePath);
    if (!fileInfo.exists() || fileInfo.size() == 0) {
        QMessageBox::critical(this, "文件错误", "临时文件创建失败");
        reply->deleteLater();
        close();
        return;
    }

    qDebug() << "临时文件创建成功:" << tempFilePath << "大小:" << fileInfo.size() << "字节";

    // 更新UI
    setWindowTitle(QString("Excel 预览 - %1").arg(QFileInfo(tempFilePath).fileName()));

    // 尝试预览Excel文件
    QTimer::singleShot(100, this, [this]() {
        if (!tryCreateExcelControl(tempFilePath)) {
            // 直接启动WPS作为后备方案
            if (!launchExternalWps(tempFilePath)) {
                QMessageBox::warning(this, "预览失败",
                                     "无法预览Excel文件\n"
                                     "系统可能未安装WPS Office或Microsoft Excel");
                close();
            } else {
                accept(); // 关闭对话框
            }
        }
    });

    reply->deleteLater();
}

bool HandleExcelDialog::hasInstalledWps()
{
    // 检查注册表中的安装信息
    QString registryPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\et.exe";
    QSettings settings(registryPath, QSettings::Registry64Format);
    QString installPath = settings.value("Default").toString();

    // 检查文件是否存在
    if (!installPath.isEmpty() && QFile::exists(installPath)) {
        return true;
    }

    // 检查默认安装路径
    QStringList possiblePaths = {
        "C:\\Program Files\\WPS Office\\et.exe",
        "C:\\Program Files (x86)\\WPS Office\\et.exe",
        "C:\\Program Files\\Kingsoft\\WPS Office\\et.exe",
        "C:\\Program Files (x86)\\Kingsoft\\WPS Office\\et.exe"
    };

    for (const QString& path : possiblePaths) {
        if (QFile::exists(path)) {
            return true;
        }
    }

    return false;
}

QString HandleExcelDialog::detectWpsProgId()
{
    // 尝试创建可能的 WPS 表格应用对象
    struct ProgIdInfo {
        const char* progId;
        const char* description;
    };

    const ProgIdInfo candidates[] = {
        {"et.Application", "WPS Office 2019 表格应用"},
        {"ket.Application", "金山 WPS 2016 表格应用"},
        {"et.Sheet", "WPS 表格控件"},
        {"ket.Sheet", "金山 WPS 表格控件"},
        {"Excel.Application", "Microsoft Excel 应用程序"},
        {"Excel.Sheet", "Microsoft Excel 工作表"}
    };

    for (const auto& candidate : candidates) {
        QAxObject app(candidate.progId);
        if (!app.isNull()) {
            qDebug() << "检测到可用的 Excel 应用类型:" << candidate.description;
            return candidate.progId;
        }
    }

    return QString();
}

bool HandleExcelDialog::tryCreateExcelControl(const QString& filePath)
{
    QString nativePath = QDir::toNativeSeparators(filePath);

    // 检测可用的 WPS/Excel ProgID
    QString progId = detectWpsProgId();
    if (progId.isEmpty()) {
        qDebug() << "未找到可用的 Excel 应用组件";
        return false;
    }

    // 创建控件
    excelControl = new QAxWidget(excelWidget);
    if (!excelControl->setControl(progId)) {
        qDebug() << "无法创建 Excel 控件:" << progId;
        delete excelControl;
        excelControl = nullptr;
        return false;
    }

    // 连接到异常信号
    connect(excelControl, SIGNAL(exception(int, const QString&, const QString&, const QString&)),
            this, SLOT(handleException(int, const QString&, const QString&, const QString&)));

    QVariant openResult;

    // 对于不同版本的 WPS/Excel 需要使用不同的打开方式
    if (progId == "et.Application" || progId == "ket.Application") {
        // WPS 应用程序对象
        try {
            // 获取 Workbooks 集合
            QAxObject* workbooks = excelControl->querySubObject("Workbooks");
            if (!workbooks) {
                qDebug() << "无法获取 Workbooks 集合";
                return false;
            }

            // 打开文件
            openResult = workbooks->dynamicCall("Open(QString)", nativePath);
            qDebug() << "通过 Workbooks.Open 打开文件结果:" << openResult.toString();
        }
        catch (...) {
            qDebug() << "尝试通过 Workbooks.Open 打开文件时出错";
            return false;
        }
    } else if (progId == "et.Sheet" || progId == "ket.Sheet") {
        // WPS 表格控件
        try {
            // 设置控件属性
            excelControl->setProperty("Visible", true);
            excelControl->setProperty("DisplayAlerts", false);

            // 打开文件
            openResult = excelControl->dynamicCall("Open(QString)", nativePath);
            qDebug() << "通过控件 Open 方法打开文件结果:" << openResult.toString();
        }
        catch (...) {
            qDebug() << "尝试通过控件 Open 方法打开文件时出错";
            return false;
        }
    } else if (progId.contains("Excel")) {
        // Microsoft Excel 的兼容方案
        try {
            QAxObject* workbooks = excelControl->querySubObject("Workbooks");
            if (workbooks) {
                openResult = workbooks->dynamicCall("Open(QString)", nativePath);
            }
        }
        catch (...) {
            qDebug() << "尝试在 Microsoft Excel 中打开文件时出错";
            return false;
        }
    }

    // 检查是否成功打开
    if (openResult.isValid() && (openResult.toBool() || !openResult.toString().isEmpty())) {
        qDebug() << "Excel 文件已成功加载";

        // 添加到布局
        excelControl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        excelControl->setParent(excelWidget);

        // 更新窗口标题
        QString fileName = QFileInfo(nativePath).fileName();
        setWindowTitle(QString("Excel 预览 - %1").arg(fileName));

        return true;
    }

    qDebug() << "无法在控件中打开 Excel 文件:" << nativePath;
    return false;
}

bool HandleExcelDialog::launchExternalWps(const QString& filePath)
{
    // 查找 WPS 安装路径
    QString wpsPath;

    // 首先检查注册表中的安装信息
    QString registryPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\et.exe";
    QSettings settings(registryPath, QSettings::Registry64Format);
    wpsPath = settings.value("Default").toString();

    // 如果未在注册表中找到，检查常见安装位置
    if (wpsPath.isEmpty() || !QFile::exists(wpsPath)) {
        QStringList possiblePaths = {
            "C:\\Program Files\\WPS Office\\et.exe",
            "C:\\Program Files (x86)\\WPS Office\\et.exe",
            "C:\\Program Files\\Kingsoft\\WPS Office\\et.exe",
            "C:\\Program Files (x86)\\Kingsoft\\WPS Office\\et.exe"
        };

        for (const QString& path : possiblePaths) {
            if (QFile::exists(path)) {
                wpsPath = path;
                break;
            }
        }
    }

    // 如果找到 WPS，启动它
    if (!wpsPath.isEmpty()) {
        QProcess::startDetached(wpsPath, {filePath});
        qDebug() << "已启动外部 WPS 应用程序:" << wpsPath;
        return true;
    }

    // 尝试直接使用系统关联程序打开
    qDebug() << "尝试使用默认程序打开文件";
    return QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void HandleExcelDialog::handleException(int code, const QString& source, const QString& desc, const QString& help)
{
    qDebug() << "COM 异常:"
             << "\n  代码:" << code
             << "\n  来源:" << source
             << "\n  描述:" << desc
             << "\n  帮助:" << help;

    // 如果出现异常，尝试使用外部程序打开
    launchExternalWps(tempFilePath);
    close();
}
