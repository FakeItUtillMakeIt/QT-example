#ifndef XMLSTORE_H
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
    static bool InitSceneFile();
    static bool InitStylePath();
    static bool ReadSceneFile(std::string filename, QList<SceneInfo> &sceneinfolist);
    static bool AddSceneToFile(std::string filename, ConfigScene *scene);
    static std::string XmlStore::handlePath(std::string  aimStr);
    static QString XmlStore::handlePath(QString  aimStr);
    static bool RemoveSceneFromFile(ConfigScene* scene);
    static bool SaveSceneToFile(ConfigScene *scene);
    //static bool ReadSceneFromFile(std::string filename);
    static bool ReadSceneFromFile(std::string filename, QMap<QString, QList<QMap<QString, QString> > >& sceneinfo);
    static bool ReadElement(tinyxml2::XMLDocument &doc, QMap<QString, QList<QMap<QString, QString> > > &sceneinfo, QString group, QString elementname);
    static bool SaveSceneButtons(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&buttonsElement, QList<ConfigButton *> &sceneButtonList);
    static bool SaveSceneGroups(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&groupsElement, QList<GroupElement *> &sceneGroupList);
    static bool SaveScenePairLabels(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&pairLabelsElement, QList<ConfigPairLabel *> &scenePairLabelList);
    static bool ReadSceneInfoFile(QString filename, SceneInfo &sceneinfot);
   // static bool ReadElementInfo(tinyxml2::XMLDocument &doc, QString groupname, QString elementname, QList<QMap<QString, QString> > &Infolist);
    static bool ReadGroupElementInfo(tinyxml2::XMLDocument &doc, QString groupname, QString elementname, QList<GroupElementInfo> &groupinfolist);
    static bool ReadElementInfo(tinyxml2::XMLElement *groupElement, QString elementname, QList<QMap<QString, QString> > &Infolist);
    static bool SaveSceneCurves(tinyxml2::XMLDocument &doc, tinyxml2::XMLElement *&curvesElement, QList<ConfigCurve *> &sceneCurveList);

};
ConfigNameSpaceEnd
#endif // XMLSTORE_H
