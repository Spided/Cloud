#ifndef HANDLEPDFWORDDIALOG_H
#define HANDLEPDFWORDDIALOG_H

#include"globaldata.h"
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVBoxLayout>
#include <QUrl>
#include <QFile>
#include <QProcess>
#include <QSslConfiguration>
#include <QAxWidget>
#include <QPdfView>
#include <QPdfDocument>
#include <QAxObject>
#include <QLabel>
#include <QScrollArea>
#include <QToolButton>
#include <QHBoxLayout>

namespace Ui {
class HandlePdfWordDialog;
}

class HandlePdfWordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HandlePdfWordDialog(const QUrl& url, QWidget* parent = nullptr,bool isword=false);
    ~HandlePdfWordDialog();

private slots:
    void downloadFinished(QNetworkReply *reply);
    void zoomIn();
    void zoomOut();
    void fitToWidth();
    QString getOfficeAppName();
private:
    Ui::HandlePdfWordDialog *ui;
    void preview(const QUrl& url);
    void resizeEvent(QResizeEvent *event) override; // 添加resize事件处理
    void updatePdfSize();
    QNetworkAccessManager *manager;
    QVBoxLayout* mainLayout;
    QHBoxLayout* navLayout;
    QString tempFilePath;
    QTemporaryFile *tempFile;
    QPdfView *pdfView;
    QPdfDocument *pdfDocument;
    bool convertWordToPdf(const QString& wordFilePath, const QString& pdfFilePath);
    bool isword=false;
    int currentPage = 0;
    QLabel* pageLabel;
    QScrollArea *scrollArea;
    double zoomFactor = 1.0;
    QWidget* centralWidget;
    // 导航按钮
    QPushButton* prevButton;
    QPushButton* nextButton;
    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    QToolButton *fitWidthButton;
};

#endif // HANDLEPDFWORDDIALOG_H
