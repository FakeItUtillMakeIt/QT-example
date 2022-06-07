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
    PROJECT_DATA_CONFIG
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
    QMap<QString,QVector<qreal>> postTypeConfigData;
    //人和岗位对应关系
    QMap<QString,QString> humanPostTypeInfo;
    //项目和人的关系
    QMap<QString,QVector<QString>> projectHumansInfo;
    //项目和时间对应关系
    QMap<QString,QVector<QDate>> projectDateInfo;
    //项目和部门对应关系
    QMap<QString,QString> projectDepartmentInfo;
    //项目和项目类型关系
    QMap<QString,QString> projectTypeInfo;
    //人和部门--根据考勤表得到一级部门，二级部门关系
    QMap<QString,QVector<QString>> humanDepartmentInfo;
    //人-工作日所在行统计  应该分配工时的工作日
    QMap<QString,QVector<int>> humanWorkdayRowInfo;
    //获取当前处理表的月份
    int currentMonth;
    //人-当月项目的总数关系
    QMap<QString,QVector<QString>> humanCurrentMonthProjectInfo;



    //岗位配置界面
    PostTypeConfig* postTyprCfgWidget;

    //工时生成页的excel数据变量
    QXlsx::Document* assemablesExcel=nullptr;
    QXlsx::Document* dingCheckExcel=nullptr;
    QXlsx::Document* hourRatioExcel=nullptr;
    QXlsx::Document* projectDataExcel=nullptr;

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

private slots:

    void projectHoursGeneratePage();
    void dingCheckWorkDataPage();
    void hoursRatioConfigPage();
    void projectDataConfigPage();


    //组装工时数据
    void assemblWorkingHoursData();

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


};
#endif // MAINWINDOW_H
