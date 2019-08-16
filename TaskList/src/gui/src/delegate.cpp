#include <QtGui>
#include "delegate.h"
#include <QDialog>
#include <QApplication>
MyDelegate::MyDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}


void MyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter, option, index);

    QStyleOptionButton delButtton;
    QStyleOptionButton editButtton;
    QRect r = option.rect;//getting the rect of the cell
    int x,y,w,h;
    x = r.left() + r.width() - 20;//the X coordinate
    y = r.top();//the Y coordinate
    w = 20;//button width
    h = 20;//button height
    delButtton.rect = QRect(x,y,w,h);
    delButtton.text = "X";
    delButtton.state = QStyle::State_Enabled;

    editButtton.rect = QRect(x-20,y,w,h);
    editButtton.text = "E";
    editButtton.state = QStyle::State_Enabled;

    QApplication::style()->drawControl( QStyle::CE_PushButton, &editButtton, painter);
    QApplication::style()->drawControl( QStyle::CE_PushButton, &delButtton, painter);
}

bool MyDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if( event->type() == QEvent::MouseButtonRelease )
    {
        QMouseEvent * e = (QMouseEvent *)event;
        int clickX = e->x();
        int clickY = e->y();

        QRect r = option.rect;//getting the rect of the cell
        int x,y,w,h;
        x = r.left() + r.width() - 15;//the X coordinate
        y = r.top();//the Y coordinate
        w = 15;//button width
        h = 15;//button height

        if( clickX > x && clickX < x + w )
            if( clickY > y && clickY < y + h )
                clicked(index.row());

    }

    return true;
}
