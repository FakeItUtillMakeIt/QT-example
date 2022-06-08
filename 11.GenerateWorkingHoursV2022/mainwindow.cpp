/**/


#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initConnect();

    currentPage=CURRENT_PAGE::DING_CHECK_DATA;

    isEditFlag=false;
    isAssemableHoursFlag=false;
    isNewImportDingCheckFlag=false;

    dingCheckWorkDataPage();

    ui->cbxCompany->addItem(QString("火箭公司"));
    ui->cbxCompany->addItem(QString("电子公司"));
    ui->cbxCompany->addItem(QString("动力公司"));


    ui->btnExcelSave->setStyleSheet("background-color:rgb(30,144,255);"
                                    "color:white");
    ui->btnInputExcelData->setStyleSheet("background-color:rgb(30,144,255);"
                                         "color:white");
    ui->btnExcelDeleteCell->setStyleSheet("border-width: 1px;"
                                          "color: red;"
                                          "border-color: red;");

    postTyprCfgWidget=nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*初始化信号槽关系*/
void MainWindow::initConnect(){
    /*换页*/
    connect(ui->btnGenerateHours,&QPushButton::clicked,this,&MainWindow::projectHoursGeneratePage);
    connect(ui->btnDingData,&QPushButton::clicked,this,&MainWindow::dingCheckWorkDataPage);
    connect(ui->btnRatioHours,&QPushButton::clicked,this,&MainWindow::hoursRatioConfigPage);
    connect(ui->btnProjectCfg,&QPushButton::clicked,this,&MainWindow::projectDataConfigPage);

    /*组装工时*/
    connect(ui->btnAssembleHoursData,&QPushButton::clicked,this,&MainWindow::assemblWorkingHoursData);
    /*导出项目工时*/
    connect(ui->btnOutputPjctHours,&QPushButton::clicked,this,&MainWindow::exportProjectHours);

    /*配置岗位类型*/
    connect(ui->btnPostTypeCfg,&QPushButton::clicked,this,&MainWindow::configPostType);

    /*Excel操作*/
    connect(ui->btnInputExcelData,&QPushButton::clicked,this,&MainWindow::importDintCheckData);
    connect(ui->btnExcelSave,&QPushButton::clicked,this,&MainWindow::saveConfigInfo);
    connect(ui->btnExcelInsertCell,&QPushButton::clicked,this,&MainWindow::insertData);
    connect(ui->btnExcelEditCell,&QPushButton::clicked,this,&MainWindow::editData);
    connect(ui->btnExcelDeleteCell,&QPushButton::clicked,this,&MainWindow::deleteData);

}

/*项目工时生成页*/
void MainWindow::projectHoursGeneratePage(){
    isNewImportDingCheckFlag=false;
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

    ui->btnPostTypeCfg->hide();
    ui->btnInputExcelData->hide();
    ui->btnExcelSave->hide();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();

    pageButtonStateChange(CURRENT_PAGE::PROJECT_WORKINGHOURS_GENERATE);

    if(!isAssemableHoursFlag){
        ui->tableWidget->clear();
        return;

    }
    projectWorkingHoursGenerate();
    assemblWorkingHoursData();
    isEditFlag=false;

}

/*钉钉数据考勤页*/
void MainWindow::dingCheckWorkDataPage(){
    isNewImportDingCheckFlag=false;
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

    ui->btnPostTypeCfg->hide();
    ui->btnInputExcelData->show();
    ui->btnExcelSave->hide();
    ui->btnExcelInsertCell->hide();
    ui->btnExcelEditCell->hide();
    ui->btnExcelDeleteCell->hide();

    pageButtonStateChange(CURRENT_PAGE::DING_CHECK_DATA);


    if(dingCheckImportFilename.isNull()){
        ui->tableWidget->clear();
        return;
    }
    dingCheckImport();
    isEditFlag=false;

}

/*工时占比配置页*/
void MainWindow::hoursRatioConfigPage(){
    isNewImportDingCheckFlag=false;
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

    ui->btnPostTypeCfg->show();
    ui->btnInputExcelData->show();
    ui->btnExcelSave->show();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();

    pageButtonStateChange(CURRENT_PAGE::GENERATE_RATIO_CONFIG);

    if(hoursRatioConfigImportFilename.isNull()){
        ui->tableWidget->clear();
        return;
    }
    hoursRatioConfigImport();
    isEditFlag=false;

}

