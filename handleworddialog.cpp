#include "handleworddialog.h"
#include "ui_handleworddialog.h"
#include<QDir>
// WPS COM 接口 CLSID (需要根据实际版本调整)
#define WPS_CLSID "{C9A3A0B0-6E89-4B7F-8F4A-3D8E5E5E5E5E}"
HandleWordDialog::HandleWordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HandleWordDialog)
{
    ui->setupUi(this);
    setMinimumSize(800, 600);
    wpsWidget=new QAxWidget(this);
    initWpsControl();
    previewDocument("C:\\Users\\13621\\Pictures\\下载\\优秀共青团干部.docx");

}

HandleWordDialog::~HandleWordDialog()
{
    delete ui;
}
bool HandleWordDialog::initWpsControl() {
    if (wpsWidget->setControl(WPS_CLSID)) {
        return true;
    }

    // 尝试通过 ProgID 创建
    const char* wpsProgIDs[] = {
        "KWPS.Application",  // WPS 文字
        "KWPP.Application",  // WPS 演示
        "KET.Application"    // WPS 表格
    };

    for (auto progID : wpsProgIDs) {
        if (wpsWidget->setControl(progID)) {
            return true;
        }
    }
    return false;
}

bool HandleWordDialog::previewDocument(const QString &filePath) {
    if (!QFile::exists(filePath)) {
        qWarning() << "File not exists:" << filePath;
        return false;
    }

    if (!initWpsControl()) {
        qWarning() << "Failed to initialize WPS control";
        return false;
    }

    try {
        // 设置显示属性
        wpsWidget->dynamicCall("SetVisible(bool)", true);
        wpsWidget->dynamicCall("SetDisplayAlerts(bool)", false);

        // 打开文档
        QVariant ret = wpsWidget->dynamicCall(
            "Open(const QString&)",
            QDir::toNativeSeparators(filePath)
        );

        if (!ret.isValid() || ret.toBool() == false) {
            qWarning() << "Failed to open document";
            return false;
        }

        // 调整视图模式
        wpsWidget->dynamicCall("SetViewType(int)", 3); // 页面视图
        return true;
    } catch (...) {
        qCritical() << "COM operation failed";
        return false;
    }
}
