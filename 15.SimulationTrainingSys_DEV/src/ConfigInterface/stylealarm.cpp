#include "stylealarm.h"
#include <QApplication>
#include <QPainter>
#include <QScreen>
#include "styleset.h"
#include "configalarm.h"
ConfigNameSpaceStart
void initFont(QFont &font, QString family, bool italic, bool bold, int pointsize)
{
    font.setFamily(family);
    font.setItalic(italic);
    font.setBold(bold);
    font.setPointSize(pointsize);
}
void initFont(QFont &font, QString family, bool italic, bool bold, double pointsizef)
{
    font.setFamily(family);
    font.setItalic(italic);
    font.setBold(bold);
    font.setPointSizeF(pointsizef);
}

StyleAlarm::StyleAlarm(QWidget *parent):QWidget(parent)
{
      m_infomap[Alarm_Style_ID] = QPair<int, QString>(Alarm_Style_ID, get_uuid());
      setAutoFillBackground(true);
 //    setWindowFlags(Qt::FramelessWindowHint);//无窗体
  //   setAttribute(Qt::WA_TranslucentBackground);//背景透明

//    double dpiX = QApplication::primaryScreen()->physicalDotsPerInchX();
//    double dpiY = QApplication::primaryScreen()->physicalDotsPerInchY();
//    resize(182,182);
//    initFont(m_text_font,"微软雅黑",false,false,12);

     //   m_infomap[Alarm_Style_ID] =  QPair<int,QString>(Alarm_Style_ID,get_uuid());
        m_namelabel = new QCheckBox();
        DefaultUiInit();
        updateStyle();
}
//void StyleAlarm::resizeEvent(QResizeEvent* event)
//{
//    m_width = width();
//    m_height =
//    QWidget::resizeEvent(event);
//}
void StyleAlarm::setTypeName(QString iTypeName)
{
    m_namelabel->setText(iTypeName);
    m_infomap[Alarm_Style_Name] = QPair<bool, QString>(false, iTypeName);
}
void StyleAlarm::record_styleusers(ConfigAlarm* configalarm)
{
    styleusers.push_back(configalarm);
}
void StyleAlarm::DefaultUiInit()
{
    m_infomap[Alarm_Diaplay_Type] = QPair<bool,QString>(false,"否");
    m_infomap[Alarm_Init_Img] = QPair<bool,QString>(false,":/rc/alarmInit.png");
    m_infomap[Alarm_Normal_Img] = QPair<bool,QString>(false,":/rc/alarmNormal.png");
    m_infomap[Alarm_Err_Img] = QPair<bool,QString>(false,":/rc/alarmError.png");
    m_infomap[Alarm_Init_Color] = QPair<bool,QString>(false,QColor(220,220,220).name(QColor::QColor::HexArgb));
    m_infomap[Alarm_Normal_Color] = QPair<bool,QString>(false,QColor(0,250,0).name(QColor::QColor::HexArgb));
    m_infomap[Alarm_Err_Color] = QPair<bool,QString>(false,QColor(250,0,0).name(QColor::QColor::HexArgb));
    QFont  font = this->font();
    initFont(font,"微软雅黑",false,false,13);
    m_infomap[ALarm_Font_Style] = QPair<bool,QString>(false,font.toString());
    m_infomap[Alarm_Text_Color] = QPair<bool,QString>(false,QColor(0,0,0).name(QColor::QColor::HexArgb));
    m_infomap[Alarm_Text_Position] = QPair<bool, QString>(false, "否");

}

