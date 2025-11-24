#ifndef HANDLEWORDDIALOG_H
#define HANDLEWORDDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFile>
#include <QWidget>
#include <QAxWidget>
#include <QFileInfo>
namespace Ui {
class HandleWordDialog;
}

class HandleWordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HandleWordDialog(QWidget *parent = nullptr);
    ~HandleWordDialog();
    bool previewDocument(const QString &filePath);

private:
    Ui::HandleWordDialog *ui;
    QAxWidget *wpsWidget;
      bool initWpsControl();
      QString filepath;
};

#endif // HANDLEWORDDIALOG_H
