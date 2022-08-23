#include "configbutton.h"
#include "configelementselector.h"
#include "configglobal.h"
#include "configinterface.h"
#include "configlabel.h"
#include "configplatform.h"
#include "groupelement.h"

#include <QDrag>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QPushButton>
#include <QDebug>
#include <QButtonGroup>
#include <QInputDialog>

ConfigNameSpaceStart
ConfigInterface::ConfigInterface(QWidget *parent) :
    QMainWindow(parent)
{
    startTimer(100);
}


void ConfigInterface::timerEvent(QTimerEvent* event)
{
    ConfigGlobal::updateDefaultState();

    if (!ConfigGlobal::m_allDeviceParamPtr) return;

    if (!ConfigGlobal::dataupdated) return;
    ConfigNameSpace::ConfigGlobal::dataupdated = false;
    for (auto it = ConfigGlobal::m_allDeviceParamPtr->begin(); it != ConfigGlobal::m_allDeviceParamPtr->end(); it++)
    {
        DeviceParam* param = it->second;
        ConfigGlobal::updateControlValue(param->m_Validity, param->m_id, param->m_time, param->m_iCurValue,param->m_curStatus.m_id);
    }

}


void  ConfigInterface::firstinitFromOut(QList<QPushButton*> btnlist)
{
    //初始化编辑入口
        connect(ConfigNameSpace::ConfigGlobal::gstylemanager,&StyleManager::Return,this,&ConfigInterface::Return);
        centralwidget = new QStackedWidget;
        centralwidget->addWidget(ConfigNameSpace::ConfigGlobal::gstylemanager);
        mainwidget = new QWidget;
        QPushButton* btn = btnlist[0];
        SetBtnStyle(btn,":/rc/edit.png","编辑");
        connect(btn,&QPushButton::clicked,[=]() {
        ConfigGlobal::isEditing = !ConfigGlobal::isEditing;
        if (!ConfigGlobal::isEditing)
        {
            SetBtnStyle(btn,":/rc/edit.png","编辑");
            mode = ConfigPlatForm::CommonMode;
        configplatform->setWorkMode(mode);
        EntryNormaltMode();
        }
        else
        {
        SetBtnStyle(btn,":/rc/quit.png","退出编辑");
        mode = ConfigPlatForm::EditMode;
        configplatform->setWorkMode(mode);
        EntryEditMode();
        }
        });
        QPushButton* savescene = btnlist[2];
        connect(savescene,&QPushButton::clicked,[=]() {
        configplatform->SaveCurrentScene();
        });

        QPushButton* OpenStyleManager = btnlist[1];
        connect(OpenStyleManager,&QPushButton::clicked,[=]() {
        propertydock->setVisible(false);
        centralwidget->setCurrentWidget(ConfigNameSpace::ConfigGlobal::gstylemanager);
        });

        configplatform = new ConfigPlatForm;

        ConfigElementSelector* groupselector = (ConfigElementSelector*)btnlist[6];
        groupselector->setSelectorInfo(cConfigGroup,"组");
        ConfigElementSelector* btnselector = (ConfigElementSelector*)btnlist[5];
        btnselector->setSelectorInfo(cConfigButton,"按钮");
        //    ConfigElementSelector* labelselector = new ConfigElementSelector(mainwidget);
        //    labelselector->setSelectorInfo(cConfigLabel,"单独标签");
        //    labelselector->setGeometry(230,10,100,30);
        ConfigElementSelector* pairlabelselector = (ConfigElementSelector*)btnlist[4];
        pairlabelselector->setSelectorInfo(cConfigPairLabel,"参数标签");

        ConfigElementSelector* curveselector = (ConfigElementSelector*)btnlist[3];
        curveselector->setSelectorInfo(cConfigCurve,"曲线");


        QPushButton* delelement = btnlist[7];
        connect(delelement, &QPushButton::clicked, [=]() {
            ControlType  ctrtype;
            QString elementid;
            bool  ingroup;
            MoveAbleFrame::GetControlWidget(ctrtype, elementid,ingroup);
            if (ingroup)
            {
                qDebug() << "grouoelement can not delete";
                return;
            }   
            configplatform->DeleteElementByIdFromOut(ctrtype, elementid);
         });

        QGridLayout* gridlayout = new QGridLayout;
        SetBtnStyle(groupselector,":/rc/group.png","组");
        SetBtnStyle(btnselector,":/rc/button.png","按钮");
        SetBtnStyle(pairlabelselector,":/rc/label.png","标签");
        SetBtnStyle(curveselector,":/rc/curve.png","曲线");
        SetBtnStyle(savescene,":/rc/save.png","保存场景");
        SetBtnStyle(OpenStyleManager,":/rc/stylemanage.png","打开样式编辑器");
        SetBtnStyle(delelement, "://rc/delete_normal.png", "删除控件");

        editbtnlist.append(groupselector);
        editbtnlist.append(btnselector);
        editbtnlist.append(pairlabelselector);
        editbtnlist.append(curveselector);
        editbtnlist.append(savescene);
        editbtnlist.append(OpenStyleManager);
        editbtnlist.append(delelement);

        gridlayout->addWidget(configplatform,1,0,10,10);
        mainwidget->setLayout(gridlayout);
        centralwidget->addWidget(mainwidget);
        centralwidget->setCurrentWidget(mainwidget);
        setCentralWidget(centralwidget);

        _addDockWidget();
        SceneAddBtn = new QPushButton("+",mainwidget);
        SceneAddBtn->hide();
        SetAddBtnStyle(SceneAddBtn);
        connect(SceneAddBtn,&QPushButton::clicked,this,&ConfigInterface::AddScene);
        seceneentrygroup = new QButtonGroup(this);
        seceneentrygroup->setExclusive(true);
        EntryNormaltMode();

}
void  ConfigInterface::firstinit()
{
    connect(ConfigNameSpace::ConfigGlobal::gstylemanager, &StyleManager::Return, this, &ConfigInterface::Return);
    centralwidget = new QStackedWidget;
    centralwidget->setObjectName("centralwidget");
    centralwidget->setStyleSheet("QStackedWidget#centralwidget{background-color:rgba(0,0,0,0)}");
    centralwidget->addWidget(ConfigNameSpace::ConfigGlobal::gstylemanager);
    mainwidget = new QWidget;
    mainwidget->setObjectName("mainwidget_1");
 //   mainwidget->setStyleSheet("QWidget#mainwidget_1{background-color:rgb(250,0,0)}");
  //  mainwidget->setStyleSheet("background-color:rgba(0,250,0,250)");
    centralwidget->setAttribute(Qt::WA_StyledBackground);
    mainwidget->setAttribute(Qt::WA_StyledBackground);

    QPushButton* btn = new QPushButton();
    SetBtnStyle(btn, ":/rc/edit.png", "编辑");
    // btn->setFixedSize(30,30);
    connect(btn, &QPushButton::clicked, [=]() {
        ConfigGlobal::isEditing = !ConfigGlobal::isEditing;
        if (!ConfigGlobal::isEditing)
        {
            SetBtnStyle(btn, ":/rc/edit.png", "编辑");
            mode = ConfigPlatForm::CommonMode;
            configplatform->setWorkMode(mode);
            EntryNormaltMode();
        }
        else
        {
            SetBtnStyle(btn, ":/rc/quit.png", "退出编辑");
            mode = ConfigPlatForm::EditMode;
            configplatform->setWorkMode(mode);
            EntryEditMode();
        }
        });
    QPushButton* savescene = new QPushButton("保存场景");
    savescene->setMaximumWidth(100);

    connect(savescene, &QPushButton::clicked, [=]() {
        configplatform->SaveCurrentScene();
        });

    QPushButton* OpenStyleManager = new QPushButton();
    OpenStyleManager->setFixedSize(30, 30);

    connect(OpenStyleManager, &QPushButton::clicked, [=]() {
        propertydock->setVisible(false);
        centralwidget->setCurrentWidget(ConfigNameSpace::ConfigGlobal::gstylemanager);
        });

    configplatform = new ConfigPlatForm;
    // configplatform->addScene();

    QPushButton* delelement = new QPushButton();
    connect(delelement, &QPushButton::clicked, [=]() {
        ControlType  ctrtype;
        QString elementid;
        bool  ingroup;
        MoveAbleFrame::GetControlWidget(ctrtype, elementid, ingroup);
        if (ingroup)
        {
            qDebug() << "grouoelement can not delete";
            return;
        }
        configplatform->DeleteElementByIdFromOut(ctrtype, elementid);
        });


    ConfigElementSelector* groupselector = new ConfigElementSelector();
    groupselector->setSelectorInfo(cConfigGroup, "组");
    ConfigElementSelector* btnselector = new ConfigElementSelector();
    btnselector->setSelectorInfo(cConfigButton, "按钮");
    //    ConfigElementSelector* labelselector = new ConfigElementSelector(mainwidget);
    //    labelselector->setSelectorInfo(cConfigLabel,"单独标签");
    //    labelselector->setGeometry(230,10,100,30);
    ConfigElementSelector* pairlabelselector = new ConfigElementSelector();
    pairlabelselector->setSelectorInfo(cConfigPairLabel, "标签");

    ConfigElementSelector* curveselector = new ConfigElementSelector();
    curveselector->setSelectorInfo(cConfigCurve, "曲线");

    QGridLayout* gridlayout = new QGridLayout;
    int colcount = 3;

    SetBtnStyle(groupselector, ":/rc/group.png", "组");
    SetBtnStyle(btnselector, ":/rc/button.png", "按钮");
    SetBtnStyle(pairlabelselector, ":/rc/label.png", "标签");
    SetBtnStyle(curveselector, ":/rc/curve.png", "曲线");
    SetBtnStyle(savescene, ":/rc/save.png", "保存场景");
    SetBtnStyle(OpenStyleManager, ":/rc/stylemanage.png", "打开样式编辑器");
    SetBtnStyle(delelement, "://rc/delete_normal.png", "删除控件");

    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(delelement);
    hlayout->addWidget(groupselector);
    hlayout->addWidget(btnselector);
    hlayout->addWidget(pairlabelselector);
    hlayout->addWidget(curveselector);
    hlayout->addWidget(savescene);
    hlayout->addWidget(OpenStyleManager);
    hlayout->addWidget(btn);
    gridlayout->addLayout(hlayout, 0, 7, 1, 3);
    editbtnlist.append(groupselector);
    editbtnlist.append(btnselector);
    editbtnlist.append(pairlabelselector);
    editbtnlist.append(curveselector);
    editbtnlist.append(savescene);
    editbtnlist.append(OpenStyleManager);
    editbtnlist.append(delelement);

    gridlayout->addWidget(configplatform, 1, 0, 10, 10);
    //  gridlayout->setSizeConstraint(QLayout::SetFixedSize);
    mainwidget->setLayout(gridlayout);
    centralwidget->addWidget(mainwidget);
    centralwidget->setCurrentWidget(mainwidget);
    setCentralWidget(centralwidget);

    _addDockWidget();
    SceneAddBtn = new QPushButton("+", mainwidget);
    SceneAddBtn->setVisible(false);
    SetAddBtnStyle(SceneAddBtn);
    connect(SceneAddBtn, &QPushButton::clicked, this, &ConfigInterface::AddScene);
    seceneentrygroup = new QButtonGroup(this);
    seceneentrygroup->setExclusive(true);
     AddSceneEntry();
    EntryNormaltMode();
}
void ConfigInterface::EntryEditMode()
{
    for(auto  btn:editbtnlist)
    {
        btn->show();
    }
    propertydock->setVisible(true);

}
void ConfigInterface::EntryNormaltMode()
{

    for(auto  btn:editbtnlist)
    {
        btn->hide();
    }
    MoveAbleFrame::hide_ctrl_point();
    propertydock->setVisible(false);

}

