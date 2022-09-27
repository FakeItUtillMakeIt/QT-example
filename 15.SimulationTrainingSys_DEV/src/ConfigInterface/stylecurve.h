#ifndef STYLECURVE_H
#define STYLECURVE_H
#include "configdefine.h"
#include "qcustomplot.h"
#include <QCheckBox>

ConfigNameSpaceStart
class StyleSet;
class ConfigCurve;


class StyleCurve:public QCustomPlot
{
    Q_OBJECT
    void AddTitle(QString title);
public:
    enum  CurveInfo
    {
       Curve_Style_ID = 0,
       Curve_Style_Name,
       Curve_Common_Color, //背景颜色
       Curve_Title_Show,//是否显示标题
       Curve_Title_Font, //标题文字字体
       Curve_Title_Color//标题文字颜色
    };
    QMap<int,QPair<bool,QString>>  m_infomap;
    void updateStyle();
    StyleSet*  styleset;
    QCheckBox* m_namelabel;
    QList<ConfigCurve*>  styleusers;
    QList<ConfigCurve*>&  getStyleUsers();
    StyleCurve(QWidget *parent = nullptr);
    void setTypeName(QString iTypeName);
    void record_styleusers(ConfigCurve *configcurve);
protected:
    void mousePressEvent(QMouseEvent *ev);
private:
    QCPTextElement* m_pCurvTitle;
};
ConfigNameSpaceEnd
#endif // STYLECURVE_H
