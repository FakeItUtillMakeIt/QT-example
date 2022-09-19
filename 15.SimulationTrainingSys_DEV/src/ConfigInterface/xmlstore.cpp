#include "xmlstore.h"
#include <QDebug>
#include <QDir>
#include "configcurve.h"
#include "stylebutton.h"
#include "stylecurve.h"
#include "stylegroup.h"
#include "stylelabel.h"
#include "configscene.h"
#include "../ControlCommand/AppCache.h"
using namespace tinyxml2;

ConfigNameSpaceStart

QMap<int, QString> XmlStore::rocketmap;
XmlStore::XmlStore()
{

}
bool XmlStore::saveStyle(std::string filename,std::string groupname, QList< QMap<int,QPair<bool,QString>> *>& stylelist)
{
   tinyxml2::XMLDocument doc;//定义doc对象
   const char* declaration="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
   XMLError result = doc.Parse(declaration);

    //<2.添加节点
   XMLElement* groupeslement = doc.NewElement(groupname.c_str());
   doc.InsertEndChild(groupeslement);
    for(auto element: stylelist)
    {
      //  XMLElement* groupelement = doc.NewElement((*element)[0].second.toLatin1().toBase64().toStdString().c_str());
        XMLElement* groupelement = doc.NewElement(QString("K%1").arg((*element)[0].second).toStdString().c_str());
        groupeslement->InsertEndChild(groupelement);
        for(auto key:(*element).keys())
        {

            XMLElement* propertyelement = doc.NewElement( QString("N%1").arg(key).toStdString().data());
            propertyelement->SetText((*element)[key].second.toStdString().c_str());
            groupelement->InsertEndChild(propertyelement);
        }
    }
   //<5.保存至文件中
   result = doc.SaveFile(filename.c_str());//会清除原来文件中的内容
   return true;
}
bool XmlStore::InitStylePath()
{
   //场景文件初始化
        bool  fileexit = false;
        QString exepath = QApplication::applicationDirPath();
        exepath += "/style/";
        QDir  dir;
        if (!dir.exists(exepath))
        {
            dir.mkpath(exepath);
        }
    return true;
}

bool XmlStore::UpdateRocketName(int rocketid, QString  rocketname)
{
    if (rocketmap.contains(rocketid)&& (rocketmap[rocketid] != rocketname))
    {
        QString exepath = QApplication::applicationDirPath();
        QString rocketpath  = exepath +  "/rocket/" + rocketmap[rocketid];
        QString newrocketpath = exepath + "/rocket/" +  rocketname;
        QDir  dir(rocketpath);
        if (dir.exists())
        {
            dir.rename(rocketpath, newrocketpath);
        }
;    }
    return true;
}