/*项目数据配置页*/
void MainWindow::projectDataConfigPage(){
    isNewImportDingCheckFlag=false;
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

    ui->btnPostTypeCfg->hide();
    ui->btnInputExcelData->show();
    ui->btnExcelSave->show();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();


    pageButtonStateChange(CURRENT_PAGE::PROJECT_DATA_CONFIG);

    if(projectConfigImportFilename.isNull()){
        ui->tableWidget->clear();
        return;
    }

    projectDataConfigImport();
    isEditFlag=false;

}

/*页面上按钮变化规则*/
void MainWindow::pageButtonStateChange(CURRENT_PAGE page){

    ui->btnGenerateHours->setFlat(false);
    ui->btnDingData->setFlat(false);
    ui->btnRatioHours->setFlat(false);
    ui->btnProjectCfg->setFlat(false);
    ui->btnGenerateHours->setDown(false);
    ui->btnDingData->setDown(false);
    ui->btnRatioHours->setDown(false);
    ui->btnProjectCfg->setDown(false);
    ui->btnGenerateHours->setStyleSheet("color:rgb(0,0,0)");
    ui->btnDingData->setStyleSheet("color:rgb(0,0,0)");
    ui->btnRatioHours->setStyleSheet("color:rgb(0,0,0)");
    ui->btnProjectCfg->setStyleSheet("color:rgb(0,0,0)");

    switch (page) {
        case CURRENT_PAGE::PROJECT_WORKINGHOURS_GENERATE:
        ui->btnGenerateHours->setFlat(true);
        ui->btnGenerateHours->setDown(true);
        ui->btnGenerateHours->setStyleSheet("color:rgb(30,144,255)");

        break;
        case CURRENT_PAGE::DING_CHECK_DATA:
        ui->btnDingData->setFlat(true);
        ui->btnDingData->setDown(true);
        ui->btnDingData->setStyleSheet("color:rgb(30,144,255)");
        break;
        case CURRENT_PAGE::GENERATE_RATIO_CONFIG:
        ui->btnRatioHours->setFlat(true);
        ui->btnRatioHours->setDown(true);
        ui->btnRatioHours->setStyleSheet("color:rgb(30,144,255)");
        break;
        case CURRENT_PAGE::PROJECT_DATA_CONFIG:
        ui->btnProjectCfg->setFlat(true);
        ui->btnProjectCfg->setDown(true);
        ui->btnProjectCfg->setStyleSheet("color:rgb(30,144,255)");
        break;
    }

}

