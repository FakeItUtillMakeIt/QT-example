#ifndef CUSTOMGRAPHICIMAGEVIEW_H
#define CUSTOMGRAPHICIMAGEVIEW_H

#include <QGraphicsItem>
#include <QWidget>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPointF>
#include <QDragEnterEvent>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <math.h>

enum Enum_ZoomState{
    NO_STATE,
    RESET,
    ZOOM_IN,
    ZOOM_OUT
};



class CustomGraphicImageView : public QGraphicsItem
{
public:
    CustomGraphicImageView(QPixmap* pixmap);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void ResetItemPos();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void setQGraphicViewWH(int width,int height);
    qreal getScaleValue() const;
    void setPixMap(QPixmap* pixmap);

private:

    qreal m_scaleValue;
    qreal m_scaleDeafault;
    QPixmap m_pix;
    int m_zoomState;
    bool m_isMove;
    QPointF m_startPos;

};

#endif // CUSTOMGRAPHICIMAGEVIEW_H
