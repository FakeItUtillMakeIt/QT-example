#ifndef CONFIGINTERFACE_H
#define CONFIGINTERFACE_H

#include "groupelement.h"
#include "configdefine.h"
#include "ctrolfactory.h"
#include "propertyset.h"
#include "configplatform.h"
#include "stylemanager.h"
#include <QDockWidget>
#include <QMainWindow>

ConfigNameSpaceStart
class ConfigElementSelector;
class ConfigInterface : public QMainWindow
{
    Q_OBJECT

public:

    explicit ConfigInterface(QWidget *parent = 0);
    ~ConfigInterface();
    CtrolFactory ctrlfactory;
    void RestoreSceneFromXml(QList<ConfigNameSpace::SceneInfo> &sceneinfolist);
    void SetBtnStyle(QPushButton *btn, QString picture, QString tooltext);
    void AddSceneFromOut(QString text, QPushButton* selBtn, QPushButton* delBtn);
    void ChangeSceneByBtnFromOut(QPushButton* selBtn);
    void DeleteSceneByBtnFromOut(QPushButton* delBtn);
    QList<QPair<QPushButton*,QPushButton*>>  autoCreateBtn;
    void firstinit();
    void firstinitFromOut(QList<QPushButton*> btnlist);
    void AddSceneEntry();

private:
    void _addDockWidget();
    QDockWidget* propertydock = nullptr;
    PropertySet* propertySet = nullptr;
    QRect  workarea;
    ConfigPlatForm::ConfigMode mode = ConfigPlatForm::EditMode;
    ConfigPlatForm * configplatform;

    QPushButton* SceneAddBtn = nullptr;
    QButtonGroup*  seceneentrygroup = nullptr;
    QWidget* mainwidget;
    QList<QPushButton* >  editbtnlist;
    void EntryNormaltMode();
    void EntryEditMode();
    QStackedWidget* centralwidget;
    void SetToolStyle(QToolButton *toolbutton);
    void SetAddBtnStyle(QPushButton *pushbutton, int fontsize = 20);
private slots:
    void AddScene();
    void Return();
};
ConfigNameSpaceEnd
#endif // CONFIGINTERFACE_H
