#include "paramselect.h"
#include "ui_paramselect.h"
#include <QStyleFactory>
#include <QStandardItemModel>
#include <QDebug>
#include "qmessagebox.h"
#include "configglobal.h"
#pragma execution_character_set("utf-8")

ParamSelect::ParamSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamSelect)
{
    ui->setupUi(this);
    setWindowTitle("参数选择");
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setStyle(QStyleFactory::create("windows")); //该风格显示节点关系连接线
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    paramModel = new QStandardItemModel;
    connect (paramModel ,&QStandardItemModel::itemChanged , this ,&ParamSelect::treeItemChanged );
    ui->treeView->setModel(paramModel);

    connect(ui->btncancel, &QPushButton::clicked, [=]() {
        reject();
        });
    connect(ui->btnconfirm, &QPushButton::clicked, [=]() {
        if (itemlist.size() > 1)
        {
            QMessageBox::warning(nullptr, tr("错误"), tr("只能选中一个参数"));
            return;
        }
        if (itemlist.size() == 0)
        {
            QMessageBox::warning(nullptr, tr("错误"), tr("未选择参数"));
            return;
        }
        paramID = paramModel->itemFromIndex(itemlist.first())->data(Qt::UserRole + 1).toInt();
        accept();
        });

    connect(ui->search, &QPushButton::clicked, [=]() {
            
        searchItems();
    });
}

bool ParamSelect::showAllChildItems(QStandardItem* parenitem)
{
    for (int i = 0; i < parenitem->rowCount(); i++)
    {
        QStandardItem* item = parenitem->child(i);
        ui->treeView->setRowHidden(i, parenitem->index(), false);
        showAllChildItems(item);
    }
    return true;
}
void ParamSelect::showAllItems()
{
    for (int i = 0; i < paramModel->rowCount(); i++)
    {
        QStandardItem* item = paramModel->item(i);
        ui->treeView->setRowHidden(i, item->index().parent(), false);
        showAllChildItems(item);
    }
}
bool ParamSelect::searchChildItems(QStandardItem* parenitem,QString text)
{
    if (parenitem->rowCount() == 0)
    {
        return false;
    }
    int rowcount = parenitem->rowCount();
    for (int i = 0; i < rowcount; i++)
    {
        QStandardItem* item = parenitem->child(i);
        QString itemtext = item->text();
        if (itemtext.contains(text))
        {
            if (item->rowCount() > 0)
            {
                searchChildItems(item, text);
                ui->treeView->expand(item->index());
            }
            return true;
        }
        else
        {
            bool searchresult = searchChildItems(item, text);
            if (searchresult)
                return true;           
        }
    }
    return false;
}

bool ParamSelect::searchChildItems_2(QStandardItem* parenitem, QString text)
{
    if (parenitem->rowCount() == 0)
    {
        return false;
    }
    int rowcount = parenitem->rowCount();
    bool  findconspondseItem = false;
    for (int i = 0; i < rowcount; i++)
    {
        QStandardItem* item = parenitem->child(i);
        QString itemtext = item->text();
        if (itemtext.contains(text))
        {
            if (item->rowCount() > 0)
            {
                searchChildItems(item, text);
                ui->treeView->expand(item->index());
            }
            findconspondseItem = true;           
        }
        else
        {
            bool searchresult = searchChildItems_2(item, text);
            if (searchresult)
            {
                findconspondseItem = true;
                ui->treeView->expand(item->index());
            }
            else
            {
                ui->treeView->setRowHidden(i,parenitem->index(),true);
            }
        }
    }
    return findconspondseItem;
}


void ParamSelect::searchItems()
{
    showAllItems();
    QString serchtext = ui->searchContent->text();
    if (serchtext.isEmpty())
    {
        return;
    }
    for (int i = 0; i < paramModel->rowCount(); i++)
    {
        QStandardItem* item = paramModel->item(i);
        if (item->text().contains(serchtext))
        {
            if (item->rowCount() > 0)
            {
                if(searchChildItems(item, serchtext))
                  ui->treeView->expand(item->index());
            }
        }
        else
        {
            bool find = searchChildItems_2(item, serchtext);
            if(!find)
              ui->treeView->setRowHidden(i, item->index().parent(), true);
            if (find)
                ui->treeView->expand(item->index());
        }
    }
}