void StyleAlarm::updateStyle()
{
    m_namelabel->setText(m_infomap[StyleAlarm::Alarm_Style_Name].second);
    m_color_init.setNamedColor(m_infomap[StyleAlarm::Alarm_Init_Color].second);
    m_color_normal.setNamedColor(m_infomap[StyleAlarm::Alarm_Normal_Color].second);
    m_color_error.setNamedColor(m_infomap[StyleAlarm::Alarm_Err_Color].second);
    m_text_color.setNamedColor(m_infomap[StyleAlarm::Alarm_Text_Color].second);
    m_img_init = m_infomap[StyleAlarm::Alarm_Init_Img].second;
    m_img_normal = m_infomap[StyleAlarm::Alarm_Normal_Img].second;
    m_img_error = m_infomap[StyleAlarm::Alarm_Err_Img].second;
    m_display_img = (m_infomap[StyleAlarm::Alarm_Diaplay_Type].second=="否"?false:true);
    m_display_vertically = (m_infomap[StyleAlarm::Alarm_Text_Position].second == "否" ? false : true);

    if(!m_infomap[ALarm_Font_Style].second.isNull())
    {
        m_text_font.fromString(m_infomap[ALarm_Font_Style].second);
    }
    for(auto user:styleusers)
    {
        user->UpdatePropertyByStyle();
    }
    update();

}
void StyleAlarm::drawLight(QPainter* painter, QRectF rect)
{
    if(!m_display_img)
        drawEcllipse(painter,rect);
    else
        drawImage(painter,rect);
}
void StyleAlarm::mousePressEvent(QMouseEvent* ev)
{
    qDebug() << "StyleButton::mousePressEvent:";
    styleset->SetElement(this);
    QWidget::mousePressEvent(ev);
}
void StyleAlarm::drawImage(QPainter* painter, QRectF  imgrect )
{
  //  QRectF  imgrect = QRectF(-m_width/2,-m_width/2,m_width,m_width);
    QString imgpath;
    if(m_state == "初始")
        imgpath = m_img_init;
    else if(m_state == "正常")
        imgpath = m_img_normal;
    else if(m_state == "异常")
        imgpath = m_img_error;
    painter->drawImage(imgrect, QImage(imgpath));

}
void StyleAlarm::drawEcllipse(QPainter* painter,QRectF ellipseRect)
{
    QColor eclipsecolor;
    if(m_state == "初始")
        eclipsecolor = m_color_init;
    else if(m_state == "正常")
        eclipsecolor = m_color_normal;
    else if(m_state == "异常")
        eclipsecolor = m_color_error;
    painter->save();
    painter->setBrush(eclipsecolor);
    //QRectF  ellipseRect = QRectF(-m_width/2,-m_width/2,m_width,m_width);
    painter->setPen(QColor(250,250,250,0));
    painter->drawEllipse(ellipseRect);
    painter->restore();
}
void StyleAlarm::paintEvent(QPaintEvent *event)
{
     QPainter painter(this);
     painter.save();     
     QRectF alarmrect;
     QRectF textrect;
   
     painter.setRenderHint(QPainter::Antialiasing);  /* 使用反锯齿(如果可用) */
     m_width = width();
     m_height = height();
     if (m_display_vertically)
     {  
         painter.translate(m_width >> 1, (m_height*3)>>3);   /* 坐标变换为窗体中心 */
         int alarmlength = (m_height * 3)>>2;
         int textlength = m_height>>2;
         alarmlength = (alarmlength > m_width ? m_width : alarmlength);
         alarmrect = QRectF(-alarmlength / 2, -alarmlength/2, alarmlength, alarmlength);
         textrect = QRectF(-m_width/2, alarmlength/2, m_width, textlength);
         drawLight(&painter, alarmrect);
         painter.setPen(m_text_color);
         painter.setFont(m_text_font);
         painter.drawText(textrect, Qt::AlignCenter, m_text);
     }
     else
     {      
         int alarmlength = m_width >> 2;
         alarmlength = (alarmlength > m_height ? m_height : alarmlength);
         int textlength =  alarmlength;
         painter.translate(m_width*3 >> 2, m_height >>1);   /* 坐标变换为窗体中心 */
         alarmrect = QRectF(-alarmlength/2,-alarmlength/2, alarmlength, alarmlength);
         drawLight(&painter, alarmrect);
         painter.setPen(m_text_color);
         painter.setFont(m_text_font);
         painter.translate(-m_width * 3 >> 2, -m_height >> 1);   /* 坐标变换为窗体中心 */
         textrect = QRectF(0, 0, m_width >> 1, m_height);
         painter.drawText(textrect, Qt::AlignHCenter | Qt::AlignVCenter, m_text);
     }
    
     painter.restore();
     QWidget::paintEvent(event);
}
ConfigNameSpaceEnd