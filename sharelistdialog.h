#ifndef SHARELISTDIALOG_H
#define SHARELISTDIALOG_H
#include<QStack>
#include <QDialog>
#include<QListWidgetItem>
#include"handleimagedialog.h"
#include"handlepdfworddialog.h"
#include"handletxtdialog.h"
#include"handlevideodialog.h"
#include"addsharedfiledialog.h"
#include"globaldata.h"
namespace Ui {
class ShareListDialog;
}

class ShareListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShareListDialog(QWidget *parent = nullptr);
    ~ShareListDialog();
    void getrootId(int id){rootId=id;}
    void getUserId(int id){UserId=id;}
    void getlink(QString link,QString code);
private slots:
    void on_pushButton_clicked();
    void on_lastBtn_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ShareListDialog *ui;
    QString decryptLink(const QString& encryptedUrl);//解码
    void downloadFile();
    void showFileList(int parentId);
    void FileItemClicked(const QPoint &pos);
    void SaveFile();
    void doubleFileItem(QListWidgetItem *item);
    void PreviewFile(int fileId);
    void PreviewItem();    
    QString baseUrl = GlobalData::instance()->getSharedString()+"/api/FileItem/";
    QListWidgetItem *currentRightClickedItem;
    QStack<int> directoryStack;
    int rootId;
    int UserId;
    int inittype;
    QMenu *fileContextMenu;//文件目录菜单
};

#endif // SHARELISTDIALOG_H
