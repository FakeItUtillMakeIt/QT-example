#include "configglobal.h"
#include "configpairlabel.h"
#include "configtypeselector.h"
#include "groupelement.h"
#include "propertyset.h"
#include <QInputDialog>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTextBrowSer>
#include <QToolBox>
#include <QVBoxLayout>
#include <QDateTime>
#include "stylemanager.h"
#include "configcurve.h"
#include "paramselect.h"
ConfigNameSpaceStart
PropertySet::PropertySet(QWidget *parent) :
    QToolBox(parent)
{
}

PropertySet::~PropertySet()
{
}

void PropertySet::setObject(ControlType ctrType, QWidget *object, QMap<QString, ConfigValueSet> &configmap)
{
    if(!istimeavaiable())
    {
        return;
    }
    if(object == m_object)
    {
        return;
    }else
    {
        qDebug() << ctrType << " is set";
        int itemcount = count();
        for(int i = itemcount-1; i >=0; i--)
        {
            removeItem(i);
        }
        spinboxes.clear();
        labels.clear();
    }
    m_configmap  = &configmap;
    m_ctrType = ctrType;
    m_object = object;
    updateUi();
}

void PropertySet::UpdateDataFromObject(QWidget* object)
{
    if(object != m_object)
    {
        return;
    }
    for(auto  key : m_configmap->keys())
    {
        updateOneSetData((*m_configmap)[key]);
    }
}

void PropertySet::updateUi()
{
    for(auto  key : m_configmap->keys())
    {
        QWidget* containter = new QWidget(this);
        updateOneSet(containter,(*m_configmap)[key]);
        addItem(containter,key);
    }
}



void PropertySet::updateOneSet(QWidget* &containter,ConfigValueSet& configset)
{
    QVBoxLayout* vlayout = new QVBoxLayout;
   // qDebug() << "map:" << configset.valuelist.keys();

    for(auto  key : configset.valuelist.keys())
    {
        ConfigValue* configvalue  =  configset.valuelist[key];
        switch (configvalue->valuetype) {
        case eConfigDouble:

            break;
        case eConfigFloat:

            break;
        case eConfigShort:

            break;
        case eConfigInt:
            addIntSelect(vlayout,configvalue);
            break;
        case eConfigString:
            addStringSelect(vlayout,configvalue);
            break;
        case eConfigDataSource:
            addDataSource(vlayout,configvalue);
            break;
        case eConfigStyleInfo:
            addStyleSelect(vlayout,configvalue);
            break;
        case eConfigMultiDataSource:
            addMultiDataSource(vlayout,configvalue);
            break;
        case eConfigCommand:
            addCommand(vlayout, configvalue);
            break;
        default:
            break;
        }
        vlayout->addSpacing(5);
    }
    vlayout->addStretch();
    containter->setLayout(vlayout);
}

void PropertySet::updateOneSetData(ConfigValueSet &configset)
{
    for(auto  key : configset.valuelist.keys())
    {
        ConfigValue* configvalue  =  configset.valuelist[key];
        if(spinboxes.contains(configvalue->valueid))
        {
            qDebug() << "updateing data";
            spinboxes[configvalue->valueid]->setValue(*(int*)configvalue->value);
        }
        if(labels.contains(configvalue->valueid))
        {
            labels[configvalue->valueid]->setPlainText(configvalue->getStrValue());
        }
        if (datasources.contains(configvalue->valueid))
        {
            string  showtext = (*ConfigGlobal::m_allDeviceParamPtr)[configvalue->getStrValue().toInt()]->m_deviceName + ":" + (*ConfigGlobal::m_allDeviceParamPtr)[configvalue->getStrValue().toInt()]->m_subParameterName;
            datasources[configvalue->valueid]->setPlainText(QString::fromLocal8Bit(showtext.c_str()));
        }
        if (cmdsources.contains(configvalue->valueid))
        {
            string  showtext = (*ConfigGlobal::m_allCommadPrt)[configvalue->getStrValue().toInt()]->m_sName;
            cmdsources[configvalue->valueid]->setPlainText(QString::fromLocal8Bit(showtext.c_str()));
        }
        if (multiDatasoueces.contains(configvalue->valueid))
        {
            QList<QString>& dataSourceList = *(QList<QString>*)configvalue->value;
            for (auto item : dataSourceList)
            {
                string  showtext = (*ConfigGlobal::m_allDeviceParamPtr)[item.toInt()]->m_deviceName + ":" + (*ConfigGlobal::m_allDeviceParamPtr)[item.toInt()]->m_subParameterName;
                multiDatasoueces[configvalue->valueid]->addItem(showtext.c_str());
            }
        }
    }

}

