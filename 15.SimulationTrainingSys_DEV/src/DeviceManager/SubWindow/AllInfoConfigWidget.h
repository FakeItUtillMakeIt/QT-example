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

	static void closeInstance() {
		if (instance != nullptr)
		{
			instance->deleteLater();
		}
	}
	//设置相关
	void setWindowTitle(QString);
	void setCurrentUI(DeviceCommonVaries::InfoWidgetFlag);
	void setInfoWidgetCfg(int rocketId, int deviceId, int cmdId);

	int delExecResult();
	void displaySuccsInfo(QString textinfo);

	int displayMsgInfo(QString title, QString textinfo, QIcon icon, QMessageBox::Icon typee);

private:
	AllInfoConfigWidget(QWidget* parent = nullptr);
	~AllInfoConfigWidget();
	//初始化相关
	void InitUILayout();
	void widgetConfig();
	void initConnect();

	//数据加载相关
	void loadRocketInfoData();
	void loadDeviceInfoData();
	void loadCmdInfoData();

private:
	int rocketID;
	int deviceID;
	int cmdID;

	DeviceCommonVaries::InfoWidgetFlag curWidget;

	//上侧栏
	QLabel* windowIcon;
	QLabel* windowTitle;
	QPushButton* windowCloseBtn;
	QFrame* hframe1;
	//下侧栏
	QFrame* hframe2;
	QPushButton* cancelBtn;;
	QPushButton* okBtn;

	//火箭型号
	//左侧栏
	//型号参数
	QLineEdit* rocketSearch;
	QPushButton* addCommuProto;
	QListWidget* rocketComProtoList;
	QLabel* rocketTypeParamTitle;
	QLineEdit* searchLineEdit;
	QTreeWidget* deviceParamTree;
	QCheckBox* selectAllBox;
	QCheckBox* cancelAllBox;
	//右侧栏
	//已选参数
	QLabel* selectParamTitle;
	QLineEdit* searchSelect;
	QListWidget* hadSelectedParamsL;

	//设备配置
	//左侧
	QLineEdit* searchDeviceCfg;
	QPushButton* addDeviceCfg;
	QListWidget* deviceCfgList;
	//右侧1设备状态
	QVector<QWidget*> deviceStatList;
	QVector<QWidget*> deviceParamList;
	QGridLayout* devStatusLayout;
	QScrollArea* scrollAreaStat;
	QPushButton* addNewStat;
	//右侧2设备参数

	QLineEdit* searchDevParam;
	QGridLayout* devParamLayout;
	QScrollArea* scrollAreaParam;

	//指令配置
	//左侧
	QLineEdit* searchCmdCfg;
	QPushButton* addCmdCfg;
	QListWidget* cmdCfgList;
	//右1
	unordered_map<int, vector<string>> cmdDevStatInfo;
	QLabel* deviceLabel;
	QComboBox* deviceCombox;
	QGridLayout* deviceStatLayout;
	QScrollArea* scrollAreaDevStat;
	QListWidget* hadSelectDevStat;
	//右2
	QLineEdit* searchCmdFrame;
	QLabel* cmdFrameLabel;
	QPushButton* addCmdFrame;
	QTableWidget* cmdFrameTable;
	//
	QPoint mLastMousePosition;
	bool mMoving;

	QGridLayout* UIGrid;
	QWidget* rocketWidget;
	QWidget* deviceWidget;
	QWidget* commandWidget;

private:
	QLayout* publicTopLayout();
	QLayout* publicBottomLayout();

	void initRocketConfigLayout();
	void initDeviceConfigLayout();
	void initCommandConfigLayout();

	void deviceStatContentUpdate();
	void deviceParamContentUpdate();

protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private slots:
	void clickedCancelBtn();
	void clickedOkBtn();

};


#endif