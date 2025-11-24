#include "mainwindow.h"
#include "globaldata.h"
#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QRegularExpression>
// 新增头文件，用于创建标签
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建对话框
    QDialog dialog(nullptr);
    dialog.setWindowTitle("设置服务器地址");
    dialog.setMinimumWidth(300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 添加 IP 地址标签和输入框
    QLabel *ipLabel = new QLabel("IP 地址：", &dialog);
    QLineEdit *ipLineEdit = new QLineEdit("110.42.231.40", &dialog);
    ipLineEdit->setPlaceholderText("输入IP地址");
    layout->addWidget(ipLabel);
    layout->addWidget(ipLineEdit);

    // 添加端口号标签和输入框
    QLabel *portLabel = new QLabel("端口号：", &dialog);
    QLineEdit *portLineEdit = new QLineEdit("5146", &dialog);
    portLineEdit->setPlaceholderText("输入端口号");
    layout->addWidget(portLabel);
    layout->addWidget(portLineEdit);

    QPushButton *okButton = new QPushButton("确定", &dialog);
    layout->addWidget(okButton);

    // 连接确定按钮信号
    QObject::connect(okButton, &QPushButton::clicked, [&]() {
        QString ip = ipLineEdit->text().trimmed();
        QString port = portLineEdit->text().trimmed();

        if (ip.isEmpty() || port.isEmpty()) {
            QMessageBox::warning(&dialog, "输入错误", "IP 地址和端口号不能为空");
            return;
        }

        // 使用 QRegularExpression 验证 IP 格式
        QRegularExpression ipRegex("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
        if (!ipRegex.match(ip).hasMatch()) {
            QMessageBox::warning(&dialog, "输入错误", "IP 地址格式不正确");
            return;
        }

        // 验证端口格式
        bool portOk;
        int portNum = port.toInt(&portOk);
        if (!portOk || portNum < 1 || portNum > 65535) {
            QMessageBox::warning(&dialog, "输入错误", "端口号必须是 1 - 65535 之间的整数");
            return;
        }

        // 构建完整 URL 并设置到全局数据
        QString url = QString("http://%1:%2").arg(ip).arg(port);
        GlobalData::instance()->setSharedString(url);

        dialog.accept();
    });

    // 显示对话框（模态方式）
    if (dialog.exec() != QDialog::Accepted) {
        // 用户取消或关闭对话框，退出应用
        return 0;
    }

    // 创建主窗口
    MainWindow w;
    w.show();
    return a.exec();
}
