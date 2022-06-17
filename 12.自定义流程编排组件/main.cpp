#include <QtWidgets/QApplication>
#include "CustomFlowEditWidget.h"

using namespace CUSTOM_FLOW_EDIT_WIDGET;

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	CustomFlowEditWidget* w = new CustomFlowEditWidget;
	
	w->show();

	return a.exec();
}
