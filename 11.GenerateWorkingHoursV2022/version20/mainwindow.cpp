﻿/*written by lijin*/


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
    postTyprCfgWidget=PostTypeConfig::getInstance();
    postTypeConfigData=postTyprCfgWidget->postTypeConfigData;
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
    /*多个文件汇总页*/
    connect(ui->btnDataClc,&QPushButton::clicked,this,&MainWindow::projectDataCalcPage);

    /*组装工时*/
    connect(ui->btnAssembleHoursData,&QPushButton::clicked,this,&MainWindow::assemblWorkingHoursData);
    connect(ui->btnAssembleHoursMonthData,&QPushButton::clicked,this,&MainWindow::assemblWorkingHoursMonthData);
    /*导出项目工时*/
    connect(ui->btnOutputPjctHours,&QPushButton::clicked,this,&MainWindow::exportProjectHours);

    /*配置岗位占比类型*/
    connect(ui->btnPostTypeCfg,&QPushButton::clicked,this,&MainWindow::configPostType);

    /*选定多个文件*/
    connect(ui->btnSelectMultiFile,&QPushButton::clicked,this,&MainWindow::selectMutliFile);

    /*Excel操作*/
    connect(ui->btnInputExcelData,&QPushButton::clicked,this,&MainWindow::importDintCheckData);
    connect(ui->btnExcelSave,&QPushButton::clicked,this,&MainWindow::saveConfigInfo);
    connect(ui->btnExcelInsertCell,&QPushButton::clicked,this,&MainWindow::insertData);
    connect(ui->btnExcelEditCell,&QPushButton::clicked,this,&MainWindow::editData);
    connect(ui->btnExcelDeleteCell,&QPushButton::clicked,this,&MainWindow::deleteData);

    connect(ui->btnClearData,&QPushButton::clicked,this,&MainWindow::clearHoursData);

}

//清空工时数据
void MainWindow::clearHoursData(){
    judgeWorkdatHoursException();

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
    ui->btnClearData->show();
    //ui->btnAssembleHoursMonthData->show();
    ui->btnOutputPjctHours->show();

    ui->btnSaveData->hide();

    ui->btnPostTypeCfg->hide();
    ui->btnInputExcelData->hide();
    ui->btnExcelSave->hide();
    ui->btnExcelInsertCell->hide();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();
    ui->btnSelectMultiFile->hide();

    pageButtonStateChange(CURRENT_PAGE::PROJECT_WORKINGHOURS_GENERATE);

    if(!isAssemableHoursFlag){
        ui->tableWidget->clear();

        return;

    }
    projectWorkingHoursGenerate();
    if(isDayHourGFlag)

        assemblWorkingHoursData();
    else
        assemblWorkingHoursMonthData();
    isEditFlag=false;

}

/*钉钉数据考勤页*/
void MainWindow::dingCheckWorkDataPage(){
    haveDayHoursDataFlag=false;
    isNewImportDingCheckFlag=false;
    currentPage=CURRENT_PAGE::DING_CHECK_DATA;

    ui->spinBoxYear->show();
    ui->labelYear->show();
    ui->spinBoxMonth->show();
    ui->labelMonth->show();
    ui->cbxProjectClsfy->hide();
    ui->cbxDepartment->show();
    ui->cbxCompany->show();
    ui->btnClearData->hide();
    ui->btnAssembleHoursData->hide();
    ui->btnAssembleHoursMonthData->hide();
    ui->btnOutputPjctHours->hide();

    ui->btnSaveData->show();

    ui->btnPostTypeCfg->hide();
    ui->btnInputExcelData->show();
    ui->btnExcelSave->hide();
    ui->btnExcelInsertCell->hide();
    ui->btnExcelEditCell->hide();
    ui->btnExcelDeleteCell->hide();
    ui->btnSelectMultiFile->hide();

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
    haveDayHoursDataFlag=false;
    isNewImportDingCheckFlag=false;
    currentPage=CURRENT_PAGE::GENERATE_RATIO_CONFIG;

    ui->spinBoxYear->hide();
    ui->labelYear->hide();
    ui->spinBoxMonth->hide();
    ui->labelMonth->hide();
    ui->cbxProjectClsfy->hide();
    ui->cbxDepartment->show();
    ui->cbxCompany->show();
    ui->btnClearData->hide();
    ui->btnAssembleHoursData->hide();
    ui->btnAssembleHoursMonthData->hide();
    ui->btnOutputPjctHours->hide();

    ui->btnSaveData->hide();

    ui->btnPostTypeCfg->show();
    ui->btnInputExcelData->show();
    ui->btnExcelSave->show();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();
    ui->btnSelectMultiFile->hide();

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
    haveDayHoursDataFlag=false;
    isNewImportDingCheckFlag=false;
    currentPage=CURRENT_PAGE::PROJECT_DATA_CONFIG;

    ui->spinBoxYear->hide();
    ui->labelYear->hide();
    ui->spinBoxMonth->hide();
    ui->labelMonth->hide();
    ui->cbxProjectClsfy->hide();
    ui->cbxDepartment->show();
    ui->cbxCompany->show();
    ui->btnClearData->hide();
    ui->btnAssembleHoursData->hide();
    ui->btnAssembleHoursMonthData->hide();
    ui->btnOutputPjctHours->hide();

    ui->btnSaveData->hide();

    ui->btnPostTypeCfg->hide();
    ui->btnInputExcelData->show();
    ui->btnExcelSave->show();
    ui->btnExcelInsertCell->show();
    ui->btnExcelEditCell->show();
    ui->btnExcelDeleteCell->show();
    ui->btnSelectMultiFile->hide();


    pageButtonStateChange(CURRENT_PAGE::PROJECT_DATA_CONFIG);

    if(projectConfigImportFilename.isNull()){
        ui->tableWidget->clear();
        return;
    }

    projectDataConfigImport();
    isEditFlag=false;

}

/*多个文件汇总*/
void MainWindow::projectDataCalcPage(){
    currentPage=CURRENT_PAGE::PROJECT_DATA_CALC;

    ui->spinBoxYear->show();
    ui->labelYear->show();
    ui->spinBoxMonth->show();
    ui->labelMonth->show();
    ui->cbxProjectClsfy->hide();
    ui->cbxDepartment->hide();
    ui->cbxCompany->hide();
    ui->btnClearData->hide();
    ui->btnAssembleHoursData->hide();
    ui->btnAssembleHoursMonthData->hide();
    ui->btnOutputPjctHours->show();

    ui->btnSaveData->hide();

    ui->btnPostTypeCfg->hide();
    ui->btnInputExcelData->hide();
    ui->btnExcelSave->hide();
    ui->btnExcelInsertCell->hide();
    ui->btnExcelEditCell->hide();
    ui->btnExcelDeleteCell->hide();
    ui->btnSelectMultiFile->show();


    pageButtonStateChange(CURRENT_PAGE::PROJECT_DATA_CALC);

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
    ui->btnDataClc->setStyleSheet("color:rgb(0,0,0)");

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
        case CURRENT_PAGE::PROJECT_DATA_CALC:
        ui->btnDataClc->setFlat(true);
        ui->btnDataClc->setDown(true);
        ui->btnDataClc->setStyleSheet("color:rgb(30,144,255)");
        break;
    }

}