void ConfigInterface::SetBtnStyle(QPushButton* btn,QString picture,QString tooltext)
{
    btn->setText("");
    btn->setFixedSize(20,20);
    btn->setStyleSheet(QString("QPushButton{border-image: url(%1);}QToolTip{border-image: url(:/rc/nValueNormal.png}").arg(picture));
    btn->setToolTip(tooltext);

}
void ConfigInterface::SetToolStyle(QToolButton* toolbutton)
{
    toolbutton->setStyleSheet("QToolButton\
    {\
color:rgb(180,180,180);border:none; background-color:rgb(223,234,254);border-radius:2px  \
    }\
    QToolButton:hover\
    {\
    color:black;border:none;\
    \
    }\
    QToolButton:checked\
    {\
     color:black;border:none;\
    }");

}
void ConfigInterface::SetAddBtnStyle(QPushButton* pushbutton,int fontsize)
{
    pushbutton->setStyleSheet(QString("QPushButton\
    {\
    border-image: url(:/rc/tabnormal.png);font-size:%1px;\
    \
    }\
    QPushButton:hover\
    {\
    border-image: url(:/rc/tablehover.png);\
    \
    }\
    QPushButton:pressed\
    {\
    border-image: url(:/rc/tableselected.png);\
    }").arg(fontsize));

}
void ConfigInterface::AddSceneEntry()
{
    int  xpos  = 10, ypos = 10, width = 120, height = 20,space = 10;
    QList<ConfigScene *> &  scenes  = configplatform->get_scenes();
    for(auto  scene: scenes)
    {
        QToolButton* btn  = scene->selectbtn;
        if(btn == nullptr)
        {
            btn = new QToolButton(mainwidget);
            SetToolStyle(btn);
            seceneentrygroup->addButton(btn);
            btn->setCheckable(true);
            connect(btn,&QToolButton::clicked,[=](bool checked){
              if(!checked)  return;
               configplatform->ChangeSceneByEntry(btn);
            });
            scene->selectbtn  = btn;
        }
        btn->setText(scene->GetName());
        btn->setGeometry(xpos,ypos,width,height);
        btn->show();
        xpos += (width+space);
    }
    SceneAddBtn->setGeometry(xpos,ypos,width,height);
}
void ConfigInterface::AddScene()
{
    QInputDialog dlg;
    dlg.setLabelText("请输入场景名称");
    dlg.setWindowTitle("输入");
    if(dlg.exec() == QDialog::Rejected)
        return;
    QString text = dlg.textValue();
    configplatform->addScene(text);
    AddSceneEntry();
}
int ConfigInterface::DeleteSceneByBtnFromOut(QPushButton* delBtn)
{
    return configplatform->DeleteSceneByBtnFromOut(delBtn);

}
void ConfigInterface::ChangeSceneByBtnFromOut(QPushButton* selBtn)
{
    configplatform->ChangeSceneByBtnFromOut(selBtn);
}
void ConfigInterface::AddSceneFromOut(QString text,QPushButton* selBtn,QPushButton* delBtn)
{
    configplatform->addScene(text, selBtn, delBtn);
  //  AddSceneEntry();
}

