#include "elementdisplayer.h"
#include "stylemanager.h"
#include "configdefine.h"
#include "configbutton.h"
#include "stylebutton.h"
#include "stylegroup.h"
#include "stylecurve.h"
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include "stylepairlabel.h"
#include "xmlstore.h"
#include "styleset.h"
#include "StyleGroup.h"
#include "stylebutton.h"
#include "stylelabel.h"
#include "stylecurve.h"
#include "stylepairlabel.h"
#include "stylealarm.h"

#include "configtypeselector.h"
ConfigNameSpaceStart
StyleManager::StyleManager(QWidget* parent)
 :QWidget(parent)
{
   // setAttribute(Qt::WA_StyledBackground);
    initStyleList();
    initUI();
    restore_from_xml();
    stylestack->setCurrentWidget(buttonstylelist);

}
void StyleManager::SetBtnStyle(QPushButton* btn,QString picture,QString tooltext)
{
    btn->setText("");
    btn->setFixedSize(20,20);
    btn->setStyleSheet(QString("QPushButton{border-image: url(%1);}QToolTip{border-image: url(:/rc/nValueNormal.png}").arg(picture));
    btn->setToolTip(tooltext);

}
void StyleManager::initStyleList()
{
    stylestack = new QStackedWidget;
    buttonstylelist = new StyleList;
    labelstylelist = new StyleList;
    groupstylelist = new StyleList;
    curvestylelist = new StyleList;
    pairLabelstylelist = new StyleList;
    alarmstylelist = new StyleList;
    stylestack->addWidget(buttonstylelist);
    stylestack->addWidget(labelstylelist);
    stylestack->addWidget(groupstylelist);
    stylestack->addWidget(curvestylelist);
    stylestack->addWidget(alarmstylelist);
}

void StyleManager::initUI()
{
  //  initStyleList();
    styleset = new StyleSet;
    vlayout = new QVBoxLayout;
    gridlayout = new QGridLayout;
    types = new QComboBox();
    types->addItem("按钮");
  //  types->addItem("单独标签");
    types->addItem("标签");
    types->addItem("组");
    types->addItem("曲线");
    types->addItem("报警灯");

    types->setMaximumWidth(200);
    connect(types,&QComboBox::currentTextChanged,[=](const QString & text){
        if(text == "按钮")
        {
           qDebug() << "text:" << text;
            stylestack->setCurrentWidget(buttonstylelist);
        }
        else if(text == "单独标签")
        {
            stylestack->setCurrentWidget(labelstylelist);

        }
        else if(text == "组")
        {
            stylestack->setCurrentWidget(groupstylelist);

        }
        else if(text == "曲线")
        {
            stylestack->setCurrentWidget(curvestylelist);

        }
        else if(text == "标签")
        {
            stylestack->setCurrentWidget(pairLabelstylelist);

        }
        else if (text == "报警灯")
        {
            stylestack->setCurrentWidget(alarmstylelist);

        }
        styleset->initStackWidget();
    });
    QPushButton* addbtn = new QPushButton("");
    SetBtnStyle(addbtn,":/rc/add.png","增加");
    connect(addbtn,&QPushButton::clicked,[=](){
        QString text = types->currentText();
        if(text == "按钮")
        {
             addBtn();
        }
        else if(text == "单独标签")
        {
            addLabel();
        }
        else if(text == "组")
        {
            addGroup();
        }
        else if(text == "曲线")
        {
            addCurve();
        }
        else if(text == "标签")
        {
            addPairLabel();
        }
        else if (text == "报警灯")
        {
            addAlarm();
        }
    });

    /////////

    QPushButton* delbtn = new QPushButton("");
    SetBtnStyle(delbtn, ":/rc/delete_normal.png", "删除");
    connect(delbtn, &QPushButton::clicked, [=]() {
        QString text = types->currentText();
        if (text == "按钮")
        {
            delBtn();
        }
        else if (text == "单独标签")
        {
            delLabel();
        }
        else if (text == "组")
        {
            delGroup();
        }
        else if (text == "曲线")
        {
            delCurve();
        }
        else if (text == "标签")
        {
            delPairLabel();
        }
        else if (text == "报警灯")
        {
            delAlarm();
        }
        });
    //////////

    QPushButton* savebtn = new QPushButton("");
    SetBtnStyle(savebtn,":/rc/save.png","保存");

    connect(savebtn,&QPushButton::clicked,[=](){
        QString text = types->currentText();
        QString exepath = QApplication::applicationDirPath();
        if(text == "按钮")
        {
            XmlStore::saveStyle(exepath.toStdString()+ "/style/button.xml","Buttons",buttonStyleRecord);

        }
        else if(text == "单独标签")
        {
            XmlStore::saveStyle(exepath.toStdString()+ "/style/label.xml","Labels",labelStyleRecord);
        }
        else if(text == "组")
        {
            XmlStore::saveStyle(exepath.toStdString()+ "/style/group.xml","Groups",groupStyleRecord);

        }
        else if(text == "曲线")
        {
            XmlStore::saveStyle(exepath.toStdString()+ "/style/curve.xml","Curves",curveStyleRecord);
        }
        else if(text == "标签")
        {
            XmlStore::saveStyle(exepath.toStdString()+ "/style/pairlabel.xml","PairLabel",pairLabelStyleRecord);
        }
        else if (text == "报警灯")
        {
            XmlStore::saveStyle(exepath.toStdString() + "/style/alarm.xml", "Alarms", alarmStyleRecord);
        }
    });
    QPushButton* retBtn = new QPushButton("返回");
    SetBtnStyle(retBtn,":/rc/return.png","返回");
    connect(retBtn,&QPushButton::clicked,[=](){
        emit Return();
    });
    gridlayout->addWidget(types,0,0,1,5);
    gridlayout->addWidget(addbtn,0,3,1,1);
    gridlayout->addWidget(delbtn, 0, 4, 1, 1);  
    gridlayout->addWidget(savebtn,0,5,1,1);
    gridlayout->addWidget(retBtn,0,6,1,1);

    gridlayout->addWidget(stylestack,1,0,20,10);
    gridlayout->addWidget(styleset,1,12,20,5);
    vlayout->addLayout(gridlayout);
    setLayout(vlayout);
}