/*钉钉考勤数据导入*/
void MainWindow::dingCheckImport(){
    if(dingCheckExcel==nullptr)
        dingCheckExcel=new QXlsx::Document(dingCheckImportFilename);
    qDebug()<<dingCheckExcel->dimension().rowCount()<<dingCheckExcel->dimension().columnCount();

    int rowC,columnC;

    rowC=dingCheckExcel->dimension().rowCount();
    columnC=dingCheckExcel->dimension().columnCount();

    if(rowC==1){
        qDebug()<<"查看Excel是否需要去掉表头多列合为一列的情况";
        return;
    }
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    QString datePattern("星期六|星期日");
    dateRegFormat.setPattern(datePattern);
    QString checkPattern("迟到");
    dingCheckRegFormat.setPattern(checkPattern);
    QString dimissionPattern("离职");
    dimissionRegFormat.setPattern(dimissionPattern);
    QString breakPattern("休息");
    breakDateRegFormat.setPattern(breakPattern);
    QString vacatePattern("请假");
    vacateRegFormat.setPattern(vacatePattern);


    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=dingCheckExcel->read(r+1,c+1);
            QString content=data.toString();

            auto item=new QTableWidgetItem(content,data.type());
            if(content.contains(dateRegFormat))
                item->setForeground(Qt::red);
            if(content.contains(dingCheckRegFormat))
                item->setBackground(QColor(120,255,0));
            if(content.contains(vacateRegFormat))
                item->setBackground(QColor(255,106,106));
            if(content.contains(breakDateRegFormat))
                item->setBackground(QColor(189,183,107));

            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }

    if(isNewImportDingCheckFlag){
        //导入之后呼出小窗，通知选定考勤年月
        QMessageBox::information(nullptr,"Info",QString("当前选定%1年%2月,\n确定要导入的文件名为:%3")
                                 .arg(ui->spinBoxYear->value())
                                 .arg(ui->spinBoxMonth->value())
                                 .arg(dingCheckImportFilename));

        /*杨晨光给的考勤表0608*/
        currentYear=dingCheckExcel->read(1,1).toString().split(QRegExp("[:：]"))[1].split(QRegExp("[/-]"))[0].toInt();
        currentMonth=dingCheckExcel->read(1,1).toString().split(QRegExp("[:：]"))[1].split(QRegExp("[/-]"))[1].toInt();

        //统计当前月人员的上班时间所在行
        int columnDepartment,columnJobNum,columnHumanName;
        for (int c=0;c<columnC;c++) {

            if(dingCheckExcel->read(3,c+1)==QString("部门"))
                columnDepartment=c+1;
            if(dingCheckExcel->read(3,c+1)==QString("工号"))
                columnJobNum=c+1;
            if(dingCheckExcel->read(3,c+1)==QString("姓名"))
                columnHumanName=c+1;

        }

        //qDebug()<<currentMonth<<currentYear;
        QVector<QString> daysOfMonth;
        for (int c=columnJobNum+1;c<=columnC;c++) {
            daysOfMonth.push_back(dingCheckExcel->read(4,c).toString());
        }


        for (int r=0;r<=rowC;r++) {
            QString content=dingCheckExcel->read(r+1,columnDepartment).toString();

            //空信息
            if(content.isNull())
                continue;
            //每个工号
            if(dingCheckExcel->read(r+1,columnJobNum).toString()==QString("工号"))
                continue;
            if(dingCheckExcel->read(r+1,columnJobNum).toString().isNull())
                continue;

            jobNumHumanInfo[dingCheckExcel->read(r+1,columnJobNum).toString()]=dingCheckExcel->read(r+1,columnHumanName).toString();

            for (int c=columnJobNum+1;c<=columnC;c++) {
                if(dingCheckExcel->read(r+1,c).toString()=="在岗")
                    jobNumWorkdayRowInfo[dingCheckExcel->read(r+1,columnJobNum).toString()].push_back(daysOfMonth[c-columnJobNum-1]);
            }
        }

        /*兵哥给的考勤表*/
//        //统计当前月人员的上班时间所在行
//        int columnBanci,columnDepartment,columnDate;
//        for (int c=0;c<columnC;c++) {
//            if(dingCheckExcel->read(1,c+1)==QString("班次"))
//                columnBanci=c+1;
//            if(dingCheckExcel->read(1,c+1)==QString("部门"))
//                columnDepartment=c+1;
//            if(dingCheckExcel->read(1,c+1)==QString("日期"))
//                columnDate=c+1;
//        }

//        auto dateList=dingCheckExcel->read(4,columnDate).toString().split(QRegExp("-"));
//        currentMonth=dateList[1].toInt();
//        currentYear=dateList[0].toInt();
//        qDebug()<<columnDate<<dateList<<currentMonth<<currentYear;

//        for (int r=0;r<=rowC;r++) {
//            QString content=dingCheckExcel->read(r+1,columnBanci).toString();
//            //请假
//            if(content.contains(vacateRegFormat))
//                continue;
//            //休息
//            if(content.contains(breakDateRegFormat))
//                continue;
//            //空信息
//            if(content.isNull())
//                continue;
//            //每个人
//            if(dingCheckExcel->read(r+1,1).toString()==QString("姓名"))
//                continue;
//            if(dingCheckExcel->read(r+1,1).toString().isNull())
//                continue;
//            humanWorkdayRowInfo[dingCheckExcel->read(r+1,1).toString()].push_back(r+1);

//        }


    }

}

