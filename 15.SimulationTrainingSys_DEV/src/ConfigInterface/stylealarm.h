#ifndef STYLEALARM_H
#define STYLEALARM_H

#include <QLabel>
#include <QPaintEvent>
#include <QWidget>
#include "configdefine.h"
#include <QCheckBox>
ConfigNameSpaceStart
class  StyleSet;
class ConfigAlarm;
class StyleAlarm:public QWidget
{
public:
    enum  AlarmInfo
     {
        Alarm_Style_ID = 0,
        Alarm_Style_Name,
        Alarm_Diaplay_Type, //展示方式
        Alarm_Text_Position,//是否竖向排布
        Alarm_Init_Img, //初始背景图片
        Alarm_Normal_Img,//正常背景图片
        Alarm_Err_Img, //错误背景图片
        Alarm_Init_Color, //初始背景颜色
        Alarm_Normal_Color,//正常背景颜色
        Alarm_Err_Color, //错误背景颜色
        ALarm_Font_Style,//文字字体
        Alarm_Text_Color//文字颜色
     };
     QMap<int,QPair<bool,QString>>  m_infomap;
     QMap<int, QPair<bool, QString>>  m_deaultinfomap;

   //  QList<ConfigButton*>  styleusers;
     QCheckBox* m_namelabel;
     StyleSet* styleset;
     void setTypeName(QString iTypeName);
     void updateStyle();

    StyleAlarm(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent *event);

    QList<ConfigAlarm*>  styleusers;
    void record_styleusers(ConfigAlarm* configalarm);

protected:
    void drawLight(QPainter *painter, QRectF rect);
    void drawEcllipse(QPainter *painter, QRectF ellipseRect);
    void drawImage(QPainter *painter, QRectF rect);
   // void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* ev);

private:
    int m_width = 90;
    int m_height = 125;
    QString m_img_error;
    QString m_img_init;
    QString m_img_normal;
    bool m_display_img = false; // 0 颜色  1 图片
    bool m_display_vertically = false; // 0 左  1 下

    QColor m_color_error;
    QColor m_color_init;
    QColor m_color_normal;
    QString m_text = "1234";
    QFont m_text_font;
    QColor m_text_color;

    QString m_state = "正常"; // 0 init  1 normal  2 error

    void DefaultUiInit();
};
ConfigNameSpaceEnd
#endif // STYLEALARM_H
