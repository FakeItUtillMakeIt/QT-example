#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QWidget>
#include "configdefine.h"
#include "elementdisplayer.h"
#include "stylelist.h"
//#include "styleset.h"


ConfigNameSpaceStart
class StyleSet;
class StyleGroup;
class StyleLabel;
class StyleButton;
class StyleCurve;
class StylePairLabel;
class ConfigCurve;
class StyleAlarm;
class StyleManager:public QWidget
{
    Q_OBJECT

public:
    StyleManager(QWidget* parent = nullptr);
private:
    QVBoxLayout* vlayout;
    QGridLayout * gridlayout;
    QComboBox*  types;
private:
    void initUI();
    void addScrollArea();
    StyleSet* styleset;
    StyleList* buttonstylelist;
    StyleList* labelstylelist;
    StyleList* groupstylelist;
    StyleList* curvestylelist;
    StyleList* pairLabelstylelist;
    StyleList* alarmstylelist;

    QStackedWidget* stylestack;
    void initStyleList();
    void addBtn();
    void addLabel();
    void addGroup();
    void addCurve();
    void addPairLabel();
    void addAlarm();

    void delBtn();
    void delLabel();
    void delGroup();
    void delCurve();
    void delPairLabel();
    void delAlarm();

    QMap<QCheckBox*, StyleButton*>  cb_btn;
    QMap<QCheckBox*, StylePairLabel*>  cb_pairlabel;
    QMap<QCheckBox*, StyleCurve*>  cb_curve;
    QMap<QCheckBox*, StyleAlarm*>  cb_alarm;
    QMap<QCheckBox*, StyleGroup*>  cb_group;

private:
    QList<StyleGroup*>  grouplist;
    QList<StyleLabel*>  labellist;
    QList<StyleButton*>  buttonlist;
    QList<StyleCurve*>  curvelist;
    QList<StylePairLabel*>  pairlabellist;
    QList<StyleAlarm*>  alarmlist;


    //界面实时更新缓冲区
    QList< QMap<int,QPair<bool,QString>>* > groupStyleRecord;
    QList< QMap<int,QPair<bool,QString>>* > labelStyleRecord;
    QList< QMap<int,QPair<bool,QString>>* > buttonStyleRecord;
    QList< QMap<int,QPair<bool,QString>>* > curveStyleRecord;
    QList< QMap<int,QPair<bool,QString>>* > pairLabelStyleRecord;
    QList< QMap<int, QPair<bool, QString>>* > alarmStyleRecord;

    void restore_from_xml();
    void SetBtnStyle(QPushButton *btn, QString picture, QString tooltext);
public:
   QList<StyleGroup*> & getGroupStyle();

   QList<StylePairLabel *> &getPairLabelStyle();
   QList<StyleButton *> &getButtonStyle();
   QList<StyleCurve *> &getCurveStyle();
   QList<StyleAlarm*>& getAlarmStyle();

   QString getStyleNameById(ControlType ctroltype, QString styleid);
   bool SelectCurveStyle(QString &styleid, QString &stylename, ConfigCurve *&object);
   bool GetCurveStyle(QString styleid, QMap<int, QPair<bool, QString> > &m_infomap);
   bool RegisterCurveByStyleId(QString styleid, ConfigCurve *object);
signals:
   void Return();
};
ConfigNameSpaceEnd
#endif // STYLEMANAGER_H
