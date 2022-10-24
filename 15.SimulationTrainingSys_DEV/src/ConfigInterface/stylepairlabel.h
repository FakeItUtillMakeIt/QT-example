#ifndef STYLEPAIRLABEL_H
#define STYLEPAIRLABEL_H
#include "configdefine.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QCheckBox>
ConfigNameSpaceStart
class StyleSet;
class ConfigPairLabel;
class StylePairLabel:public QWidget
{
    Q_OBJECT
public:
    enum  PairLabelInfo
    {
       PairLabel_Style_ID = 0,
       PairLabel_Style_Name,
       PairLabel_Name_Style, //常态背景图片
       PairLabel_Value_Style,//
       PairLabel_Error_Name_Style, //常态背景图片
       PairLabel_Error_Value_Style,//
       PairLabel_Font_Style,//文字字体
       PairLabel_Text_Color,//文字颜色
       PairLabel_Name_Strech,
       PairLabel_Value_Strech,
       PairLabel_Space
    };
    QMap<int,QPair<bool,QString>>  m_infomap;
    QMap<int, QPair<bool, QString>>  m_defaultinfomap;

    QCheckBox* m_namelabel;
    StyleSet*  styleset;
    QLabel* paramnamelabel;
    QLabel* paramvaluelabel;
    QHBoxLayout* paramlayout;
    void setTypeName(QString iTypeName);
    StylePairLabel(QWidget* parent =nullptr);
    void updateStyle();
    QList<ConfigPairLabel*>  styleusers;
    void record_styleusers(ConfigPairLabel *pairlabel);
private:
    int m_space = 5;
    int m_nameLabelStrech = 2;
    int m_valueLabelStrech = 3;
    int m_labelheight = 50;
    QString m_namelabelstyle;
    QString m_valuelabelstyle;
    QString m_error_namelabelstyle;
    QString m_error_valuelabelstyle;
    QFont  m_textfont;
    QColor  m_textcolor;


    void mousePressEvent(QMouseEvent *ev);
    void DefaultUiInit();
};
ConfigNameSpaceEnd
#endif // STYLEPAIRLABEL_H