void StyleManager::addBtn()
{
    StyleButton* stylebutton = new  StyleButton("按钮");
    QCheckBox* selectbtn = new QCheckBox();
    stylebutton->resize(200,50);
    stylebutton->styleset = styleset;
    int btnlistsize = buttonlist.size();
    QString newname = QString("按钮样式%1").arg(++btnlistsize);
    for (auto btn : buttonlist)
    {
        if (btn->m_namelabel->text() == newname)
        {
            newname = QString("按钮样式%1").arg(++btnlistsize);
        }
    }
    stylebutton->setTypeName(newname);
    buttonstylelist->addWidget(stylebutton->m_namelabel,stylebutton);
    buttonlist.append(stylebutton);
    buttonStyleRecord.push_back(&stylebutton->m_infomap);
}

void StyleManager::addLabel()
{

    StyleLabel* stylelabel = new  StyleLabel("单独标签");
    stylelabel->resize(200,50);

    stylelabel->styleset = styleset;
    stylelabel->setTypeName(QString("单独标签样式%1").arg(labellist.size()+1));
    labelstylelist->addWidget(stylelabel->m_namelabel,stylelabel);
    labellist.append(stylelabel);
    labelStyleRecord.push_back(&stylelabel->m_infomap);

}

void StyleManager::addGroup()
{
    StyleGroup* stylegroup = new  StyleGroup();
    stylegroup->resize(400,200);
    stylegroup->styleset = styleset;
    int grouplistsize = grouplist.size();
    QString newname = QString("组样式%1").arg(++grouplistsize);
    for (auto groupinfo : grouplist)
    {
        if (groupinfo->m_namelabel->text() == newname)
        {
            newname = QString("组样式%1").arg(++grouplistsize);
        }
    }
    stylegroup->setTypeName(newname);
    //stylegroup->setTypeName(QString("组样式%1").arg(grouplist.size()+1));
    groupstylelist->addWidget(stylegroup->m_namelabel,stylegroup);
    grouplist.append(stylegroup);
    groupStyleRecord.push_back(&stylegroup->m_infomap);
}

