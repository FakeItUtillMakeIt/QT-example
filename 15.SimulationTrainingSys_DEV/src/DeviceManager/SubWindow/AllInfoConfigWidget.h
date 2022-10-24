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
	//�������
	void setWindowTitle(QString);
	void setCurrentUI(DeviceCommonVaries::InfoWidgetFlag);
	void setInfoWidgetCfg(int rocketId, int deviceId, int cmdId);

	int delExecResult();
	void displaySuccsInfo(QString textinfo);

	int displayMsgInfo(QString title, QString textinfo, QIcon icon, QMessageBox::Icon typee);

private:
	AllInfoConfigWidget(QWidget* parent = nullptr);
	~AllInfoConfigWidget();
	//��ʼ�����
	void InitUILayout();
	void widgetConfig();
	void initConnect();

	//���ݼ������
	void loadRocketInfoData();
	void loadDeviceInfoData();
	void loadCmdInfoData();

private:
	int rocketID;
	int deviceID;
	int cmdID;

	DeviceCommonVaries::InfoWidgetFlag curWidget;

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
	QLineEdit* rocketSearch;
	QPushButton* addCommuProto;
	QListWidget* rocketComProtoList;
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
	QVector<QWidget*> deviceStatList;
	QVector<QWidget*> deviceParamList;
	QGridLayout* devStatusLayout;
	QScrollArea* scrollAreaStat;
	QPushButton* addNewStat;
	//�Ҳ�2�豸����

	QLineEdit* searchDevParam;
	QGridLayout* devParamLayout;
	QScrollArea* scrollAreaParam;

	//ָ������
	//���
	QLineEdit* searchCmdCfg;
	QPushButton* addCmdCfg;
	QListWidget* cmdCfgList;
	//��1
	unordered_map<int, vector<string>> cmdDevStatInfo;
	QLabel* deviceLabel;
	QComboBox* deviceCombox;
	QGridLayout* deviceStatLayout;
	QScrollArea* scrollAreaDevStat;
	QListWidget* hadSelectDevStat;
	//��2
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