/*选定多个文件*/
void MainWindow::selectMutliFile(){

    /*先选择年月,统计当月天数*/
    QCalendar caledar;
    int currentMonthDays=caledar.daysInMonth(ui->spinBoxMonth->value(),ui->spinBoxYear->value());

    qDebug()<<"days:"<<ui->spinBoxYear->value()<<ui->spinBoxMonth->value()<<currentMonthDays<<"导入多个文件";

    QFileDialog* dialog=new QFileDialog(this);

    dialog->setFileMode(QFileDialog::ExistingFile);
    QStringList selectFile;

    selectFile=dialog->getOpenFileNames(this,tr("打开文件"),"./",tr("Excel (*.xlsx)"));
    if(selectFile.size()==0){
        return;
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setColumnCount(9+currentMonthDays);

    /*记录工号与项目和日期的关系*/
    //一个工号对应多个项目
    //一个项目对应多个日期和占比
    QMap<QString,QVector<prj2DateRatio>> workNum2PrjDate;
    QMap<QString,QMap<QString,QVector<prj2DateRatio1>>> workNum2PrjDate1;
    /**/
    QMap<QString,QString> workNum2Name;
    QMap<QString,QString> workNum2Career;
    QMap<QString,QString> workNum2Department;
    QMap<QString,QString> prj2FirstDepart;
    QMap<QString,QString> prj2SecondDepart;
    QMap<QString,QString> prj2Type;
    QMap<QString,QString> prj2Attribute;

    bool first=true;

    int rowC,columnC;
    int prjIndex=0;
    QStringList firstRowContent;
    for (QString filename:selectFile) {

        QXlsx::Document* excelFile=new QXlsx::Document(filename);
        rowC=excelFile->dimension().rowCount();
        columnC=excelFile->dimension().columnCount();
        qDebug()<<rowC<<columnC<<ui->tableWidget->rowCount();
        int rowww=ui->tableWidget->rowCount();

        //得到项目名称的索引,检索每个文件的首行
        if(first){
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            rowww+=1;
            for(int k=0;k<columnC;k++){
                if(excelFile->read(1,k+1).toString()==QString("项目名称")){
                    prjIndex=k+1;
                }
                ui->tableWidget->setItem(0,k,new QTableWidgetItem(excelFile->read(1,k+1).toString()));
                firstRowContent.push_back(excelFile->read(1,k+1).toString());
            }

            first=false;
        }

        qDebug()<<"项目列:"<<prjIndex<<ui->tableWidget->rowCount();

        for(int i=0;i<rowC;i++){
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            //工号--项目--日--zhanbi
            QVector<prj2DateRatio1> prj2DateRatioV;
            if(workNum2PrjDate1[excelFile->read(i+2,1).toString()][excelFile->read(i+2, prjIndex).toString()].size()){
                prj2DateRatioV=workNum2PrjDate1[excelFile->read(i+2,1).toString()][excelFile->read(i+2, prjIndex).toString()];
            }
            for(int j=0;j<columnC;j++){
                prj2DateRatio1 prj2DateRatioTmp;
                ui->tableWidget->setItem(i+rowww,j,new QTableWidgetItem(excelFile->read(i+2,j+1).toString()));

                if((j+1)>9 && !excelFile->read(i+2,j+1).toString().isNull()){
                    prj2DateRatioTmp.column=(j);
                    prj2DateRatioTmp.ratio=(excelFile->read(i+2,j+1).toString());
                    prj2DateRatioV.push_back(prj2DateRatioTmp);
                }

            }
            /**************************************************/
            if(!excelFile->read(i+2,1).toString().isNull() || !excelFile->read(i+2,1).toString().isEmpty())
                workNum2PrjDate1[excelFile->read(i+2,1).toString()][excelFile->read(i+2, prjIndex).toString()]=prj2DateRatioV;
            /*************************************************/

            if(!workNum2Name[excelFile->read(i+2,1).toString()].size()){
                workNum2Name[excelFile->read(i+2,1).toString()]=excelFile->read(i+2,2).toString();
            }
            if(!workNum2Career[excelFile->read(i+2,1).toString()].size()){
                workNum2Career[excelFile->read(i+2,1).toString()]=excelFile->read(i+2,3).toString();
            }
            if(!workNum2Department[excelFile->read(i+2,1).toString()].size()){
                workNum2Department[excelFile->read(i+2,1).toString()]=excelFile->read(i+2,4).toString();
            }
            if(!prj2FirstDepart[excelFile->read(i+2,7).toString()].size()){
                prj2FirstDepart[excelFile->read(i+2,7).toString()]=excelFile->read(i+2,5).toString();
            }
            if(!prj2SecondDepart[excelFile->read(i+2,7).toString()].size()){
                prj2SecondDepart[excelFile->read(i+2,7).toString()]=excelFile->read(i+2,6).toString();
            }
            if(!prj2Type[excelFile->read(i+2,7).toString()].size()){
                prj2Type[excelFile->read(i+2,7).toString()]=excelFile->read(i+2,8).toString();
            }
            if(!prj2Attribute[excelFile->read(i+2,7).toString()].size()){
                prj2Attribute[excelFile->read(i+2,7).toString()]=excelFile->read(i+2,9).toString();
            }

        }
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(9+currentMonthDays+1);
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    rowC=ui->tableWidget->rowCount();
    for(int i=0;i<firstRowContent.size();i++){
        //ui->tableWidget->setItem(rowC,i,new QTableWidgetItem(firstRowContent[i]));
        ui->tableWidget->setItem(rowC-1,i,new QTableWidgetItem(firstRowContent[i]));
    }
    ui->tableWidget->setItem(rowC-1,9+currentMonthDays,new QTableWidgetItem("占比"));
    qDebug()<<rowC<<firstRowContent;
    columnC=0;
    //项目vector
    auto ele=workNum2PrjDate1.constBegin();
    //工号
    while(ele!=workNum2PrjDate1.constEnd()){
        if(ele.key().isNull()){
            ele++;
            continue;
        }
        //项目
        auto subEle=ele.value();

        auto ssubEle=subEle.constBegin();
        int curWorkNumStartRow=ui->tableWidget->rowCount();
        int curWorkNumEndRow=curWorkNumStartRow;
        QVector<float> totalRatioV;
        while(ssubEle!=subEle.constEnd()){
            curWorkNumEndRow++;
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(rowC,0,new QTableWidgetItem(ele.key()));
            //项目名称
            ui->tableWidget->setItem(rowC,6,new QTableWidgetItem(ssubEle.key()));
            float totalRatio=0;

            for(auto sssubEle:ssubEle.value()){
                //工时
                totalRatio+=sssubEle.ratio.split("%")[0].toFloat();
                ui->tableWidget->setItem(rowC,sssubEle.column,new QTableWidgetItem(sssubEle.ratio));
                ui->tableWidget->setItem(rowC,1,new QTableWidgetItem(workNum2Name[ele.key()]));
                ui->tableWidget->setItem(rowC,2,new QTableWidgetItem(workNum2Career[ele.key()]));
                ui->tableWidget->setItem(rowC,3,new QTableWidgetItem(workNum2Department[ele.key()]));

                ui->tableWidget->setItem(rowC,4,new QTableWidgetItem(prj2FirstDepart[ssubEle.key()]));
                ui->tableWidget->setItem(rowC,5,new QTableWidgetItem(prj2SecondDepart[ssubEle.key()]));
                ui->tableWidget->setItem(rowC,7,new QTableWidgetItem(prj2Type[ssubEle.key()]));
                ui->tableWidget->setItem(rowC,8,new QTableWidgetItem(prj2Attribute[ssubEle.key()]));
            }

            totalRatioV.push_back(totalRatio);
            qDebug()<<totalRatio;

            rowC++;
            ssubEle++;
        }


        int maxWorkDay=0;
        QVector<bool> maxWorkDayFlag(currentMonthDays);
        qDebug()<<"maxWorkDayFlag.size:"<<maxWorkDayFlag.size()<<"row:"<<curWorkNumStartRow<<totalRatioV;
        for (int flagEleIndex=0;flagEleIndex<currentMonthDays;flagEleIndex++) {
            maxWorkDayFlag[flagEleIndex]=false;
        }
        for(int tmpRow=curWorkNumStartRow;tmpRow<curWorkNumEndRow;tmpRow++){
            for(int tmpi=0;tmpi<currentMonthDays;tmpi++){
                if(ui->tableWidget->item(tmpRow,tmpi+9)){
                    maxWorkDayFlag[tmpi]=true;
                }
            }
        }
        for (auto flagEle:maxWorkDayFlag) {
            if(flagEle)
                maxWorkDay++;
        }
        qDebug()<<"工号:"<<ele.key()<<"最大天数:"<<maxWorkDay<<maxWorkDayFlag;
        float sumRatio=0;
        float totalRatio2;
        int workendRow;
        for(int tmpRow=curWorkNumStartRow;tmpRow<curWorkNumEndRow;tmpRow++){
            totalRatio2=qRound((100*(totalRatioV[tmpRow-curWorkNumStartRow]))/float(maxWorkDay))/float(100);
            sumRatio+=totalRatio2;
            QString eleTotal=QString::number(totalRatio2)+"%";
            qDebug()<<eleTotal;
            ui->tableWidget->setItem(tmpRow,9+currentMonthDays,new QTableWidgetItem(eleTotal));
            workendRow=tmpRow;
        }
        if(sumRatio>100){
            float subRatio=qRound(100*sumRatio-100*100)/(float)100;
            QString eleTotal=QString::number(ui->tableWidget->item(workendRow,9+currentMonthDays)->text().split("%")[0].toFloat()-subRatio)+"%";
            ui->tableWidget->setItem(workendRow,9+currentMonthDays,new QTableWidgetItem(eleTotal));
        }
        else if(sumRatio<100){

            float subRatio=qRound(100*100-100*sumRatio)/(float)100;
            QString eleTotal=QString::number(ui->tableWidget->item(workendRow,9+currentMonthDays)->text().split("%")[0].toFloat()+subRatio)+"%";
            ui->tableWidget->setItem(workendRow,9+currentMonthDays,new QTableWidgetItem(eleTotal));

        }

        ele++;
    }

    /***计算汇总***/


}

/*钉钉考勤数据导入*/
void MainWindow::dingCheckImport(){
    if(dingCheckExcel==nullptr)
        dingCheckExcel=new QXlsx::Document(dingCheckImportFilename);
    qDebug()<<dingCheckExcel->dimension().rowCount()<<dingCheckExcel->dimension().columnCount();

    dingCheckExcel=new QXlsx::Document(dingCheckImportFilename);
    //assemablesExcel=nullptr;

    int rowC,columnC;

    rowC=dingCheckExcel->dimension().rowCount();
    columnC=dingCheckExcel->dimension().columnCount();

    //先确认是否为标准钉钉数据
    if(rowC==1){
        qDebug()<<"查看Excel是否需要去掉表头多列合为一列的情况";
        return;
    }

    bool hasDepart=false;
    bool hasJobNum=false;
    bool hasHumanName=false;

    jobNumWorkdayRowInfo.clear();

    for (int c=0;c<columnC;c++) {

        if(dingCheckExcel->read(3,c+1)==QString("部门"))
            hasDepart=true;
        if(dingCheckExcel->read(3,c+1)==QString("工号"))
            hasJobNum=true;
        if(dingCheckExcel->read(3,c+1)==QString("姓名"))
            hasHumanName=true;

    }
    if(!(hasDepart && hasJobNum && hasHumanName)){
        QMessageBox::warning(this,"error","不是标准人力给定钉钉考勤数据");
        return;
    }


    ui->tableWidget->setRowCount(rowC);
    ui->tableWidget->setColumnCount(columnC);

    QString datePattern("星期六|星期日");
    dateRegFormat.setPattern(datePattern);
    QString checkPattern("迟到");
    dingCheckRegFormat.setPattern(checkPattern);
    QString dimissionPattern("不在岗");
    dimissionRegFormat.setPattern(dimissionPattern);
    QString breakPattern("休息");
    breakDateRegFormat.setPattern(breakPattern);
    QString vacatePattern("在岗");
    vacateRegFormat.setPattern(vacatePattern);


    for (int r=0;r<=rowC;r++) {
        for (int c=0;c<columnC;c++) {
            //qDebug()<<xlsxDoc.read(r,c).userType()<<xlsxDoc.read(r,c).type()<<xlsxDoc.read(r,c).toString();
            auto data=dingCheckExcel->read(r+1,c+1);
            QString content=data.toString();

            auto item=new QTableWidgetItem(content,data.type());
            if(content.contains(dimissionRegFormat))
                item->setForeground(Qt::red);
            if(content.contains(dingCheckRegFormat))
                item->setBackground(QColor(120,255,0));
            if(content.contains(vacateRegFormat))
                item->setBackground(QColor(100,250,106));
            if(content.contains(breakDateRegFormat))
                item->setBackground(QColor(189,183,107));


            item->setTextAlignment(Qt::AlignCenter);
            item->setFlags(Qt::ItemIsEditable);
            ui->tableWidget->setItem(r,c,item);
        }
    }

    if(dingCheckImportFilename.isNull()){
        return;
    }

    if(isNewImportDingCheckFlag ){


        /*杨晨光给的考勤表0608*/
        currentYear=dingCheckExcel->read(1,1).toString().split(QRegExp("[:：]"))[1].split(QRegExp("[/-]"))[0].toInt();
        currentMonth=dingCheckExcel->read(1,1).toString().split(QRegExp("[:：]"))[1].split(QRegExp("[/-]"))[1].toInt();

        if(currentYear!=ui->spinBoxYear->value() || currentMonth!=ui->spinBoxMonth->value()){
            //导入之后呼出小窗，通知选定考勤年月
            QMessageBox::information(nullptr,"Info",QString("当前选定考勤年月,和导入的文件年月不匹配"));
            return;
        }

        //导入之后呼出小窗，通知选定考勤年月
        QMessageBox::information(nullptr,"Info",QString("当前选定%1年%2月,\n确定要导入的文件名为:%3")
                                 .arg(ui->spinBoxYear->value())
                                 .arg(ui->spinBoxMonth->value())
                                 .arg(dingCheckImportFilename));

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

            jobNumHumanInfo[QString("%1").arg(dingCheckExcel->read(r+1,columnJobNum).toString().toInt(),3,10,QChar('0'))]=dingCheckExcel->read(r+1,columnHumanName).toString();

            for (int c=columnJobNum+1;c<=columnC;c++) {
                if(dingCheckExcel->read(r+1,c).toString()=="在岗")
                    jobNumWorkdayRowInfo[QString("%1").arg(dingCheckExcel->read(r+1,columnJobNum).toString().toInt(),3,10,QChar('0'))].push_back(daysOfMonth[c-columnJobNum-1]);
            }
        }

    }

}

/*工时占比配置导入*/
void MainWindow::hoursRatioConfigImport(){
    if(hourRatioExcel==nullptr)
        hourRatioExcel=new QXlsx::Document(hoursRatioConfigImportFilename);
    qDebug()<<hourRatioExcel->dimension().rowCount()<<hourRatioExcel->dimension().columnCount();


    hourRatioExcel=new QXlsx::Document(hoursRatioConfigImportFilename);
    int rowC,columnC;
    rowC=hourRatioExcel->dimension().rowCount();
    columnC=hourRatioExcel->dimension().columnCount();

    bool hasJobNum=false;
    bool hasHumanName=false;
    bool hasLeaveTime=false;
    bool hasCompany=false;
    bool hasBU_Depart=false;
    bool hasDepart2=false;
    bool hasHumanType=false;


    for (int c=0;c<columnC;c++) {
        if(hourRatioExcel->read(1,c+1)==QString("工号"))
            hasJobNum=true;
        if(hourRatioExcel->read(1,c+1)==QString("姓名"))
            hasHumanName=true;
        if(hourRatioExcel->read(1,c+1)==QString("离职时间"))
            hasLeaveTime=true;
        if(hourRatioExcel->read(1,c+1)==QString("所属公司"))
            hasCompany=true;
        if(hourRatioExcel->read(1,c+1)==QString("所属BU/部门"))
            hasBU_Depart=true;
        if(hourRatioExcel->read(1,c+1)==QString("二级部门"))
            hasDepart2=true;
        if(hourRatioExcel->read(1,c+1)==QString("工时人员性质分类"))
            hasHumanType=true;

    }
    if(!(hasJobNum && hasHumanName && hasLeaveTime && hasCompany && hasBU_Depart && hasDepart2 && hasHumanType)){
        QMessageBox::warning(this,"error","不是标准人力给定工时人员信息分类数据");
        return;
    }

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

        jobNumPostTypeInfo[QString("%1").arg(hourRatioExcel->read(r+1,columnJobNum).toString().toInt(),3,10,QChar('0'))]=hourRatioExcel->read(r+1,columnCharClsfy).toString();
        //每个工号的部门信息
        jobNumDepartmentInfo[QString("%1").arg(hourRatioExcel->read(r+1,columnJobNum).toString().toInt(),3,10,QChar('0'))].push_back(hourRatioExcel->read(r+1,columnDepart1).toString());
        jobNumDepartmentInfo[QString("%1").arg(hourRatioExcel->read(r+1,columnJobNum).toString().toInt(),3,10,QChar('0'))].push_back(hourRatioExcel->read(r+1,columnDepart2).toString());

        if(hourRatioExcel->read(r+1,columnLeaveDate).isNull())
            continue;
        QString tmp=hourRatioExcel->read(r+1,columnLeaveDate).toString();
        if(tmp.contains(QRegExp("[-/]")))
        {
            auto leaveT=hourRatioExcel->read(r+1,columnLeaveDate).toString().split(QRegExp("[/-]"));
            jobNumLeaveTimeInfo[hourRatioExcel->read(r+1,columnJobNum).toString()]=QDate(leaveT[0].toInt(),leaveT[1].toInt(),leaveT[2].toInt());
        }
    }

}

