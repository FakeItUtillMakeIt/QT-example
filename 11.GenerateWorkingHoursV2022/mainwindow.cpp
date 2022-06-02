#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    columnName=columnStartName+currentColumnNum;
    elementLoc=QString(columnName)+QString::number(currentRowNum);

    //当前数据表的数据维度
    QXlsx::Document xlsxDoc("xxx.xlsx");
    xlsxDoc.dimension().rowCount();
    //QCalendar日历

    currentPage=CURRENT_PAGE::DING_CHECK_DATA;

    isEditFlag=false;
    dingCheckWorkDataPage();
    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect(){
    /*换页*/
    connect(ui->btnGenerateHours,&QPushButton::clicked,this,&MainWindow::projectHoursGeneratePage);
    connect(ui->btnDingData,&QPushButton::clicked,this,&MainWindow::dingCheckWorkDataPage);
    connect(ui->btnRatioHours,&QPushButton::clicked,this,&MainWindow::hoursRatioConfigPage);
    connect(ui->btnProjectCfg,&QPushButton::clicked,this,&MainWindow::projectDataConfigPage);

    /*导出项目工时*/
    connect(ui->btnOutputPjctHours,&QPushButton::clicked,this,&MainWindow::exportProjectHours);

    /*Excel操作*/
    connect(ui->btnInputExcelData,&QPushButton::clicked,this,&MainWindow::importDintCheckData);
    connect(ui->btnExcelSave,&QPushButton::clicked,this,&MainWindow::saveConfigInfo);
    connect(ui->btnExcelInsertCell,&QPushButton::clicked,this,&MainWindow::insertData);
    connect(ui->btnExcelEditCell,&QPushButton::clicked,this,&MainWindow::editData);
    connect(ui->btnExcelDeleteCell,&QPushButton::clicked,this,&MainWindow::deleteData);
}


void MainWindow::projectHoursGeneratePage(){
    currentPage=CURRENT_PAGE::PROJECT_WORKINGHOURS_GENERATE;

    ui->spinBoxYear->show();
    ui->labelYear->show();
    ui->spinBoxMonth->show();
    ui->labelMonth->show();
    ui->cbxProjectClsfy->show();
    ui->cbxDepartment->show();
    ui->cbxCompany->show();
    ui->btnAssembleHoursData->show();
    ui->btnOutputPjctHours->show();

    ui->btnSaveData->hide();

    ui->btnInputExcelData->hide();
    ui->btnExcelSave->hide();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();

    if(generateHoursImportFilename.isNull()){
        ui->tableWidget->clear();
        return;

    }
    projectWorkingHoursGenerate();
    isEditFlag=false;

}
void MainWindow::dingCheckWorkDataPage(){
    currentPage=CURRENT_PAGE::DING_CHECK_DATA;

    ui->spinBoxYear->show();
    ui->labelYear->show();
    ui->spinBoxMonth->show();
    ui->labelMonth->show();
    ui->cbxProjectClsfy->hide();
    ui->cbxDepartment->show();
    ui->cbxCompany->show();
    ui->btnAssembleHoursData->hide();
    ui->btnOutputPjctHours->hide();

    ui->btnSaveData->show();

    ui->btnInputExcelData->show();
    ui->btnExcelSave->hide();
    ui->btnExcelInsertCell->hide();
    ui->btnExcelEditCell->hide();
    ui->btnExcelDeleteCell->hide();

    if(dingCheckImportFilename.isNull()){
        ui->tableWidget->clear();
        return;
    }
    dingCheckImport();
    isEditFlag=false;
}
void MainWindow::hoursRatioConfigPage(){
    currentPage=CURRENT_PAGE::GENERATE_RATIO_CONFIG;

    ui->spinBoxYear->hide();
    ui->labelYear->hide();
    ui->spinBoxMonth->hide();
    ui->labelMonth->hide();
    ui->cbxProjectClsfy->hide();
    ui->cbxDepartment->show();
    ui->cbxCompany->show();
    ui->btnAssembleHoursData->hide();
    ui->btnOutputPjctHours->hide();

    ui->btnSaveData->hide();

    ui->btnInputExcelData->show();
    ui->btnExcelSave->show();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();

    if(hoursRatioConfigImportFilename.isNull()){
        ui->tableWidget->clear();
        return;
    }
    hoursRatioConfigImport();
    isEditFlag=false;
}
void MainWindow::projectDataConfigPage(){
    currentPage=CURRENT_PAGE::PROJECT_DATA_CONFIG;

    ui->spinBoxYear->hide();
    ui->labelYear->hide();
    ui->spinBoxMonth->hide();
    ui->labelMonth->hide();
    ui->cbxProjectClsfy->hide();
    ui->cbxDepartment->show();
    ui->cbxCompany->show();
    ui->btnAssembleHoursData->hide();
    ui->btnOutputPjctHours->hide();

    ui->btnSaveData->hide();

    ui->btnInputExcelData->show();
    ui->btnExcelSave->show();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();

    if(projectConfigImportFilename.isNull()){
        ui->tableWidget->clear();
        return;
    }

    projectDataConfigImport();
    isEditFlag=false;
}

