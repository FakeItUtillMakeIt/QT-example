#pragma once
#pragma execution_character_set("utf-8")

#ifndef WIDGET_STYLESHEET_H
#define WIDGET_STYLESHEET_H


#include <QString>

class WidgetStyleSheet
{
private:
	WidgetStyleSheet() {
		styleSheetInit();
	}
	~WidgetStyleSheet() {

	}

public:
	static WidgetStyleSheet* instance;
	static WidgetStyleSheet* getInstace() {
		if (instance == nullptr)
		{
			instance = new WidgetStyleSheet;
		}
		return instance;
	}

	static void closeInstance() {
		if (instance)
		{
			instance->~WidgetStyleSheet();
		}
	}

	QString widgetStyleSheet;
	QString coverStyleSheet;
	QString itemStyleSheet;
	QString buttonStyleSheet;
	QString listStyleSheet;
	QString listItemStyleSheet;
	QString lineEditStyleSheet;
	QString comboxStyleSheet;
	QString labelStyleSheet;
	QString iconButtonStyleSheet;
	QString insertButtonStyleSheet;
	QString deleteButtonStyleSheet;

	QString backGround;

	QString infoConfigLabelStyleSheet;
	QString infoConfigLineEditStyleSheet;
	QString infoConfigPushButtonStyleSheet;
	QString infoConfigComboBoxStyleSheet;
	QString infoConfigFrameStyleSheet;


	void styleSheetInit();

};

#endif // WIDGET_STYLESHEET_H

