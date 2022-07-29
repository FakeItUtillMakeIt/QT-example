#ifndef PARAMSELECT_H
#define PARAMSELECT_H
#include <QDialog>
#include <QStandardItemModel>
#include <qmap.h>
#include <QModelIndex>
#include "../Model/DeviceParam.h"
#include "../Model/Command.h"
namespace Ui {
class ParamSelect;
}
class QStandardItem;
class AppCache;
class  ParamInterfaceStruct;

struct ParamWrapper
{
    QMap<int, QMap<QPair<int, QString>, QList<ParamInterfaceStruct* > > > filemap;
};


//选择参数的对话框类
class ParamSelect : public QDialog
{
    Q_OBJECT

public:
    explicit ParamSelect(QWidget *parent = 0);
    ~ParamSelect();
    void update_data(map<int, DeviceParam*>& m_allDeviceParam);
    void update_data(map<int, Command*>& m_allCommadPrt);

    void add_params(QStandardItem *groupnode, QList<ParamInterfaceStruct *>& paramlist);
    void add_groups(QStandardItem *filenode, QMap<QPair<int, QString>, QList<ParamInterfaceStruct* > >& groupmap);
    QString get_group_name(int groupid);
    int get_select_key() { return paramID; }
    void select_item_by_key(int key);
private slots:
    void treeItemChanged(QStandardItem *item);
private:
    Ui::ParamSelect *ui;
    QStandardItemModel* paramModel;
    AppCache* app = nullptr;
    QModelIndexList  itemlist;
    int paramID;
    QMap<int, QStandardItem*> KeyItemMap;
};

#endif // PARAMSELECT_H
