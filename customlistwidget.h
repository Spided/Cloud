#ifndef CUSTOMLISTWIDGET_H
#define CUSTOMLISTWIDGET_H
#include"globaldata.h"
#include <QListWidget>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QPixmap>
#include <QApplication>
#include <QMutex>
#include <QRect>
class CustomListWidget : public QListWidget
{
    Q_OBJECT
public:
    CustomListWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
signals:
    void moveFileOrFolder(int ownerId, int sourceId, int destinationId);

private:
    QPoint startPos;
    //QDrag *drag = nullptr;
    QListWidgetItem *draggedItem = nullptr;
    bool isDragging=false;
    int currentUserId;
    QPixmap dragPixmap;
    QPoint dragOffset;
     bool isSelecting = false;
    QRect selectionRect;



public slots:
    void setCurrentUserId(int userId);
    // 添加获取所有选中项 ID 的函数
    QList<int> getSelectedItemIds() const;

};


#endif // CUSTOMLISTWIDGET_H
