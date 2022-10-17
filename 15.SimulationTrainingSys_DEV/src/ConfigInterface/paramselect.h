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
    void update_data(map<int, DeviceParam*>& m_allDeviceParam, QVector<int> oldparamid = {});
    void update_data(map<int, Command*>& m_allCommadPrt, QVector<int> oldparamid = {});
    void update_turn_data(map<int, DeviceParam*>& m_allDeviceParam, QVector<int> oldparamid = {});
    bool haveValidParam(QStandardItem* parent);
    void add_params(QStandardItem *groupnode, QList<ParamInterfaceStruct *>& paramlist);
    void searchItems();
    bool searchChildItems(QStandardItem* parenitem, QString text);
    bool searchChildItems_2(QStandardItem* parenitem, QString text);    
    bool showAllChildItems(QStandardItem* parenitem);
    void showAllItems();
    void add_groups(QStandardItem *filenode, QMap<QPair<int, QString>, QList<ParamInterfaceStruct* > >& groupmap);
    QString get_group_name(int groupid);
    int get_select_key() { return paramID; }
    QVector<int> get_multiselect_key() { return paramIDs; }

    void select_item_by_key(int key);
    void enableMultiSelect(bool  enabled) { multiselected = enabled; }
private slots:
    void treeItemChanged(QStandardItem *item);
private:
    Ui::ParamSelect *ui;
    QStandardItemModel* paramModel;
    AppCache* app = nullptr;
    QModelIndexList  itemlist;
    int paramID;
    QVector<int>  paramIDs;
    QMap<int, QStandardItem*> KeyItemMap;
    bool  multiselected = false;

};

#endif // PARAMSELECT_H
