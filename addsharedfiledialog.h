#ifndef ADDSHAREDFILEDIALOG_H
#define ADDSHAREDFILEDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include"globaldata.h"
namespace Ui {
class AddSharedFileDialog;
}

class AddSharedFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSharedFileDialog(QWidget *parent = nullptr,int id=-1,int fileid=-1);
    ~AddSharedFileDialog();

private slots:
    void on_newfoldbtn_clicked();

    void on_cancelbtn_clicked();

    void on_confirmbtn_clicked();

private:
    Ui::AddSharedFileDialog *ui;
    void onRootFilesReceived(QNetworkReply *reply);
    void onDirectoryFilesReceived(QNetworkReply *reply);
    int UserId; // 全局变量，当前用户 ID
    int rootId;
    int fileId;
    void buildFolderTree(const QJsonArray &files, QTreeWidgetItem *parentItem = nullptr);
};

#endif // ADDSHAREDFILEDIALOG_H
