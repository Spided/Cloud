#ifndef HANDLETXTDIALOG_H
#define HANDLETXTDIALOG_H
#include"globaldata.h"
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QProcess>
#include <QSslConfiguration>
#include <QSslCertificate>

namespace Ui {
class HandleTxtDialog;
}

class HandleTxtDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HandleTxtDialog(const QUrl& txtUrl, QWidget* parent = nullptr);
    ~HandleTxtDialog();

private slots:
    void downloadFinished(QNetworkReply *reply);

private:
    Ui::HandleTxtDialog *ui;
    QNetworkAccessManager *manager;
    QTextEdit *textEdit;  // 用于显示文本内容的文本编辑框
    QVBoxLayout *layout;
    QString tempFilePath;
    QTemporaryFile *tempFile;  // 用于保存临时文件

    void previewTxt(const QUrl& txtUrl);
};
#endif // HANDLETXTDIALOG_H
