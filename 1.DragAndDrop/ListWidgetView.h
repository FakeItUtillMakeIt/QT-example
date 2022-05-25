#pragma once
#include <QtWidgets>


class ListWidgetView:public QListWidget
{

protected:
	void startDrag(Qt::DropActions supportedActions);

};

