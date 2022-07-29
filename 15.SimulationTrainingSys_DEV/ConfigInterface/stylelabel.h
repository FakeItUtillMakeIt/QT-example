#ifndef STYLELABEL_H
#define STYLELABEL_H
#include "configdefine.h"
#include <QLabel>
#include <QMap>
ConfigNameSpaceStart

class StyleSet;
class StyleLabel:public QLabel
{
public:
    enum  LabelInfo
    {
       Label_Style_ID = 0,
       Label_Style_Name,
       Label_Common_Style, //常态背景图片
       Label_Font_Style,//文字字体
       Label_Text_Color//文字颜色
    };
    QMap<int,QPair<bool,QString>>  m_infomap;
    void setTypeName(QString iTypeName);
    StyleLabel(const QString &text, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags()); 
public:
    void mousePressEvent(QMouseEvent *ev);
    StyleSet*  styleset;
    void updateStyle();
    QLabel* m_namelabel;

};

ConfigNameSpaceEnd

#endif // STYLELABEL_H
