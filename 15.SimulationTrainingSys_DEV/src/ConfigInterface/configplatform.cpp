#include "configplatform.h"
#include <QDebug>
#include "xmlstore.h"
#include "QApplication"
ConfigNameSpaceStart

ConfigPlatForm::ConfigPlatForm(QWidget *parent):QScrollArea(parent)
{

    m_tabstyle = "/* 向在移动 */\
QTabWidget::pane { \
border: 0px;\
\
        QTabWidget::tab-bar{\
            left:10px;border:0px;\
    }\
        \
        QTabBar::tab{\
            background:qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,\
                            stop : 0 #E1E1E1, stop: 0.4 #DDDDDD,\
                                           stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\
            border:1px solid #C4C4C3;\
    /*   border-bottom-color:red;*/\
       border-top-left-radius:4px;\
       border-top-right-radius:4px;\
       min-width:100px;\
       padding:4px;\
    }\
\
        /* 标签被选中时或鼠标悬浮时 */\
        QTabBar::tab:selected, QTabBar::tab:hover{\
            background:qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1,\
                                        stop : 0 #fafafa, stop: 0.4 #f4f4f4,\
                                        stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);\
    }\
\
        /* 标签被选中时 */\
        QTabBar::tab:selected{\
        /*   border-color:#9B9B9B;\
       border-bottom-color:#C2C7CB;*/\
    }\
\
    /* 标签未被选中时 */\
        QTabBar::tab:!selected{\
            margin-top:2px;\
    }\
        ";

    setFrameShape(QFrame::NoFrame);
    InitContainer(m_showtype);
   // setStyleSheet("QAbstractScrollArea{ background-color: transparent;}");


}
void ConfigPlatForm::InitContainer(int type)
{
    if (type == 1)
    {
        sceneContainer = new QStackedWidget;
        sceneContainer->setObjectName("sceneContainer");
        sceneContainer->setAttribute(Qt::WA_StyledBackground);
        setStyleSheet("background-color:rgba(0,250,0,0)");
        sceneContainer->setStyleSheet("background-color:rgba(0,0,250,0)");

        setWidget(sceneContainer);
    }
    else
    {
        tabSceneContainer = new QTabWidget;
        tabSceneContainer->setTabsClosable(true);
        tabSceneContainer->setMovable(true);
        tabSceneContainer->setTabPosition(QTabWidget::South);
        tabSceneContainer->setStyleSheet(m_tabstyle);

        setWidget(tabSceneContainer);

    }
}
void ConfigPlatForm::restoreScenesFromXml()
{
    QString exepath  = QApplication::applicationDirPath();
    QList<ConfigNameSpace::SceneInfo>  sceneinfolist;
    bool result =  ConfigNameSpace::XmlStore::ReadSceneFile(exepath.toStdString()+ "/scene/scenes.xml",sceneinfolist);
    qDebug() << "reult:" <<result;
}
void ConfigPlatForm::setWorkMode(ConfigMode mode)
{
    m_mode = mode;
}

void ConfigPlatForm::ChangeSceneByEntry(QToolButton* entry)
{
    for(auto scene: scenes)
    {
        if(scene->selectbtn == entry)
        {
         qDebug() << " find correct entry";
            sceneContainer->setCurrentWidget(scene);
        }

    }

}

void ConfigPlatForm::SaveCurrentScene()
{
    if(m_showtype == 1)
      XmlStore::SaveSceneToFile((ConfigScene*)sceneContainer->currentWidget());
    else
      XmlStore::SaveSceneToFile((ConfigScene*)tabSceneContainer->currentWidget());

}

void ConfigPlatForm::addScene(QString text)
{
    ConfigScene* newscene = new ConfigScene;
    newscene->SetName(text);
    QString exepath  = QApplication::applicationDirPath();

    bool result =  ConfigNameSpace::XmlStore::AddSceneToFile(exepath.toStdString()+ "/scene/scenes.xml",newscene);

    if (m_showtype == 1)
    {
        sceneContainer->addWidget(newscene);
        scenes.append(newscene);
        sceneContainer->setCurrentWidget(newscene);
    }
    else
    {
        tabSceneContainer->addTab(newscene,newscene->GetName());
        scenes.append(newscene);
        tabSceneContainer->setCurrentWidget(newscene);
    }
}