void PropertySet::updateObject()
{

     if(m_ctrType == cConfigGroup)
     {
         ((GroupElement*)m_object)->updataDataFromTool();

     }
     else if(m_ctrType == cConfigPairLabel)
     {
         ((ConfigPairLabel*)m_object)->updataDataFromTool();

     }
     else if(m_ctrType == cConfigButton)
     {
         ((ConfigButton*)m_object)->updataDataFromTool();

     }
     else if(m_ctrType == cConfigCurve)
     {
         ((ConfigCurve*)m_object)->updataDataFromTool();

     }
}

void PropertySet::addIntSelect(QVBoxLayout* vlayout, ConfigValue* configvalue )
{
    //界面生成
    QLabel* namelabel = new QLabel(configvalue->valuename);
    namelabel->setAlignment(Qt::AlignCenter);
    QSpinBox* spinbox = new QSpinBox();
    spinbox->setRange(configvalue->downlimit,configvalue->uplimit);
    //数据填充
    int intvalue = *(int*)configvalue->value;
 //   qDebug() <<"intvalue:" <<intvalue;
    spinbox->setValue(intvalue);

    connect(spinbox,static_cast<void (QSpinBox ::*)(int)>(&QSpinBox::valueChanged),[=](int){
        *(int*)configvalue->value  = spinbox->value();
   //     qDebug() << "spinbox valuechanged:"<<*(int*)configvalue->value;

        updateObject();
    });
    spinboxes[configvalue->valueid] = spinbox;
    spinbox->setAlignment(Qt::AlignCenter);
    spinbox->setPrefix("");
    vlayout->addWidget(namelabel);
    vlayout->addWidget(spinbox);

}

void PropertySet::addStringSelect(QVBoxLayout *vlayout, ConfigValue *configvalue)
{
    //界面生成
    QPushButton* stringbtn = new QPushButton(configvalue->valuename);
    QTextBrowser* label = new QTextBrowser();
    label->setMaximumHeight(50);
    connect(stringbtn,&QPushButton::clicked,[=](){
            QInputDialog dlg;
            dlg.setWindowTitle("输入");
            dlg.setLabelText("请输入字符");

            if(dlg.exec() == QDialog::Rejected)
                return;
            QString text = dlg.textValue();
            label->setPlainText(dlg.textValue());
//            memset(configvalue->value,0,MaximumPathLength);
//            memcpy(configvalue->value,text.toStdString().c_str(),strlen(text.toStdString().c_str()));
            configvalue->SetWidthStrValue(text);
            if(m_ctrType == cConfigGroup)
              ((GroupElement*)m_object)->updateFromValueSet();
            else if(m_ctrType == cConfigCurve)
                ((ConfigCurve*)m_object)->updataDataFromTool();


    });
    labels[configvalue->valueid] = label;
    vlayout->addWidget(stringbtn);
    vlayout->addWidget(label);
    //数据填充
    label->setPlainText(configvalue->getStrValue());
}

void PropertySet::SelectStyle( ControlType  ictrltype,QTextBrowser* showlabel,ConfigValue* configvalue)
{
    if(ictrltype == cConfigGroup)
    {
        StyleGroup * stylegroup = nullptr;                 
        GroupElement* groupelement  = (GroupElement*)m_object;
        if(groupelement->SelectStyle(stylegroup))
        {
            char* styleidinfo =  (char*)configvalue->value;
            memset(styleidinfo,0,MaximumPathLength);
            QString styleid  = stylegroup->m_infomap[StyleGroup::Group_Style_ID].second;
            memcpy(styleidinfo,styleid.toStdString().c_str(),strlen(styleid.toStdString().c_str()));
            showlabel->setPlainText(stylegroup->m_infomap[StyleGroup::Group_Style_Name].second);
        }

    }
    else if(ictrltype == cConfigPairLabel)
    {
        StylePairLabel* stylepariLabel = nullptr;
        ConfigPairLabel* pairLabel  = (ConfigPairLabel*)m_object;
        if(pairLabel->SelectStyle(stylepariLabel))
        {
            char* styleidinfo =  (char*)configvalue->value;
            memset(styleidinfo,0,MaximumPathLength);
            QString styleid  = stylepariLabel->m_infomap[StylePairLabel::PairLabel_Style_ID].second;
            memcpy(styleidinfo,styleid.toStdString().c_str(),strlen(styleid.toStdString().c_str()));
            showlabel->setPlainText(stylepariLabel->m_infomap[StylePairLabel::PairLabel_Style_Name].second);
        }
    }
    else if(ictrltype == cConfigButton)
    {
        StyleButton* stylebutton = nullptr;
        ConfigButton* configbtn  = (ConfigButton*)m_object;
        if(configbtn->SelectStyle(stylebutton))
        {
            char* styleidinfo =  (char*)configvalue->value;
            memset(styleidinfo,0,MaximumPathLength);
            QString styleid  = stylebutton->m_infomap[StyleButton::Button_Style_ID].second;
            memcpy(styleidinfo,styleid.toStdString().c_str(),strlen(styleid.toStdString().c_str()));
            showlabel->setPlainText(stylebutton->m_infomap[StyleButton::Button_Style_Name].second);
        }
    }
    else if(ictrltype == cConfigCurve)
    {
        ConfigCurve* configcurve  = (ConfigCurve*)m_object;
        QString istyleid,istylename;
        bool result  = ConfigGlobal::gstylemanager->SelectCurveStyle(istyleid,istylename,configcurve);
        if(result)
        {
            configvalue->SetWidthStrValue(istyleid);
            showlabel->setPlainText(istylename);
            configcurve->UpdatePropertyByStyle();
        }
    }
}

