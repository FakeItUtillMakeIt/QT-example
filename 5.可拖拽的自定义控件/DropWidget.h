#pragma once
#include <QtWidgets>
class DropWidget:public QWidget
{
public:
	DropWidget();
	~DropWidget();

	int typeint;

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);
	void mousePressEvent(QMouseEvent* event);

protected slots:
	void dealPressEvent(int number);

};
//零壹初印象
//见过清晨第一缕阳光下的来时零壹路
//也走过暮夜虫鸟齐鸣中的走时零壹路
//不变是你努力拼搏的模样

