#ifndef PROPERTYSET_H
#define PROPERTYSET_H
#include "configdefine.h"

#include <QSpinBox>
#include <QStackedWidget>
#include <QTextBrowSer>
#include <QToolBox>
#include <QVBoxLayout>
#include <QWidget>

ConfigNameSpaceStart
class PropertySet : public QToolBox
{
    Q_OBJECT
public:          
    explicit PropertySet(QWidget *parent = 0);
    ~PropertySet();
    void setObject(ControlType ctrType,QWidget* object, QMap<QString,ConfigValueSet>&  configmap);
    void updateUi();    
    void UpdateDataFromObject(QWidget *object);
private:
    QMap<QString,ConfigValueSet>*  m_configmap;
    ControlType m_ctrType;
    QWidget* m_object;
    void updateOneSet(QWidget *&containter, ConfigValueSet &configset);
    void updateOneSetData(ConfigValueSet &configset);

    void addIntSelect(QVBoxLayout *vlayout, ConfigValue *configvalue);
    void addStringSelect(QVBoxLayout *vlayout, ConfigValue *configvalue);
    void addDataSource(QVBoxLayout *vlayout,ConfigValue *configvalue);
     void addCommand(QVBoxLayout* vlayout, ConfigValue* configvalue);

    void addStyleSelect(QVBoxLayout *vlayout, ConfigValue *configvalue);
    void addMultiDataSource(QVBoxLayout *vlayout, ConfigValue *configvalue);
    bool  VerifyNumber(QString str);

    QMap<QString,QSpinBox*>  spinboxes;
    QMap<QString,QTextBrowser*>  labels;
    unsigned  long timelastset = 0;

    void updateObject();
    void SelectStyle(ControlType ictrltype, QTextBrowser *showlabel, ConfigValue *configvalue);
    unsigned long lastsettime  = 0;
    bool  istimeavaiable();
    void updateDataSource(QString datasourceid, QString datasourcename, int addordelete);
};

ConfigNameSpaceEnd
#endif // PROPERTYSET_H