void StyleManager::addCurve()
{
    StyleCurve* stylecurve = new  StyleCurve();
    stylecurve->resize(400,200);
    stylecurve->styleset = styleset;

    int curvelistsize = curvelist.size();
    QString newname = QString("曲线样式%1").arg(++curvelistsize);
    for (auto curveinfo : curvelist)
    {
        if (curveinfo->m_namelabel->text() == newname)
        {
            newname = QString("曲线样式%1").arg(++curvelistsize);
        }
    }
    stylecurve->setTypeName(newname);

  //  stylecurve->setTypeName(QString("曲线样式%1").arg(curvelist.size()+1));
    curvestylelist->addWidget(stylecurve->m_namelabel,stylecurve);
    curvelist.append(stylecurve);
    curveStyleRecord.push_back(&stylecurve->m_infomap);

}

void StyleManager::addPairLabel()
{
    StylePairLabel* stylelabel = new  StylePairLabel();
    stylelabel->resize(400,100);

    stylelabel->styleset = styleset;

    int stylelistsize = pairlabellist.size();
    QString newname = QString("标签样式%1").arg(++stylelistsize);
    for (auto curveinfo : pairlabellist)
    {
        if (curveinfo->m_namelabel->text() == newname)
        {
            newname = QString("标签样式%1").arg(++stylelistsize);
        }
    }
    stylelabel->setTypeName(newname);
//    stylelabel->setTypeName(QString("标签样式%1").arg(pairlabellist.size()+1));
    pairLabelstylelist->addWidget(stylelabel->m_namelabel,stylelabel);
    pairlabellist.append(stylelabel);
    labelStyleRecord.push_back(&stylelabel->m_infomap);
    pairLabelStyleRecord.push_back(&stylelabel->m_infomap);
}

void StyleManager::addAlarm()
{
    StyleAlarm* stylealarm = new  StyleAlarm();
    stylealarm->resize(400, 100);

    stylealarm->styleset = styleset;

    int stylelistsize = alarmlist.size();
    QString newname = QString("报警灯样式%1").arg(++stylelistsize);
    for (auto curveinfo : alarmlist)
    {
        if (curveinfo->m_namelabel->text() == newname)
        {
            newname = QString("报警灯样式%1").arg(++stylelistsize);
        }
    }
    stylealarm->setTypeName(newname);
  //  stylealarm->setTypeName(QString("报警灯样式%1").arg(alarmlist.size() + 1));
    alarmstylelist->addWidget(stylealarm->m_namelabel, stylealarm);
    alarmlist.append(stylealarm);
    alarmStyleRecord.push_back(&stylealarm->m_infomap);
}

void StyleManager::delBtn()
{
    for (auto btn : buttonlist)
    {
        if (btn->m_namelabel->isChecked())
        {
            buttonstylelist->removeWidget(btn->m_namelabel, btn);
            buttonlist.removeOne(btn);
            buttonStyleRecord.removeOne(&btn->m_infomap);
        }
    }
    //StyleButton* stylebutton = new  StyleButton("按钮");
    //stylebutton->resize(200, 50);
    //stylebutton->styleset = styleset;
    //stylebutton->setTypeName(QString("按钮样式%1").arg(buttonlist.size() + 1));
    //buttonstylelist->addWidget(stylebutton->m_namelabel, stylebutton);
    //buttonlist.append(stylebutton);
    //buttonStyleRecord.push_back(&stylebutton->m_infomap);
}
void StyleManager::delLabel() 
{

}
void StyleManager::delGroup() 
{
    for (auto group : grouplist)
    {
        if (group->m_namelabel->isChecked())
        {
            groupstylelist->removeWidget(group->m_namelabel, group);
            grouplist.removeOne(group);
            groupStyleRecord.removeOne(&group->m_infomap);
        }
    }
}
void StyleManager::delCurve() 
{
    for (auto curve : curvelist)
    {
        if (curve->m_namelabel->isChecked())
        {
            curvestylelist->removeWidget(curve->m_namelabel, curve);
            curvelist.removeOne(curve);
            curveStyleRecord.removeOne(&curve->m_infomap);
        }
    }
}
void StyleManager::delPairLabel() 
{
    for (auto pairlabel : pairlabellist)
    {
        if (pairlabel->m_namelabel->isChecked())
        {
            pairLabelstylelist->removeWidget(pairlabel->m_namelabel, pairlabel);
            pairlabellist.removeOne(pairlabel);
            pairLabelStyleRecord.removeOne(&pairlabel->m_infomap);
        }
    }
}
void StyleManager::delAlarm() 
{
    for (auto alarminfo : alarmlist)
    {
        if (alarminfo->m_namelabel->isChecked())
        {
            alarmstylelist->removeWidget(alarminfo->m_namelabel, alarminfo);
            alarmlist.removeOne(alarminfo);
            alarmStyleRecord.removeOne(&alarminfo->m_infomap);
        }
    }
}