/*项目配置数据导入*/
void MainWindow::projectDataConfigImport(){
    projectHumansInfo.clear();

    if(projectDataExcel==nullptr)
        projectDataExcel=new QXlsx::Document(projectConfigImportFilename);


    qDebug()<<projectDataExcel->dimension().rowCount()<<projectDataExcel->dimension().columnCount();

    projectDataExcel=new QXlsx::Document(projectConfigImportFilename);
    int rowC,columnC;
    rowC=projectDataExcel->dimension().rowCount();
    columnC=projectDataExcel->dimension().columnCount();


    bool hasPrjName=false;
    bool hasPrjType1=false;
    bool hasPrjType2=false;
    bool hasPrjBU=false;
    bool hasPrjDepart=false;
    bool hasPrjStartTime=false;
    bool hasPrjEndTime=false;
    bool hasPrjHuman=false;


    for (int c=0;c<columnC;c++) {
        if(projectDataExcel->read(1,c+1)==QString("项目名称"))
            hasPrjName=true;
        if(projectDataExcel->read(1,c+1)==QString("项目类型"))
            hasPrjType1=true;
        if(projectDataExcel->read(1,c+1)==QString("项目属性"))
            hasPrjType2=true;
        if(projectDataExcel->read(1,c+1)==QString("所属BU"))
            hasPrjBU=true;
        if(projectDataExcel->read(1,c+1)==QString("二级部门"))
            hasPrjDepart=true;
        if(projectDataExcel->read(1,c+1)==QString("项目开始时间"))
            hasPrjStartTime=true;
        if(projectDataExcel->read(1,c+1)==QString("项目结束时间"))
            hasPrjEndTime=true;
        if(projectDataExcel->read(1,c+1)==QString("项目成员"))
            hasPrjHuman=true;

    }
    if(!(hasPrjName && hasPrjType1 && hasPrjType2 && hasPrjBU && hasPrjDepart && hasPrjStartTime && hasPrjEndTime && hasPrjHuman)){
        QMessageBox::warning(this,"error","不是标准人力给定项目配置信息数据");
        return  ;
    }

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
    int columnPrjName,columnBU,columnDepartment,columnStartTime,columnEndTime,columnPrjType,columnPrjWorker;
    int columnPrjChar;
    for (int c=0;c<columnC;c++) {
        if(projectDataExcel->read(1,c+1).toString().contains("项目名称"))
            columnPrjName=c+1;
        if(projectDataExcel->read(1,c+1).toString().contains("所属BU"))
            columnBU=c+1;
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

        projectDepartmentInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(projectDataExcel->read(r+1,columnBU).toString());
        projectDepartmentInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(projectDataExcel->read(r+1,columnDepartment).toString());

        projectDateInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(QDate::fromString(projectDataExcel->read(r+1,columnStartTime).toString(),"yyyyMMdd"));

        projectDateInfo[projectDataExcel->read(r+1,columnPrjName).toString()].push_back(QDate::fromString(projectDataExcel->read(r+1,columnEndTime).toString(),"yyyyMMdd"));

        auto workersList=projectDataExcel->read(r+1,columnPrjWorker).toString().split(QRegExp("[,，/、]"));
        foreach (QString worker, workersList) {
            projectHumansInfo[projectDataExcel->read(r+1,1).toString()].push_back(worker);
        }
    }

}


