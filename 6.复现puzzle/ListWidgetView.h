#pragma once
#include <QtWidgets>



class ListWidgetView:public QListWidget
{

signals:
	void emitdrag(int);
public:
	ListWidgetView();
	int indexofItem;

protected:
	void startDrag(Qt::DropActions supportedActions);

private slots:
	void dealPressEvent(QListWidgetItem* item);



};