void ParamSelect::add_params(QStandardItem* groupnode,QList<ParamInterfaceStruct *>&  paramlist)
{
  /*  for (auto param : paramlist)
    {
        QStandardItem* paramnode = new QStandardItem(QString::fromStdString(param->value->m_strName));
        paramnode->setData(param->key, Qt::UserRole + 1);
        groupnode->appendRow(paramnode);
        paramnode->setCheckable(true);       
        KeyItemMap[param->key] = paramnode;
    }*/
}
void ParamSelect::select_item_by_key(int key)
{
    if (KeyItemMap.contains(key))
    {
       QStandardItem* item =    KeyItemMap[key];
       item->setCheckState(Qt::Checked);
       QStandardItem* parentitem =  item->parent();
       while (parentitem)
       {
           ui->treeView->expand(parentitem->index());
           parentitem = parentitem->parent();
       }
    }
    else
    {
       if(itemlist.size() > 0)
        {
            QStandardItem* item = paramModel->itemFromIndex(itemlist.first());
            item->setCheckState(Qt::Unchecked);
            QStandardItem* parentitem = item->parent();
            while (parentitem)
            {
                ui->treeView->collapse(parentitem->index());
                parentitem = parentitem->parent();
            }
            ui->treeView->expand(paramModel->item(0,0)->index());
        }
    }
}

QString ParamSelect::get_group_name(int groupid)
{
   /* for (auto it : app->m_vDataFlow->m_groups)
    {
        if (it->m_id == groupid)
        {
            return QString::fromStdString(it->m_name);
        }
    }*/
    return "none";
}
void ParamSelect::add_groups(QStandardItem* filenode,QMap<QPair<int, QString>, QList<ParamInterfaceStruct *> >& groupmap)
{
    for (auto it = groupmap.begin(); it != groupmap.end(); it++)
    {
       QStandardItem* groupnode = new QStandardItem(it.key().second);
       filenode->appendRow(groupnode);
       if (it.value().size() > 0)
          add_params(groupnode, it.value());
    }
}
ParamSelect::~ParamSelect()
{
    delete ui;
}
void ParamSelect::update_data(map<int, Command*>& m_allCommadPrt)
{
    map<int, QStandardItem*> tableItemMap;
    int rocketid = ConfigNameSpace::ConfigGlobal::currentRocketID;
    for (auto it = m_allCommadPrt.begin(); it != m_allCommadPrt.end(); it++)
    {
        if (it->second->m_iRocketId != rocketid || it->second->m_iType != 1)
            continue;

        if (it->second->m_tableId != -1)
        {
            QStandardItem* tabblenode = nullptr;  //获取参数表节点
            if (tableItemMap.find(it->second->m_tableId) != tableItemMap.end())
            {
                tabblenode = tableItemMap[it->second->m_tableId];
            }
            else
            {
                tabblenode = new QStandardItem(QString::fromLocal8Bit(it->second->m_tableName.c_str()) + "(参数表)");
                paramModel->appendRow(tabblenode);
                tableItemMap[it->second->m_tableId] = tabblenode;
            }
            QStandardItem* paramnode = new QStandardItem(QString::fromLocal8Bit(it->second->m_sName.c_str()));
            paramnode->setData(it->second->m_id, Qt::UserRole + 1);
            tabblenode->appendRow(paramnode);
            paramnode->setCheckable(true);
        }
        else
        {
            QStandardItem* paramnode = new QStandardItem(QString::fromLocal8Bit(it->second->m_sName.c_str()));
            paramnode->setData(it->second->m_id, Qt::UserRole + 1);
            paramModel->appendRow(paramnode);
            paramnode->setCheckable(true);

        }
       
        
    }

}
//void ParamSelect::update_data(map<int, DeviceParam*>& m_allDeviceParam)
//{
//    map<int, QStandardItem*> deviceItemMap;
//    map<int, QStandardItem*> tableItemMap;
//    int rocketid = ConfigNameSpace::ConfigGlobal::currentRocketID;
//    for (auto it = m_allDeviceParam.begin(); it != m_allDeviceParam.end(); it++)
//    {
//        if (it->second->m_rockcketid != rocketid)
//            continue;
//        QStandardItem* devicenode = nullptr;  //获取设备节点
//        if (deviceItemMap.find(it->second->m_deviceId) != deviceItemMap.end())
//        {
//            devicenode = deviceItemMap[it->second->m_deviceId];
//        }
//        else
//        {
//            devicenode = new QStandardItem(QString::fromLocal8Bit(it->second->m_deviceName.c_str()));
//            paramModel->appendRow(devicenode);
//            deviceItemMap[it->second->m_deviceId] = devicenode;
//        }
//        //添加参数节点
//        QStandardItem*  paramnode = new QStandardItem(QString::fromLocal8Bit(it->second->m_subParameterName.c_str()));
//        paramnode->setData(it->second->m_id, Qt::UserRole + 1);
//        devicenode->appendRow(paramnode);
//        paramnode->setCheckable(true);
//    }
//
//}