/*工时占比配置导入*/
void MainWindow::hoursRatioConfigImport(){
    if(hourRatioExcel==nullptr)
        hourRatioExcel=new QXlsx::Document(hoursRatioConfigImportFilename);
    qDebug()<<hourRatioExcel->dimension().rowCount()<<hourRatioExcel->dimension().columnCount();
    int rowC,columnC;
    rowC=hourRatioExcel->dimension().rowCount();
    columnC=hourRatioExcel->dimension().columnCount();
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=hourRatioExcel->read(r+1,c+1);
            QString content=data.toString();
            auto item=new QTableWidgetItem(content,data.type());
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }

    //统计当前月人员的岗位类别和工时占比信息
    int columnJobNum,columnHumanName,columnLeaveDate;
    int columnCharClsfy,columnDepart1,columnDepart2;
    for (int c=0;c<columnC;c++) {
        if(hourRatioExcel->read(1,c+1).toString().contains("工号"))
            columnJobNum=c+1;
        if(hourRatioExcel->read(1,c+1).toString().contains("姓名"))
            columnHumanName=c+1;
        if(hourRatioExcel->read(1,c+1).toString().contains("离职时间"))
            columnLeaveDate=c+1;
        if(hourRatioExcel->read(1,c+1).toString().contains("工时人员性质"))
            columnCharClsfy=c+1;

        if(hourRatioExcel->read(1,c+1).toString().contains("BU"))
            columnDepart1=c+1;
        if(hourRatioExcel->read(1,c+1).toString().contains("二级部门"))
            columnDepart2=c+1;
    }
    for (int r=0;r<=rowC;r++) {

        if(hourRatioExcel->read(r+1,columnJobNum).toString()==QString("工号"))
            continue;
        if(hourRatioExcel->read(r+1,columnJobNum).toString().isNull())
            continue;
        //每个工号的性质信息
        jobNumPostTypeInfo[hourRatioExcel->read(r+1,columnJobNum).toString()]=hourRatioExcel->read(r+1,columnCharClsfy).toString();
        //每个工号的部门信息
        jobNumDepartmentInfo[hourRatioExcel->read(r+1,columnJobNum).toString()].push_back(hourRatioExcel->read(r+1,columnDepart1).toString());
        jobNumDepartmentInfo[hourRatioExcel->read(r+1,columnJobNum).toString()].push_back(hourRatioExcel->read(r+1,columnDepart2).toString());
        QString tmp=hourRatioExcel->read(r+1,columnLeaveDate).toString();
        if(tmp.contains(QRegExp("[0-9/]")))
        {
            auto leaveT=hourRatioExcel->read(r+1,columnLeaveDate).toString().split(QRegExp("[/-]"));
            jobNumLeaveTimeInfo[hourRatioExcel->read(r+1,columnJobNum).toString()]=QDate(leaveT[0].toInt(),leaveT[1].toInt(),leaveT[2].toInt());
        }
    }

}

/*项目配置数据导入*/
void MainWindow::projectDataConfigImport(){

    if(projectDataExcel==nullptr)
        projectDataExcel=new QXlsx::Document(projectConfigImportFilename);

    qDebug()<<projectDataExcel->dimension().rowCount()<<projectDataExcel->dimension().columnCount();

    int rowC,columnC;
    rowC=projectDataExcel->dimension().rowCount();
    columnC=projectDataExcel->dimension().columnCount();
    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=projectDataExcel->read(r+1,c+1);
            QString content=data.toString();
            auto item=new QTableWidgetItem(content,data.type());
            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }

    //统计项目和人员对应关系 项目和时间的关系
    int columnPrjName,columnDepartment,columnStartTime,columnEndTime,columnPrjType,columnPrjWorker;
    int columnPrjChar;
    for (int c=0;c<columnC;c++) {
        if(projectDataExcel->read(1,c+1).toString().contains("项目名称"))
            columnPrjName=c+1;
        if(projectDataExcel->read(1,c+1).toString().contains("二级部门"))
            columnDepartment=c+1;
        if(projectDataExcel->read(1,c+1).toString().contains("开始时间"))
            columnStartTime=c+1;
        if(projectDataExcel->read(1,c+1).toString().contains("结束时间"))
            columnEndTime=c+1;
        if(projectDataExcel->read(1,c+1).toString().contains("项目类型"))
            columnPrjType=c+1;
        if(projectDataExcel->read(1,c+1).toString().contains("项目属性"))
            columnPrjChar=c+1;
        if(projectDataExcel->read(1,c+1).toString().contains("项目成员"))
            columnPrjWorker=c+1;
    }
    for (int r=0;r<=rowC;r++) {
        //每个人的岗位
        if(projectDataExcel->read(r+1,columnPrjName).toString()==QString("项目名称") )
            continue;
        if(projectDataExcel->read(r+1,columnPrjName).toString().isNull())
            continue;
        projectTypeInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(projectDataExcel->read(r+1,columnPrjType).toString());
        projectTypeInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(projectDataExcel->read(r+1,columnPrjChar).toString());

        projectDepartmentInfo[projectDataExcel->read(r+1,columnPrjName).toString()]=projectDataExcel->read(r+1,columnDepartment).toString();

        projectDateInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(QDate::fromString(projectDataExcel->read(r+1,columnStartTime).toString(),"yyyyMMdd"));

        projectDateInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(QDate::fromString(projectDataExcel->read(r+1,columnEndTime).toString(),"yyyyMMdd"));

        auto workersList=projectDataExcel->read(r+1,columnPrjWorker).toString().split(QRegExp("[,，/、]"));
        foreach (QString worker, workersList) {
            projectHumansInfo[projectDataExcel->read(r+1,1).toString()].push_back(worker);
        }
    }

}


