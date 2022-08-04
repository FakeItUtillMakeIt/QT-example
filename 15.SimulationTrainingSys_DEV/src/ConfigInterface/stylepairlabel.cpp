#include "stylepairlabel.h"
#include "configdefine.h"
#include <QHBoxLayout>
#include <QLabel>
#include "styleset.h"

#include <QDebug>
#include "configpairlabel.h"
ConfigNameSpaceStart
StylePairLabel::StylePairLabel(QWidget *parent):QWidget(parent)
{
    m_infomap[PairLabel_Style_ID] = QPair<int,QString>(PairLabel_Style_ID,get_uuid());
    m_namelabel = new QLabel("");
   // setStyleSheet("background-color:transparent");
    paramlayout = new QHBoxLayout;
    paramnamelabel = new QLabel("参数名称");
    paramnamelabel->setAlignment(Qt::AlignCenter);

    paramnamelabel->setStyleSheet("background-color:rgb(150,150,150)");
    paramvaluelabel = new QLabel("参数值");
    paramvaluelabel->setStyleSheet("background-color:rgb(200,200,200)");
    paramvaluelabel->setAlignment(Qt::AlignCenter);

    paramlayout->addWidget(paramnamelabel);
    paramlayout->addWidget(paramvaluelabel);
    paramlayout->setSpacing(m_space);
    paramlayout->setStretch(0,m_nameLabelStrech);
    paramlayout->setStretch(1,m_valueLabelStrech);
    setLayout(paramlayout);
    DefaultUiInit();
    updateStyle();
}

void StylePairLabel::DefaultUiInit()
{
   // m_infomap[StylePairLabel::PairLabel_Style_Name] =  QPair<bool,QString>(false,":/rc/normal.png");
    m_infomap[StylePairLabel::PairLabel_Name_Strech] = QPair<bool,QString>(false,"2");
    m_infomap[StylePairLabel::PairLabel_Value_Strech] = QPair<bool,QString>(false,"3");
    m_infomap[StylePairLabel::PairLabel_Name_Style] = QPair<bool,QString>(false,":/rc/nValueNormal.png");
    m_infomap[StylePairLabel::PairLabel_Value_Style] = QPair<bool,QString>(false,":/rc/vValuenormal.png");

    m_infomap[StylePairLabel::PairLabel_Error_Name_Style] = QPair<bool,QString>(false,":/rc/nValueError.png");
    m_infomap[StylePairLabel::PairLabel_Error_Value_Style] = QPair<bool,QString>(false,":/rc/vValueError.png");

    m_infomap[StylePairLabel::PairLabel_Font_Style] = QPair<bool,QString>(false,this->font().toString());
    m_infomap[StylePairLabel::PairLabel_Text_Color] = QPair<bool,QString>(false,QColor(0,0,0).name(QColor::QColor::HexArgb));


    m_infomap[StylePairLabel::PairLabel_Space] = QPair<bool,QString>(false,"5");

}

void StylePairLabel::record_styleusers(ConfigPairLabel* pairlabel)
{
    styleusers.push_back(pairlabel);
}
void StylePairLabel::updateStyle()
{
    qDebug() << "PairLabel_Name_Style:"<<m_infomap[StylePairLabel::PairLabel_Name_Style].second;

    if(m_infomap.contains(StylePairLabel::PairLabel_Style_Name))
      m_namelabel->setText(m_infomap[StylePairLabel::PairLabel_Style_Name].second);
    if(m_infomap.contains(StylePairLabel::PairLabel_Name_Strech))
        m_nameLabelStrech = m_infomap[StylePairLabel::PairLabel_Name_Strech].second.toInt();
    if(m_infomap.contains(StylePairLabel::PairLabel_Value_Strech))
        m_valueLabelStrech = m_infomap[StylePairLabel::PairLabel_Value_Strech].second.toInt();
    if(m_infomap.contains(StylePairLabel::PairLabel_Name_Style))
        m_namelabelstyle = m_infomap[StylePairLabel::PairLabel_Name_Style].second;
    if(m_infomap.contains(StylePairLabel::PairLabel_Value_Style))
        m_valuelabelstyle =m_infomap[StylePairLabel::PairLabel_Value_Style].second;

    if(m_infomap.contains(StylePairLabel::PairLabel_Error_Name_Style))
        m_error_namelabelstyle = m_infomap[StylePairLabel::PairLabel_Error_Name_Style].second;
    if(m_infomap.contains(StylePairLabel::PairLabel_Error_Value_Style))
        m_error_valuelabelstyle =m_infomap[StylePairLabel::PairLabel_Error_Value_Style].second;

    if(m_infomap.contains(StylePairLabel::PairLabel_Font_Style))
    {
        m_textfont.fromString(m_infomap[StylePairLabel::PairLabel_Font_Style].second);
    }
    if(m_infomap.contains(StylePairLabel::PairLabel_Text_Color))
    {
        m_textcolor.setNamedColor(m_infomap[StylePairLabel::PairLabel_Text_Color].second);
    }
    if(m_infomap.contains(StylePairLabel::PairLabel_Space))
    {
       m_space =  m_infomap[StylePairLabel::PairLabel_Space].second.toInt();
    }
    QString stylecolor =  QString("color:rgb(%1,%2,%3);").arg(m_textcolor.red()).arg(m_textcolor.green()).arg(m_textcolor.blue());
    paramnamelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_namelabelstyle).arg(stylecolor));
    paramvaluelabel->setStyleSheet(QString("border-image: url(%1);%2").arg(m_valuelabelstyle).arg(stylecolor));
    qDebug() << "m_nameLabelStrech:"<<m_nameLabelStrech;
    qDebug() << "m_valueLabelStrech:"<<m_valueLabelStrech;

    paramlayout->setStretch(0,m_nameLabelStrech);
    paramlayout->setStretch(1,m_valueLabelStrech);
    paramlayout->setSpacing(m_space);
    paramnamelabel->setFont(m_textfont);
    paramvaluelabel->setFont(m_textfont);

    for(auto user:styleusers)
    {
        user->UpdatePropertyByStyle();
    }
}

void StylePairLabel::mousePressEvent(QMouseEvent *ev)
{
    styleset->SetElement(this);
    QWidget::mousePressEvent(ev);
}

void StylePairLabel::setTypeName(QString iTypeName)
{
    m_namelabel->setText(iTypeName);
    m_infomap[PairLabel_Style_Name] = QPair<bool,QString>(false,iTypeName);
}

ConfigNameSpaceEnd