void MainWindow::dingCheckImport(){
    QXlsx::Document xlsxDoc(dingCheckImportFilename);
    qDebug()<<xlsxDoc.dimension().rowCount()<<xlsxDoc.dimension().columnCount();
    int rowC,columnC;
    rowC=xlsxDoc.dimension().rowCount();
    columnC=xlsxDoc.dimension().columnCount();
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=xlsxDoc.read(r+1,c+1);
            QString content=data.toString();
            auto item=new QTableWidgetItem(content,data.type());
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }
}

/*工时占比配置导入*/
void MainWindow::hoursRatioConfigImport(){
    QXlsx::Document xlsxDoc(hoursRatioConfigImportFilename);
    qDebug()<<xlsxDoc.dimension().rowCount()<<xlsxDoc.dimension().columnCount();
    int rowC,columnC;
    rowC=xlsxDoc.dimension().rowCount();
    columnC=xlsxDoc.dimension().columnCount();
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=xlsxDoc.read(r+1,c+1);
            QString content=data.toString();
            auto item=new QTableWidgetItem(content,data.type());
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }
}
void MainWindow::projectDataConfigImport(){
    QXlsx::Document xlsxDoc(projectConfigImportFilename);
    qDebug()<<xlsxDoc.dimension().rowCount()<<xlsxDoc.dimension().columnCount();
    int rowC,columnC;
    rowC=xlsxDoc.dimension().rowCount();
    columnC=xlsxDoc.dimension().columnCount();
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=xlsxDoc.read(r+1,c+1);
            QString content=data.toString();
            auto item=new QTableWidgetItem(content,data.type());
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }
}
void MainWindow::projectWorkingHoursGenerate(){
    ui->tableWidget->clear();
}


//组装工时数据
void MainWindow::assemblWorkingHoursData(){
    //将excel进行操作
}

//导出项目工时
void MainWindow::exportProjectHours(){
    //输出excel
    QFileDialog dialog;
    //dialog->setNameFilter(tr("save data (*.xlsx)"));

    QString saveFileName=dialog.getSaveFileName(this,tr("保存文件"),"./",tr("Excel (*.xlsx)"));

    QFile file(saveFileName);
    if(!file.exists())
    {
        qDebug()<<"not file";
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    QXlsx::Document xlsxExcel(saveFileName);
    int tableRowCount=ui->tableWidget->rowCount();
    int tableColumnCount=ui->tableWidget->columnCount();

    for (int r=1;r<=tableRowCount;r++) {
        for (int c=1;c<=tableColumnCount;c++) {
            auto type= ui->tableWidget->item(r-1,c-1)->type();
            auto data=ui->tableWidget->item(r-1,c-1)->text();
            qDebug()<<type<<data;
            switch(type)
            {
            case 6:
                xlsxExcel.write(r,c,data.toInt());
                break;
            case 10:
                xlsxExcel.write(r,c,data);
                break;
            }
        }
    }

    xlsxExcel.save();
}

//导入数据
void MainWindow::importDintCheckData(){
    QFileDialog* dialog=new QFileDialog(this);

    dialog->setFileMode(QFileDialog::ExistingFile);
    QString selectFile;

    selectFile=dialog->getOpenFileName(this,tr("打开文件"),"./",tr("Excel (*.xlsx)"));

    switch (currentPage) {

    case CURRENT_PAGE::DING_CHECK_DATA:
        dingCheckImportFilename=selectFile;
        break;
    case CURRENT_PAGE::GENERATE_RATIO_CONFIG:
        hoursRatioConfigImportFilename=selectFile;
        break;
    case CURRENT_PAGE::PROJECT_DATA_CONFIG:
        projectConfigImportFilename=selectFile;
        break;
    case CURRENT_PAGE::PROJECT_WORKINGHOURS_GENERATE:
        generateHoursImportFilename=selectFile;
        break;

    }

    qDebug()<<dingCheckImportFilename<<hoursRatioConfigImportFilename<<projectConfigImportFilename<<generateHoursImportFilename;

    QXlsx::Document xlsxDoc(selectFile);
    qDebug()<<xlsxDoc.dimension().rowCount()<<xlsxDoc.dimension().columnCount();
    int rowC,columnC;
    rowC=xlsxDoc.dimension().rowCount();
    columnC=xlsxDoc.dimension().columnCount();
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=xlsxDoc.read(r+1,c+1);
            QString content=data.toString();
            auto item=new QTableWidgetItem(content,data.type());
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }

}



