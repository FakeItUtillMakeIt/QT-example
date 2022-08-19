#pragma once

#ifndef ALL_INFO_CONFIG_WIDGET_H
#define ALL_INFO_CONFIG_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "../UI/WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"


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

	static void closeInstance() {
		if (instance != nullptr)
		{
			instance->deleteLater();
		}
	}

	void setCurrentUI(DeviceCommonVaries::InfoWidgetFlag);

private:
	AllInfoConfigWidget(QWidget* parent = nullptr);
	~AllInfoConfigWidget();
	void InitUILayout();
	void widgetConfig();
	void initConnect();

private:

	//�ϲ���
	QLabel* windowIcon;
	QLabel* windowTitle;
	QPushButton* windowCloseBtn;
	QFrame* hframe1;
	//�²���
	QFrame* hframe2;
	QPushButton* cancelBtn;;
	QPushButton* okBtn;

	//����ͺ�
	//�����
	//�ͺŲ���
	QLabel* rocketTypeParamTitle;
	QLineEdit* searchLineEdit;
	QTreeWidget* deviceParamTree;
	QCheckBox* selectAllBox;
	QCheckBox* cancelAllBox;
	//�Ҳ���
	//��ѡ����
	QLabel* selectParamTitle;
	QLineEdit* searchSelect;
	QListWidget* hadSelectedParamsL;

	//�豸����
	//���
	QLineEdit* searchDeviceCfg;
	QPushButton* addDeviceCfg;
	QListWidget* deviceCfgList;
	//�Ҳ�1�豸״̬
	QGridLayout* devStatusLayout;
	//�Ҳ�2�豸����
	QLineEdit* searchDevParam;
	QGridLayout* devParamLayout;

	//ָ������
	//���
	QLineEdit* searchCmdCfg;
	QPushButton* addCmdCfg;
	QListWidget* cmdCfgList;
	//��1
	QLabel* deviceLabel;
	QComboBox* deviceCombox;
	QGridLayout* deviceStatLayout;
	//��2
	QLineEdit* searchCmdFrame;
	QLabel* cmdFrameLabel;
	QPushButton* addCmdFrame;
	QTableWidget* cmdFrameTable;



	QPoint mLastMousePosition;
	bool mMoving;

	QGridLayout* UIGrid;

private:
	QLayout* publicTopLayout();
	QLayout* publicBottomLayout();

	QLayout* initRocketConfigLayout();
	QLayout* initDeviceConfigLayout();
	QLayout* initCommandConfigLayout();



protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private slots:
	void clickedCancelBtn();
	void clickedOkBtn();

};


#endif