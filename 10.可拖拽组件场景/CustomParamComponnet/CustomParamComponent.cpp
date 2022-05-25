#include "CustomParamComponent.h"

CustomParamComponent::CustomParamComponent(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    


    customPlot = new CUSTOM_PLOT::CustomPlot();
    customPlot->setBaseSize(size());
    QVector<double> xData;
    QVector<double> yData;

    customPlot->setCurveXYAxis(0, 10, 0, 5, QString::fromLocal8Bit("xÖá"),QString::fromLocal8Bit( "yÖá"));

    customPlot->addOneCurve("test1",Qt::blue);
    customPlot->addOneCurve("test2", Qt::black);

    xData = { 0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0 };
    yData = { 0.0,1.0,2.0,3.0,5.0,3.0,2.0,1.0 };
    
    customPlot->setDataSource(0, xData, yData);

    customPlot->setDataSource(1, yData, xData);

    customButton = new CUSTOM_PUSHBUTTON::CustomPushButton("testtext");
    //customCurvePlot = new CUSTOM_CURVE_PLOT::CustomCurvePlot();
    customLabel = new CUSTOM_LABEL::CustomLabel("textLabel");
    dropWidget =new DROP_WIDGET::DropWidget(this);
    


    customPlot->resize(QSize(100, 60));
    customPlot->move(QPoint(60, 60));


    customListWidget = new CUSTOM_LIST_WIDGET::CustomListWidget(this);
    
    dropWidget->setMinimumWidth(500);
    

    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/boat.png")), "1"));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/car.png")), "2"));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/house.png")), "3"));

    customListWidget->setFixedWidth(100);

    QFrame* attributeWidget = new QFrame;
    attributeWidget->setFixedWidth(200);

    QGridLayout* layout = new QGridLayout();

    layout->addWidget(dropWidget,0,1);
    layout->addWidget(customListWidget, 0, 0);
    layout->addWidget(attributeWidget, 0, 2);

    ui.centralWidget->setLayout(layout);

}


CustomParamComponent::~CustomParamComponent() {

}