bool PropertySet::istimeavaiable()
{
    unsigned long  curtiem = QDateTime::currentMSecsSinceEpoch();
    if(lastsettime == 0)
    {
        lastsettime = curtiem;
        return true;
    }
    if((curtiem - lastsettime) < 1000)
    {
        lastsettime  = curtiem;
        return false;
    }
    lastsettime = curtiem;
    return true;

}

void PropertySet::addStyleSelect(QVBoxLayout* vlayout, ConfigValue* configvalue )
{
    //界面生成
    QPushButton* btnsel = new QPushButton(configvalue->valuename);
    QTextBrowser* showlabel = new QTextBrowser();
    ControlType  ictrltype = m_ctrType;
    connect(btnsel,&QPushButton::clicked,[=](int){
        SelectStyle(ictrltype,showlabel,configvalue);
    });

    labels[configvalue->valueid] = showlabel;
    QString  styleID = (char*)configvalue->value;
    if(styleID == "")
        showlabel->setPlainText("未选择");
    else
    {
        QString stylename = ConfigGlobal::gstylemanager->getStyleNameById(m_ctrType,styleID);
        showlabel->setPlainText(stylename);
    }
    showlabel->setStyleSheet("background-color:rgb(200,200,200)");
    showlabel->setMinimumHeight(30);
    showlabel->setMaximumHeight(60);
    showlabel->setAlignment(Qt::AlignCenter);
    vlayout->addWidget(btnsel);
    vlayout->addWidget(showlabel);
}
bool  PropertySet::VerifyNumber(QString str)
{
    std::string temp = str.toStdString();
    for (int i = 0; i < str.length(); i++)
    {
        if (temp[i] < '0' || temp[i]>'9')
        {
            return false;
        }
    }

    return true;
}
void PropertySet::addDataSource(QVBoxLayout *vlayout,ConfigValue* configvalue )
{
    QPushButton* btnsel = new QPushButton(configvalue->valuename);
    QTextBrowser* showlabel = new QTextBrowser();
    showlabel->setAlignment(Qt::AlignCenter);

    connect(btnsel,&QPushButton::clicked,[=](int){       
        ParamSelect  paramselect;
        paramselect.update_data(*ConfigGlobal::m_allDeviceParamPtr);
        if (paramselect.exec() == QDialog::Rejected)
            return;
        int paramid = paramselect.get_select_key();
        configvalue->SetWidthStrValue(QString::number(paramid));
        string  showtext = (*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_deviceName + ":" +(*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_subParameterName;
        showlabel->setPlainText(QString::fromLocal8Bit(showtext.c_str()));
        QString paramname = QString::fromLocal8Bit((*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_subParameterName.c_str());
        updateDataSource(QString::number(paramid), paramname, 1);
       });

    datasources[configvalue->valueid] = showlabel;
    QString  datasourceid = configvalue->getStrValue();
    if (VerifyNumber(datasourceid))
    {
        if ( (*ConfigGlobal::m_allDeviceParamPtr).find(datasourceid.toInt()) != (*ConfigGlobal::m_allDeviceParamPtr).end())
        {
              string showtext = (*ConfigGlobal::m_allDeviceParamPtr)[datasourceid.toInt()]->m_deviceName + ":" + (*ConfigGlobal::m_allDeviceParamPtr)[datasourceid.toInt()]->m_subParameterName;
              datasourceid = QString::fromLocal8Bit(showtext.c_str());
        }
    }
    showlabel->setPlainText(datasourceid);
    showlabel->setStyleSheet("background-color:rgb(200,200,200)");
    showlabel->setMinimumHeight(30);
    showlabel->setMaximumHeight(60);
    vlayout->addWidget(btnsel);
    vlayout->addWidget(showlabel);
}

void PropertySet::addCommand(QVBoxLayout* vlayout, ConfigValue* configvalue)
{
   QPushButton* btnsel = new QPushButton(configvalue->valuename);
    QTextBrowser* showlabel = new QTextBrowser();
    showlabel->setAlignment(Qt::AlignCenter);

    connect(btnsel, &QPushButton::clicked, [=](int) {
        ParamSelect  paramselect;
        paramselect.update_data(*ConfigGlobal::m_allCommadPrt);
        if (paramselect.exec() == QDialog::Rejected)
            return;
        int paramid = paramselect.get_select_key();
        configvalue->SetWidthStrValue(QString::number(paramid));
        string showtext = (*ConfigGlobal::m_allCommadPrt)[paramid]->m_sName;
        showlabel->setPlainText(QString::fromLocal8Bit(showtext.c_str()));
        QString paramname = QString::fromLocal8Bit((*ConfigGlobal::m_allCommadPrt)[paramid]->m_sName.c_str());
        updateDataSource(QString::number(paramid), paramname, 1);
        });

    cmdsources[configvalue->valueid] = showlabel;
    QString  datasourceid = configvalue->getStrValue();
    if (VerifyNumber(datasourceid))
    {
        if ((*ConfigGlobal::m_allCommadPrt).find(datasourceid.toInt()) != (*ConfigGlobal::m_allCommadPrt).end())
        {
            string showtext = (*ConfigGlobal::m_allCommadPrt)[datasourceid.toInt()]->m_sName;
            datasourceid = QString::fromLocal8Bit(showtext.c_str());
        }
    }
    showlabel->setPlainText(datasourceid);
    showlabel->setStyleSheet("background-color:rgb(200,200,200)");
    showlabel->setMinimumHeight(30);
    showlabel->setMaximumHeight(60);
    vlayout->addWidget(btnsel);
    vlayout->addWidget(showlabel);
}

void PropertySet::updateDataSource(QString datasourceid,QString datasourcename,int  addordelete)
{

     if(m_ctrType == cConfigCurve)
    {
        ((ConfigCurve*)m_object)->updateCurve(datasourceid,datasourcename,addordelete);
     }
     else if (m_ctrType == cConfigPairLabel)
     {
         ((ConfigPairLabel*)m_object)->updateDataSource(datasourceid, datasourcename, addordelete);

     }
     else if (m_ctrType == cConfigButton)
     {
         ((ConfigButton*)m_object)->updateDataSource(datasourceid, datasourcename, addordelete);

     }
}


void PropertySet::addMultiDataSource(QVBoxLayout *vlayout,ConfigValue* configvalue )
{
    QPushButton* btnsel = new QPushButton(configvalue->valuename);
    QPushButton* delSel = new QPushButton("删除");
    QListWidget* listwidget = new QListWidget();
    listwidget->setSelectionMode(QAbstractItemView::SingleSelection);
    multiDatasoueces[configvalue->valueid] = listwidget;
    connect(btnsel,&QPushButton::clicked,[=](int){

        {
            QList<QString>& dataSourceList = *(QList<QString>*)configvalue->value;  
            ParamSelect  paramselect;
            paramselect.update_data(*ConfigGlobal::m_allDeviceParamPtr);
            if (paramselect.exec() == QDialog::Rejected)
                return;
            int paramid = paramselect.get_select_key();
            string  showtext = (*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_deviceName + ":" + (*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_subParameterName;
            listwidget->addItem(QString::fromLocal8Bit(showtext.c_str()));
            QString paramname = QString::fromLocal8Bit((*ConfigGlobal::m_allDeviceParamPtr)[paramid]->m_subParameterName.c_str());
            dataSourceList.push_back(QString::number(paramid));
            updateDataSource(QString::number(paramid), paramname, 1);        
        }
    });

    connect(delSel,&QPushButton::clicked,[=](int){
        int selectindex = -1;
        QString seleteitemname, selectid;
        for(int i =0; i < listwidget->count(); i++)
        {
            if(listwidget->item(i)->isSelected())
            {
                selectindex = i;
                QListWidgetItem* listitem =  listwidget->takeItem(i);
                seleteitemname = listitem->text();
                delete listitem;
                break;
            }
        }
        QList<QString>&  dataSourceList = *(QList<QString>*)configvalue->value;
        if(selectindex != -1)
        {
            selectid = dataSourceList.at(selectindex);
            updateDataSource(selectid,seleteitemname,2);
            dataSourceList.removeAt(selectindex);
        }
    });

    QList<QString>& dataSourceList = *(QList<QString>*)configvalue->value;
    for (auto item : dataSourceList)
    {
        string  showtext = (*ConfigGlobal::m_allDeviceParamPtr)[item.toInt()]->m_deviceName + ":" + (*ConfigGlobal::m_allDeviceParamPtr)[item.toInt()]->m_subParameterName;
        listwidget->addItem(QString::fromLocal8Bit(showtext.c_str()));
    }
    vlayout->addWidget(btnsel);
    vlayout->addWidget(delSel);
    vlayout->addWidget(listwidget);
}
ConfigNameSpaceEnd
