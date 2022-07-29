#include "stylecurve.h"
#include "configcurve.h"
#include "styleset.h"
#include <QDebug>

ConfigNameSpaceStart

StyleCurve::StyleCurve(QWidget *parent):QCustomPlot(parent)
{
    m_namelabel = new QLabel();
    m_infomap[Curve_Style_ID] = QPair<int,QString>(Curve_Style_ID,get_uuid());

    AddTitle("实时曲线");
}
void StyleCurve::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "StyleButton::mousePressEvent";
    styleset->SetElement(this);
    QCustomPlot::mousePressEvent(ev);
}
void StyleCurve::record_styleusers(ConfigCurve* configcurve)
{
    styleusers.push_back(configcurve);
}

void StyleCurve::updateStyle()
{
    m_namelabel->setText(m_infomap[StyleCurve::Curve_Style_Name].second);
    QString fontstr = m_infomap[StyleCurve::Curve_Title_Font].second;
    if(!fontstr.isNull())
    {
        qDebug() << "curve set font";
        QFont font;
        font.fromString(fontstr);
        m_pCurvTitle->setFont(font);
    }
    QString colorstr = m_infomap[StyleCurve::Curve_Title_Color].second;
    if(!colorstr.isNull())
    {
        QColor color;
        color.setNamedColor(colorstr);
        m_pCurvTitle->setTextColor(color);

    }
    colorstr = m_infomap[StyleCurve::Curve_Common_Color].second;
    if(!colorstr.isNull())
    {
        QColor color;
        color.setNamedColor(colorstr);
        setBackground(color);
    }
    QString titleshowstr = m_infomap[StyleCurve::Curve_Title_Show].second;
    if(!titleshowstr.isNull())
    {
        if(titleshowstr == "是")
        {
            m_pCurvTitle->setVisible(true);
        }
        else if(titleshowstr == "否")
        {
            m_pCurvTitle->setVisible(false);
        }
    }

    replot();


}

QList<ConfigCurve *> &StyleCurve::getStyleUsers()
{
    return styleusers;
}

void StyleCurve::AddTitle(QString title)
{
    plotLayout()->insertRow(0);     // pCurveWidget、curveWidget为在ui中定义的QCustomPlot对象指针
    QString sTitle = title;
    m_pCurvTitle = new QCPTextElement(this, sTitle);
    plotLayout()->addElement(0, 0, m_pCurvTitle);    //给坐标设置标题
}
void StyleCurve::setTypeName(QString iTypeName)
{
    m_namelabel->setText(iTypeName);
    m_infomap[Curve_Style_Name] = QPair<bool,QString>(false,iTypeName);
}

ConfigNameSpaceEnd
