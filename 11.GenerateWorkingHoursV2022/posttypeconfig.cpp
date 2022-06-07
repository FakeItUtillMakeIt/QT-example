#include "posttypeconfig.h"

PostTypeConfig* PostTypeConfig::instance=nullptr;

PostTypeConfig::PostTypeConfig(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("各岗位工时占比配置");

    this->setFixedSize(QSize(640,480));

    QGridLayout* layout=new QGridLayout(this);
    labelInfo=new QLabel("各岗位工时占比");
    btnAdd=new QPushButton("新增");
    btnEdit=new QPushButton("编辑");
    btnRemove=new QPushButton("删除");
    stackWidget=new QStackedWidget;
    tableConfig=new QTableWidget(6,5);
    tableConfig->setFixedWidth(this->width());

    btnCancel=new QPushButton("取消");
    btnOK=new QPushButton("确定");
    btnOK->setStyleSheet("background-color:rgb(30,144,255);color:white");

    layout->addWidget(labelInfo,0,0);
    layout->addWidget(btnAdd,0,4);
    layout->addWidget(btnEdit,0,5);
    layout->addWidget(btnRemove,0,6);
    layout->addWidget(stackWidget,1,0,1,7);
    layout->addWidget(btnCancel,2,5);
    layout->addWidget(btnOK,2,6);

    configData<<"序号"<<"岗位类型"<<"研发占比"<<"生产占比"<<"管理占比"\
             <<"1"<<"研发岗位"<<"0.95"<<"0.05"<<"0"\
            <<"2"<<"生产岗位"<<"0"<<"1.0"<<"0"\
            <<"3"<<"质量岗位"<<"0"<<"0"<<"0.5"\
            <<"4"<<"采购岗位"<<"0"<<"0"<<"0.5"\
            <<"5"<<"其他岗位"<<"0"<<"0"<<"0";
    for (int r=0;r<6;r++) {
        tableConfig->setRowHeight(r,40);
        for (int c=0;c<5;c++) {
            QTableWidgetItem* item=new QTableWidgetItem(configData[r*5+c]);
            item->setFlags(Qt::NoItemFlags);
            tableConfig->setItem(r,c,item);
            tableConfig->setColumnWidth(c,120);
        }
    }
    tableConfig->verticalHeader()->hide();
    tableConfig->horizontalHeader()->hide();

    stackWidget->addWidget(tableConfig);

    connect(btnAdd,&QPushButton::clicked,this,&PostTypeConfig::addNewConfig);
    connect(btnEdit,&QPushButton::clicked,this,&PostTypeConfig::editConfig);
    connect(btnRemove,&QPushButton::clicked,this,&PostTypeConfig::deleteConfig);
    connect(btnCancel,&QPushButton::clicked,this,&PostTypeConfig::clickConfigCancle);
    connect(btnOK,&QPushButton::clicked,this,&PostTypeConfig::clickConfigOK);


}

void PostTypeConfig::editConfig(){

    for (int r=0;r<tableConfig->rowCount();r++) {
        for (int c=0;c<tableConfig->columnCount();c++) {
            tableConfig->item(r,c)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable);
        }
    }

}
void PostTypeConfig::addNewConfig(){

    tableConfig->insertRow(tableConfig->rowCount());

}
void PostTypeConfig::deleteConfig(){
    tableConfig->removeRow(tableConfig->currentRow());
}

void PostTypeConfig::clickConfigCancle(){

    this->hide();
    emit configCancle();
}

void PostTypeConfig::clickConfigOK(){

    this->hide();

    for (int r=1;r<tableConfig->rowCount();r++) {
        QVector<qreal> allRatio;
        for (int c=2;c<tableConfig->columnCount();c++) {
            allRatio.push_back(tableConfig->item(r,c)->text().toDouble());
        }
        postTypeConfigData[tableConfig->item(r,1)->text()]=allRatio;
    }

    emit configOK();
}
