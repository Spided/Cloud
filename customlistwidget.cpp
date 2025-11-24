// customlistwidget.cpp
#include "customlistwidget.h"
#include <QtConcurrent/QtConcurrent>
#include<QPainter>
CustomListWidget::CustomListWidget(QWidget *parent) : QListWidget(parent) {
    setSelectionMode(QAbstractItemView::MultiSelection);

}

void CustomListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPos = event->pos();
        isDragging = false;
        isSelecting = false;

        // 检查鼠标按下位置是否在某个项上
        QListWidgetItem *item = itemAt(startPos);
        if (item) {
            draggedItem = item;
        } else {
            selectionRect.setTopLeft(startPos);
            selectionRect.setBottomRight(startPos);
            isSelecting = true;
        }
    }
    QListWidget::mousePressEvent(event);
}

void CustomListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance()) {
            if (draggedItem) {
                this->setCursor(Qt::OpenHandCursor); // 改变鼠标样式为手

                isDragging = true;
            } else if (isSelecting) {
                selectionRect.setBottomRight(event->pos());
                viewport()->update();
            }
        }
    }

}
// 实现获取所有选中项 ID 的函数
QList<int> CustomListWidget::getSelectedItemIds() const
{
    QList<int> ids;
    QList<QListWidgetItem*> selectedItems = this->selectedItems();
    for (QListWidgetItem* item : selectedItems) {
        ids.append(item->data(Qt::UserRole).toInt());
    }
    return ids;
}
void CustomListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (isDragging) {
            QListWidgetItem *targetItem = itemAt(event->pos());
            if (targetItem) {
                qDebug() << "Target item found at release position";
                int targetType = targetItem->data(Qt::UserRole + 1).toInt();
                if (targetType == 0) { // 目标是文件夹
                    int targetId = targetItem->data(Qt::UserRole).toInt();
                    int sourceId = draggedItem->data(Qt::UserRole).toInt();
                    emit moveFileOrFolder(currentUserId, sourceId, targetId);
                }
            } else {
                qDebug() << "No target item found at release position";
            }
            draggedItem = nullptr;
            this->setCursor(Qt::ArrowCursor); // 恢复鼠标样式为箭头
            isDragging = false;
        } else if (isSelecting) {
            isSelecting = false;
            viewport()->update();

            // 将 selectionRect 转换为视口坐标
            QRect viewportSelectionRect = QRect(
                viewport()->mapFrom(this, selectionRect.topLeft()),
                viewport()->mapFrom(this, selectionRect.bottomRight())
            );

            // 计算框选区域内的项并选中
            clearSelection();
            for (int i = 0; i < count(); ++i) {
                QListWidgetItem *item = this->item(i);
                QRect itemRect = visualItemRect(item);
                if (viewportSelectionRect.intersected(itemRect).isValid()) {
                    qDebug() << "Selecting item:" << item->text();
                    item->setSelected(true);
                }
            }
        }
    }
    QListWidget::mouseReleaseEvent(event);
}

void CustomListWidget::paintEvent(QPaintEvent *event)
{
    QListWidget::paintEvent(event);

    if (isSelecting) {
        QPainter painter(viewport());
        painter.setPen(Qt::blue);
        painter.setBrush(QBrush(QColor(0, 0, 255, 50)));
        painter.drawRect(selectionRect.normalized());
    }
}


void CustomListWidget::setCurrentUserId(int userId)
{
    currentUserId = userId;
}
