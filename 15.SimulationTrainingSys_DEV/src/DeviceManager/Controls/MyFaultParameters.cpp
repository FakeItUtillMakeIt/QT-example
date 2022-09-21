#pragma execution_character_set("utf-8")
#include "MyFaultParameters.h" 

MyFaultParameters::MyFaultParameters(QWidget* parent)
	: QWidget(parent)
	, m_index(0)
	, _parent(parent)
{
	//layout = new QVBoxLayout(this); 
	//layout->setMargin(0);
	//layout->setSpacing(0); 

	Gridlayout = new QGridLayout(this);
	Gridlayout->setMargin(0);
	Gridlayout->setSpacing(0);

	//for (int i = 0; i < instances.size(); i++)// instances.size()
	//{
	//	addItem(instances[i],i);
	//}

	//horizontalSpacer = new QSpacerItem(90, 20, QSizePolicy::Minimum, QSizePolicy::Expanding); //设置垂直弹簧
	/*layout->addSpacerItem(horizontalSpacer);*/
	//();
}

void MyFaultParameters::Init(vector<ItemStruct> instances)
{
	items.clear();
	for (int i = 0; i < instances.size(); i++)
	{
		addItem(instances[i], i);
	}
}

void MyFaultParameters::setHeight()
{
	int height = 60 + 40 * items.size();
	this->setGeometry(20, 0, 300, height); //显示树状导航一级节点大小
}

MyFaultParameters::~MyFaultParameters()
{

}

void MyFaultParameters::addToolItem(ItemStruct inItem)
{

	addItem(inItem, m_itemNumber + 1);
	//setHeight();
	//selectItem(text);
}

/// <summary>
/// 响应移除事件
/// </summary>
/// <param name="text"></param>
void MyFaultParameters::RemoveToolItem(QString text)
{
	//移除某项(TODO没有成功)
	QLayoutItem* layoutItem;
	while ((layoutItem = Gridlayout->takeAt(0)) != 0)
	{
		if (layoutItem->widget()->windowTitle() == text)
		{
			Gridlayout->removeWidget(layoutItem->widget());
			layoutItem->widget()->setParent(0);
			delete layoutItem->widget();
		}
	}
}

void MyFaultParameters::addItem(ItemStruct inItem, int itemNumber)
{
	ToolItem item;
	item.is_selected = false;
	item.tab = new MySonButton(inItem.Name, this);
	item.id = inItem.ID;
	item.code = inItem.code;
	item.type = inItem.type;
	item.faultType = inItem.faultType;
	item.FaultCommandID = inItem.FaultCommandID;
	item.responseCommandID = inItem.responseCommandID;
	item.deviceParamInfoID = inItem.deviceParamInfoID;

	item.m_deviceID = inItem.deviceID;

	item.createTime = inItem.createTime;
	item.lastUpdateTime = inItem.lastUpdateTime;

	item.tab->setMinimumSize(247, 40);
	connect(item.tab, &MySonButton::onClick, this, &MyFaultParameters::onClick);
	//item.border = new QLabel(this);
	//item.border->setFixedHeight(20);//设置上下间距
	//item.border->setStyleSheet(QStringLiteral("background-color:#F6F45F8; border:0px;"));
	//layout->insertWidget(m_index++, item.tab);
	//layout->insertWidget(m_index++, item.border);
	//items.push_back(item);

	int itemX = itemNumber / 2;
	int itemY = itemNumber % 2;
	m_itemNumber = itemNumber;

	Gridlayout->addWidget(item.tab, itemX, itemY);
	// 设置水平间距

	Gridlayout->setHorizontalSpacing(40);
	// 设置垂直间距
	Gridlayout->setVerticalSpacing(5);
	// 设置外间距
	Gridlayout->setContentsMargins(35, 20, 35, 30);
	items.push_back(item);



	//QGridLayout* pLayout = new QGridLayout();
	//// 头像 第0行，第0列开始，占3行1列
	//pLayout->addWidget(pImageLabel, 0, 0, 3, 1);
	//// 用户名输入框 第0行，第1列开始，占1行2列
	//pLayout->addWidget(pUserLineEdit, 0, 1, 1, 2);
	//pLayout->addWidget(pRegisterButton, 0, 4);
	//// 密码输入框 第1行，第1列开始，占1行2列
	//pLayout->addWidget(pPasswordLineEdit, 1, 1, 1, 2);
	//pLayout->addWidget(pForgotButton, 1, 4);
	//// 记住密码 第2行，第1列开始，占1行1列 水平居左 垂直居中
	//pLayout->addWidget(pRememberCheckBox, 2, 1, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
	//// 自动登录 第2行，第2列开始，占1行1列 水平居右 垂直居中
	//pLayout->addWidget(pAutoLoginCheckBox, 2, 2, 1, 1, Qt::AlignRight | Qt::AlignVCenter);
	//// 登录按钮 第3行，第1列开始，占1行2列
	//pLayout->addWidget(pLoginButton, 3, 1, 1, 2);
	//// 设置水平间距
	//pLayout->setHorizontalSpacing(10);
	//// 设置垂直间距
	//pLayout->setVerticalSpacing(10);
	//// 设置外间距
	//pLayout->setContentsMargins(10, 10, 10, 10);
	//setLayout(pLayout);

}

/// <summary>
/// 修改当前故障注入情况，（在解析故障回令之后再进行控件的修改）
/// </summary>
/// <param name="isOK"></param>
void MyFaultParameters::backonFaultClick(int faultype,bool isOK)
{
	for (auto& item : items)
	{
		bool selected = false; //取反
		if (m_clickButtonname == item.tab->getName())
		{
			if (faultype != item.faultType)
			{
				return;
			}

			if (isOK)
			{
				//选中
				selected = !item.is_selected;

				item.is_selected = selected;
				item.tab->selectItem(selected);
				//存放号当前组中所有已经选择的参数
			}
			else
			{
				selected = !item.is_selected;

				item.is_selected = selected;
				item.tab->selecterror();
			}
		}	
	}
}


/// <summary>
/// 点击按钮只发送故障命令
/// </summary>
/// <param name="obj"></param>
void MyFaultParameters::onClick(QObject* obj)
{
	MySonButton* my_button = static_cast<MySonButton*>(obj);
#pragma region 单选
	for (auto& item : items)
	{
		bool selected = false; //取反
		if (my_button == item.tab)
		{
			//选中
			selected = !item.is_selected;

			//告诉已选参数列表，最新的操作
			m_clickButtonname = item.tab->getName();
			emit onFaultClick(m_clickButtonname, item.code, item.type, item.faultType, selected);

			//item.is_selected = selected;
			//item.tab->selectItem(selected);
			//存放号当前组中所有已经选择的参数
		}
	}

#pragma endregion
}

/// <summary>
/// 是否选择
/// </summary>
/// <param name="text"></param>
/// <param name="isSelect"></param>
void MyFaultParameters::IsSelectParam(QString text, bool isSelect)
{
	bool selected = isSelect; //是否选择
	for (auto& item : items)
	{
		if (text == item.tab->m_oneButton->text())
		{
			//选中
			item.is_selected = selected;
			item.tab->selectItem(selected);
		}
	}
}