void StyleManager::restore_from_xml()
{
    QString exepath  = QApplication::applicationDirPath();
    QList< QMap<int,QPair<bool,QString>> > groupStyleListRestore;
    XmlStore::restoreStyle(exepath.toStdString()+ "/style/group.xml","Groups", groupStyleListRestore);
    for(auto  info: groupStyleListRestore)
    {
        addGroup();
        StyleGroup* stylegroup = grouplist.last();
        stylegroup->m_infomap = info;
        stylegroup->updateStyle();
    }

    QList< QMap<int,QPair<bool,QString>> > buttonStyleListRestore;
    XmlStore::restoreStyle(exepath.toStdString()+ "/style/button.xml","Buttons", buttonStyleListRestore);
 //   qDebug() << "buttonlist:" << buttonStyleListRestore;
    for(auto  info: buttonStyleListRestore)
    {
        addBtn();
        StyleButton* stylebutton = buttonlist.last();
        stylebutton->m_infomap = info;
        stylebutton->updateStyle();
    }

    QList< QMap<int,QPair<bool,QString>> > lableStyleListRestore;
    XmlStore::restoreStyle(exepath.toStdString()+ "/style/label.xml","Labels", lableStyleListRestore);
    for(auto  info: lableStyleListRestore)
    {
        addLabel();
        StyleLabel* stylelabel = labellist.last();
        stylelabel->m_infomap = info;
        stylelabel->updateStyle();
    }

    QList< QMap<int,QPair<bool,QString>> > curveStyleListRestore;
    XmlStore::restoreStyle(exepath.toStdString()+ "/style/curve.xml","Curves", curveStyleListRestore);
    for(auto  info: curveStyleListRestore)
    {
        addCurve();
        StyleCurve* stylecurve = curvelist.last();
        stylecurve->m_infomap = info;
        stylecurve->updateStyle();
    }

    QList< QMap<int,QPair<bool,QString>> > pairLabelStyleListRestore;
    XmlStore::restoreStyle(exepath.toStdString()+ "/style/pairlabel.xml","PairLabel", pairLabelStyleListRestore);
    for(auto  info: pairLabelStyleListRestore)
    {
        addPairLabel();
        StylePairLabel* stylepairlabel = pairlabellist.last();
        stylepairlabel->m_infomap = info;
        stylepairlabel->updateStyle();
    }

    QList< QMap<int, QPair<bool, QString>> > alarmStyleListRestore;
    XmlStore::restoreStyle(exepath.toStdString() + "/style/alarm.xml", "Alarms", alarmStyleListRestore);
    for (auto info : alarmStyleListRestore)
    {
        addAlarm();
        StyleAlarm* stylealarm = alarmlist.last();
        stylealarm->m_infomap = info;
        stylealarm->updateStyle();
    }
}

