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
    tableConfig=new QTableWidget(6,7);
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

    configData<<"序号"<<"岗位归类"<<"自研项目占比"<<"外接技术类项目占比"<<"外接产品类项目占比"<<"其他占比"<<"说明"\
             <<"1"<<"研发岗位"<<"0.8-1.0"<<"0-0.2"<<"0-0.1"<<"0"<<"单个外接技术类项目不超过0.2,所有外接类项目不超过0.2"\
            <<"2"<<"生产岗位"<<"0"<<"0"<<"随机"<<"倒算"<<"在外接产品和管理中分配，无比例限制"\
            <<"3"<<"质量岗位"<<"0"<<"0-0.5"<<"0-0.6"<<"倒算"<<"合计外接不超过0.6"\
            <<"4"<<"采购岗位"<<"0"<<"0"<<"随机"<<"倒算"<<"在外接生产和管理中分配，无比例限制"\
            <<"5"<<"其他岗位"<<"0"<<"0"<<"0"<<"0"<<"不分配工时";
    columnPrjTypeName[2]=QString("自研");
    columnPrjTypeName[3]=QString("外接技术");
    columnPrjTypeName[4]=QString("外接产品");
    columnPrjTypeName[5]=QString("其他");

    for (int r=0;r<6;r++) {
        tableConfig->setRowHeight(r,40);
        for (int c=0;c<7;c++) {
            QTableWidgetItem* item=new QTableWidgetItem(configData[r*7+c]);
            item->setFlags(Qt::NoItemFlags);
            tableConfig->setItem(r,c,item);
            tableConfig->setColumnWidth(c,85);
        }
    }
    tableConfig->verticalHeader()->hide();
    tableConfig->horizontalHeader()->hide();

    stackWidget->addWidget(tableConfig);


    QRandomGenerator rdGen;
    for (int r=1;r<tableConfig->rowCount();r++) {
        double ratio=0;
        double sumRatio=0;
        double daosuan=0;
        double restRatio=100;
        for (int c=2;c<tableConfig->columnCount()-1;c++) {
            if(tableConfig->item(r,c)->text().contains("-")){
                int bottom=tableConfig->item(r,c)->text().split(QRegExp("-"))[0].toDouble()*100;
                int top=tableConfig->item(r,c)->text().split(QRegExp("-"))[1].toDouble()*100;
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(bottom);
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(top);
                continue;
            }
            if(tableConfig->item(r,c)->text().contains("随机")){
                rdGen.seed(restRatio);
                ratio=rdGen.bounded(0,restRatio);
                sumRatio+=ratio;
                restRatio-=ratio;
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(ratio);
                continue;
            }
            if(tableConfig->item(r,c)->text().contains("倒算")){
                daosuan=100-sumRatio;
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(daosuan);
                continue;
            }
            double a=tableConfig->item(r,c)->text().toDouble()*100;
            postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(a);
            restRatio-=a;
            sumRatio+=a;
        }

    }

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


    QRandomGenerator rdGen;
    for (int r=1;r<tableConfig->rowCount();r++) {
        double ratio=0;
        double sumRatio=0;
        double daosuan=0;
        double restRatio=100;
        for (int c=2;c<tableConfig->columnCount()-1;c++) {
            if(tableConfig->item(r,c)->text().contains("-")){
                int bottom=tableConfig->item(r,c)->text().split(QRegExp("-"))[0].toDouble()*100;
                int top=tableConfig->item(r,c)->text().split(QRegExp("-"))[1].toDouble()*100;
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(bottom);
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(top);
                continue;
            }
            if(tableConfig->item(r,c)->text().contains("随机")){
                ratio=rdGen.bounded(0,restRatio);
                sumRatio+=ratio;
                restRatio-=ratio;
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(ratio);
                continue;
            }
            if(tableConfig->item(r,c)->text().contains("倒算")){
                daosuan=100-sumRatio;
                postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(daosuan);
                continue;
            }
            double a=tableConfig->item(r,c)->text().toDouble()*100;
            postTypeConfigData[tableConfig->item(r,1)->text()][columnPrjTypeName[c]].push_back(a);
            restRatio-=a;
            sumRatio+=a;
        }

    }

    emit configOK();
}