void MainWindow::projectWorkingHoursGenerate(){
    ui->tableWidget->clear();
}

/**/
void MainWindow::appendRowInAssemableTable(int rowNum,QVector<QString> dataList){
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    for (int c=0;c<dataList.count();c++) {
        ui->tableWidget->setItem(rowNum,c,new QTableWidgetItem(dataList[c]));
    }
}

//组装工时数据
void MainWindow::assemblWorkingHoursData(){

    if(dingCheckImportFilename.isNull() || hoursRatioConfigImportFilename.isNull() || projectConfigImportFilename.isNull())
        return;
    isAssemableHoursFlag=true;

    //将excel进行组装操作
    //1.应该以考勤表为基础添加当天的分配的项目类型，工时，
    //2.生成的工时文件需要和OPM输出的文件格式尽量保持一致
    //3.生成信息包括 姓名 日期 所属部门 一级部门 二级部门 岗位类别 项目名称 项目类型 工时占比
    //先生成当前月人员的项目信息
    calHumanCurrentMonthProjectInfo();
    //显示在tableWidget上
    int newRowNum=0;
    ui->tableWidget->setColumnCount(9);

    QVector<QString> rowData;
    rowData<<"工号"<<"姓名"<<"一级部门"<<"二级部门"<<"项目名称"<<"项目类型"<<"项目属性"<<QString("日期/号(%1)").arg(QString::number(currentYear)+"年"+QString::number(currentMonth)+"月")<<"工时占比";
    appendRowInAssemableTable(newRowNum,rowData);
    newRowNum++;
    //这个人的工号
    foreach(QString jobNumKey,jobNumWorkdayRowInfo.keys()){
        qDebug()<<jobNumKey;
        //这个人员当前月工作日工作

        foreach (QString humanWorkDay,jobNumWorkdayRowInfo[jobNumKey]) {

            //确定这个人当天的项目个数  <3
            //根据这个人的当月项目总数进行排
            int prjCount=0;
            if(jobNumCurrentMonthProjectInfo[jobNumKey].size()>3){
                prjCount=3;
            }
            else{
                prjCount=jobNumCurrentMonthProjectInfo[jobNumKey].size();
            }

            if(prjCount==0)
                continue;

            qDebug()<<"日期："+humanWorkDay+"项目个数:"+prjCount;
            for (int numPrj=0;numPrj<prjCount;numPrj++) {

                QVector<QString> rowTmp;
                //工时占比需要由配置的项目类型占比比例进行分配，分配原则为最小分配比例1%
                rowTmp<<jobNumKey<<jobNumHumanInfo[jobNumKey]<<jobNumDepartmentInfo[jobNumKey][0]<<jobNumDepartmentInfo[jobNumKey][1]
                      <<jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]<<projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0]
                     <<projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1]<<humanWorkDay<<"工时占比";
                appendRowInAssemableTable(newRowNum,rowTmp);
                newRowNum++;
            }
        }
    }

    qDebug()<<"共写入行："+QString::number(newRowNum);



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

    }


    QXlsx::Document xlsxExcel(saveFileName);
    int tableRowCount=ui->tableWidget->rowCount();
    int tableColumnCount=ui->tableWidget->columnCount();

    for (int r=1;r<=tableRowCount;r++) {
        for (int c=1;c<=tableColumnCount;c++) {
            if(ui->tableWidget->item(r-1,c-1)!=0){

                auto type= ui->tableWidget->item(r-1,c-1)->type();
                auto data=ui->tableWidget->item(r-1,c-1)->text();

                //qDebug()<<type<<data;
                xlsxExcel.write(r,c,data);
            }
        }
    }

    xlsxExcel.save();
    file.close();
}