void ConfigInterface::Return()
{
    centralwidget->setCurrentWidget(mainwidget);
    propertydock->setVisible(true);

}
void ConfigInterface::RestoreSceneFromXml(QList<ConfigNameSpace::SceneInfo>&  sceneinfolist)
{
    for(auto& info: sceneinfolist)
    {
     configplatform->addSceneFromXmlData(info);
    }
  //  AddSceneEntry();
    configplatform->getAutoCreatedBtn(autoCreateBtn);
}


void ConfigInterface::_addDockWidget()
{
    //停靠窗口1
    propertydock = new QDockWidget("属性配置",this);//构建停靠窗口，指定父类
    propertydock->setMinimumWidth(200);
    propertydock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);//设置停靠窗口特性，可移动，可关闭
    propertydock->setAllowedAreas(Qt::RightDockWidgetArea);//设置可停靠区域为主窗口左边和右边
    propertydock->setStyleSheet("QDockWidget{background-color:rgb(255,255,255);color:rgb(0,0,0);font: 10pt \"微软雅黑\";min-height:100px;}");

    propertySet = new PropertySet();
    ConfigGlobal::gpropeetyset = propertySet;
    propertydock->setWidget(propertySet);
    addDockWidget(Qt::RightDockWidgetArea, propertydock);
    propertydock->setVisible(false);
    mode = ConfigPlatForm::CommonMode;
}
ConfigInterface::~ConfigInterface()
{



}




ConfigNameSpaceEnd
