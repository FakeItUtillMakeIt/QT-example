#include "customgraphicimageview.h"

CustomGraphicImageView::CustomGraphicImageView(QPixmap* pixmap) :
    QGraphicsItem()
{
    m_pix = *pixmap;
    setAcceptDrops(true);
    m_scaleValue=0;
    m_scaleDeafault=0;
    m_isMove-false;
}

void CustomGraphicImageView::setPixMap(QPixmap *pixmap){
    m_pix=*pixmap;
}

QRectF CustomGraphicImageView::boundingRect() const{
    return QRectF(-m_pix.width()/2,-m_pix.height()/2,m_pix.width(),m_pix.height());
}

void CustomGraphicImageView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    
    painter->drawPixmap(-m_pix.width()/2,-m_pix.height()/2,m_pix);
    //painter->drawEllipse(QRect(-m_pix.width() / 2, -m_pix.height() / 2,m_pix.width()+20,m_pix.height()+20));
}

void CustomGraphicImageView::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (event->button()==Qt::LeftButton){
        m_startPos=event->pos();
        m_isMove=true;
    }
    else if(event->button()==Qt::RightButton){
        ResetItemPos();
    }
}

void CustomGraphicImageView::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(m_isMove){
        QPointF point=(event->pos()-m_startPos)*m_scaleValue;
       //QGraphicsItem函数moveBy 通过给定x,y进行移动
        moveBy(point.x(),point.y());
    }
}

void CustomGraphicImageView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    m_isMove=false;
}

void CustomGraphicImageView::wheelEvent(QGraphicsSceneWheelEvent *event){
    if(event->delta()>0 && m_scaleValue>=50){
        return;
    }
    else if(event->delta()<0 && m_scaleValue<=m_scaleDeafault){
        ResetItemPos();
    }
    else{
        qreal qrealOriginScale=m_scaleValue;
        if(event->delta()>0){//滚轮向前滑动
            m_scaleValue*=1.1;//缩放比例
        }
        else{
            m_scaleValue*=0.9;
        }
        setScale(m_scaleValue);
        if(event->delta()>0){
            moveBy(-event->pos().x()*qrealOriginScale*0.1,-event->pos().y()*qrealOriginScale*0.1);
        }
        else{
            moveBy(event->pos().x()*qrealOriginScale*0.1,event->pos().y()*qrealOriginScale*0.1);
        }
    }
}

void CustomGraphicImageView::setQGraphicViewWH(int width, int height){
    int imgWidth=m_pix.width();
    int imgHeight=m_pix.height();
    qreal tmp1=width*1.0/imgWidth;
    qreal tmp2=height*1.0/imgHeight;
    if(tmp1>tmp2){
        m_scaleDeafault=tmp2;
    }
    else{
        m_scaleDeafault=tmp1;
    }
    setScale(m_scaleDeafault);
    m_scaleValue=m_scaleDeafault;
}


void CustomGraphicImageView::ResetItemPos(){
    m_scaleValue=m_scaleDeafault;
    setScale(m_scaleDeafault);
    setPos(0,0);
}

qreal CustomGraphicImageView::getScaleValue() const{
    return  m_scaleValue;
}
