#include "stylebutton.h"
#include <QDebug>
#include "configbutton.h"
#include "styleset.h"
ConfigNameSpaceStart
StyleButton::StyleButton(const QString &text, QWidget *parent):QPushButton(text,parent)
{
    m_infomap[Button_Style_ID] =  QPair<int,QString>(Button_Style_ID,get_uuid());
  //  m_namelabel = new QLabel();
    m_namelabel = new QCheckBox();;

    DefaultUiInit();
    updateStyle();
}
StyleButton::~StyleButton()
{

}
void StyleButton::DefaultUiInit()
{
    m_infomap[Button_Common_Style] = QPair<bool,QString>(false,":/rc/normal.png");
    m_infomap[Button_Hoverd_Style] = QPair<bool,QString>(false,":/rc/hover.png");
    m_infomap[Button_Pressed_Style] = QPair<bool,QString>(false,":/rc/pressed.png");
    m_infomap[Button_Success_Style] = QPair<bool,QString>(false,":/rc/sucess.png");
    m_infomap[Button_Failed_Style] = QPair<bool,QString>(false,":/rc/error.png");
    m_infomap[Button_Font_Style] = QPair<bool,QString>(false,this->font().toString());
    m_infomap[Button_Text_Color] = QPair<bool,QString>(false,QColor(0,0,0).name(QColor::QColor::HexArgb));
    m_infomap[Button_Diable_Style] = QPair<bool,QString>(false,":/rc/forbit.png");
    m_defaultinfomap = m_infomap;

}
void StyleButton::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "StyleButton::mousePressEvent:";
    styleset->SetElement(this);
    QPushButton::mousePressEvent(ev);
}
void StyleButton::record_styleusers(ConfigButton* configbtn)
{
    styleusers.push_back(configbtn);
}

void StyleButton::updateStyle()
{
    m_namelabel->setText(m_infomap[StyleButton::Button_Style_Name].second);

    QString commstyle = QString("QPushButton{ border-image: url(%1);color:%2}").arg(m_infomap[Button_Common_Style].second).arg(m_infomap[Button_Text_Color].second);
    QString hoverstyle = QString("QPushButton:hover{ border-image: url(%1);}").arg( m_infomap[Button_Hoverd_Style].second);
    QString pressedstyle = QString("QPushButton:pressed{ border-image: url(%1);}").arg(m_infomap[Button_Pressed_Style].second);
    QString disablestye =  QString("QPushButton:disabled{ border-image: url(%1);}").arg(m_infomap[Button_Diable_Style].second);
    NormalStyle = commstyle+hoverstyle+ pressedstyle + disablestye;
    setStyleSheet(NormalStyle);
    if(!m_infomap[Button_Font_Style].second.isNull())
    {
        QFont  font;
        font.fromString(m_infomap[Button_Font_Style].second);
        setFont(font);
    }
    for(auto user:styleusers)
    {
        user->UpdatePropertyByStyle();
    }
    successstyle = m_infomap[Button_Success_Style].second;
    failedstyle = m_infomap[Button_Failed_Style].second;
}
void StyleButton::setTypeName(QString iTypeName)
{
    m_namelabel->setText(iTypeName);
    m_infomap[Button_Style_Name] = QPair<bool,QString>(false,iTypeName);
}
ConfigNameSpaceEnd
