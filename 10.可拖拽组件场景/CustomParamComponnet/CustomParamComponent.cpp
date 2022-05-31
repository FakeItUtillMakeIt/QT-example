#include "CustomParamComponent.h"

using namespace CUSTOM_PARAM_COMPONENT;




CustomParamComponent::CustomParamComponent(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    /**/
    ////plot
    //customPlot = new CUSTOM_PLOT::CustomPlot();
    //customPlot->setBaseSize(size());
    //QVector<double> xData;
    //QVector<double> yData;

    //customPlot->setCurveXYAxis(0, 10, 0, 5, QString::fromLocal8Bit("x轴"),QString::fromLocal8Bit( "y轴"));

    //customPlot->addOneCurve("test1",Qt::blue);
    //customPlot->addOneCurve("test2", Qt::black);

    //xData = { 0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0 };
    //yData = { 0.0,1.0,2.0,3.0,5.0,3.0,2.0,1.0 };
    //
    //customPlot->setDataSource(0, xData, yData);

    //customPlot->setDataSource(1, yData, xData);

    //customPlot->resize(QSize(100, 60));
    //customPlot->move(QPoint(60, 60));
    /**/
    
  

	dropWidget = new DROP_WIDGET::DropWidget(this);

	dropWidget->setMinimumWidth(500);
    customListWidget = new CUSTOM_LIST_WIDGET::CustomListWidget(this);
    

    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/label.png").scaled(QSize(60,60))), QString::fromLocal8Bit("标签")));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/button.png").scaled(QSize(60, 60))), QString::fromLocal8Bit("按钮")));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/plot.png").scaled(QSize(60, 60))), QString::fromLocal8Bit("信号图")));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/groupbox.png").scaled(QSize(60, 60))), QString::fromLocal8Bit("容器")));

    customListWidget->setFixedWidth(90);

    //attributeWidget->setFixedWidth(200);
    //windowLayout->addWidget(attributeWidget, 0, 2);

    windowLayout->addWidget(dropWidget,0,1);
    windowLayout->addWidget(customListWidget, 0, 0);
    
    ui.centralWidget->setLayout(windowLayout);
    
}

    
CustomParamComponent::~CustomParamComponent() {

    QFrame* attributeWidget = new QFrame;
    attributeWidget->setFixedWidth(200);

}

 
void CustomParamComponent::paintEvent(QPaintEvent* event) {

}

/**
    @brief 调出属性窗口
    @param widget - 
**/
void CustomParamComponent::displayAttributeWindow(QWidget& widget) {
   
    /*this->findChild<QWidget*>("attr")->*/

	//QGridLayout* windowLay = static_cast<QGridLayout*>(this->layout());
	//if (windowLay->itemAtPosition(0, 2))
	//{
	//	auto itemPos = windowLay->itemAtPosition(0, 2)->widget();
 //       itemPos->hide();
	//	windowLay->removeWidget(itemPos);
 //       itemPos->close();
	//	
	//}
 //   /*this->windowLayout->addWidget(widget,0,2);
 //   widget->setParent(attributeWidget);*/
	//windowLay->addWidget(widget, 0, 2);
 //   widget->show();

    QGridLayout* windowLay = static_cast<QGridLayout*>(this->layout());
    if (windowLay->itemAtPosition(0, 2))
    {
        auto itemPos = windowLay->itemAtPosition(0, 2)->widget();
        itemPos->hide();
        windowLay->removeWidget(itemPos);
        
        //itemPos->close();

        //itemPos->show();

    }
   
	windowLay->addWidget(&widget, 0, 2);
	widget.show();
   
    

}