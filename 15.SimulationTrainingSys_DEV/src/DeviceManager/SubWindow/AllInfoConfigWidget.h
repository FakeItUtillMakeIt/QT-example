#pragma once

#include <QWidget>

/**

	@class   AllInfoConfigWidget
	@brief   优化的设备管理配置界面
	@details ~

**/
class AllInfoConfigWidget : public QWidget
{
	Q_OBJECT

public:
	static AllInfoConfigWidget* instance;
	static AllInfoConfigWidget* getInstance() {
		if (instance == nullptr)
		{
			instance = new AllInfoConfigWidget;
		}
		return instance;
	}

private:
	AllInfoConfigWidget(QWidget* parent = nullptr);
	~AllInfoConfigWidget();
};
