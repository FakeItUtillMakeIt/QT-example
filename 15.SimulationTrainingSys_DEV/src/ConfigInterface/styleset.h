#ifndef STYLESET_H
#define STYLESET_H


#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "configdefine.h"


ConfigNameSpaceStart
class StyleLabel;
class StyleButton;
class StyleGroup;
class StyleCurve;
class StylePairLabel;
class StyleAlarm;

class StyleSet:public QWidget
{
public:
    StyleSet(QWidget* parent = nullptr);
    void initUi();
    void SetElement(StyleLabel *labelEx);
    void SetElement(StyleButton *btnEx);   
    void SetElement(StyleGroup *groupEx);   
    void SetElement(StyleCurve *curveEx);
    void SetElement(StylePairLabel *pairlabelEx);
    void SetElement(StyleAlarm* alarmEx);

    void addIconSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle);
    void addFontSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle);
    void addTextColorSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle);
    void addBoolSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle);
    void addTextEdit(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle);
    void addIntSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle);

    void initbuttonSetEntry();
    void initlabelSetEntry();
    void initgroupSetEntry();
    void initcurveSetEntry();
    void initPairLabelSetEntry();
    void initStackWidget();
    void iniAlarmSetEntry();
private:
    QStackedWidget* stackwidget = nullptr;
    QWidget* labelSetEntry;
    QWidget* buttonSetEntry;
    QWidget* grouoSetEntry;
    QWidget* curveSetEntry;
    QWidget* pairLabelSetEntry;
    QWidget* alarmSetEntry;

    QMap<ControlType,QMap<int,QLabel*>>  m_tbmap;

    StyleButton* m_btnEx =nullptr;
    StyleLabel* m_labelEx = nullptr;
    StyleGroup* m_groupEx = nullptr;
    StyleCurve* m_curveEx = nullptr;
    StylePairLabel* m_pairlabelEx = nullptr;
    StyleAlarm* m_alarmEx = nullptr;

    QWidget  * initwidget;

    void update_property(ControlType ctrltyle, int enumvalue, QString result);
};
ConfigNameSpaceEnd
#endif // STYLESET_H
