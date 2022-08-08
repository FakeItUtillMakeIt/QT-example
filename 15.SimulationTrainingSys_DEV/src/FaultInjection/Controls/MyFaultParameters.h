#pragma once
#pragma execution_character_set("utf-8")
#include <QLineEdit>
#include <QPushButton> 
#include <QCheckBox>
#include <QLabel>  
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QScrollArea>
using namespace std;

struct ItemStruct
{
	int ID;
	int type;//1：参数故障；2：指令故障
	int code;
	QString Name;

	int FaultCommandID;//故障指令id
	vector<int> responseCommandID;//受影响指令id
	vector<int> deviceParamInfoID;//参数id  device_param_info_id
	int deviceID;//设备id

	QString createTime;
	QString lastUpdateTime;
};

class MySonButton : public QWidget {

	Q_OBJECT//使用信号槽要此宏
public:
	MySonButton(QString instance_name, QWidget* parent) : QWidget(parent)
		, m_instanceName(instance_name)
	{  
		m_wgt = new QWidget(this);
		m_wgt->setGeometry(QRect(0, 0, 247, 30));
		m_wgt->setStyleSheet("background-color:rgb(251, 251, 251);border:1px solid grey;");// border:0px;
		m_oneButton = new QPushButton(this);
		m_oneButton->setGeometry(QRect(0, 0, 247, 30));
		m_oneButton->setText(instance_name);
		//m_oneButton->setStyleSheet("border-image:url(:/FaultInjection/images/故障默认未点击.png);border:0px;font: 12pt 微软雅黑;");
		m_oneButton->setStyleSheet("QPushButton:hover{border-image:url(:/FaultInjection/images/故障悬浮.png);"
			"font:12pt 微软雅黑;"
			"border:0px;}"
			"QPushButton:pressed{border-image:url(:/FaultInjection/images/故障默认未点击.png);"
			"font:12pt 微软雅黑;"
			"border:0px;}"
			"QPushButton{border-image:url(:/FaultInjection/images/故障默认未点击.png);"
			"font:12pt 微软雅黑;"
			"border:0px;}");
		
		connect(m_oneButton, &QPushButton::clicked, this, &MySonButton::onSingleClick);
		m_oneButton->setCursor(Qt::PointingHandCursor);

	}

	~MySonButton()
	{
		delete m_oneButton;
		//delete m_oneIcon;
	}

	//取消设定故障状态显示
	void selectItem(bool is_selected)
	{
		if (is_selected)
		{
			//m_oneButton->setStyleSheet("border-image:url(:/FaultInjection/images/故障配置成功.png);border:0px;font: 12pt 微软雅黑;");
			m_oneButton->setStyleSheet("QPushButton:hover{border-image:url(:/FaultInjection/images/故障悬浮.png);"
				"font:12pt 微软雅黑;"
				"border:0px;}"
				"QPushButton:pressed{border-image:url(:/FaultInjection/images/故障配置成功.png);"
				"font:12pt 微软雅黑;"
				"border:0px;}"
				"QPushButton{border-image:url(:/FaultInjection/images/故障配置成功.png);"
				"font:12pt 微软雅黑;"
				"border:0px;}");
			
			
		}
		else
		{
			//m_oneButton->setStyleSheet("border-image:url(:/FaultInjection/images/故障默认未点击.png);border:0px;font: 12pt 微软雅黑;");
			
			m_oneButton->setStyleSheet("QPushButton:hover{border-image:url(:/FaultInjection/images/故障悬浮.png);"
				"font:12pt 微软雅黑;"
				"border:0px;}"
				"QPushButton:pressed{border-image:url(:/FaultInjection/images/故障默认未点击.png);"
				"font:12pt 微软雅黑;"
				"border:0px;}"
				"QPushButton{border-image:url(:/FaultInjection/images/故障默认未点击.png);"
				"font:12pt 微软雅黑;"
				"border:0px;}");
		
		}
	}

	void selecterror()
	{	
		m_oneButton->setStyleSheet("QPushButton:hover{border-image:url(:/FaultInjection/images/故障悬浮.png);"
			"font:12pt 微软雅黑;"
			"border:0px;}"
			"QPushButton:pressed{border-image:url(:/FaultInjection/images/故障配置异常.png);"
			"font:12pt 微软雅黑;"
			"border:0px;}"
			"QPushButton{border-image:url(:/FaultInjection/images/故障配置异常.png);"
			"font:12pt 微软雅黑;"
			"border:0px;}");
	}
	inline QString getName() {
		return m_instanceName;
	}

public:
	QPushButton* m_oneButton;

private:
	QString m_instanceName;

	QPushButton* m_oneIcon;
	QWidget* m_wgt;
	void onSingleClick()
	{
		emit onClick(this);
	}

signals: 
	void onClick(QObject *);
};
 
