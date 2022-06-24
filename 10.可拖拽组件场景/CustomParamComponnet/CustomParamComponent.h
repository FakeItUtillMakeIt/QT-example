#pragma once

#ifndef CUSTOM_PARAM_COMPONENT_H
#define CUSTOM_PARAM_COMPONENT_H



#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include <QObject>

#include "ui_CustomParamComponent.h"
#include "CustomPlot.h"
#include "CustomCurvePlot.h"
#include "CustomPushButton.h"
#include "CustomLabel.h"
#include "DropWidget.h"
#include "CustomListWidget.h"
#include "MysqlOP.h"


using namespace std;
/**
    @namespace CUSTOM_PARAM_COMPONENT
    @brief     组件命名空间
**/
namespace CUSTOM_PARAM_COMPONENT {

    /**

        @class   CustomParamComponent
        @brief   自定义组件类，容纳所有控件
        @details ~

    **/

    class CustomParamComponent : public QMainWindow
    {
        Q_OBJECT

    public:
	    CustomParamComponent(QWidget* parent = Q_NULLPTR);
	    ~CustomParamComponent();
	    QWidget* attributeWidget = new QWidget(this);
	    QGridLayout* windowLayout = new QGridLayout(this);

    private:
        Ui::CustomParamComponentClass ui;

        int selectElementIndex;

    public:
        CUSTOM_PLOT::CustomPlot* customPlot;
        CUSTOM_PUSHBUTTON::CustomPushButton* customButton;
        CUSTOM_LABEL::CustomLabel* customLabel;
        CUSTOM_CURVE_PLOT::CustomCurvePlot* customCurvePlot;
        DROP_WIDGET::DropWidget* dropWidget;
        CUSTOM_LIST_WIDGET::CustomListWidget* customListWidget;


        protected:
            void paintEvent(QPaintEvent* event);
   
        public slots:
            void displayAttributeWindow(QWidget&);

            void saveDropWidgetComponent();
    };
}



#endif