bool XmlStore::ReadRocketFile(QString  rocketfile)
{
    tinyxml2::XMLDocument doc;//定义doc对象
    int error = doc.LoadFile(rocketfile.toLocal8Bit().data());
    if (error)
    {
        qDebug() << "打开配置文件‘" << rocketfile << "’时出错,请确认配置文件路径是否正确:" << error;
        return false;
    }
    tinyxml2::XMLElement* rocketsElement = doc.FirstChildElement("rockets");
    if (rocketsElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* rocketElement = rocketsElement->FirstChildElement("rocket");
    while (rocketElement)
    {
        int rocketid = rocketElement->IntAttribute("id");
        QString rocketname = QString::fromLocal8Bit(rocketElement->Attribute("name"));
        rocketmap[rocketid] = rocketname;
        rocketElement = rocketElement->NextSiblingElement("rocket");
    }
    return true;
}

bool XmlStore::InitRocketFile( bool adddefault)
{
     QString filename;
     bool  fileexit = false;

    {//场景文件初始化
        QString exepath = QApplication::applicationDirPath();
        exepath += "/rocket/";
        QDir  dir;
        if (!dir.exists(exepath))
        {
            dir.mkpath(exepath);
        }

        filename = exepath + "/rocket.xml";
        QFile  file(filename);
        if (file.exists())
        {
            qDebug() << "文件存在";
            fileexit = true;
            //读取 rocket.xml，并建立 rocket的 ID name map，在后面发现有变化时，将对应rocket目录名称修改
            ReadRocketFile(filename);
        }
        else
        {
            file.open(QIODevice::WriteOnly);
            file.close();
        }
      //  if (fileexit)
        //    return true;
    }
    tinyxml2::XMLDocument doc;//定义doc对象
    const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    XMLError result = doc.Parse(declaration);

    //<2.添加节点
    XMLElement* groupeslement = doc.NewElement("rockets");
    doc.InsertEndChild(groupeslement);

    //生成火箭节点
    for (auto item: AppCache::instance()->m_allRocketType)
    {
        XMLElement* rocketlement = doc.NewElement("rocket");
        rocketlement->SetAttribute("id", item.first);
        rocketlement->SetAttribute("name", item.second->m_name.c_str());
        UpdateRocketName(item.second->m_id,QString::fromLocal8Bit(item.second->m_name.c_str()));
        rocketlement->SetAttribute("code", item.second->m_code.c_str());
        rocketlement->SetAttribute("path", ("/rocket/" + item.second->m_name + "/scenes.xml").c_str());
        groupeslement->InsertEndChild(rocketlement);
        //生成场景文件
      //  InitSceneFile( QString::fromLocal8Bit(("/rocket/" + item.second->m_name + "/").c_str()), "scenes.xml", adddefault);
       // InitSoftWareSceneFile(QString::fromLocal8Bit(("/rocket/" + item.second->m_name + "/").c_str()), "scenes.xml");
    }
    //为当前登录火箭及软件创建场景文件，并将软件添加入其中
    InitSoftWareSceneFile(QString::fromLocal8Bit(("/rocket/" + AppCache::instance()->m_CurrentRocketType->m_name + "/").c_str()), "scenes.xml");
    //<5.保存至文件中
    result = doc.SaveFile(filename.toStdString().data());//会清除原来文件中的内容
    qDebug() << "result:" << result << " filename:" << filename;
    return true;
}

bool XmlStore::AppenSoftWareIfNotExist(QString filename)
{
    tinyxml2::XMLDocument doc;//定义doc对象
    int error = doc.LoadFile(filename.toLocal8Bit().data());
    if (error)
    {
        QToolTip::showText(QCursor::pos(), QString("打开配置文件 %1 时出错（%2）,请确认配置文件路径是否正确").arg(filename).arg(error));

        qDebug() << "打开配置文件‘" << filename << "’时出错,请确认配置文件路径是否正确:" << error;
        return false;
    }
    tinyxml2::XMLElement* softwareElement = doc.FirstChildElement("software");
    bool  softwareExit =  false;
    while (softwareElement)
    {
        QString  softName = QString::fromLocal8Bit(softwareElement->Attribute("name"));
        int softId  = softwareElement->IntAttribute("id");
        if (softId == ConfigGlobal::currentSoftWareID)
        {
            if (softName != ConfigGlobal::currentSoftWare)
            {
                softwareElement->SetAttribute("name", ConfigGlobal::currentSoftWare.toLocal8Bit().data());
                int result = doc.SaveFile(filename.toLocal8Bit().data());
                if (result == 0)
                    QToolTip::showText(QCursor::pos(), QString("场景文件初始化成功"));
                else
                    QToolTip::showText(QCursor::pos(), QString("场景文件初始化时软件名称更新失败，不影响继续使用"));
            }
            softwareExit = true;
            break;
        }
        softwareElement = softwareElement->NextSiblingElement("software");
    }
    if (!softwareExit)
    {
        tinyxml2::XMLElement* softwareElement = doc.NewElement("software");
        softwareElement->SetAttribute("name", ConfigGlobal::currentSoftWare.toLocal8Bit().data());
        softwareElement->SetAttribute("id", ConfigGlobal::currentSoftWareID);
        doc.InsertEndChild(softwareElement);
        tinyxml2::XMLElement* sceneElement = doc.NewElement("scenes");
        softwareElement->InsertEndChild(sceneElement);
        int result = doc.SaveFile(filename.toLocal8Bit().data());
        if (result == 0)
            QToolTip::showText(QCursor::pos(), QString("场景文件初始化成功"));
        else
            QToolTip::showText(QCursor::pos(), QString("场景文件初始化失败"));
    }
    return true;
}

bool XmlStore::InitSoftWareSceneFile(QString path, QString ifilename)
{
    QString filename;
    bool  fileexit = false;
    {//场景文件初始化
        QString exepath = QApplication::applicationDirPath();
        exepath += path;
        QString dirneeded = QString("%1/%2").arg(exepath).arg(ConfigGlobal::currentSoftWareID);
        QDir  dir;
        if (!dir.exists(dirneeded))
        {
            dir.mkpath(dirneeded);
        }
        filename = exepath + ifilename;
        QFile  file(filename);
        if (file.exists())
        {
            qDebug() << "文件存在";
            fileexit = true;
        }
        else
        {
            file.open(QIODevice::WriteOnly);
            file.close();
        }
    }
    if (fileexit)
    {
        //判断是否有当前软件，没有则添加
        AppenSoftWareIfNotExist(filename);
        return true;
    }
    tinyxml2::XMLDocument doc;//定义doc对象
    const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    XMLError result = doc.Parse(declaration);
    //添加当前软件
    XMLElement* softwarelement = doc.NewElement("software");
    softwarelement->SetAttribute("name",ConfigGlobal::currentSoftWare.toLocal8Bit().data());
    softwarelement->SetAttribute("id", ConfigGlobal::currentSoftWareID);
    doc.InsertEndChild(softwarelement);
    //<2.添加节点
    XMLElement* groupeslement = doc.NewElement("scenes");
    softwarelement->InsertEndChild(groupeslement);
    //<5.保存至文件中
    result = doc.SaveFile(filename.toLocal8Bit().data());//会清除原来文件中的内容
  //  qDebug() << "result:" <<result << " filename:" << filename;
    return true;
}
bool XmlStore::InitSceneFile(QString path,QString ifilename,bool adddefault)
{
    QString filename;
    {//场景文件初始化
        bool  fileexit = false;
        QString exepath = QApplication::applicationDirPath();
        exepath += path;
        QDir  dir;
        if (!dir.exists(exepath))
        {
            dir.mkpath(exepath);
        }

        filename = exepath + ifilename;
        QFile  file(filename);
        if (file.exists())
        {
            qDebug() << "文件存在";
            fileexit = true;
        }
        else
        {
            file.open(QIODevice::WriteOnly);
            file.close();
        }
        if (fileexit)
            return true;
    }
   tinyxml2::XMLDocument doc;//定义doc对象
   const char* declaration="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
   XMLError result = doc.Parse(declaration);

    //<2.添加节点
   XMLElement* groupeslement = doc.NewElement("scenes");
   doc.InsertEndChild(groupeslement);

   //如果是主控，自动添加前后端两个场景
   if(adddefault)
     AddDefaultScene(groupeslement,doc,path);
   //<5.保存至文件中
   result = doc.SaveFile(filename.toLocal8Bit().data());//会清除原来文件中的内容
 //  qDebug() << "result:" <<result << " filename:" << filename;
   return true;
}
bool XmlStore::AddDefaultScene(XMLElement* groupeslement, tinyxml2::XMLDocument& doc,QString ipath)
{
    QMap<QString, QString>  scenemap;
    scenemap[get_uuid()] = "主控软件";
    scenemap[get_uuid()] = "前端软件";

    for (auto key : scenemap.keys())
    {
        tinyxml2::XMLElement* sceneElement = doc.NewElement("scene");
        sceneElement->SetAttribute("name", scenemap[key].toLocal8Bit().data());
        sceneElement->SetAttribute("id", key.toLocal8Bit().data());
        QString filepath = ipath  + scenemap[key] + ".xml";
        sceneElement->SetAttribute("path", filepath.toLocal8Bit().data());
        groupeslement->InsertEndChild(sceneElement);
    }
    return true;
}

bool  XmlStore::ReadElementInfo(tinyxml2::XMLElement* groupElement,QString elementname,QList<QMap<QString,QString>>&  Infolist)
{
    XMLElement* eLement =  groupElement->FirstChildElement(elementname.toLocal8Bit().data());
    while(eLement != nullptr)
    {
        QMap<QString,QString>  elementinfo;
        const XMLAttribute * attibute  =  eLement->FirstAttribute();
        while(attibute)
        {
            QString name =  QString::fromLocal8Bit( attibute->Name());
            QString value =  QString::fromLocal8Bit( attibute->Value());
            elementinfo[name] = value;
            attibute =  attibute->Next();
        }
        Infolist.push_back(elementinfo);
        eLement = eLement->NextSiblingElement(elementname.toLocal8Bit().data());
    }
    return true;
}
bool  XmlStore::ReadGroupElementInfo(tinyxml2::XMLDocument& doc,QString groupname,QString elementname,QList<GroupElementInfo>&  groupinfolist)
{
    tinyxml2::XMLElement* groupsElement = doc.FirstChildElement(groupname.toLocal8Bit().data());
    if(groupsElement == nullptr)
    {
        return false;
    }
    XMLElement* groupElement =  groupsElement->FirstChildElement(elementname.toLocal8Bit().data());
    while(groupElement != nullptr)
    {
        GroupElementInfo  elementinfo;
        const XMLAttribute * attibute  =  groupElement->FirstAttribute();
        while(attibute)
        {
            QString name =  QString::fromLocal8Bit( attibute->Name());
            QString value =  QString::fromLocal8Bit( attibute->Value());
            elementinfo.groupinfo[name] = value;
            attibute =  attibute->Next();
        }
        {
            tinyxml2::XMLElement* ButtonsElement = groupElement->FirstChildElement("Buttons");
            if(groupElement != nullptr)
            {
                ReadElementInfo(ButtonsElement,"Button",elementinfo.buttonInfolist);
            }
            tinyxml2::XMLElement* pairLabelsElement = groupElement->FirstChildElement("PairLabels");
            if(pairLabelsElement != nullptr)
            {
                ReadElementInfo(pairLabelsElement,"PairLabel",elementinfo.PairLabelInfolist);
            }
            tinyxml2::XMLElement* alarmsElement = groupElement->FirstChildElement("Alarms");
            if (alarmsElement != nullptr)
            {
                ReadElementInfo(alarmsElement, "Alarm", elementinfo.alarmInfolist);
            }            
        }

        groupinfolist.push_back(elementinfo);
        groupElement = groupElement->NextSiblingElement(elementname.toLocal8Bit().data());
    }
    return true;
}
bool  XmlStore::ReadSceneInfoFile(QString filename,SceneInfo& sceneinfolist)
{
    QString exepath  = QApplication::applicationDirPath();
    QString scenefile  = exepath + filename;
    tinyxml2::XMLDocument doc;//定义doc对象
    int error = doc.LoadFile(scenefile.toLocal8Bit().data());
    if (error)
    {
       qDebug() << "打开配置文件‘" << filename << "’时出错,请确认配置文件路径是否正确:" <<error;
        return false;
    }
    tinyxml2::XMLElement* ButtonsElement = doc.FirstChildElement("Buttons");
    if(ButtonsElement != nullptr)
    {
        ReadElementInfo(ButtonsElement,"Button",sceneinfolist.buttonInfolist);
    }
    tinyxml2::XMLElement* pairLabelsElement = doc.FirstChildElement("PairLabels");
    if(pairLabelsElement != nullptr)
    {
        ReadElementInfo(pairLabelsElement,"PairLabel",sceneinfolist.PairLabelInfolist);
    }
    tinyxml2::XMLElement* alarmsElement = doc.FirstChildElement("Alarms");
    if (alarmsElement != nullptr)
    {
        ReadElementInfo(alarmsElement, "Alarm", sceneinfolist.alarmInfolist);
    }
    tinyxml2::XMLElement* curvesElement = doc.FirstChildElement("Curves");
    if(curvesElement != nullptr)
    {
        ReadElementInfo(curvesElement,"Curve",sceneinfolist.curveInfolist);
    }

    ReadGroupElementInfo(doc,"Groups","Group",sceneinfolist.groupinfolist);
    qDebug() << "ReadSceneInfoFile end";
    return true;
}

bool  XmlStore::ReadSceneFile(std::string  filename,QList<SceneInfo>& sceneinfolist)
{

    tinyxml2::XMLDocument doc;//定义doc对象
    int error = doc.LoadFile(filename.c_str());
    if (error)
    {
        QToolTip::showText(QCursor::pos(), QString("打开配置文件 %1 时出错（%2）,请确认配置文件路径是否正确").arg(filename.c_str()).arg(error));

       //qDebug() << "打开配置文件‘" << filename.c_str() << "’时出错,请确认配置文件路径是否正确:" <<error;
        return false;
    }
    tinyxml2::XMLElement* softwareElement = doc.FirstChildElement("software");
    while (softwareElement)
    {
        int softwareid = softwareElement->IntAttribute("id");
        if (ConfigGlobal::currentSoftWareID == softwareid)
        {
            tinyxml2::XMLElement* groupElement = softwareElement->FirstChildElement("scenes");
            if (groupElement == nullptr)
            {
                return false;
            }
            XMLElement* idNode = groupElement->FirstChildElement("scene");
            while (idNode != nullptr)
            {
                SceneInfo  sceneinfo;
                sceneinfo.scenename = QString::fromLocal8Bit(idNode->Attribute("name"));
                sceneinfo.sceneid = QString::fromLocal8Bit(idNode->Attribute("id"));
                sceneinfo.scenepath = QString::fromLocal8Bit(idNode->Attribute("path"));
                if (idNode->FindAttribute("useschedule"))
                    sceneinfo.useschedule = idNode->BoolAttribute("useschedule");
                else
                    sceneinfo.useschedule = false;

                sceneinfolist.push_back(sceneinfo);
                idNode = idNode->NextSiblingElement("scene");
            }

            break;
        }
        softwareElement = softwareElement->NextSiblingElement("software");
    }

    for(auto&  sceneinfo:sceneinfolist)
    {
        ReadSceneInfoFile(sceneinfo.scenepath,sceneinfo);
    }
    qDebug() << " XmlStore::ReadSceneFil end";
    return true;
}
std::string XmlStore::handlePath(std::string aimStr)
{
    QString tmpstr = QString::fromLocal8Bit(aimStr.c_str());
    tmpstr.replace("\/","\\");
    return   tmpstr.toLocal8Bit().data();
}
QString XmlStore::handlePath(QString  aimStr)
{   
    aimStr.replace("\/", "\\");
    return   aimStr;
}

bool XmlStore::AddSoftWareSceneToFile(std::string filename, ConfigScene* scene)
{
    tinyxml2::XMLDocument doc;//定义doc对象
   // filename = handlePath(filename);
    int error = doc.LoadFile(filename.c_str());
    if (error)
    {
        QToolTip::showText(QCursor::pos(), QString("打开配置文件 %1 时出错（%2）,请确认配置文件路径是否正确").arg(filename.c_str()).arg(error));

        qDebug() << "打开配置文件‘" << filename.c_str() << "’时出错,请确认配置文件路径是否正确:" << error;
        return false;
    }
    tinyxml2::XMLElement* softwareElement = doc.FirstChildElement("software");
    while(softwareElement)
    {
        int  softwareid = softwareElement->IntAttribute("id");
        if (ConfigGlobal::currentSoftWareID == softwareid)
        {
            break;
        }
        softwareElement = softwareElement->NextSiblingElement("software");
    }
    if (!softwareElement)
    {
        return false;
    }
    tinyxml2::XMLElement* groupElement = softwareElement->FirstChildElement("scenes");
    if (groupElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* sceneElement = doc.NewElement("scene");
    sceneElement->SetAttribute("name", scene->GetName().toLocal8Bit().data());
    sceneElement->SetAttribute("id", scene->GetID().toLocal8Bit().data());
    QString filepath =QString("/rocket/%1/%2/%3.xml").arg(ConfigNameSpace::ConfigGlobal::currentRocket).arg(QString::number(ConfigGlobal::currentSoftWareID)).arg(scene->GetName());
    scene->SetPath(filepath);
    sceneElement->SetAttribute("path", filepath.toLocal8Bit().data());
    groupElement->InsertEndChild(sceneElement);
    int result = doc.SaveFile(filename.c_str());
    if (result == 0)
        QToolTip::showText(QCursor::pos(), QString("场景添加完成,路径%1").arg(filepath));
    else
        QToolTip::showText(QCursor::pos(), QString("场景添加失败,错误%1 路径:%2").arg(result).arg(filepath));
    return true;
}
bool XmlStore::AddSceneToFile(std::string filename, ConfigScene *scene)
{
    tinyxml2::XMLDocument doc;//定义doc对象
   // filename = handlePath(filename);
    int error = doc.LoadFile(filename.c_str());
    if (error)
    {
        QToolTip::showText(QCursor::pos(), QString("打开配置文件 %1 时出错（%2）,请确认配置文件路径是否正确").arg(filename.c_str()).arg(error));

       qDebug() << "打开配置文件‘" << filename.c_str() << "’时出错,请确认配置文件路径是否正确:" <<error;
        return false;
    }
    tinyxml2::XMLElement* groupElement = doc.FirstChildElement("scenes");
    if(groupElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* sceneElement = doc.NewElement("scene");
    sceneElement->SetAttribute("name",scene->GetName().toLocal8Bit().data());
    sceneElement->SetAttribute("id",scene->GetID().toLocal8Bit().data());
    QString filepath  = "/rocket/" + ConfigNameSpace::ConfigGlobal::currentRocket + "/" + scene->GetName()+ ".xml";
    scene->SetPath(filepath);
    sceneElement->SetAttribute("path",filepath.toLocal8Bit().data());
    groupElement->InsertEndChild(sceneElement);
    int result =  doc.SaveFile(filename.c_str());
    if (result == 0)
        QToolTip::showText(QCursor::pos(), QString("场景添加完成,路径%1").arg(filepath));
    else
        QToolTip::showText(QCursor::pos(), QString("场景添加失败,错误%1 路径:%2").arg(result).arg(filepath));
    return true;
}

bool XmlStore::UpdateSceneNameToFile(ConfigScene* scene)
{
    QString  exepath = QApplication::applicationDirPath();
    QString rocketpath = exepath + "/rocket/" + ConfigGlobal::currentRocket;
    QString scenesxmlpath = rocketpath + "/scenes.xml";
    QString pripath;
    tinyxml2::XMLDocument doc;//定义doc对象
   // filename = handlePath(filename);
    int error = doc.LoadFile(scenesxmlpath.toLocal8Bit().data());
    if (error)
    {
        QToolTip::showText(QCursor::pos(), QString("打开配置文件 %1 时出错（%2）,请确认配置文件路径是否正确").arg(scenesxmlpath).arg(error));

        qDebug() << "打开配置文件‘" << scenesxmlpath << "’时出错,请确认配置文件路径是否正确:" << error;
        return false;
    }
    tinyxml2::XMLElement* softwareElement = doc.FirstChildElement("software");
    bool  softwareExit = false;
    while (softwareElement)
    {
        QString  softName = QString::fromLocal8Bit(softwareElement->Attribute("name"));
        int softId = softwareElement->IntAttribute("id");
        if (softId == ConfigGlobal::currentSoftWareID)
        {           
            softwareExit = true;
            break;
        }
        softwareElement = softwareElement->NextSiblingElement("software");
    }
    if (!softwareExit)
    {
        return false;
    }

    tinyxml2::XMLElement* groupElement = softwareElement->FirstChildElement("scenes");
    if (groupElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* sceneElement = groupElement->FirstChildElement("scene");
    while(sceneElement)
    {
        QString sceneid = sceneElement->Attribute("id");
        if (sceneid == scene->GetID())
        {
            pripath = QString::fromLocal8Bit(sceneElement->Attribute("path"));
            sceneElement->SetAttribute("name",scene->GetName().toLocal8Bit().data());
            QString filepath = QString("/rocket/%1/%2/%3.xml").arg(ConfigNameSpace::ConfigGlobal::currentRocket).arg(QString::number(ConfigGlobal::currentSoftWareID)).arg(scene->GetName());
         //   QString filepath = "/rocket/" + ConfigNameSpace::ConfigGlobal::currentRocket + "/" + scene->GetName() + ".xml";
            scene->SetPath(filepath);
            sceneElement->SetAttribute("path", filepath.toLocal8Bit().data());
            break;
        }
        sceneElement = sceneElement->NextSiblingElement("scene");
    }
 
    int result = doc.SaveFile(scenesxmlpath.toLocal8Bit().data());
    if (result == 0)
    {
        QToolTip::showText(QCursor::pos(), QString("场景更新完成,路径%1").arg(scenesxmlpath));
        pripath = exepath + pripath;
        QString newpath = exepath + scene->GetPath();
        QFile  file(pripath);
        if (file.exists())
            file.rename(newpath);
    }
    else
    {
        QToolTip::showText(QCursor::pos(), QString("场景更新失败,错误%1 路径:%2").arg(result).arg(scenesxmlpath));
    }
    return true;
}
bool XmlStore::UpdateSceneSchedule(ConfigScene* scene)
{
    QString  exepath = QApplication::applicationDirPath();
    QString rocketpath = exepath + "/rocket/" + ConfigGlobal::currentRocket;
    QString scenesxmlpath = rocketpath + "/scenes.xml";
    QString pripath;
    tinyxml2::XMLDocument doc;//定义doc对象
   // filename = handlePath(filename);
    int error = doc.LoadFile(scenesxmlpath.toLocal8Bit().data());
    if (error)
    {
        QToolTip::showText(QCursor::pos(), QString("打开配置文件 %1 时出错（%2）,请确认配置文件路径是否正确").arg(scenesxmlpath).arg(error));

        qDebug() << "打开配置文件‘" << scenesxmlpath << "’时出错,请确认配置文件路径是否正确:" << error;
        return false;
    }
    tinyxml2::XMLElement* softwareElement = doc.FirstChildElement("software");
    bool  softwareExit = false;
    while (softwareElement)
    {
        QString  softName = QString::fromLocal8Bit(softwareElement->Attribute("name"));
        int softId = softwareElement->IntAttribute("id");
        if (softId == ConfigGlobal::currentSoftWareID)
        {
            softwareExit = true;
            break;
        }
        softwareElement = softwareElement->NextSiblingElement("software");
    }
    if (!softwareExit)
    {
        return false;
    }
    tinyxml2::XMLElement* groupElement = softwareElement->FirstChildElement("scenes");
    if (groupElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* sceneElement = groupElement->FirstChildElement("scene");
    while (sceneElement)
    {
        QString sceneid = sceneElement->Attribute("id");
        if (sceneid == scene->GetID())
        {  
            sceneElement->SetAttribute("useschedule", scene->userSchedule);
            break;
        }
        sceneElement = sceneElement->NextSiblingElement("scene");
    }

    int result = doc.SaveFile(scenesxmlpath.toLocal8Bit().data());
    if (result == 0)
    {
        QToolTip::showText(QCursor::pos(), QString("流程使能完成,"));      
    }
    else
    {
        QToolTip::showText(QCursor::pos(), QString("流程使能失败,错误%1").arg(result));
    }
    return true;
}
bool XmlStore::ReadElement(tinyxml2::XMLDocument &doc, QMap<QString, QList<QMap<QString,QString>>>& sceneinfo,QString group,QString elementname)
{
    tinyxml2::XMLElement* ctrlgroupElement = doc.FirstChildElement(group.toLocal8Bit().data());
    if(ctrlgroupElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* ctrlElement = ctrlgroupElement->FirstChildElement(elementname.toLocal8Bit().data());
    while(ctrlElement)
    {
        QMap<QString,QString>  eleinfo;
       const XMLAttribute * attibute  =  ctrlElement->FirstAttribute();
       while(attibute)
       {
           QString name =  QString::fromLocal8Bit( attibute->Name());
           QString value =  QString::fromLocal8Bit( attibute->Value());
           eleinfo[name] = value;
           attibute  =     attibute->Next();
       }
       sceneinfo[group].push_back(eleinfo);
       ctrlElement = ctrlElement->NextSiblingElement(elementname.toLocal8Bit().data());
    }
}

bool XmlStore::ReadSceneFromFile(std::string filename,QMap<QString, QList<QMap<QString,QString>>>& sceneinfo)
{
    tinyxml2::XMLDocument doc;//定义doc对象
    int error = doc.LoadFile(filename.c_str());
    if (error)
    {
       qDebug() << "打开配置文件‘" << filename.c_str() << "’时出错,请确认配置文件路径是否正确:" <<error;
        return false;
    }

    ReadElement(doc,sceneinfo,"Buttons","Button");
    return true;
}
bool XmlStore::SaveSceneButtons(tinyxml2::XMLDocument& doc, XMLElement*& buttonsElement,QList<ConfigButton*>&  sceneButtonList)
{
    for(auto button: sceneButtonList)
     {
         QMap<QString,ConfigValueSet>&  m_valueSetMap  = button->m_valueSetMap;
         XMLElement* buttonElement = doc.NewElement("Button");
         buttonElement->SetAttribute("uuid",button->GetID().toLocal8Bit().data());
         for(auto setkey: m_valueSetMap.keys())
         {
             for(auto  valuekey:m_valueSetMap[setkey].valuelist.keys())
             {
                 QString  attributekey  = setkey + ":" + QString("%1").arg(valuekey);
                 QString  strvalue = m_valueSetMap[setkey].valuelist[valuekey]->getStrValue();
                 buttonElement->SetAttribute(attributekey.toLocal8Bit().data(),strvalue.toLocal8Bit().data());
             }
         }
         buttonsElement->InsertEndChild(buttonElement);
      }
    return true;
}

bool XmlStore::SaveSceneCurves(tinyxml2::XMLDocument& doc, XMLElement*& curvesElement,QList<ConfigCurve*>&  sceneCurveList)
{
    for(auto curve: sceneCurveList)
     {
         QMap<QString,ConfigValueSet>&  m_valueSetMap  = curve->m_valueSetMap;
         XMLElement* curveElement = doc.NewElement("Curve");
         curveElement->SetAttribute("uuid",curve->GetID().toLocal8Bit().data());
         for(auto setkey: m_valueSetMap.keys())
         {
             for(auto  valuekey:m_valueSetMap[setkey].valuelist.keys())
             {
                 QString  attributekey  = setkey + ":" + QString("%1").arg(valuekey);
                 QString  strvalue = m_valueSetMap[setkey].valuelist[valuekey]->getStrValue();
                 curveElement->SetAttribute(attributekey.toLocal8Bit().data(),strvalue.toLocal8Bit().data());
             }
         }
         curvesElement->InsertEndChild(curveElement);
      }
    return true;
}
bool XmlStore::SaveSceneAlarms(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement*& alarmsElement, QList<ConfigAlarm*>& sceneAlarmList)
{
    for (auto alarm : sceneAlarmList)
    {
        QMap<QString, ConfigValueSet>& m_valueSetMap = alarm->getPropertyMap();
        XMLElement* alarmElement = doc.NewElement("Alarm");
        alarmElement->SetAttribute("uuid", alarm->GetID().toLocal8Bit().data());
        for (auto setkey : m_valueSetMap.keys())
        {
            for (auto valuekey : m_valueSetMap[setkey].valuelist.keys())
            {
                QString  attributekey = setkey + ":" + QString("%1").arg(valuekey);
                QString  strvalue = m_valueSetMap[setkey].valuelist[valuekey]->getStrValue();
                alarmElement->SetAttribute(attributekey.toLocal8Bit().data(), strvalue.toLocal8Bit().data());
            }
        }
        alarmsElement->InsertEndChild(alarmElement);
    }
    return true;
}
bool XmlStore::SaveScenePairLabels(tinyxml2::XMLDocument& doc,XMLElement*& pairLabelsElement,QList<ConfigPairLabel*>&  scenePairLabelList)
{
     for(auto pairlabel: scenePairLabelList)
     {
         QMap<QString,ConfigValueSet>&  m_valueSetMap  = pairlabel->getPropertyMap();
         XMLElement* pairlabelElement = doc.NewElement("PairLabel");
         pairlabelElement->SetAttribute("uuid",pairlabel->GetID().toLocal8Bit().data());
         for(auto setkey: m_valueSetMap.keys())
         {
             for(auto  valuekey:m_valueSetMap[setkey].valuelist.keys())
             {
                 QString  attributekey  = setkey + ":" + QString("%1").arg(valuekey);
                 QString  strvalue = m_valueSetMap[setkey].valuelist[valuekey]->getStrValue();
                 pairlabelElement->SetAttribute(attributekey.toLocal8Bit().data(),strvalue.toLocal8Bit().data());
             }
         }
         pairLabelsElement->InsertEndChild(pairlabelElement);
      }
    return true;
}
bool XmlStore::SaveSceneGroups(tinyxml2::XMLDocument& doc,XMLElement*& groupsElement,QList<GroupElement*>&  sceneGroupList)
{
     for(auto group: sceneGroupList)
     {
         //存储分组基本属性
         QMap<QString,ConfigValueSet>&  m_valueSetMap  = group->getPropertyMap();
         XMLElement* groupElement = doc.NewElement("Group");
         groupElement->SetAttribute("uuid",group->GetID().toLocal8Bit().data());
         for(auto setkey: m_valueSetMap.keys())
         {
             for(auto  valuekey:m_valueSetMap[setkey].valuelist.keys())
             {
                 QString  attributekey  = setkey + ":" + QString("%1").arg(valuekey);
                 QString  strvalue = m_valueSetMap[setkey].valuelist[valuekey]->getStrValue();
                 groupElement->SetAttribute(attributekey.toLocal8Bit().data(),strvalue.toLocal8Bit().data());
             }
         }
         XMLElement* ButtonsElement = doc.NewElement("Buttons");
         XMLElement* PairLabelsElement = doc.NewElement("PairLabels");
         XMLElement* alarmsElement = doc.NewElement("Alarms");

         groupElement->InsertEndChild(ButtonsElement);
         groupElement->InsertEndChild(PairLabelsElement);
         groupElement->InsertEndChild(alarmsElement);

         //存储分组按钮成员
         QList<ConfigButton *> & buttonList  =  group->getButtonList();
         SaveSceneButtons(doc,ButtonsElement,buttonList);
         //存储分组标签成员
         QList<ConfigPairLabel *> & pairLabelList  =  group->getPairLabelList();
         SaveScenePairLabels(doc,PairLabelsElement,pairLabelList);
         //存储报警灯
         QList<ConfigAlarm*>& alarmList = group->getAlarmList();
         SaveSceneAlarms(doc, alarmsElement, alarmList);
         groupsElement->InsertEndChild(groupElement);
      }
    return true;
}

int XmlStore::RemoveSceneElementFromFile(QString filename, QString elementid,ControlType ctrtype)
{
    QString exepath = QApplication::applicationDirPath();
    exepath = exepath + filename;
    tinyxml2::XMLDocument doc;//定义doc对象
    int error = doc.LoadFile(exepath.toLocal8Bit().data());
    if (error)
    {
        return error;
    }
    QString groupName = getXmlGroupName(ctrtype);
    if (groupName == "null")
    {
        return -2;
    }
    QString elementName = groupName;
    elementName.chop(1);
    tinyxml2::XMLElement* groupsElement = doc.FirstChildElement(groupName.toLocal8Bit().data());
    if (groupsElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* Element = groupsElement->FirstChildElement(elementName.toLocal8Bit().data());
    int  result = -1;
    while (Element)
    {
        if (Element->Attribute("uuid") != nullptr)
        {
            QString uuid = Element->Attribute("uuid");
            if (uuid == elementid)
            {
                groupsElement->DeleteChild(Element);
                result = 0;
                break;
            }
        }
        Element = groupsElement->NextSiblingElement(elementName.toLocal8Bit().data());
    }
    if (result == 0)
        result = doc.SaveFile(exepath.toLocal8Bit().data());
    return 0;
}

int XmlStore::RemoveSceneFromFile(ConfigScene* scene)
{

    QString exepath = QApplication::applicationDirPath();
  //  QString totalpath = exepath + "/scene/scenes.xml";
    QString totalpath = exepath + "/rocket/" + ConfigGlobal::currentRocket + "/scenes.xml";
    tinyxml2::XMLDocument doc;//定义doc对象
// filename = handlePath(filename);
    int error = doc.LoadFile(totalpath.toLocal8Bit().data());
    if (error)
    {
        QToolTip::showText(QCursor::pos(), QString("打开配置文件 %1 时出错（%2）,请确认配置文件路径是否正确").arg(totalpath).arg(error));

        return error;
    }
    tinyxml2::XMLElement* softwareElement = doc.FirstChildElement("software");
    tinyxml2::XMLElement* currentsoftwareElement = nullptr;
    int softwareid = -1;
    while (softwareElement)
    {
        softwareid = softwareElement->IntAttribute("id");
        if (softwareid == ConfigGlobal::currentSoftWareID)
        {
            currentsoftwareElement = softwareElement;
            break;
        }
        softwareElement = softwareElement->NextSiblingElement("software");

    }
    if (!currentsoftwareElement)
        return 4;
    tinyxml2::XMLElement* scenesElement = currentsoftwareElement->FirstChildElement("scenes");
    if (scenesElement == nullptr)
    {
        return false;
    }
    tinyxml2::XMLElement* sceneElement = scenesElement->FirstChildElement("scene");
    int  result = -1;
    while (sceneElement)
    {
        if (sceneElement->Attribute("id") != nullptr)
        {
            QString id = sceneElement->Attribute("id");
            if (id == scene->GetID())
            {
                scenesElement->DeleteChild(sceneElement);
                result = 0;
                break;
            }
        }
        sceneElement = sceneElement->NextSiblingElement("scene");
    }  
    if(result == 0)
        result = doc.SaveFile(totalpath.toLocal8Bit().data());  
   return result;

}

bool XmlStore::SaveSceneToFile(ConfigScene *scene)
{
    if (scene == nullptr)
        return false;
    QString exepath  = QApplication::applicationDirPath();
    QString  filepath  = exepath + scene->GetPath();
    qDebug() << "scene file path:" << filepath;
    tinyxml2::XMLDocument doc;//定义doc对象
    const char* declaration="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    XMLError result = doc.Parse(declaration);

    XMLElement* buttonsElement = doc.NewElement("Buttons");
    doc.InsertEndChild(buttonsElement);
   SaveSceneButtons(doc,buttonsElement,scene->sceneButtonList);

   XMLElement* pairLabelsElement = doc.NewElement("PairLabels");
   doc.InsertEndChild(pairLabelsElement);
   SaveScenePairLabels(doc,pairLabelsElement,scene->scenePairLabelList);

   XMLElement* groupsElement = doc.NewElement("Groups");
   doc.InsertEndChild(groupsElement);
   SaveSceneGroups(doc,groupsElement,scene->sceneGroupList);


   XMLElement* curvesElement = doc.NewElement("Curves");
   doc.InsertEndChild(curvesElement);
   SaveSceneCurves(doc,curvesElement,scene->sceneCurveList);

   XMLElement* alarmsElement = doc.NewElement("Alarms");
   doc.InsertEndChild(alarmsElement);
   SaveSceneAlarms(doc, alarmsElement, scene->sceneAlarmList);
    //<5.保存至文件中
    result = doc.SaveFile(filepath.toLocal8Bit().data());//会清除原来文件中的内容
    if(result == 0)
     QToolTip::showText(QCursor::pos(), QString("保存完成,路径%1").arg(filepath));
    else
      QToolTip::showText(QCursor::pos(), QString("保存失败,错误%1 路径:%2").arg(result).arg(filepath));

    return true;
}


bool XmlStore::restoreStyle(std::string filename,std::string groupname, QList< QMap<int,QPair<bool,QString>> >& stylelist)
{
   tinyxml2::XMLDocument doc;//定义doc对象

   int error = doc.LoadFile(filename.c_str());
   if (error)
   {
      qDebug() << "打开配置文件‘" << filename.c_str() << "’时出错,请确认配置文件路径是否正确:" <<error;
       return false;
   }
   tinyxml2::XMLElement* groupElement = doc.FirstChildElement(groupname.c_str());
   if(groupElement == nullptr)
   {
       return false;
   }
   tinyxml2::XMLNode* idNode =  groupElement->FirstChild();

   while(idNode != nullptr)
   {

       QMap<int,QPair<bool,QString>>  infomap;
       tinyxml2::XMLNode* infoNode =  idNode->FirstChild();
       while(infoNode != nullptr)
       {
           int key = QString(infoNode->Value()).remove('N').toInt();
            QString value = infoNode->ToElement()->GetText();
            qDebug() << "key:"<< key <<  " value:" <<value;
            infomap[key] = QPair<bool,QString>(false,value);
           infoNode = infoNode->NextSibling();
       }
       stylelist.push_back(infomap);
       idNode = idNode->NextSibling();
   }

   return true;
}

//bool XmlStore::saveGroup(std::string filename,QList<StyleGroup*>  grouplist)
//{
//   tinyxml2::XMLDocument doc;//定义doc对象
//   const char* declaration="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
//   XMLError result = doc.Parse(declaration);

//    //<2.添加节点
//   XMLElement* groupeslement = doc.NewElement("GroupTypes");
//   doc.InsertEndChild(groupeslement);
//    for(auto element: grouplist)
//    {

//        XMLElement* groupelement = doc.NewElement(element->m_infomap[StyleGroup::Group_Style_ID].second.toStdString().c_str());
//        groupeslement->InsertEndChild(groupelement);
//        int first = true;
//        for(auto key:element->m_infomap.keys())
//        {
//            if(first)
//            {
//                first = false;
//                continue;
//            }
//            XMLElement* propertyelement = doc.NewElement(QString::number(key).toStdString().c_str());
//            propertyelement->SetText(element->m_infomap[key].second.toStdString().c_str());
//            groupelement->InsertEndChild(propertyelement);
//        }

//    }

//   //<5.保存至文件中
//   result = doc.SaveFile(filename.c_str());//会清除原来文件中的内容
//   return true;
//}

//bool XmlStore::saveLabel(std::string filename,QList<StyleLabel*>  labellist)
//{
//   tinyxml2::XMLDocument doc;//定义doc对象
//   const char* declaration="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
//   XMLError result = doc.Parse(declaration);

//    //<2.添加节点
//   XMLElement* groupeslement = doc.NewElement("LabelTypes");
//   doc.InsertEndChild(groupeslement);
//    for(auto element: labellist)
//    {
//        XMLElement* groupelement = doc.NewElement(element->m_infomap[StyleLabel::Label_Style_ID].second.toStdString().c_str());
//        groupeslement->InsertEndChild(groupelement);
//        int first = true;
//        for(auto key:element->m_infomap.keys())
//        {
//            if(first)
//            {
//                first = false;
//                continue;
//            }
//            XMLElement* propertyelement = doc.NewElement(QString::number(key).toStdString().c_str());
//            propertyelement->SetText(element->m_infomap[key].second.toStdString().c_str());
//            groupelement->InsertEndChild(propertyelement);
//        }

//    }

//   //<5.保存至文件中
//   result = doc.SaveFile(filename.c_str());//会清除原来文件中的内容
//   return true;
//}


//bool XmlStore::saveButton(std::string filename,QList<StyleButton*>  buttonlist)
//{
//   tinyxml2::XMLDocument doc;//定义doc对象
//   const char* declaration="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
//   XMLError result = doc.Parse(declaration);

//    //<2.添加节点
//   XMLElement* groupeslement = doc.NewElement("ButtonTypes");
//   doc.InsertEndChild(groupeslement);
//    for(auto element: buttonlist)
//    {
//        XMLElement* groupelement = doc.NewElement(element->m_infomap[StyleButton::Button_Style_ID].second.toStdString().c_str());
//        groupeslement->InsertEndChild(groupelement);
//        int first = true;
//        for(auto key:element->m_infomap.keys())
//        {
//            if(first)
//            {
//                first = false;
//                continue;
//            }
//            XMLElement* propertyelement = doc.NewElement(QString::number(key).toStdString().c_str());
//            propertyelement->SetText(element->m_infomap[key].second.toStdString().c_str());
//            groupelement->InsertEndChild(propertyelement);
//        }

//    }

//   //<5.保存至文件中
//   result = doc.SaveFile(filename.c_str());//会清除原来文件中的内容
//   return true;
//}
//bool XmlStore::saveCurve(std::string filename,QList<StyleCurve*>  curvelist)
//{
//   tinyxml2::XMLDocument doc;//定义doc对象
//   const char* declaration="<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
//   XMLError result = doc.Parse(declaration);

//    //<2.添加节点
//   XMLElement* groupeslement = doc.NewElement("CurveTypes");
//   doc.InsertEndChild(groupeslement);
//    for(auto element: curvelist)
//    {
//        XMLElement* groupelement = doc.NewElement(element->m_infomap[StyleCurve::Curve_Style_ID].second.toStdString().c_str());
//        groupeslement->InsertEndChild(groupelement);
//        int first = true;
//        for(auto key:element->m_infomap.keys())
//        {
//            if(first)
//            {
//                first = false;
//                continue;
//            }
//            XMLElement* propertyelement = doc.NewElement(QString::number(key).toStdString().c_str());
//            propertyelement->SetText(element->m_infomap[key].second.toStdString().c_str());
//            groupelement->InsertEndChild(propertyelement);
//        }

//    }

//   //<5.保存至文件中
//   result = doc.SaveFile(filename.c_str());//会清除原来文件中的内容
//   return true;
//}



ConfigNameSpaceEnd
