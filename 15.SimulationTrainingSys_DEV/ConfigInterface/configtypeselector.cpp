#include "configtypeselector.h"
#include "stylebutton.h"
#include "stylecurve.h"
#include "stylegroup.h"
#include "stylepairlabel.h"
#include <QGridLayout>
#include <QPushButton>
ConfigNameSpaceStart
ConfigTypeSelector::ConfigTypeSelector(QWidget *parent):QDialog(parent)
{
     listwidget = new QListWidget;
     listwidget->setViewMode(QListView::IconMode);
     QVBoxLayout* vlayout = new  QVBoxLayout;
     vlayout->addWidget(listwidget);
     QHBoxLayout* hlayout = new QHBoxLayout;
     QPushButton* okbtn  = new QPushButton("确认");
     connect(okbtn,&QPushButton::clicked,[=](){
           if(!listwidget->currentIndex().isValid())
                return;
           QListWidgetItem* item = listwidget->currentItem();
           m_selectindex = item->data(Qt::UserRole+1).toInt();
         accept();
     });
     QPushButton* cancelbtn  = new QPushButton("取消");
     connect(cancelbtn,&QPushButton::clicked,[=](){

         reject();
     });

     hlayout->addStretch();
     hlayout->addWidget(okbtn);
     hlayout->addWidget(cancelbtn);
     vlayout->addLayout(hlayout);
     setLayout(vlayout);
}

void ConfigTypeSelector::UpdateTyleInfo(QList<StyleGroup *>& stylegroup)
{
    qDebug() << "stylegroup:" <<stylegroup;
    int  styleindex = 0;
    for(auto  style: stylegroup)
    {
        listwidget->setIconSize(QSize(500,500));
        QListWidgetItem *google_item = new QListWidgetItem(QIcon(style->grab()), style->m_infomap[StyleGroup::Group_Style_Name].second);
//        QString  groupid = style->m_infomap[StyleGroup::Group_Style_ID].second;
        google_item->setData(Qt::UserRole+1,styleindex++);
        listwidget->addItem(google_item);
    }
}
void ConfigTypeSelector::UpdateTyleInfo(QList<StylePairLabel *>& stylepairlabel)
{
    qDebug() << "stylegroup:" <<stylepairlabel;
    int  styleindex = 0;
    for(auto  style: stylepairlabel)
    {
        listwidget->setIconSize(QSize(500,500));
        QListWidgetItem *google_item = new QListWidgetItem(QIcon(style->grab()), style->m_infomap[StylePairLabel::PairLabel_Style_Name].second);
//        QString  groupid = style->m_infomap[StyleGroup::Group_Style_ID].second;
        google_item->setData(Qt::UserRole+1,styleindex++);
        listwidget->addItem(google_item);
    }
}

void ConfigTypeSelector::UpdateTyleInfo(QList<StyleButton *>& stylebuttons)
{
    int  styleindex = 0;
    for(auto  style: stylebuttons)
    {
        listwidget->setIconSize(QSize(500,500));
        QListWidgetItem *google_item = new QListWidgetItem(QIcon(style->grab()), style->m_infomap[StyleButton::Button_Style_Name].second);
//        QString  groupid = style->m_infomap[StyleGroup::Group_Style_ID].second;
        google_item->setData(Qt::UserRole+1,styleindex++);
        listwidget->addItem(google_item);
    }
}

void ConfigTypeSelector::UpdateTyleInfo(QList<StyleCurve *>& stylebuttons)
{
    int  styleindex = 0;
    for(auto  style: stylebuttons)
    {
        listwidget->setIconSize(QSize(500,500));
        QListWidgetItem *google_item = new QListWidgetItem(QIcon(style->grab()), style->m_infomap[StyleCurve::Curve_Style_Name].second);
//        QString  groupid = style->m_infomap[StyleGroup::Group_Style_ID].second;
        google_item->setData(Qt::UserRole+1,styleindex++);
        listwidget->addItem(google_item);
    }
}

int ConfigTypeSelector::getResult()
{
    return m_selectindex;
}

ConfigNameSpaceEnd
