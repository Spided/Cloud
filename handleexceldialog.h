#ifndef HANDLEEXCELDIALOG_H
#define HANDLEEXCELDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QWidget>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QTimer>
#include <QMessageBox>
#include <QAxWidget>
#include <QFileIconProvider>

namespace Ui {
class HandleExcelDialog;
}
//这个excel的预览一直没有做好
class HandleExcelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HandleExcelDialog(const QUrl& excelUrl, QWidget* parent = nullptr);
    ~HandleExcelDialog();

private:
    Ui::HandleExcelDialog *ui;
    QVBoxLayout* layout;
    QWidget* excelWidget;
    QNetworkAccessManager *manager;
    QTemporaryFile *tempFile;
    QString tempFilePath;

    QAxWidget* excelControl;

    void downloadFinished(QNetworkReply *reply);
    void previewExcel(const QString& filePath);
    void startDownload(const QUrl& excelUrl);
    void cleanUpResources();
    void initUi();
    bool tryCreateExcelControl(const QString& filePath);

    // 详细的 WPS 检测功能
    QString detectWpsProgId();
    bool hasInstalledWps();
    QString getWpsExcelProgId();
    bool launchExternalWps(const QString& filePath);

private slots:
    void handleException(int code, const QString& source, const QString& desc, const QString& help);
};

#endif // HANDLEEXCELDIALOG_H
