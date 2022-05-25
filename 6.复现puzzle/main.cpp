#include "DragWidgetTest.h"
#include "ListWidgetView.h"
#include "DropWidget.h"
#include "instrumentpanel.h"
#include <QtWidgets/QApplication>
#include <QHBoxLayout>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget mainWidget;
    QGridLayout* horizontallayout = new QGridLayout(&mainWidget);
    /*horizontallayout->addWidget(new DragWidgetTest);
    horizontallayout->addWidget(new DragWidgetTest);*/

    ListWidgetView* listwidgetview = new ListWidgetView();
    DragWidgetTest* dragwidget = new DragWidgetTest();
    
	DropWidget* widget = new DropWidget();
    
    
	horizontallayout->addWidget(widget,0,0,1,2);

    horizontallayout->addWidget(listwidgetview,1,0);
    horizontallayout->addWidget(dragwidget,1,1);
    

    mainWidget.show();
    return a.exec();
}


