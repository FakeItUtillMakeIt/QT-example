#include "paramselect.h"
#include "ui_paramselect.h"
#include <QStyleFactory>
#include <QStandardItemModel>
#include <QDebug>
#include "qmessagebox.h"
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
    for (auto it = m_allCommadPrt.begin(); it != m_allCommadPrt.end(); it++)
    {
        QStandardItem* paramnode = new QStandardItem(QString::fromLocal8Bit(it->second->m_sName.c_str()));
        paramnode->setData(it->second->m_id, Qt::UserRole + 1);
        paramModel->appendRow(paramnode);
        paramnode->setCheckable(true);
        
    }

}
void ParamSelect::update_data(map<int, DeviceParam*>& m_allDeviceParam)
{
    map<int, QStandardItem*> deviceItemMap;
    for (auto it = m_allDeviceParam.begin(); it != m_allDeviceParam.end(); it++)
    {
        QStandardItem* devicenode = nullptr;  //获取设备节点
        if (deviceItemMap.find(it->second->m_deviceId) != deviceItemMap.end())
        {
            devicenode = deviceItemMap[it->second->m_deviceId];
        }
        else
        {
            devicenode = new QStandardItem(QString::fromLocal8Bit(it->second->m_deviceName.c_str()));
            paramModel->appendRow(devicenode);
            deviceItemMap[it->second->m_deviceId] = devicenode;
        }
        //添加参数节点
        QStandardItem*  paramnode = new QStandardItem(QString::fromLocal8Bit(it->second->m_subParameterName.c_str()));
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
