#pragma once
#pragma execution_character_set("utf-8")
#include <QLineEdit>
#include <QPushButton> 
#include <QLabel>  
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include "MyFaultParameters.h" 


using namespace std;


class MyButton : public QWidget {

	Q_OBJECT//使用信号槽要此宏
public:
	MyButton(QString instance_name, QWidget* parent) : QWidget(parent)
		, m_instanceName(instance_name)
	{  
		m_wgt = new QWidget(this);
		m_wgt->setGeometry(QRect(0, 0, 300, 40));
		m_wgt->setStyleSheet("background-color:rgb(246, 245, 248);border:0px;");
		m_oneButton = new QPushButton(this);
		m_oneButton->setGeometry(QRect(32, 0, 240, 40));
		m_oneButton->setText(instance_name);
		m_oneButton->setStyleSheet("text-align:left;color:rgb(54, 58, 68);border:0px;background-color:rgb(246, 245, 248);font: 12pt 微软雅黑;;");
		
		//QFont font("Microsoft YaHei", 12, QFont::Normal);


		connect(m_oneButton, &QPushButton::clicked, this, &MyButton::onSingleClick);
		m_oneButton->setCursor(Qt::PointingHandCursor);

		m_oneIcon = new QPushButton(this);
		m_oneIcon->setGeometry(QRect(5, 12, 16, 16));
		m_oneIcon->setText("");
		m_oneIcon->setStyleSheet("QPushButton {\n"
			"background-color:transparent;\n"
			"background-color:rgb(246, 245, 248);\n"
			"border:0px;\n"
			"border-image:url(:/FaultInjection/images/指令2.png);\n"
			"}");
		m_oneIcon->setCursor(Qt::PointingHandCursor);
		connect(m_oneIcon, &QPushButton::clicked, this, &MyButton::onSingleClick);
	}

	~MyButton()
	{
		delete m_oneButton;
		delete m_oneIcon;
	}

	void selectItem(bool is_selected)
	{
		if (is_selected)
		{
			m_oneIcon->setStyleSheet("QPushButton {\n"
				"background-color:transparent;\n"
				"border-image:url(:/FaultInjection/images/指令1.png);\n"
				"border:0px;\n"
				"}");
			//m_oneIcon->setGeometry(QRect(23, 6, 20, 23));
			m_oneButton->setStyleSheet("text-align:left;background-color: rgb(73, 136, 250);color: rgb(255, 255, 255); border:0px;font: 12pt 微软雅黑;");
			m_wgt->setStyleSheet("background-color: rgb(73, 136, 250); border:0px;");
		}
		else
		{
			m_oneIcon->setStyleSheet("QPushButton {\n"
				"background-color:transparent;\n"
				"border-image:url(:/FaultInjection/images/指令2.png);\n"
				"border:0px;\n"
				"}");
			//m_oneIcon->setGeometry(QRect(23, 12, 16, 16));
			m_oneButton->setStyleSheet("text-align:left;background-color: rgb(246, 245, 248);color: rgb(54, 58, 68); border:0px;font: 12pt 微软雅黑;");
			m_wgt->setStyleSheet("background-color: rgb(246, 245, 248); border:0px;");
		}
	}
	inline QString getName() {
		return m_instanceName;
	}
private:
	QString m_instanceName;
	QPushButton* m_oneButton; 
	QPushButton* m_oneIcon;
	QWidget* m_wgt;
	void onSingleClick()
	{
		emit onClick(this);
	}

signals: 
	void onClick(QObject *);
};
 
class MyFaultTree : public QWidget
{
	Q_OBJECT

public:
	MyFaultTree(QWidget* parent = Q_NULLPTR);
	~MyFaultTree(); 
	void addToolItem(ItemStruct oneFault); 
	void EditToolItem(ItemStruct oneFault);
	void DelToolItem(ItemStruct oneFault);
	void selectItem(QString text);

	void InitFaultTree(vector<ItemStruct> faultItems);
private:
	struct ToolItem
	{
		MyButton* tab; //主节点显示区
		QLabel* border;
		bool is_selected;

		int itemIndex;//记录当前项的索引，方便后续修改。
		int type;//1：参数故障；2：指令故障

		int FaultCommandID;//故障指令id
		vector<int> responseCommandID;//受影响指令id
		vector<int> deviceParamInfoID;//参数id  device_param_info_id
		int m_deviceID;//设备id

		QString createTime;
		QString lastUpdateTime;
	};
	vector<ToolItem> items; 
	QVBoxLayout* layout; 
	QSpacerItem* horizontalSpacer;
	QWidget* _parent;

	int m_index;
	void addItem(ItemStruct oneFault);
	void onClick(QObject* obj); 
	void setHeight();
signals:
	void onFaultNodeClick(QString name); 
};
