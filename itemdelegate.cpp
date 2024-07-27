#include <QPainter>
#include <QDebug>
#include "itemdelegate.h"
#include "listItemdata.h"

MuItemDelegate::MuItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void MuItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        painter->save();
        QVariant var = index.data(Qt::UserRole+1);
        MuItemData itemData = var.value<MuItemData>();

        // item 矩形区域
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);
//        qDebug()<<"option.rect.x()="<<option.rect.x()<<"\n";
//        qDebug()<<"option.rect.y()="<<option.rect.y()<<"\n";
//        qDebug()<<"option.rect.width()="<<option.rect.width()<<"\n";
//        qDebug()<<"option.rect.height()="<<option.rect.height()<<"\n";
        QPainterPath path;
        path.moveTo(rect.topRight());
        path.lineTo(rect.topLeft());
        path.quadTo(rect.topLeft(), rect.topLeft());
        path.lineTo(rect.bottomLeft());
        path.quadTo(rect.bottomLeft(), rect.bottomLeft());
        path.lineTo(rect.bottomRight());
        path.quadTo(rect.bottomRight(), rect.bottomRight());
        path.lineTo(rect.topRight());
        path.quadTo(rect.topRight(), rect.topRight());

        // 鼠标悬停或者选中时改变背景色
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setPen(QPen(QColor("#ebeced")));
            painter->setBrush(QColor("#ebeced"));
            painter->drawPath(path);
        }
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setPen(QPen(QColor("#e3e3e5")));
            painter->setBrush(QColor("#e3e3e5"));
            painter->drawPath(path);
        }

        // 绘制图片，图片信息，报警时间
        QRectF iconRect = QRect(rect.left()+2, rect.top()+10, 150, 100);
        QRectF singerRect = QRect(iconRect.right()+20, iconRect.top() + 30, rect.width()-10-iconRect.width(), 20);
        QRectF songNbRect = QRect(singerRect.left(), singerRect.bottom()+20, rect.width()-10-iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(itemData.iconPath));
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Microsoft Yahei", 10));
        painter->drawText(singerRect, itemData.singer);

        painter->setPen(QPen(Qt::gray));
        painter->drawText(songNbRect, itemData.songsNb);
        painter->restore();
    }
}

QSize MuItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 120);
}
