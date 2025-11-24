#ifndef SHAREFILEDIALOG_H
#define SHAREFILEDIALOG_H

#include <QDialog>
#include"globaldata.h"
namespace Ui {
class ShareFileDialog;
}

class ShareFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShareFileDialog(QWidget *parent = nullptr);
    ~ShareFileDialog();
    void getid(int id){fileId=id;}
    QString returnTheUrl(bool &issecret){issecret=isencoded;return FinalUrl;}

private slots:
    void on_pushButton_clicked();
    void onSelfBtnToggled(bool checked);
    void checkExtractionCodeInput();

    void on_pushButton_2_clicked();

private:
    Ui::ShareFileDialog *ui;
    int fileId;
    QString baseUrl;
    QString FinalUrl;
    bool isencoded=false;
    QString generateRandomExtractionCode();
    QString encryptLink();
    QString decryptLink(const QString& encryptedUrl);
};

#endif // SHAREFILEDIALOG_H
