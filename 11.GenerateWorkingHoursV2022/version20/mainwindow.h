#pragma execution_character_set("utf-8")

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXlsx>
#include <QtWidgets>
#include <QMap>

#include "posttypeconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


enum CURRENT_PAGE{
    PROJECT_WORKINGHOURS_GENERATE,
    DING_CHECK_DATA,
    GENERATE_RATIO_CONFIG,
    PROJECT_DATA_CONFIG,
    PROJECT_DATA_CALC
};

struct prj2DateRatio{
    QString prjName;
    QVector<int> column;
    QVector<QString> ratio;
};

struct prj2DateRatio1{
    int column;
    QString ratio;
};

struct workNumPrj{
    QString workNum;
    QString prjName;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int cycleNum=26;
    char columnStartName='A';
    int currentColumnNum=0;
    int currentRowNum=1;
    char columnName;
    QString elementLoc;

    bool isImportDingCheckData;
    bool isNewImportDingCheckFlag;
    bool isDayHourGFlag;

    QString dingCheckImportFilename;
    QString hoursRatioConfigImportFilename;
    QString projectConfigImportFilename;
    QString generateHoursImportFilename;

    CURRENT_PAGE currentPage;
    bool isEditFlag;
    //是否点击组装
    bool isAssemableHoursFlag;

    /*正则表达式*/
    QRegExp dateRegFormat;
    QRegExp dingCheckRegFormat;
    QRegExp dimissionRegFormat;
    QRegExp breakDateRegFormat;
    QRegExp vacateRegFormat;


    //岗位占比配置信息
    QMap<QString,QMap<QString,QVector<qreal>>> postTypeConfigData;
    //工号和岗位对应关系
    QMap<QString,QString> jobNumPostTypeInfo;
    //工号和人对应
    QMap<QString,QString> jobNumHumanInfo;
    //项目和人的关系
    QMap<QString,QVector<QString>> projectHumansInfo;
    //项目和时间对应关系
    QMap<QString,QVector<QDate>> projectDateInfo;
    //项目和部门对应关系
    QMap<QString,QVector<QString>> projectDepartmentInfo;
    //项目和项目类型/项目属性关系
    QMap<QString,QVector<QString>> projectTypeInfo;
    //工号和部门--根据工时人员信息分类得到所属公司，一级部门，二级部门关系
    QMap<QString,QVector<QString>> jobNumDepartmentInfo;
    //工号和离职时间
    QMap<QString,QDate> jobNumLeaveTimeInfo;
    //工号-工作日所在行统计  应该分配工时的工作日
    QMap<QString,QVector<QString>> jobNumWorkdayRowInfo;
    //生成数据工号和所在行关系
    QMap<QString,QVector<int>> jobNumInRowInfo;

    //获取当前处理表的月份
    int currentMonth;
    int currentYear;
    //2022/07/12 每日项目数最大为4
    //每月项目数满足全覆盖
    //项目最小比例为5
    //每日项目符合当日条件
    int prjMinRatio=5;
    int everydayMaxPrjCount=3;
    QMap<QString,QVector<QString>> jobNumCurrentDayProjectInfo;
    QMap<QString,QMap<QString,int>> jobNumCurrentDayPrjTypeInfo;
    //工号-当月项目的总数关系
    QMap<QString,QVector<QString>> jobNumCurrentMonthProjectInfo;
    //工号-当月项目的类型关系
    QMap<QString,QMap<QString,int>> jobNumCurrentMonthPrjTypeInfo;

    QMap<QString,int> dayToColumnNum;

    //岗位配置界面
    PostTypeConfig* postTyprCfgWidget;

    //工时生成页的excel数据变量
    QXlsx::Document* assemablesExcel=nullptr;
    QXlsx::Document* dingCheckExcel=nullptr;
    QXlsx::Document* hourRatioExcel=nullptr;
    QXlsx::Document* projectDataExcel=nullptr;

    bool haveDayHoursDataFlag=false;

private:
    Ui::MainWindow *ui;

private:
    void initConnect();

    //
    void setTableItemEditable(bool editFlag);
    //
    void saveExcelData(QString saveFileName);

    void dingCheckImport();
    void hoursRatioConfigImport();
    void projectDataConfigImport();
    void projectWorkingHoursGenerate();

    void pageButtonStateChange(CURRENT_PAGE page);

    //计算人员当月项目信息
    void calHumanCurrentMonthProjectInfo();
    //添加一行至组装工时表
    void appendRowInAssemableTable(int rowNum,QVector<QString> dataList);
    //修改已添加行的休息
    void changeAssemableColumnInfo(int rowNum,int columnNum,QString info);

    //分配工时
    void assignWorkingHourRatio(int jobNumType,int restYanfa,int restWaijie,int restQita,int restYanfaRatio,int restWaijieRatio,int restQitaRatio);

    //判断每日工时是否为1
    void judgeWorkdatHoursException();
private slots:

    void projectHoursGeneratePage();
    void dingCheckWorkDataPage();
    void hoursRatioConfigPage();
    void projectDataConfigPage();

    void projectDataCalcPage();


    //组装工时数据
    void assemblWorkingHoursData();
    void assemblWorkingHoursMonthData();

    //导出项目工时
    void exportProjectHours();
    //岗位类型配置
    void configPostType();


    //导入考勤数据
    void importDintCheckData();
    //保存配置信息
    void saveConfigInfo();
    //新增数据
    void insertData();
    //编辑数据
    void editData();
    //删除数据
    void deleteData();

    void clearHoursData();

    /*选定多个文件*/
    void selectMutliFile();

};
#endif // MAINWINDOW_H
