#pragma once

#include <QWidget>

/**

	@class   AllInfoConfigWidget
	@brief   �Ż����豸�������ý���
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
