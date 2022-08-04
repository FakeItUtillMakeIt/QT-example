#include "stylelabel.h"
#include "styleset.h"
#include <QDebug>
ConfigNameSpaceStart
StyleLabel::StyleLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):QLabel(text,parent,f)
{
    m_namelabel = new QLabel("");
    m_infomap[Label_Style_ID] = QPair<int,QString>(Label_Style_ID,get_uuid());
    setStyleSheet("background-color:#CCCCCCCC");
    setAlignment(Qt::AlignCenter);
}

void StyleLabel::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "StyleButton::mousePressEvent";

    styleset->SetElement(this);
    QLabel::mousePressEvent(ev);
}

void StyleLabel::updateStyle()
{
    if(m_infomap.contains(StyleLabel::Label_Style_Name))
      m_namelabel->setText(m_infomap[StyleLabel::Label_Style_Name].second);

    QString borderimg = "",colorsty ="";
    if( m_infomap.contains(Label_Common_Style)&& (!m_infomap[Label_Common_Style].second.isNull()))
       borderimg = QString("border-image: url(%1);").arg(m_infomap[Label_Common_Style].second);
    if( m_infomap.contains(Label_Text_Color)&& (!m_infomap[Label_Text_Color].second.isNull()))
         colorsty = QString("color:%1;").arg(m_infomap[Label_Text_Color].second);

    QString commstyle = QString("QLabel{%1%2background-color:#CCCCCCCC}").arg(borderimg).arg(colorsty);
    setStyleSheet(commstyle);
    if( m_infomap.contains(Label_Font_Style)&& (!m_infomap[Label_Font_Style].second.isNull()))
    {
        QFont  font;
        font.fromString(m_infomap[Label_Font_Style].second);
        setFont(font);
    }
}

void StyleLabel::setTypeName(QString iTypeName)
{
    m_namelabel->setText(iTypeName);
    m_infomap[Label_Style_Name] = QPair<bool,QString>(false,iTypeName);
}

ConfigNameSpaceEnd