QList<StyleGroup *>& StyleManager::getGroupStyle()
{
    qDebug() << "grouplist:" <<grouplist.size();
    return grouplist;
}
QList<StyleAlarm*>& StyleManager::getAlarmStyle()
{
    qDebug() << "alarmlist:" << alarmlist.size();
    return alarmlist;
}
QList<StylePairLabel *>& StyleManager::getPairLabelStyle()
{
    qDebug() << "pairlabellist:" <<pairlabellist.size();
    return pairlabellist;
}
bool StyleManager::RegisterCurveByStyleId(QString  styleid,ConfigCurve*object)
{

    for(auto  curvestyle:curvelist)
    {
        if(curvestyle->m_infomap[StyleCurve::Curve_Style_ID].second == styleid)
        {
            curvestyle->record_styleusers(object);
            return true;
        }

    }
    return false;

}
bool StyleManager::GetCurveStyle(QString  styleid,QMap<int,QPair<bool,QString>> & m_infomap)
{

    for(auto  curvestyle:curvelist)
    {
        if(curvestyle->m_infomap[StyleCurve::Curve_Style_ID].second == styleid)
        {
            m_infomap = curvestyle->m_infomap;
            return true;
        }

    }
    return false;

}

bool StyleManager::SelectCurveStyle(QString&  styleid,QString& stylename,ConfigCurve*&object)
{
    StyleCurve* returnstyle = nullptr;
    ConfigTypeSelector  selector;
    selector.UpdateTyleInfo(curvelist);
    if(selector.exec() == QDialog::Rejected)
        return false;
    int styleindex =  selector.getResult();
    returnstyle = curvelist.at(styleindex);

    styleid = returnstyle->m_infomap[StyleButton::Button_Style_ID].second;
    stylename = returnstyle->m_infomap[StyleCurve::Curve_Style_Name].second;

    returnstyle->record_styleusers(object);
    return true;
}
QList<StyleButton *>& StyleManager::getButtonStyle()
{
    return buttonlist;
}
QList<StyleCurve *>& StyleManager::getCurveStyle()
{
    return curvelist;
}
QString  StyleManager::getStyleNameById(ControlType ctroltype,QString styleid)
{
    if(styleid == "")
        return "";
    if(ctroltype == cConfigPairLabel)
    {
        for(auto  style: pairlabellist)
        {
            if(style->m_infomap[StylePairLabel::PairLabel_Style_ID].second == styleid)
            {
                return  style->m_infomap[StylePairLabel::PairLabel_Style_Name].second;
            }

        }
        return "";

    }
    else if(ctroltype == cConfigGroup)
    {
        for(auto  groupinfo: grouplist)
        {
            if(groupinfo->m_infomap[StyleGroup::Group_Style_ID].second == styleid)
            {
                return  groupinfo->m_infomap[StyleGroup::Group_Style_Name].second;
            }

        }
        return "";

    }
    else if(ctroltype == cConfigLabel)
    {
        for(auto  labelinfo: labellist)
        {
            if(labelinfo->m_infomap[StyleLabel::Label_Style_ID].second == styleid)
            {
                return  labelinfo->m_infomap[StyleLabel::Label_Style_Name].second;
            }

        }
        return "";

    }
    else if(ctroltype == cConfigButton)
    {
        for(auto  buttoninfo: buttonlist)
        {
            if(buttoninfo->m_infomap[StyleButton::Button_Style_ID].second == styleid)
            {
                return  buttoninfo->m_infomap[StyleButton::Button_Style_Name].second;
            }

        }
        return "";

    }
    else if(ctroltype == cConfigCurve)
    {
        for(auto  curveinfo: curvelist)
        {
            if(curveinfo->m_infomap[StyleCurve::Curve_Style_ID].second == styleid)
            {
                return  curveinfo->m_infomap[StyleCurve::Curve_Style_Name].second;
            }

        }
        return "";

    }
    else if (ctroltype == cConfigAlarm)
    {
        for (auto alarminfo : alarmlist)
        {
            if (alarminfo->m_infomap[StyleAlarm::Alarm_Style_ID].second == styleid)
            {
                return  alarminfo->m_infomap[StyleAlarm::Alarm_Style_Name].second;
            }

        }
        return "";

    }
}





ConfigNameSpaceEnd