void MainWindow::projectWorkingHoursGenerate(){
    ui->tableWidget->clearContents();
    ui->tableWidget->clear();

}

/**/
void MainWindow::appendRowInAssemableTable(int rowNum,QVector<QString> dataList){

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    for (int c=0;c<dataList.count();c++) {
        ui->tableWidget->setItem(rowNum,c,new QTableWidgetItem(dataList[c]));

    }
    if(!dataList[0].contains("工号")){
        jobNumInRowInfo[dataList[0]].push_back(rowNum);
    }

}

//修改已添加行的休息
void MainWindow::changeAssemableColumnInfo(int rowNum,int columnNum,QString info){
    ui->tableWidget->setItem(rowNum,columnNum,new QTableWidgetItem(info));
}

//判断每日工时是否为1
void MainWindow::judgeWorkdatHoursException(){
    //jobNumInRowInfo
    //jobNumWorkdayRowInfo
    for (QString gonghao:jobNumWorkdayRowInfo.keys()) {
        int ff=false;
        for(QString c:jobNumWorkdayRowInfo[gonghao]){
            if(ff){break;}
            int totalRatio=0;
            for (int r:jobNumInRowInfo[gonghao]) {
                if(ui->tableWidget->item(r,8+c.toInt()))
                {
                    auto aa=ui->tableWidget->item(r,8+c.toInt())->text();
                    totalRatio+=aa.split("%")[0].toInt();
                }
            }
            if(totalRatio!=100){
                ff=true;
                for (int r:jobNumInRowInfo[gonghao]) {

                    ui->tableWidget->item(r,0)->setBackground(Qt::red);
                    ui->tableWidget->item(r,1)->setBackground(Qt::red);
                }
                break;
            }

        }

    }
}

//组装工时数据  按天
void MainWindow::assemblWorkingHoursData(){

    if(dingCheckImportFilename.isNull() || hoursRatioConfigImportFilename.isNull() || projectConfigImportFilename.isNull())
        return;

    ui->tableWidget->clearContents();
    isAssemableHoursFlag=true;
    isDayHourGFlag=true;

    jobNumInRowInfo.clear();
    //将excel进行组装操作
    //1.应该以考勤表为基础添加当天的分配的项目类型，工时，
    //2.生成的工时文件需要和OPM输出的文件格式尽量保持一致
    //3.生成信息包括 姓名 日期 所属部门 一级部门 二级部门 岗位类别 项目名称 项目类型 工时占比
    //先生成当前月人员的项目信息
    calHumanCurrentMonthProjectInfo();
    //显示在tableWidget上
    int newRowNum=0;


    int yanfaMaxChanpin=postTypeConfigData["研发岗位"]["外接产品"][1];
    int yanfaMaxJishu=postTypeConfigData["研发岗位"]["外接技术"][1];
    int shengchanMaxChanpin=postTypeConfigData["生产岗位"]["外接产品"][0];
    int zhiliangMaxJishu=postTypeConfigData["质量岗位"]["外接技术"][1];
    int zhiliangMaxChanpin=postTypeConfigData["质量岗位"]["外接产品"][1];
    int caigouMaxChanpin=postTypeConfigData["采购岗位"]["外接产品"][0];

    QVector<QString> rowData;
    QVector<QString> headerrowData;

    rowData<<"工号"<<"姓名"<<"岗位类型"<<"所属部门"<<"一级部门"<<"二级部门"<<"项目名称"<<"类型"<<"项目属性";

    QCalendar caledar;
    int currentMonthDays=caledar.daysInMonth(currentMonth,currentYear);
    int coluC=rowData.size();
    for (int i=1;i<=currentMonthDays;i++) {
        rowData.append(QString::number(currentYear)+QString("%1").arg(currentMonth,2,10,QChar('0'))+QString("%1").arg(i,2,10,QChar('0')));
        dayToColumnNum[QString::number(i)]=coluC+i-1;
    }
    ui->tableWidget->setColumnCount(rowData.size());
    headerrowData=rowData;
    //先对工号的项目进行统计  得到工号对应的项目和工号对应的类型及性质
    appendRowInAssemableTable(newRowNum,rowData);
    newRowNum++;


    //这个人的工号
    foreach(QString jobNumKey,jobNumWorkdayRowInfo.keys()){
        QRandomGenerator rdGen;

        /*占比分配算法*/
        //这个工号岗位为什么岗位
        int jobNumTypeFlag;
        if(jobNumPostTypeInfo[jobNumKey].contains("研发")){
            jobNumTypeFlag=1;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("生产")){
            jobNumTypeFlag=2;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("质量")){
            jobNumTypeFlag=3;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("采购")){
            jobNumTypeFlag=4;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("其他")){
            jobNumTypeFlag=5;
        }

        rdGen.seed(1);
        //当前工号当前月有的所有项目
       auto jobNumCurrentTotalPrj=jobNumCurrentMonthProjectInfo[jobNumKey];
       int tmpi=0,tmpj=0;
       int jobNumThisMonthEveryDayPrjC=0;
       QMap<QString,int> prj2RowNum;
       prj2RowNum.clear();

       //分为研发项目和非研发项目
       QVector<QString> yanfaPrjV;
       QVector<QString> feiyanfaPrjV;
       yanfaPrjV.clear();
       feiyanfaPrjV.clear();
        //先将所有项目的行给出
       for (int kk=0;kk<jobNumCurrentTotalPrj.size();kk++) {

           QVector<QString> rowInsert;
           rowInsert<<jobNumKey<<jobNumHumanInfo[jobNumKey]<<jobNumPostTypeInfo[jobNumKey]<<jobNumDepartmentInfo[jobNumKey][0]+"-"+jobNumDepartmentInfo[jobNumKey][1]
                 <<projectDepartmentInfo[jobNumCurrentTotalPrj[kk]][0]<<projectDepartmentInfo[jobNumCurrentTotalPrj[kk]][1]
                   <<jobNumCurrentTotalPrj[kk]<<projectTypeInfo[jobNumCurrentTotalPrj[kk]][0]<<projectTypeInfo[jobNumCurrentTotalPrj[kk]][1];
           qDebug()<<"添加一行："<<rowInsert;
           appendRowInAssemableTable(newRowNum+kk,rowInsert);
           prj2RowNum[jobNumCurrentTotalPrj[kk]]=newRowNum+kk;

           projectTypeInfo[jobNumCurrentTotalPrj[kk]][0].contains("研发")?yanfaPrjV.push_back(jobNumCurrentTotalPrj[kk]):
                                                                        feiyanfaPrjV.push_back(jobNumCurrentTotalPrj[kk]);
       }

       //每个工号当前月应该添加多少行
       qDebug()<<newRowNum<<jobNumCurrentTotalPrj.size();
       newRowNum+=jobNumCurrentTotalPrj.size();


       //每个工作日
       foreach (QString humanWorkDay,jobNumWorkdayRowInfo[jobNumKey]) {
           int ratio;

           int restYanfa=0;
           int restWaijijishu=0;
           int restWaijichanpin=0;
           int restWaiji=0;

           int restQita=0;
           /**/

           //确定这个人当天的项目个数
           //当天的项目数等于当月项目数减去当天没有的项目数
           //根据当前月，统计所有项目的开始结束时间
           //当天的各个类型项目数
           int restYanfaC=0;
           int restWaijieJishuC=0;
           int restWaijieChanpinC=0;
           int restQitaC=0;

            //给当前工作日分配项目
           QVector<QString> currentDayPrj;
           currentDayPrj.clear();

           {
               //为研发人员时必须至少分配一个研发项目 且此研发项目必须在开发时间内
               QString yanfaPrj;
               if(jobNumTypeFlag==1){
                   for(int i=0;i<yanfaPrjV.size();i++){
                       //判断这个项目在当日是否开始
                       int startYear=projectDateInfo[yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]][0].year();
                       int endYear=projectDateInfo[yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]][1].year();
                       int startMonth=projectDateInfo[yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]][0].month();
                       int endMonth=projectDateInfo[yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]][1].month();
                       int start1=projectDateInfo[yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]][0].day();
                       int end1=projectDateInfo[yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]][1].day();
                       int curWorkday=humanWorkDay.toUInt();
                       //仅在月份相同的情况下才存在
