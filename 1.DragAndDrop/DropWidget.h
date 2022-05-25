#pragma once
#include <QtWidgets>
class DropWidget:public QWidget
{
public:
	DropWidget();
	~DropWidget();

protected:
	void dragEnterEvent(QDragEnterEvent* event);
	void dragLeaveEvent(QDragLeaveEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	void dropEvent(QDropEvent* event);

};

