#ifndef STYLEGROUP_H
#define STYLEGROUP_H

#include "configdefine.h"
#include <QLabel>
#include <QMap>
#include <QWidget>

ConfigNameSpaceStart

class StyleSet;
class GroupElement;
class StyleGroup : public QWidget
{
    Q_OBJECT
public:

    enum  GroupInfo
    {
       Group_Style_ID = 0,
       Group_Style_Name,
       Group_Common_Style, //常态背景图片
       Group_Title_Style,
       Group_Font_Style,//文字字体
       Group_Text_Color,//文字颜色
       Group_Icon_Style
    };
    QMap<int,QPair<bool,QString>>  m_infomap;
    QLabel* m_namelabel;
    QList<GroupElement*>  styleusers;


    explicit StyleGroup(QWidget *parent = 0);
    StyleSet*  styleset;

    void updateStyle();
    void setTypeName(QString iTypeName);
    void record_styleusers(GroupElement *groupelement);
protected:
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int m_row = 5;
    int m_col = 2;
    int m_colspace = 5;
    int m_rowspace = 5;

    int m_rowspan = 20;
    int m_leftmagin = 2;
    int m_rightmargin = 2;
    int m_topmagin = 50;
    int m_titleheight = 45;

    void drawTitle(QPainter &painter);
    void drawDeleteEntry(QPainter &painter);
    void drawTitleText(QPainter &painter);
    void drawIconEntry(QPainter &painter);

    QImage  deleteicon;
    QImage  deleteiconpressed;
    QRect deleteentry;
    bool  deletehitted = false;

    QRect titletextentry;
    bool  titletexthitted = false;
    QString titletext ="标题";

    void DefaultUiInit();
};
ConfigNameSpaceEnd

#endif // STYLEGROUP_H