//                       if(currentMonth==startMonth && currentYear<endYear){
//                           if( curWorkday>=start1 && curWorkday<=end1){
//                               currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
//                               break;
//                           }
//                       }
//                       else{
//                           currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
//                           break;
//                       }
                        //
                       if(currentYear>startYear){
                           if(currentYear<endYear){
                               currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                               break;
                           }
                           else if(currentYear==endYear){
                               if(endMonth>currentMonth){
                                   currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                   break;
                               }
                               else if(endMonth==currentMonth){
                                   if(curWorkday<=end1){
                                       currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                       break;
                                   }
                               }
                           }
                       }
                       else if(currentYear==startYear){
                           if(currentYear< endYear){
                               if(currentMonth>startMonth){
                                   currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                   break;
                               }
                               else if(currentMonth==startMonth){
                                   if(curWorkday>=start1){
                                       currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                       break;
                                   }
                               }
                           }
                           else if(currentYear==endYear){
                               if(startMonth<currentMonth){
                                   if(currentMonth<endMonth){
                                       currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                       break;
                                   }
                                   else if(currentMonth==endMonth){
                                       if(curWorkday<=end1){
                                           currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                           break;
                                       }
                                   }

                               }
                               else if(startMonth==currentMonth){
                                   if(currentMonth<endMonth){
                                       if(curWorkday>=start1){
                                           currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                           break;
                                       }
                                   }
                                   else if(currentMonth==endMonth){
                                       if(curWorkday>=start1 && curWorkday<=end1){
                                           currentDayPrj.push_back(yanfaPrjV[(i+tmpj)%yanfaPrjV.size()]);
                                           break;
                                       }
                                   }
                               }
                           }
                       }


                       }
               }
               for (int i=0;currentDayPrj.size()<everydayMaxPrjCount&& i<jobNumCurrentTotalPrj.size();i++) {
                   //保证当前日只取everydayMaxPrjCount个项目
                   QString prjTmp=jobNumCurrentTotalPrj[(i+tmpi)%jobNumCurrentTotalPrj.size()];
                   if(prjTmp=="管理工作"){
                       currentDayPrj.push_back(prjTmp);
                       restQitaC=1;
                       continue;
                   }
                   if(currentDayPrj.contains(prjTmp))
                       continue;
                   //判断这个项目在当日是否开始
                   int startYear=projectDateInfo[prjTmp][0].year();
                   int endYear=projectDateInfo[prjTmp][1].year();
                   int startMonth=projectDateInfo[prjTmp][0].month();
                   int endMonth=projectDateInfo[prjTmp][1].month();
                   int start1=projectDateInfo[prjTmp][0].day();
                   int end1=projectDateInfo[prjTmp][1].day();
                   int curWorkday=humanWorkDay.toUInt();
//                   if(currentMonth==startMonth || currentMonth==endMonth){
//                       if(curWorkday>=start1 && curWorkday<=end1){
//                           currentDayPrj.push_back(prjTmp);
//                           continue;
//                       }
//                       i-=1;
//                       tmpi+=1;
//                   }
//                   else{
//                       currentDayPrj.push_back(prjTmp);
//                   }

                   if(currentYear>startYear){
                       if(currentYear<endYear){
                           currentDayPrj.push_back(prjTmp);

                       }
                       else if(currentYear==endYear){
                           if(endMonth>currentMonth){
                               currentDayPrj.push_back(prjTmp);

                           }
                           else if(endMonth==currentMonth){
                               if(curWorkday<=end1){
                                   currentDayPrj.push_back(prjTmp);

                               }
                           }
                       }
                   }
                   else if(currentYear==startYear){
                       if(currentYear< endYear){
                           if(currentMonth>startMonth){
                               currentDayPrj.push_back(prjTmp);

                           }
                           else if(currentMonth==startMonth){
                               if(curWorkday>=start1){
                                   currentDayPrj.push_back(prjTmp);

                               }
                           }
                       }
                       else if(currentYear==endYear){
                           if(startMonth<currentMonth){
                               if(currentMonth<endMonth){
                                   currentDayPrj.push_back(prjTmp);

                               }
                               else if(currentMonth==endMonth){
                                   if(curWorkday<=end1){
                                       currentDayPrj.push_back(prjTmp);

                                   }
                               }

                           }
                           else if(startMonth==currentMonth){
                               if(currentMonth<endMonth){
                                   if(curWorkday>=start1){
                                       currentDayPrj.push_back(prjTmp);

                                   }
                               }
                               else if(currentMonth==endMonth){
                                   if(curWorkday>=start1 && curWorkday<=end1){
                                       currentDayPrj.push_back(prjTmp);

                                   }
                               }
                           }
                       }
                   }
//                   else{
//                      i-=1;
//                      tmpi+=1;
//                   }

               }
           }

           for(QString ssPrjName:currentDayPrj){
               projectTypeInfo[ssPrjName][0].contains("研发")?restYanfaC+=1:restYanfaC+=0;
               projectTypeInfo[ssPrjName][0].contains("外接")&&projectTypeInfo[ssPrjName][1].contains("产品")?restWaijieChanpinC+=1:restWaijieChanpinC+=0;
               projectTypeInfo[ssPrjName][0].contains("外接")&&projectTypeInfo[ssPrjName][1].contains("技术")?restWaijieJishuC+=1:restWaijieJishuC+=0;

           }

            int prjCount=currentDayPrj.size();
            if(prjCount==0)
                continue;
            //先分配占比 防止工号当前月没有外接项目造成比例错误的情况
            switch (jobNumTypeFlag) {
                case 1://研发岗位
                    restQita=0;
                    //有外接技术
                    if((restWaijieJishuC)>0){
                        //外接产品
                        if(restWaijieChanpinC>0){
                            restWaijichanpin=rdGen.bounded(0,yanfaMaxChanpin/prjMinRatio);
                            restWaijijishu=rdGen.bounded(0,(yanfaMaxJishu/prjMinRatio-restWaijichanpin));
                            restYanfa=(100/prjMinRatio-restWaijijishu-restWaijichanpin);
                        }
                        else{
                            restWaijijishu=rdGen.bounded(0,yanfaMaxJishu/prjMinRatio);
                            restYanfa=(100/prjMinRatio-restWaijijishu);
                        }

                    }
                    //有产品 无技术
                    if(restWaijieChanpinC>0){
                        if(restWaijieJishuC==0){
                            restWaijichanpin=rdGen.bounded(0,(yanfaMaxChanpin)/prjMinRatio);
                            restYanfa=(100/prjMinRatio-restWaijichanpin);
                        }
                    }

                    if((restWaijieJishuC+restWaijieChanpinC)==0){
                        restYanfa=100/prjMinRatio;
                    }

                break;
                case 2://生产岗位 //应该自动生成一个管理类项目
                    restQitaC=1;
                    //prjCount+=1;

                    if(!currentDayPrj.contains(QString("管理工作")))
                    {
                        currentDayPrj.push_back(QString("管理工作"));
                    }
                    restYanfa=0;
                    restWaijijishu=0;
                    //有外接产品
                    if((restWaijieChanpinC)>0){
                        //you其他
                        if( restQitaC>0){
                            restWaijichanpin=rdGen.bounded(0,(shengchanMaxChanpin)/prjMinRatio);
                            restQita=(100/prjMinRatio-restWaijichanpin);
                        }
                        else{
                            restWaijichanpin=100/prjMinRatio;
                            restQita=0;
                        }

                    }
                    //有其他  无产品
                    if(restQitaC>0){
                        if(restWaijieChanpinC==0){
                            restQita=100/prjMinRatio;
                            restWaijichanpin=0;
                        }
                    }


                break;
                case 3://质量岗位
                    restQitaC=1;

                    if(!currentDayPrj.contains(QString("管理工作")))
                    {
                        currentDayPrj.push_back(QString("管理工作"));
                    }
                    restYanfa=0;
                    //有外接  //肯定有其他项目
                    //外接合计不超过0.6
                    //有产品
                    if(restWaijieChanpinC>0){
                        //有技术
                        if(restWaijieJishuC>0){
                            restWaijijishu=rdGen.bounded(0,zhiliangMaxJishu/prjMinRatio);
                            restWaijichanpin=rdGen.bounded(0,(zhiliangMaxChanpin/prjMinRatio-restWaijijishu));
                            restQita=(100/prjMinRatio-restWaijijishu-restWaijichanpin);
                        }
                        //无技术
                        else{
                            restWaijichanpin=rdGen.bounded(0,zhiliangMaxChanpin/prjMinRatio);
                            restWaijijishu=0;
                            restQita=(100/prjMinRatio-restWaijijishu-restWaijichanpin);
                        }
                    }
                    //有技术
                    if(restWaijieJishuC>0){
                        //没产品
                        if(restWaijieChanpinC==0){
                            restWaijijishu=rdGen.bounded(0,zhiliangMaxJishu/prjMinRatio);
                            restWaijichanpin=0;
                            restQita=(100/prjMinRatio-restWaijijishu-restWaijichanpin);
                        }
                    }
                    if((restWaijieJishuC+restWaijieChanpinC)==0){
                        restQita=100/prjMinRatio;
                        restWaijijishu=0;
                        restWaijichanpin=0;
                    }

                break;
                case 4://采购岗位
                    restQitaC=1;

                    if(!currentDayPrj.contains(QString("管理工作")))
                    {
                        currentDayPrj.push_back(QString("管理工作"));
                    }
                    restYanfa=0;
                    restWaijijishu=0;
                    //有产品
                    if(restWaijieChanpinC>0){
                        //无其他
                        if(restQitaC==0){
                            restWaijichanpin=100/prjMinRatio;
                        }
                        //有其他
                        if(restQitaC>0){
                            restWaijichanpin=rdGen.bounded(0,caigouMaxChanpin/prjMinRatio);
                            restQita=(100/prjMinRatio-restWaijichanpin);
                        }
                    }
                    //无产品
                    if(restWaijieChanpinC==0){
                        if(restQitaC>0){
                            restQita=100/prjMinRatio;
                        }
                    }
                break;
                case 5://其他岗位
                    restQita=0;
                    restYanfa=0;
                    restWaiji=0;
                    restWaijijishu=0;
                    restWaijichanpin=0;
                break;
            }
            prjCount=currentDayPrj.size();
            jobNumThisMonthEveryDayPrjC=prjCount;


            for (int numPrj=0;numPrj<prjCount;numPrj++) {

                QVector<QString> rowTmp;
                if(currentDayPrj[numPrj]=="管理工作"){//jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]==("管理工作")){
                    ratio=restQita;
                    rowTmp<<jobNumKey<<jobNumHumanInfo[jobNumKey]<<jobNumPostTypeInfo[jobNumKey]<<jobNumDepartmentInfo[jobNumKey][0]+"-"+jobNumDepartmentInfo[jobNumKey][1]
                            <<QString("管理工作")<<QString("管理工作")
                              <<currentDayPrj[numPrj]<<QString("管理工作")//jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]<<QString("管理工作")
                           <<QString("管理工作");//<<humanWorkDay<<QString::number(ratio)+QString("%");

                    //appendRowInAssemableTable(newRowNum+numPrj,rowTmp);
                    changeAssemableColumnInfo(prj2RowNum[currentDayPrj[numPrj]],dayToColumnNum[humanWorkDay],QString::number(ratio*prjMinRatio)+QString("%"));

                      //qDebug()<<rowTmp<<dayToColumnNum[humanWorkDay]<<QString::number(ratio)+QString("%");
                    //break;
                    continue;
                  }
                //根据工号岗位和项目类型及属性共同确定这个项目的占比分配
                //当前项目属性
                //项目为研发
                if(projectTypeInfo[currentDayPrj[numPrj]][0].contains("研发")){//projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0].contains("研发")){
                    if(restYanfaC==1){
                        ratio=restYanfa;
                        restYanfa-=restYanfa;

                    }
                    else{
                        if(restYanfa==0){
                            ratio=0;
                        }
                        else{
                            ratio=rdGen.bounded(0,restYanfa);
                        }

                        restYanfa-=ratio;


                    }
                    restYanfaC--;
                }
                if(projectTypeInfo[currentDayPrj[numPrj]][0].contains("外接")){//projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0].contains("外接")){
                    if(projectTypeInfo[currentDayPrj[numPrj]][1].contains("技术")){//projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1].contains("技术")){
                        if(restWaijieJishuC==1){
                            ratio=restWaijijishu;
                            restWaijijishu-=restWaijijishu;
                        }
                        else{
                            if(restWaijijishu==0){
                                ratio=0;
                            }
                            else{
                                ratio=rdGen.bounded(0,restWaijijishu);
                            }
                            restWaijijishu-=ratio;

                        }
                        restWaijieJishuC--;
                    }
                    if(projectTypeInfo[currentDayPrj[numPrj]][1].contains("产品")){//projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1].contains("产品")){
                        if(restWaijieChanpinC==1){
                            ratio=restWaijichanpin;
                           restWaijichanpin-=restWaijichanpin;
                        }
                        else{
                            if(restWaijichanpin==0){
                                ratio=0;
                            }
                            else{
                                ratio=rdGen.bounded(0,restWaijichanpin);
                            }
                            restWaijichanpin-=ratio;

                        }
                        restWaijieChanpinC--;
                    }
                }
                if(projectTypeInfo[currentDayPrj[numPrj]][0].contains("管理工作")){//projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0].contains("管理工作")){
                    if(restQitaC==1){
                        ratio=restQita;
                        restQita-=restQita;
                    }
                    else{
                        if(restQita==0){
                            ratio=0;
                        }
                        else{
                            ratio=rdGen.bounded(0,restQita);
                        }

                        restQita-=ratio;

                    }
                    restQitaC--;
                }


                //工时占比需要由配置的项目类型占比比例进行分配，分配原则为最小分配比例1%
                //当前项目
                /*rowTmp<<jobNumKey<<jobNumHumanInfo[jobNumKey]<<jobNumPostTypeInfo[jobNumKey]<<jobNumDepartmentInfo[jobNumKey][0]+"-"+jobNumDepartmentInfo[jobNumKey][1]
                      <<projectDepartmentInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0]<<projectDepartmentInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1]
                        <<jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]<<projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0]
                     <<projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1];*/
                rowTmp<<jobNumKey<<jobNumHumanInfo[jobNumKey]<<jobNumPostTypeInfo[jobNumKey]<<jobNumDepartmentInfo[jobNumKey][0]+"-"+jobNumDepartmentInfo[jobNumKey][1]
                      <<projectDepartmentInfo[currentDayPrj[numPrj]][0]<<projectDepartmentInfo[currentDayPrj[numPrj]][1]
                        <<currentDayPrj[numPrj]<<projectTypeInfo[currentDayPrj[numPrj]][0]
                     <<projectTypeInfo[currentDayPrj[numPrj]][1];
                //appendRowInAssemableTable(newRowNum+numPrj,rowTmp);
                changeAssemableColumnInfo(prj2RowNum[currentDayPrj[numPrj]],dayToColumnNum[humanWorkDay],QString::number(ratio*prjMinRatio)+QString("%"));
                qDebug()<<humanWorkDay+":"+QString::number(ratio*prjMinRatio)+QString("%");
            }


           tmpi+=1;
           tmpj+=1;
       }

    }

    appendRowInAssemableTable(0,headerrowData);
    qDebug()<<"共写入行："+QString::number(newRowNum);
    qDebug()<<"当前共有行:"<<QString::number(ui->tableWidget->rowCount());
    ui->tableWidget->setRowCount(newRowNum);

    //判断数据是否满足
    judgeWorkdatHoursException();

    //暂存  为后面按月生成工时作为底版
    assemablesExcel=new QXlsx::Document;

    int tableRowCount=ui->tableWidget->rowCount();
    int tableColumnCount=ui->tableWidget->columnCount();

    for (int r=1;r<=tableRowCount;r++) {
        for (int c=1;c<=tableColumnCount;c++) {
            if(ui->tableWidget->item(r-1,c-1)!=0){

                auto data=ui->tableWidget->item(r-1,c-1)->text();

                //qDebug()<<type<<data;
                assemablesExcel->write(r,c,data);
                if(c>9 && r>1){
                    qDebug()<<"row:"<<r<<" column:"<<c<<" ratio:"<<data;
                }
            }
        }
    }

    haveDayHoursDataFlag=true;
    qDebug()<<"Excel:"<<assemablesExcel->dimension().rowCount()<<assemablesExcel->dimension().columnCount()<<assemablesExcel->read(1,1);
}
//组装工时数据  按月
void MainWindow::assemblWorkingHoursMonthData(){

    if(dingCheckImportFilename.isNull() || hoursRatioConfigImportFilename.isNull() || projectConfigImportFilename.isNull())
        return;
    ui->tableWidget->clear();
    isAssemableHoursFlag=true;
    isDayHourGFlag=false;


    //将excel进行组装操作
    //1.应该以考勤表为基础添加当天的分配的项目类型，工时，
    //2.生成的工时文件需要和OPM输出的文件格式尽量保持一致
    //3.生成信息包括 姓名 日期 所属部门 一级部门 二级部门 岗位类别 项目名称 项目类型 工时占比
    //先生成当前月人员的项目信息
    calHumanCurrentMonthProjectInfo();
    //显示在tableWidget上
    int newRowNum=0;


    QVector<QString> rowData;
    rowData<<"工号"<<"姓名"<<"岗位类型"<<"所属部门"<<"一级部门"<<"二级部门"<<"项目名称"<<"类型"<<"项目属性"
              <<QString("日期/(%1)").arg(QString::number(currentYear)+"年"+QString::number(currentMonth)+"月")
             <<"占比";
    int monthTotalColumn=rowData.size();
    //先对工号的项目进行统计  得到工号对应的项目和工号对应的类型及性质
    ui->tableWidget->setColumnCount(rowData.size());
    appendRowInAssemableTable(newRowNum,rowData);

    if(haveDayHoursDataFlag && assemablesExcel!=nullptr){

        int assembleRow=assemablesExcel->dimension().rowCount();
        int assembleColumn=assemablesExcel->dimension().columnCount();
        for (int r=2;r<=assembleRow;r++) {
            for (int c=1;c<monthTotalColumn-1;c++) {
                qDebug()<<assemablesExcel->read(r,c).toString();
                ui->tableWidget->setItem(r-1,c-1,new QTableWidgetItem(assemablesExcel->read(r,c).toString()));
            }
            int sumRatioTmp=0;
            int ratioCount=0;
            for (int c=dayToColumnNum["1"]+1;c<=dayToColumnNum["1"]+31;c++) {
                if(assemablesExcel->read(r,c).toString().isNull())
                    continue;
                sumRatioTmp+=assemablesExcel->read(r,c).toString().split(QRegExp("%"))[0].toInt();
                ratioCount++;
            }
            double currentRatio=((double)sumRatioTmp/(double)ratioCount);
            ui->tableWidget->setItem(r-1,monthTotalColumn-2,new QTableWidgetItem(QString::number(currentMonth)));
            ui->tableWidget->setItem(r-1,monthTotalColumn-1,new QTableWidgetItem(QString::number(currentRatio)+QString("%")));

        }

        return;
    }

    newRowNum++;
    QRandomGenerator* rdGen=new QRandomGenerator;

    int restYanfaC,restWaijieJishuC,restWaijieChanpinC,restQitaC;


    //根据这个人的工号 + 当月项目
    foreach (QString jobNumKey, jobNumCurrentMonthProjectInfo.keys()) {
        int ratio;
        int restYanfa=0;
        int restWaijijishu=0;
        int restWaijichanpin=0;
        int restWaiji=0;
        int restQita=0;

        /*占比分配算法*/
        //这个工号岗位为什么岗位
        int jobNumTypeFlag;
        if(jobNumPostTypeInfo[jobNumKey].contains("研发")){

            jobNumTypeFlag=1;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("生产")){

            jobNumTypeFlag=2;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("质量")){

            jobNumTypeFlag=3;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("采购")){

            jobNumTypeFlag=4;
        }
        if(jobNumPostTypeInfo[jobNumKey].contains("其他")){

            jobNumTypeFlag=5;
        }
        /**/

        //确定这个人当天的项目个数
        int prjCount=jobNumCurrentMonthProjectInfo[jobNumKey].size();

        if(prjCount==0)
            continue;

        //项目类型
        //先分配占比 防止工号当前月没有外接项目造成比例错误的情况

        auto tmpPrjTypeCount=jobNumCurrentMonthPrjTypeInfo;
        //当天的各个类型项目数
        restYanfaC=tmpPrjTypeCount[jobNumKey]["研发"];
        restWaijieJishuC=tmpPrjTypeCount[jobNumKey]["外接技术"];
        restWaijieChanpinC=tmpPrjTypeCount[jobNumKey]["外接产品"];
        restQitaC=tmpPrjTypeCount[jobNumKey]["其他"];

        switch (jobNumTypeFlag) {
            case 1://研发岗位
                restQita=0;
                //有外接技术
                if((restWaijieJishuC)>0){
                    //外接产品
                    if(restWaijieChanpinC>0){
                        restWaijichanpin=rdGen->bounded(0,10);
                        restWaijijishu=rdGen->bounded(0,20-restWaijichanpin);
                        restYanfa=100-restWaijijishu-restWaijichanpin;
                    }
                    else{
                        restWaijijishu=rdGen->bounded(0,20);
                        restYanfa=100-restWaijijishu;
                    }

                }
                //有产品 无技术
                if(restWaijieChanpinC>0){
                    if(restWaijieJishuC==0){
                        restWaijichanpin=rdGen->bounded(0,10);
                        restYanfa=100-restWaijichanpin;
                    }
                }

                if((restWaijieJishuC+restWaijieChanpinC)==0){
                    restYanfa=100;
                }

            break;
            case 2://生产岗位
                restQitaC=1;
                if(!jobNumCurrentMonthProjectInfo[jobNumKey].contains(QString("管理工作")))
                    jobNumCurrentMonthProjectInfo[jobNumKey].push_back(QString("管理工作"));
                restYanfa=0;
                restWaijijishu=0;
                //有外接产品
                if((restWaijieChanpinC)>0){
                    //you其他
                    if( restQitaC>0){
                        restWaijichanpin=rdGen->bounded(0,100);
                        restQita=100-restWaijichanpin;
                    }
                    else{
                        restWaijichanpin=100;
                        restQita=0;
                    }

                }
                //有其他  无产品
                if(restQitaC>0){
                    if(restWaijieChanpinC==0){
                        restQita=100;
                        restWaijichanpin=0;
                    }
                }


            break;
            case 3://质量岗位
                restQitaC=1;
                if(!jobNumCurrentMonthProjectInfo[jobNumKey].contains(QString("管理工作")))
                    jobNumCurrentMonthProjectInfo[jobNumKey].push_back(QString("管理工作"));
                restYanfa=0;
                //有外接  //肯定有其他项目
                //外接合计不超过0.6
                //有产品
                if(restWaijieChanpinC>0){
                    //有技术
                    if(restWaijieJishuC>0){
                        restWaijijishu=rdGen->bounded(0,50);
                        restWaijichanpin=rdGen->bounded(0,60-restWaijijishu);
                        restQita=100-restWaijijishu-restWaijichanpin;
                    }
                    //无技术
                    else{
                        restWaijichanpin=rdGen->bounded(0,60);
                        restWaijijishu=0;
                        restQita=100-restWaijijishu-restWaijichanpin;
                    }
                }
                //有技术
                if(restWaijieJishuC>0){
                    //没产品
                    if(restWaijieChanpinC==0){
                        restWaijijishu=rdGen->bounded(0,50);
                        restWaijichanpin=0;
                        restQita=100-restWaijijishu-restWaijichanpin;
                    }
                }
                if((restWaijieJishuC+restWaijieChanpinC)==0){
                    restQita=100;
                    restWaijijishu=0;
                    restWaijichanpin=0;
                }

            break;
            case 4://采购岗位
                restQitaC=1;
                if(!jobNumCurrentMonthProjectInfo[jobNumKey].contains(QString("管理工作")))
                    jobNumCurrentMonthProjectInfo[jobNumKey].push_back(QString("管理工作"));
                restYanfa=0;
                restWaijijishu=0;
                //有产品
                if(restWaijieChanpinC>0){
                    //无其他
                    if(restQitaC==0){
                        restWaijichanpin=100;
                    }
                    //有其他
                    if(restQitaC>0){
                        restWaijichanpin=rdGen->bounded(0,100);
                        restQita=100-restWaijichanpin;
                    }
                }
                //无产品
                if(restWaijieChanpinC==0){
                    if(restQitaC>0){
                        restQita=100;
                    }
                }
            break;
            case 5://其他岗位
                restQita=0;
                restYanfa=0;
                restWaiji=0;
                restWaijijishu=0;
                restWaijichanpin=0;
            break;
        }

        prjCount=jobNumCurrentMonthProjectInfo[jobNumKey].size();
        qDebug()<<"jobNum："+jobNumKey+"项目个数:"+prjCount;

        for (int numPrj=0;numPrj<prjCount;numPrj++) {
            QVector<QString> rowTmp;
            if(jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]==("管理工作")){

                ratio=restQita;
                rowTmp<<jobNumKey<<jobNumHumanInfo[jobNumKey]<<jobNumPostTypeInfo[jobNumKey]<<jobNumDepartmentInfo[jobNumKey][0]+"-"+jobNumDepartmentInfo[jobNumKey][1]
                        <<QString("管理工作")<<QString("管理工作")
                      <<jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]<<QString("管理工作")
                     <<QString("管理工作")<<QString::number(currentMonth)<<QString::number(ratio)+QString("%");
                appendRowInAssemableTable(newRowNum,rowTmp);
                qDebug()<<rowTmp;
                newRowNum++;
                continue;
            }

            //根据工号岗位和项目类型及属性共同确定这个项目的占比分配
            //当前项目属性
            //项目为研发
            if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0].contains("研发")){
                if(restYanfaC==1){
                    ratio=restYanfa;
                    restYanfa-=restYanfa;
                }
                else{
                    ratio=rdGen->bounded(0,restYanfa);
                    restYanfa-=ratio;
                    restYanfaC-=1;
                }

            }

            if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0].contains("外接")){
                if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1].contains("技术")){
                    if(restWaijieJishuC==1){
                        ratio=restWaijijishu;
                        restWaijijishu-=restWaijijishu;
                    }
                    else{

                        ratio=rdGen->bounded(0,restWaijijishu);
                        restWaijijishu-=ratio;
                        restWaijieJishuC-=1;
                    }

                }
                if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1].contains("产品")){
                    if(restWaijieChanpinC==1){
                        ratio=restWaijichanpin;
                       restWaijichanpin-=restWaijichanpin;
                    }
                    else{
                        ratio=rdGen->bounded(0,restWaijichanpin);
                        restWaijichanpin-=ratio;
                        restWaijieChanpinC-=1;
                    }

                }
            }
            if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0].contains("管理工作")){
                if(restQitaC==1){
                    ratio=restQita;
                    restQita-=restQita;
                }
                else{
                    ratio=rdGen->bounded(0,restQita);
                    restQita-=ratio;
                    restQitaC-=1;
                }

            }


            //工时占比需要由配置的项目类型占比比例进行分配，分配原则为最小分配比例1%

            rowTmp<<jobNumKey<<jobNumHumanInfo[jobNumKey]<<jobNumPostTypeInfo[jobNumKey]<<jobNumDepartmentInfo[jobNumKey][0]+"-"+jobNumDepartmentInfo[jobNumKey][1]
                    <<projectDepartmentInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0]<<projectDepartmentInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1]
                  <<jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]<<projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][0]
                 <<projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][numPrj]][1]<<QString::number(currentMonth)<<QString::number(ratio)+QString("%");



            appendRowInAssemableTable(newRowNum,rowTmp);
            qDebug()<<rowTmp;
            newRowNum++;
        }

    }

    qDebug()<<"共写入行："+QString::number(newRowNum);
    qDebug()<<"当前共有行:"<<QString::number(ui->tableWidget->rowCount());
    ui->tableWidget->setRowCount(newRowNum);


}


