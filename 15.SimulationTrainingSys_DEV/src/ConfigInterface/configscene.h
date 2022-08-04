#ifndef CONFIGSCENE_H
#define CONFIGSCENE_H

#include <QMouseEvent>
#include <QToolButton>
#include <QWidget>
#include "configdefine.h"
#include "configpairlabel.h"
#include "groupelement.h"
#include "xmlstore.h"
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

    int DeleteElementByIdFromOut(ControlType ctrtype, QString elementid);

    template<typename T>
    int RemoveElement(QString elementid,QList<T*>& t,ControlType ctype)
    {
        bool result = 0;
        for (int i = 0; i < t.size(); i++)
        {
            if (t[i]->GetID() == elementid)
            {
  /*              result = XmlStore::RemoveSceneElementFromFile(GetPath(), elementid, ctype);
                if (result == 0)
                {
                    T*  ele =  t.takeAt(i);
                    delete ele;
                    ele = nullptr;
                }*/
                T* ele = t.takeAt(i);
                delete ele;
                ele = nullptr;
                return result;
            }
        }
        result = -1;
    }

 /*   int RemoveGroupElement(QString elementid);
    int RemoveButtonElement(QString elementid);
    int RemovePairLabelElement(QString elementid);
    int RemoveCurveElement(QString elementid);*/

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
    void CreateNewElement(ControlType  ctrltype, QPoint pos);
    void CopyElement(ControlType  ctrltype, QPoint pos, QString copyedid);
    bool GetSrcElementProperty(QString& outinput,ControlType itype,QMap<int,int>&  propertylist );
};
ConfigNameSpaceEnd
#endif // CONFIGSCENE_H
