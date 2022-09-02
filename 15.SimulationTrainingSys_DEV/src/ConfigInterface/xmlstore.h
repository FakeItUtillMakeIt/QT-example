﻿#ifndef XMLSTORE_H
#define XMLSTORE_H
#include "../Public/tinyxml2.h"
#include "configdefine.h"
#include "configscene.h"
#include <QMap>
#include <string>

ConfigNameSpaceStart
class StyleGroup;
class StyleLabel;
class StyleButton;
class StyleCurve;
class ConfigScene;
class XmlStore
{

public:
    XmlStore();    
//    static bool saveGroup(std::string filename, QList<StyleGroup *> grouplist);
//    static bool saveLabel(std::string filename, QList<StyleLabel *> labellist);
//    static bool saveButton(std::string filename, QList<StyleButton *> buttonlist);
//    static bool saveCurve(std::string filename, QList<StyleCurve *> curvelist);
    static bool saveStyle(std::string filename, std::string groupname, QList<QMap<int, QPair<bool, QString> > *> &stylelist);
    static bool restoreStyle(std::string filename, std::string groupname, QList<QMap<int, QPair<bool, QString> >> &stylelist);
    static bool InitSceneFile(QString path, QString ifilename, bool adddefault = false);
    static bool InitSoftWareSceneFile(QString path, QString ifilename);
    static bool InitRocketFile(bool adddefault = false);
    static bool ReadRocketFile(QString  rocketfile);
    static bool UpdateRocketName(int rocketid,QString  rocketname);

    static bool InitStylePath();
    static bool ReadSceneFile(std::string filename, QList<SceneInfo> &sceneinfolist);
    static bool AddSceneToFile(std::string filename, ConfigScene *scene);
    static bool AddSoftWareSceneToFile(std::string filename, ConfigScene* scene);

    static std::string handlePath(std::string  aimStr);
    static QString handlePath(QString  aimStr);
    static int RemoveSceneFromFile(ConfigScene* scene);
    static int RemoveSceneElementFromFile(QString filename,QString elementid, ControlType ctrtype);
    static void initMainControlScene();
    static bool SaveSceneToFile(ConfigScene *scene);
    //static bool ReadSceneFromFile(std::string filename);
    static bool ReadSceneFromFile(std::string filename, QMap<QString, QList<QMap<QString, QString> > >& sceneinfo);
    static bool ReadElement(tinyxml2::XMLDocument &doc, QMap<QString, QList<QMap<QString, QString> > > &sceneinfo, QString group, QString elementname);
    static bool SaveSceneButtons(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&buttonsElement, QList<ConfigButton *> &sceneButtonList);
    static bool SaveSceneGroups(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&groupsElement, QList<GroupElement *> &sceneGroupList);
    static bool SaveScenePairLabels(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&pairLabelsElement, QList<ConfigPairLabel *> &scenePairLabelList);
    static bool SaveSceneAlarms(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement*& alarmsElement, QList<ConfigAlarm*>& sceneAlarmList);
    static bool AppenSoftWareIfNotExist(QString filename);

    static bool ReadSceneInfoFile(QString filename, SceneInfo &sceneinfot);
   // static bool ReadElementInfo(tinyxml2::XMLDocument &doc, QString groupname, QString elementname, QList<QMap<QString, QString> > &Infolist);
    static bool ReadGroupElementInfo(tinyxml2::XMLDocument &doc, QString groupname, QString elementname, QList<GroupElementInfo> &groupinfolist);
    static bool ReadElementInfo(tinyxml2::XMLElement *groupElement, QString elementname, QList<QMap<QString, QString> > &Infolist);
    static bool SaveSceneCurves(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&curvesElement, QList<ConfigCurve *> &sceneCurveList);
    static bool AddDefaultScene(tinyxml2::XMLElement* groupeslement,tinyxml2::XMLDocument& doc, QString ipath);
    static bool UpdateSceneNameToFile(ConfigScene* scene);
    static bool UpdateSceneSchedule(ConfigScene* scene);
    static QMap<int, QString> rocketmap;

};
ConfigNameSpaceEnd
#endif // XMLSTORE_H
