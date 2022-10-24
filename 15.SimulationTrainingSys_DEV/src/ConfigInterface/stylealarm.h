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
        Alarm_Diaplay_Type, //չʾ��ʽ
        Alarm_Text_Position,//�Ƿ������Ų�
        Alarm_Init_Img, //��ʼ����ͼƬ
        Alarm_Normal_Img,//��������ͼƬ
        Alarm_Err_Img, //���󱳾�ͼƬ
        Alarm_Init_Color, //��ʼ������ɫ
        Alarm_Normal_Color,//����������ɫ
        Alarm_Err_Color, //���󱳾���ɫ
        ALarm_Font_Style,//��������
        Alarm_Text_Color//������ɫ
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
    bool m_display_img = false; // 0 ��ɫ  1 ͼƬ
    bool m_display_vertically = false; // 0 ��  1 ��

    QColor m_color_error;
    QColor m_color_init;
    QColor m_color_normal;
    QString m_text = "1234";
    QFont m_text_font;
    QColor m_text_color;

    QString m_state = "����"; // 0 init  1 normal  2 error

    void DefaultUiInit();
};
ConfigNameSpaceEnd
#endif // STYLEALARM_H
