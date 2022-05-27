#pragma once

#ifndef CUSTOM_GROUP_BOX_H
#define CUSTOM_GROUP_BOX_H

#include <QWidget>
#include <QGroupBox>
#include "CustomCurvePlot.h"
#include "CustomLabel.h"

class CUSTOM_CURVE_PLOT::CustomCurvePlot;
class CUSTOM_LABEL::CustomLabel;

namespace CUSTOM_GROUP_BOX {

	enum CUSTOM_GROUP_TYPE
	{
		LABEL_GROUP,
		CURVE_PLOT
	};
 /**

     @class   CustomGroupBox
     @brief   外部包含一个Box的控件容器
     @details ~

 **/
	class CustomGroupBox
	{
	public:
		QGroupBox* groupBox;

	public:
		CustomGroupBox(QWidget* parent=nullptr);
		~CustomGroupBox();
		QGroupBox* getGroupWidget(CUSTOM_GROUP_TYPE type);
		void addWidget2Group(CUSTOM_GROUP_TYPE type);

	private:
		CUSTOM_CURVE_PLOT::CustomCurvePlot* customCurvePlot;
		CUSTOM_LABEL::CustomLabel* customLabel;

		QString styleSheet;
	};
}



#endif