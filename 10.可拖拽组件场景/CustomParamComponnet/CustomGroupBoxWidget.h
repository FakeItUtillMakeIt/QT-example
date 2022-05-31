#pragma once

#ifndef CUSTOM_GROUP_BOX_WIDGET_H
#define CUSTOM_GROUP_BOX_WIDGET_H

#include <QWidget>
#include <QGroupBox>
#include "CustomCurvePlot.h"
#include "CustomLabel.h"
#include "CustomGroupBox.h"

class CUSTOM_CURVE_PLOT::CustomCurvePlot;
class CUSTOM_LABEL::CustomLabel;
class CUSTOM_GROUP_BOX::CustomGroupBox;

namespace CUSTOM_GROUP_BOX_WIDGET {

	enum CUSTOM_GROUP_WIDGET_TYPE
	{
		LABEL_GROUP,
		CURVE_PLOT
	};
 /**

     @class   CustomGroupBox
     @brief   外部包含一个Box的控件容器
     @details ~

 **/
	class CustomGroupBoxWidget
	{
	public:
		CUSTOM_GROUP_BOX::CustomGroupBox* groupBox;

	public:
		CustomGroupBoxWidget(QWidget* parent=nullptr);
		~CustomGroupBoxWidget();
		CUSTOM_GROUP_BOX::CustomGroupBox* getGroupWidget(CUSTOM_GROUP_WIDGET_TYPE type);
		void addWidget2Group(CUSTOM_GROUP_WIDGET_TYPE type);

	private:
		CUSTOM_CURVE_PLOT::CustomCurvePlot* customCurvePlot;
		CUSTOM_LABEL::CustomLabel* customLabel;

		QWidget* elementWidget;

		QString styleSheet;
	};
}



#endif