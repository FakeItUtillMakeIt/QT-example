#ifndef STYLEBUTTON_H
#define STYLEBUTTON_H
#include "configdefine.h"

#include <QLabel>
#include <QMap>
#include <QPushButton>
ConfigNameSpaceStart

class StyleSet;
class ConfigButton;
class StyleButton:public QPushButton
{
    Q_OBJECT
public:
    enum  ButtonInfo
    {
       Button_Style_ID = 0,
       Button_Style_Name,
       Button_Common_Style, //常态背景图片
       Button_Pressed_Style,//鼠标按下背景图片
       Button_Hoverd_Style, //鼠标经过背景图片
       Button_Success_Style,
       Button_Failed_Style,
       Button_Diable_Style,
       Button_Font_Style,//文字字体
       Button_Text_Color//文字颜色
    };
    QMap<int,QPair<bool,QString>>  m_infomap;
    QList<ConfigButton*>  styleusers;

    StyleButton(const QString &text, QWidget *parent = Q_NULLPTR);
    ~StyleButton();
protected:
    void mousePressEvent(QMouseEvent *ev);
public:

    StyleSet*  styleset;
    void updateStyle();
    QLabel* m_namelabel;

    void setTypeName(QString iTypeName);
    void record_styleusers(ConfigButton *configbtn);
    QString NormalStyle;
    QString successstyle;
    QString failedstyle;
private:
    void DefaultUiInit();

};
ConfigNameSpaceEnd
#endif // STYLEBUTTON_H
