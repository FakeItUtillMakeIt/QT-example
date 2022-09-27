#include "StyleGroup.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QImage>
#include "styleset.h"
#include "groupelement.h"
ConfigNameSpaceStart
StyleGroup::StyleGroup(QWidget *parent) : QWidget(parent)
{
    DefaultUiInit();
    m_namelabel = new QCheckBox();
    m_infomap[Group_Style_ID] = QPair<int,QString>(Group_Style_ID,get_uuid());

    this->setAttribute(Qt::WA_StyledBackground, true);
    deleteicon = QImage("://rc/delete_normal.png");
    deleteiconpressed = QImage("://rc/delete_pressed.png");
   // setStyleSheet("background-color:rgb(0,0,250,50)");    
}
void StyleGroup::DefaultUiInit()
{
    m_infomap[Group_Common_Style] = QPair<bool,QString>(false,":/rc/background.png");
    m_infomap[Group_Title_Style] = QPair<bool,QString>(false,":/rc/grouptitle.png");
    m_infomap[Group_Font_Style] = QPair<bool,QString>(false,this->font().toString());
    m_infomap[Group_Text_Color] = QPair<bool,QString>(false,QColor(0,0,0).name(QColor::QColor::HexArgb));
    m_infomap[Group_Icon_Style] = QPair<bool,QString>(false,":/rc/icon.png");
}
void StyleGroup::record_styleusers(GroupElement *groupelement)
{
    styleusers.push_back(groupelement);
}


void StyleGroup::paintEvent(QPaintEvent *event)
{
    QPainter  painter(this);
    painter.save();
    QPen pen;
    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(0);
    painter.setBrush(QColor(0,250,250));
    if(m_infomap[Group_Common_Style].second.isNull())
    {
        painter.drawRect(rect());
    }else
    {
        painter.drawPixmap(rect(),m_infomap[Group_Common_Style].second);

    }
    painter.restore();
    drawTitle(painter);
    drawDeleteEntry(painter);
    drawTitleText(painter);
    drawIconEntry(painter);
    QWidget::paintEvent(event);
}
void StyleGroup::drawTitleText(QPainter& painter)
{
    QColor brushColor;
    int penwidth = 0;
    brushColor = QColor(250,0,0);
    penwidth = 0;

    painter.save();
    QPen pen;
    pen.setColor(QColor(0,0,0));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(penwidth);
    painter.setBrush(brushColor);
    painter.setPen(pen);
    titletextentry = QRect(width()/5,2,width()*4/5 - width()/10 ,m_titleheight -4);
//    if(m_infomap[Group_Title_Text_Style].second.isNull())
//    {
//        painter.drawRect(titletextentry);
//    }else
//    {
//        painter.drawPixmap(titletextentry,m_infomap[Group_Title_Text_Style].second);

//    }

    painter.restore();
    {
        painter.save();
        QPen pen;
        QColor color(0,0,0);
        if(!m_infomap[Group_Text_Color].second.isNull())
            color.setNamedColor(m_infomap[Group_Text_Color].second);
        pen.setColor(color);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        painter.setBrush(brushColor);
        painter.setPen(pen);
        if(!m_infomap[Group_Font_Style].second.isNull())
        {
            QFont font = painter.font();
            font.fromString(m_infomap[Group_Font_Style].second);
            painter.setFont(font);
        }
        titletextentry.adjust(titletextentry.width()/20,0,0,0);
        painter.drawText(titletextentry,Qt::AlignVCenter,titletext);
        painter.restore();
    }


}

void StyleGroup::drawIconEntry(QPainter &painter)
{
    QRect iconrect = QRect(m_titleheight/4,m_titleheight/4,m_titleheight ,m_titleheight/2);
    painter.drawPixmap(iconrect,m_infomap[Group_Icon_Style].second);
}
void StyleGroup::drawTitle(QPainter& painter)
{
    painter.save();
    QPen pen;
    pen.setColor(QColor(250,0,0));
    pen.setStyle(Qt::DotLine);
    pen.setWidth(2);
    painter.setBrush(QColor(0,250,0));
    painter.setPen(pen);
    QRect  titlerect = QRect(0,0,width(),m_titleheight);
  //  titlerect.adjust(0,0,0,-10);
    if(m_infomap[Group_Title_Style].second.isNull())
        painter.drawRect(titlerect);
    else
        painter.drawPixmap(titlerect,m_infomap[Group_Title_Style].second);
    painter.restore();

}
void StyleGroup::drawDeleteEntry(QPainter& painter)
{
    QColor brushColor;
    if(deletehitted)
        brushColor = QColor(0,250,0);
    else
        brushColor = QColor(250,0,0);

    qDebug() << "delete brushColor:" << brushColor << " deletehitted:" << deletehitted;
    painter.save();
    QPen pen;
    pen.setColor(QColor(250,0,0));
    pen.setStyle(Qt::DotLine);
    pen.setWidth(0);
    painter.setBrush(brushColor);
    painter.setPen(pen);
    deleteentry = QRect(width()-m_titleheight + 2,2,m_titleheight -4,m_titleheight -4);
  //  painter.drawRect(deleteentry);
    painter.drawImage(deleteentry,deleteicon);

    painter.restore();

}
void StyleGroup::mousePressEvent(QMouseEvent *event)
{
    styleset->SetElement(this);
    QWidget::mousePressEvent(event);
}

void StyleGroup::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}


void StyleGroup::updateStyle()
{
    m_namelabel->setText(m_infomap[StyleGroup::Group_Style_Name].second);
    update();
    for(auto user:styleusers)
    {
        user->UpdatePropertyByStyle();
    }
}

void StyleGroup::setTypeName(QString iTypeName)
{
    m_namelabel->setText(iTypeName);
    m_infomap[Group_Style_Name] = QPair<bool,QString>(false,iTypeName);
}

ConfigNameSpaceEnd
