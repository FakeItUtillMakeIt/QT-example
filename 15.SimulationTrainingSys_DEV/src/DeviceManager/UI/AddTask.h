#pragma once

#include <QWidget>
#include<QEvent>
#include<QPainter> 
#include <QMouseEvent>
#include <qmath.h>
#include "ui_AddTask.h"
#include "../AppCache.h"

using namespace std;
struct TaskInfo
{
	QString jobName;
	QString softName;

};

class AddTask : public QWidget
{
	Q_OBJECT

public:
	AddTask(string rocktName,QWidget *parent = nullptr);
	~AddTask();

protected:

	QPoint mLastMousePosition;
	bool mMoving;
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

private:
	Ui::AddTaskClass ui;
	AppCache* m_app;
	QString m_rocktName;

	void Init();
	void IsOK();
	void IsCancel();

	bool ValidityCheck(TaskInfo oneTask);

signals:

	void addTask(TaskInfo oneTask);
};