//保存配置信息
void MainWindow::saveConfigInfo(){

    QString saveFileName;
    switch (currentPage) {
    case CURRENT_PAGE::GENERATE_RATIO_CONFIG:
        saveFileName=hoursRatioConfigImportFilename;
        break;
    case CURRENT_PAGE::PROJECT_DATA_CONFIG:
        saveFileName=projectConfigImportFilename;
        break;
    default:
        break;
    }

    saveExcelData(saveFileName);

}
//新增数据
void MainWindow::insertData(){
    if(!isEditFlag)
        return;

    int currentSelectRow=ui->tableWidget->currentRow();
    int currentSelectColumn=ui->tableWidget->currentColumn();
    if(currentSelectRow==-1 && currentSelectColumn==-1){
        //在最后添加
        int insertRow=ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(insertRow);
        for (int c=0;c<ui->tableWidget->columnCount();c++) {
            ui->tableWidget->setItem(insertRow,c,new QTableWidgetItem(""));
        }
        qDebug()<<"insert a row in back" ;
    }
    else
    {
        //判断添加行还是列
        if(currentSelectRow==0){
            //添加列
        }
        else{
            //添加行
        }
    }

}
//编辑数据
void MainWindow::editData(){
        setTableItemEditable(true);
        isEditFlag=true;
}
//删除数据
void MainWindow::deleteData(){

    if(!isEditFlag)
        return;

    int currentSelectRow=ui->tableWidget->currentRow();
    int currentSelectColumn=ui->tableWidget->currentColumn();
    qDebug()<<currentSelectRow<<currentSelectColumn;
    //判断删除行或列
    if(currentSelectRow==0){
        //删除列
    }
    else{
        //删除行
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }

}

//TableWidgetItem是否可编辑
void MainWindow::setTableItemEditable(bool editFlag){
    Qt::ItemFlags itemF;
    if(editFlag)
        itemF=Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable;
    else
        itemF=Qt::NoItemFlags;

    int currentRow=ui->tableWidget->rowCount();
    int currentColumn=ui->tableWidget->columnCount();
    for (int r=0;r<currentRow;r++) {
        for (int c=0;c<currentColumn;c++) {
            try {
                ui->tableWidget->item(r,c)->setFlags(itemF);
                }
            catch (QException) {

                }

        }
    }
}

//保存Excel信息
void MainWindow::saveExcelData(QString saveFileName){
    if(saveFileName.isNull())
    {
        qDebug()<<"not import a excel file";
        return;
    }
    QXlsx::Document xlsxExcel(saveFileName);
    int tableRowCount=ui->tableWidget->rowCount();
    int tableColumnCount=ui->tableWidget->columnCount();

    for (int r=1;r<=tableRowCount;r++) {
        for (int c=1;c<=tableColumnCount;c++) {
//            int type=0;
//            try {
//                type= ui->tableWidget->item(r-1,c-1)->type();

//                } catch (_exception) {
//                    qDebug()<<"write null";
//                }

            auto data=ui->tableWidget->item(r-1,c-1)->text();
            xlsxExcel.write(r,c,data);
//            qDebug()<<type<<data;
//            switch(type)
//            {
//            case 6:
//                xlsxExcel.write(r,c,data.toInt());
//                break;
//            case 10:
//                xlsxExcel.write(r,c,data);
//                break;
//            default:
//                xlsxExcel.write(r,c,"");
//                qDebug()<<"typy:0";
//            }
        }
    }

    xlsxExcel.save();

    qDebug()<<"save excel successfully";
    setTableItemEditable(false);
}