class MyFaultParameters : public QWidget
{
	Q_OBJECT

public:
	MyFaultParameters(QWidget* parent = Q_NULLPTR);
	~MyFaultParameters(); 
	void Init(vector<ItemStruct> instances);
	void addToolItem(ItemStruct inItem);
	void RemoveToolItem(QString text);
	void IsSelectParam(QString text,bool isSelect);
private:
	struct ToolItem
	{
		MySonButton* tab; //主节点显示区
		bool is_selected;
		int id;//故障的标志
		int code;//故障code
		int type;//1：参数故障；2：指令故障

		int FaultCommandID;//故障指令id
		vector<int> responseCommandID;//受影响指令id
		vector<int> deviceParamInfoID;//参数id  device_param_info_id
		int m_deviceID;//设备id

		QString createTime;
		QString lastUpdateTime;
	};
	vector<ToolItem> items; 
	QGridLayout* Gridlayout;
	QLineEdit edit;
	QPushButton button;   /*按钮是窗口*/

	//QSpacerItem* horizontalSpacer;
	QWidget* _parent;

	int m_itemNumber;

	QString m_clickButtonname;//记录当前点击的名称

	int m_index;
	void addItem(ItemStruct item, int itemNumber);
	void onClick(QObject* obj); 
	void setHeight();

public slots:
	void backonFaultClick(bool isOK);

signals:
	void onFaultClick(QString name, int code, int type);
};





/// <summary>
/// 用于查找参数
/// </summary>
class MySeachSlectButton : public QWidget {

	Q_OBJECT//使用信号槽要此宏
public:
	MySeachSlectButton(QString instance_name, QWidget* parent) : QWidget(parent)
		, m_instanceName(instance_name)
	{
		m_wgt = new QWidget(this);
		m_wgt->setGeometry(QRect(0, 0, 740, 40));
		m_wgt->setStyleSheet("background-color: rgb(251, 251, 251);border:2px");// border:0px;
		m_oneButton = new QPushButton(this);
		m_oneButton->setGeometry(QRect(20, 10, 600, 20));
		m_oneButton->setText(instance_name);
		m_oneButton->setStyleSheet("text-align:left;color: rgb(54, 58, 68); border:0px;");
		//connect(m_oneButton, &QPushButton::clicked, this, &MySonButton::onSingleClick);
		m_oneButton->setCursor(Qt::PointingHandCursor);

		m_oneIcon = new QPushButton(this);
		m_oneIcon->setGeometry(QRect(680, 12, 18, 18));
		m_oneIcon->setText("");
		m_oneIcon->setStyleSheet("QPushButton {\n"
			"	background-color:transparent;\n"
			"	border-image: url(:/HarpiaTerminal/images/Input Fields／Checkbox／On.png);\n"
			"   border:0px;\n"
			"}");
		m_oneIcon->setCursor(Qt::PointingHandCursor);
		connect(m_oneIcon, &QPushButton::clicked, this, &MySeachSlectButton::onSingleClick);
	}

	~MySeachSlectButton()
	{
		delete m_oneButton;
		delete m_oneIcon;
	}

	void selectItem(bool is_selected)
	{
		if (is_selected)
		{
			m_oneIcon->setStyleSheet("QPushButton {\n"
				"	background-color:transparent;\n"
				"	border-image: url(:/HarpiaTerminal/images/Input Fields／Checkbox／Off.png);\n"
				"   border:0px;\n"
				"}");
			m_oneIcon->setGeometry(QRect(680, 12, 18, 18));

			m_oneButton->setStyleSheet("text-align:left;background-color: rgb(224, 239, 255);color: rgb(0, 0, 0); border:0px;");
			m_wgt->setStyleSheet("background-color: rgb(224, 239, 255); border:2px;");
		}
		else
		{
			m_oneIcon->setStyleSheet("QPushButton {\n"
				"	background-color:transparent;\n"
				"	border-image: url(:/HarpiaTerminal/images/Input Fields／Checkbox／On.png);\n"
				"   border:0px;\n"
				"}");
			m_oneIcon->setGeometry(QRect(680, 12, 18, 18));
			m_oneButton->setStyleSheet("text-align:left;background-color: rgb(246, 245, 248);color: rgb(54, 58, 68); border:0px;");
			m_wgt->setStyleSheet("background-color: rgb(251, 251, 251); border:0px;");
		}
	}
	inline QString getName() {
		return m_instanceName;
	}

public:
	QPushButton* m_oneButton;

private:
	QString m_instanceName;
	QPushButton* m_oneIcon;
	QWidget* m_wgt;


	void onSingleClick()
	{
		emit onClick(this);
	}

signals:
	void onClick(QObject*);
};

class MySeachSlectParamsTree : public QWidget
{
	Q_OBJECT

public:
	MySeachSlectParamsTree(vector<QString> instances, QWidget* parent = Q_NULLPTR);
	~MySeachSlectParamsTree();
	void selectItem(QString text);
private:
	struct ToolItem
	{
		MySeachSlectButton* tab; //主节点显示区
		QLabel* border;
		bool is_selected;
	};
	vector<ToolItem> items;

	QVBoxLayout* layout;
	int m_index;


	void addItem(QString text);
	void onClick(QObject* obj);
signals:
	void onSeachMainNodeClick(QString paramName,bool isSave);
};