int ConfigPlatForm::DeleteElementByIdFromOut(ControlType ctrtype, QString elementid)
{
    int curindex   =   sceneContainer->currentIndex();
    if (curindex == -1)
        return  -1;

    ConfigScene* scene = (ConfigScene*)sceneContainer->currentWidget();
    scene->DeleteElementByIdFromOut(ctrtype, elementid);
    MoveAbleFrame::hide_ctrl_point();
}

int ConfigPlatForm::DeleteSceneByBtnFromOut(QPushButton* delBtn)
{
    for (int i = 0; i<  scenes.size(); i++)
    {
        if (scenes[i]->delbutton == delBtn)
        {
            int result = XmlStore::RemoveSceneFromFile(scenes[i]);
            if (result != 0)
            {
                return result;
            }
            sceneContainer->removeWidget(scenes[i]);
            scenes.removeAt(i);
            return result;
        }
    }
}

void ConfigPlatForm::ChangeSceneByBtnFromOut(QPushButton* selBtn)
{
    for (auto scene : scenes)
    {
        if (scene->selbutton == selBtn)
        {
            sceneContainer->setCurrentWidget(scene);
            break;
        }

    }
}
void ConfigPlatForm::addScene(QString text,QPushButton* selbtn,QPushButton* delbtn)
{
    ConfigScene* newscene = new ConfigScene;
    newscene->SetName(text);
    QString exepath = QApplication::applicationDirPath();

    bool result = ConfigNameSpace::XmlStore::AddSceneToFile(exepath.toStdString() + "/scene/scenes.xml", newscene);

    if (m_showtype == 1)
    {
        sceneContainer->addWidget(newscene);
        scenes.append(newscene);
        sceneContainer->setCurrentWidget(newscene);
    }
    else
    {
        tabSceneContainer->addTab(newscene, newscene->GetName());
        scenes.append(newscene);
        tabSceneContainer->setCurrentWidget(newscene);
    }
    newscene->selbutton = selbtn;
    newscene->delbutton = delbtn;
}
void ConfigPlatForm::getAutoCreatedBtn(QList<QPair<QPushButton*, QPushButton*>>& iautoCreateBtn)
{
    for (auto scene : scenes)
    {
        iautoCreateBtn.push_back(QPair<QPushButton*,QPushButton*>(scene->selbutton,scene->delbutton));
    }
}

void ConfigPlatForm::addSceneFromXmlData(ConfigNameSpace::SceneInfo& sceneinfo)
{
    ConfigScene* newscene = new ConfigScene;
    newscene->RestoreSceneInfo(sceneinfo);
    QString exepath  = QApplication::applicationDirPath();
    if (m_showtype == 1)
    {
        sceneContainer->addWidget(newscene);
        scenes.append(newscene);
        sceneContainer->setCurrentWidget(newscene);
    }
    else
    {
        tabSceneContainer->addTab(newscene, newscene->GetName());
        scenes.append(newscene);
        tabSceneContainer->setCurrentWidget(newscene);
    }
}
QList<ConfigScene *> &ConfigPlatForm::get_scenes()
{
    return scenes;
}


void ConfigPlatForm::SetWorkArea(QRect area)
{
    qDebug() << "SetWorkArea:" << area;
    if (m_showtype == 1)
        sceneContainer->setMinimumSize(area.width(),area.height());
    else
        tabSceneContainer->setMinimumSize(area.width(), area.height());

}

void ConfigPlatForm::resizeEvent(QResizeEvent *event)
{
    if(m_mode == CommonMode)
    {
        if (m_showtype == 1)
            sceneContainer->setMinimumSize(size() - QSize(5, 5));
        else
            tabSceneContainer->setMinimumSize(size() - QSize(5, 5));
    }
    QScrollArea::resizeEvent(event);
}

ConfigNameSpaceEnd
