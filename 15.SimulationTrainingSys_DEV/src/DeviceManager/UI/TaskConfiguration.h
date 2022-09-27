#pragma once

#include <QWidget>
#include "ui_TaskConfiguration.h"

#include "../AppCache.h"
#include "AddTask.h"

#include "../Database/RocketDataDAO.h"

using namespace DataBase;

class TaskConfiguration : public QWidget
{
	Q_OBJECT

public:
	TaskConfiguration(QWidget *parent = nullptr);
	~TaskConfiguration();

private:
	Ui::TaskConfigurationClass ui;

	AppCache* m_app;
	AddTask* addTask;
	QStandardItemModel* m_tableModel;
	QToolButton* m_deleteBtn;

	RocketDataDAO* taskDataDAO;

	map<int, TaskManageInfo*> m_TaskManageSearchFrames;//²éÕÒµÄ

	bool m_isAdd;

	void Init();
	void TaskInit(bool isSeach);
	void FalshTableView();
	void DeleteOneTask();
	void SearchSoft(QString softName);
	void FalshSeachTableView();


public slots:

	void AddTaskFun(TaskInfo oneTask);
	void AddTaskShow();

};