//导入数据
void MainWindow::importDintCheckData(){
    qDebug()<<"导入数据";

    isEditFlag=false;
    QFileDialog* dialog=new QFileDialog(this);

    dialog->setFileMode(QFileDialog::ExistingFile);
    QString selectFile;

    selectFile=dialog->getOpenFileName(this,tr("打开文件"),"./",tr("Excel (*.xlsx)"));

    switch (currentPage) {

    case CURRENT_PAGE::DING_CHECK_DATA:
        isNewImportDingCheckFlag=true;
        dingCheckImportFilename=selectFile;
        dingCheckImport();
        break;
    case CURRENT_PAGE::GENERATE_RATIO_CONFIG:
        hoursRatioConfigImportFilename=selectFile;
        hoursRatioConfigImport();
        break;
    case CURRENT_PAGE::PROJECT_DATA_CONFIG:
        projectConfigImportFilename=selectFile;
        projectDataConfigImport();
        break;
    case CURRENT_PAGE::PROJECT_WORKINGHOURS_GENERATE:
        generateHoursImportFilename=selectFile;
        projectWorkingHoursGenerate();
        break;

    }

    qDebug()<<dingCheckImportFilename<<hoursRatioConfigImportFilename<<projectConfigImportFilename<<generateHoursImportFilename;

}



//保存配置信息
void MainWindow::saveConfigInfo(){

    qDebug()<<"保存数据";

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
    qDebug()<<"新增数据";

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
            //添加列，在当前选中列之后添加
        }
        else{
            //添加行,在当前选中行之后添加
        }
    }

}
//编辑数据
void MainWindow::editData(){
    qDebug()<<"编辑数据";

        setTableItemEditable(true);
        isEditFlag=true;
}
//删除数据
void MainWindow::deleteData(){
    qDebug()<<"删除数据";

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
            ui->tableWidget->item(r-1,c-1)->setTextAlignment(Qt::AlignCenter);
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

/*配置岗位类型*/
void MainWindow::configPostType(){
    if(postTyprCfgWidget==nullptr){

        postTyprCfgWidget=PostTypeConfig::getInstance();
        //岗位占比配置
        connect(postTyprCfgWidget,&PostTypeConfig::configOK,this,[=](){
            qDebug()<<"config UI";
            //第2 3，4,5列数据充当键值数据
            postTypeConfigData=postTyprCfgWidget->postTypeConfigData;
        });

    }

    postTyprCfgWidget->show();


}


/*计算人员当月项目信息*/
void MainWindow::calHumanCurrentMonthProjectInfo(){
    //必须先导入项目信息
    if(projectConfigImportFilename.isNull())
        return;


    //根据当前月，项目参与人员进行配置
    jobNumCurrentMonthProjectInfo.clear();
    //更新humanCurrentMonthProjectInfo
    //1.projectHumansInfo 项目-人名
    //2.humanWorkdayRowInfo.keys() 当前月的人名
    //3.必须确保当前月有该项目，项目周期和当前月的匹配

    int prjStartYear,prjEndYear,prjStartMonth,prjEndMonth;
    foreach (QString jobNum, jobNumWorkdayRowInfo.keys()) {
        foreach (QString prjName, projectHumansInfo.keys()) {
            prjStartYear=projectDateInfo[prjName][0].year();
            prjEndYear=projectDateInfo[prjName][1].year();
            prjStartMonth=projectDateInfo[prjName][0].month();
            prjEndMonth=projectDateInfo[prjName][1].month();
            if(currentYear>=prjStartYear && currentYear<=prjEndYear &&currentMonth>=prjStartMonth && currentMonth<=prjEndMonth){
                if(projectHumansInfo[prjName].contains(jobNumHumanInfo[jobNum])){
                    jobNumCurrentMonthProjectInfo[jobNum].push_back(prjName);
                }
            }
        }
    }

}