//分配工时
void MainWindow::assignWorkingHourRatio(int jobNumType,int restYanfa,int restWaijie,int restQita,int restYanfaRatio,int restWaijieRatio,int restQitaRatio){

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
    if(selectFile.isNull()){
        return;
    }

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
            if(ui->tableWidget->item(r,c)!=nullptr)
                ui->tableWidget->item(r,c)->setFlags(itemF);

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
            if(ui->tableWidget->item(r-1,c-1)!=nullptr){
                ui->tableWidget->item(r-1,c-1)->setTextAlignment(Qt::AlignCenter);
                auto data=ui->tableWidget->item(r-1,c-1)->text();
                xlsxExcel.write(r,c,data);
            }
        }
    }

    xlsxExcel.save();

    qDebug()<<"save excel successfully";
    setTableItemEditable(false);
}

/*配置岗位占比类型*/
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
    jobNumCurrentMonthPrjTypeInfo.clear();
    //更新humanCurrentMonthProjectInfo
    //1.projectHumansInfo 项目-人名
    //2.humanWorkdayRowInfo.keys() 当前月的人名
    //3.必须确保当前月有该项目，项目周期和当前月的匹配

    //统计工号对应的项目
    int prjStartYear,prjEndYear,prjStartMonth,prjEndMonth;
    foreach (QString jobNum, jobNumWorkdayRowInfo.keys()) {
        foreach (QString prjName, projectHumansInfo.keys()) {
            prjStartYear=projectDateInfo[prjName][0].year();
            prjEndYear=projectDateInfo[prjName][1].year();
            prjStartMonth=projectDateInfo[prjName][0].month();
            prjEndMonth=projectDateInfo[prjName][1].month();
            //统计当前年月的项目信息
            if(currentYear==prjStartYear ){
                if(currentYear==prjEndYear){
                    if(currentMonth>=prjStartMonth && currentMonth<=prjEndMonth)
                    {
                        foreach(QString humanInfo,projectHumansInfo[prjName]){
                            //jobNumHumanInfo[
                            if(humanInfo.contains(jobNum)){
                                jobNumCurrentMonthProjectInfo[jobNum].push_back(prjName);
                                qDebug()<<jobNum+":prj"+prjName+"prj start:"<<QString::number(prjStartYear)+"年"+QString::number(prjStartMonth)+"prj end:"<<QString::number(prjEndYear)+"年"+QString::number(prjEndMonth);
                            }
                        }

                    }
                }
                if(currentYear<prjEndYear){
                    if(currentMonth>=prjStartMonth)
                    {
                        foreach(QString humanInfo,projectHumansInfo[prjName]){
                            //jobNumHumanInfo[
                            if(humanInfo.contains(jobNum)){
                                jobNumCurrentMonthProjectInfo[jobNum].push_back(prjName);
                                qDebug()<<jobNum+":prj"+prjName+"prj start:"<<QString::number(prjStartYear)+"年"+QString::number(prjStartMonth)+"prj end:"<<QString::number(prjEndYear)+"年"+QString::number(prjEndMonth);
                            }
                        }
                    }
                }

            }

            if(currentYear>prjStartYear){
                if( currentYear<prjEndYear){
                    foreach(QString humanInfo,projectHumansInfo[prjName]){
                        //jobNumHumanInfo[
                        if(humanInfo.contains(jobNum)){
                            jobNumCurrentMonthProjectInfo[jobNum].push_back(prjName);
                            qDebug()<<jobNum+":prj"+prjName+"prj start:"<<QString::number(prjStartYear)+"年"+QString::number(prjStartMonth)+"prj end:"<<QString::number(prjEndYear)+"年"+QString::number(prjEndMonth);
                        }
                    }
                }
                if(currentYear==prjEndYear){
                    if(currentMonth<=prjEndMonth){
                        foreach(QString humanInfo,projectHumansInfo[prjName]){
                            //jobNumHumanInfo[
                            if(humanInfo.contains(jobNum)){
                                jobNumCurrentMonthProjectInfo[jobNum].push_back(prjName);
                                qDebug()<<jobNum+":prj"+prjName+"prj start:"<<QString::number(prjStartYear)+"年"+QString::number(prjStartMonth)+"prj end:"<<QString::number(prjEndYear)+"年"+QString::number(prjEndMonth);
                            }
                        }
                    }
                }

            }

        }
    }

    //统计工号对应项目的类型和性质
    foreach(QString jobNumKey,jobNumCurrentMonthProjectInfo.keys()){
        for(int i=0;i<jobNumCurrentMonthProjectInfo[jobNumKey].size();i++){
            //第i个项目的信息 /项目类型
            if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][i]][0].contains("研发")){
                jobNumCurrentMonthPrjTypeInfo[jobNumKey]["研发"]+=1;
            }
            if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][i]][0].contains("外接")){
                if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][i]][1].contains("技术")){
                    jobNumCurrentMonthPrjTypeInfo[jobNumKey]["外接技术"]+=1;
                }
                if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][i]][1].contains("产品")){
                    jobNumCurrentMonthPrjTypeInfo[jobNumKey]["外接产品"]+=1;
                }
            }
            if(projectTypeInfo[jobNumCurrentMonthProjectInfo[jobNumKey][i]][0].contains("其他")){
                jobNumCurrentMonthPrjTypeInfo[jobNumKey]["其他"]+=1;
            }
        }

           // jobNumCurrentMonthPrjTypeInfo
    }

}
