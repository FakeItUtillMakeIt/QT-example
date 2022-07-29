#ifndef CONFIGSCENE_H
#define CONFIGSCENE_H

#include <QMouseEvent>
#include <QToolButton>
#include <QWidget>
#include "configdefine.h"
#include "configpairlabel.h"
#include "groupelement.h"
ConfigNameSpaceStart
class ConfigCurve;
class ConfigScene:public QWidget
{
    Q_OBJECT
public:
    ConfigScene(QWidget* parent = nullptr);
    void RestoreSceneInfo(ConfigNameSpace::SceneInfo& sceneinfo);
    void AutoCreateOpBtn(QString scenename);
    void SetName(QString scenename);
    QString GetName();
    void SetPath(QString path);
    QString GetPath();
    void SetID(QString id);

    QString GetID();
    QToolButton* selectbtn = nullptr;
    QPushButton* selbutton = nullptr;
    QPushButton* delbutton = nullptr;


    QList<GroupElement*>  sceneGroupList;
    QList<ConfigButton*>  sceneButtonList;
    QList<ConfigPairLabel*>  scenePairLabelList;
    QList<ConfigCurve*>  sceneCurveList;

protected:
    void dropEvent(QDropEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QString m_sceneid;
    QString m_sceneName;
    QString m_path;
    void AddButtonElement(QMap<QString, QString> &buttoninfo);
    void AddPairLabelElement(QMap<QString, QString> &PairLabelinfo);
    void AddGroupElement(GroupElementInfo &groupElementInfo);
    void AddCurveElement(QMap<QString, QString> &PairLabelinfo);

};
ConfigNameSpaceEnd
#endif // CONFIGSCENE_H
