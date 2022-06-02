#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtXlsx>
#include <QtWidgets>

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

    QString dingCheckImportFilename;
    QString hoursRatioConfigImportFilename;
    QString projectConfigImportFilename;
    QString generateHoursImportFilename;

    CURRENT_PAGE currentPage;
    bool isEditFlag;

private:
    Ui::MainWindow *ui;

private:
    void initConnect();

    void projectHoursGeneratePage();
    void dingCheckWorkDataPage();
    void hoursRatioConfigPage();
    void projectDataConfigPage();

    //组装工时数据
    void assemblWorkingHoursData();
    //导出项目工时
    void exportProjectHours();

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

    //
    void setTableItemEditable(bool editFlag);
    //
    void saveExcelData(QString saveFileName);

    void dingCheckImport();
    void hoursRatioConfigImport();
    void projectDataConfigImport();
    void projectWorkingHoursGenerate();
};
#endif // MAINWINDOW_H
