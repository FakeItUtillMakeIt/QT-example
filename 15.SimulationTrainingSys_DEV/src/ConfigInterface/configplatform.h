#ifndef CONFIGPLATFORM_H
#define CONFIGPLATFORM_H
#include "configdefine.h"
#include "configscene.h"
#include <QScrollArea>
#include <QStackedWidget>
#include <qtabwidget.h>

ConfigNameSpaceStart
class ConfigPlatForm:public QScrollArea
{
    Q_OBJECT
public:
    enum ConfigMode
    {
        EditMode,
        CommonMode
    };
    ConfigPlatForm(QWidget *parent = Q_NULLPTR);
    void setWorkMode(ConfigMode mode);
    void SetWorkArea(QRect area);
    void addScene(QString text);
    QList<ConfigScene*>&  get_scenes();
    void addScene(QString text, ConfigTabButton* selbtn, ConfigTabButton* delbtn);
    void ChangeSceneByBtnFromOut(ConfigTabButton* selBtn);
    int DeleteSceneByBtnFromOut(ConfigTabButton* delBtn);
    int DeleteElementByIdFromOut(ControlType ctrtype,QString elementid);

    void ChangeSceneByEntry(QToolButton *entry);
    void SaveCurrentScene();
    void restoreScenesFromXml();
    void InitContainer(int type);

    void addSceneFromXmlData(SceneInfo &sceneinfo);
    void getAutoCreatedBtn(QList<QPair<ConfigTabButton*, ConfigTabButton*>> & autoCreateBtn);
protected:
    void resizeEvent(QResizeEvent * event);
private:
    QStackedWidget * sceneContainer = nullptr;
    QTabWidget* tabSceneContainer = nullptr;
    ConfigMode  m_mode = CommonMode;

    QList<ConfigScene*>  scenes;
    int  m_showtype = 1;
    QString  m_tabstyle;
};
ConfigNameSpaceEnd
#endif // CONFIGPLATFORM_H