void ParamSelect::update_data(map<int, DeviceParam*>& m_allDeviceParam)
{
    map<int, QStandardItem*> deviceItemMap;
    map<int, QStandardItem*> tableItemMap;
    map<int, QList<int>> tabledevicemap;

    int rocketid = ConfigNameSpace::ConfigGlobal::currentRocketID;
    for (auto it = m_allDeviceParam.begin(); it != m_allDeviceParam.end(); it++)
    {
        if (it->second->m_rockcketid != rocketid)
            continue;


        QStandardItem* devicenode = nullptr;  //获取设备节点
        if (deviceItemMap.find(it->second->m_deviceId) != deviceItemMap.end())
        {
            devicenode = deviceItemMap[it->second->m_deviceId];
        }
        else
        {
            devicenode = new QStandardItem(QString::fromLocal8Bit(it->second->m_deviceName.c_str()) + "(设备)");
            if(it->second->m_tableId == -1)
              paramModel->appendRow(devicenode);
            deviceItemMap[it->second->m_deviceId] = devicenode;
        }
        if (it->second->m_tableId != -1)
        {
            QStandardItem* tabblenode = nullptr;  //获取参数表节点
            if (tableItemMap.find(it->second->m_tableId) != tableItemMap.end())
            {
                tabblenode = tableItemMap[it->second->m_tableId];
            }
            else
            {
                tabblenode = new QStandardItem(QString::fromLocal8Bit(it->second->m_tableName.c_str()) + "(参数表)");
                paramModel->appendRow(tabblenode);
                tableItemMap[it->second->m_tableId] = tabblenode;
            }
            if(tabledevicemap[it->second->m_tableId].contains(it->second->m_deviceId) == false)
            {
                tabblenode->appendRow(devicenode);
                tabledevicemap[it->second->m_tableId].push_back(it->second->m_deviceId);
            }
        }       

        //添加参数节点
        QStandardItem* paramnode = new QStandardItem(QString::fromLocal8Bit(it->second->m_subParameterName.c_str()));
        paramnode->setData(it->second->m_id, Qt::UserRole + 1);
        devicenode->appendRow(paramnode);
        paramnode->setCheckable(true);
    }
}

void ParamSelect::treeItemChanged(QStandardItem *item)
{
    if ( item == nullptr )
       return;

    if ( item->isCheckable ())
    {
       Qt::CheckState state = item->checkState (); //获取当前的选择状态
        int  paramKey = item->data(Qt::UserRole+1).toInt();
       if(state == Qt::Checked)
       {
           for(auto iIndex:itemlist )
           {
              item->model()->itemFromIndex(iIndex)->setCheckState(Qt::Unchecked);
           }
           itemlist.clear();
           itemlist << item->index();
       }
       else
       {
            itemlist.clear();
       }
    }
}
