#pragma execution_character_set("utf-8")
#include "MyFaultTree.h" 

MyFaultTree::MyFaultTree(QWidget* parent)
	: QWidget(parent)
	, m_index(0)
	, layout(nullptr)
	, _parent(parent)
{
	layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding); //设置垂直弹簧
	layout->addSpacerItem(horizontalSpacer);

	//setHeight();
}

/// <summary>
/// 加载故障参数初始项
/// </summary>
/// <param name="faultItems"></param>
void MyFaultTree::InitFaultTree(vector<ItemStruct> faultItems)
{
	for (size_t i = 0; i < items.size(); i++)
	{
		delete items[i].tab;
		items[i].tab = nullptr;

		delete items[i].border;
		items[i].border = nullptr;
	}
	items.clear();
	m_index = 0;

	for (int i = 0; i < faultItems.size(); i++)
	{
		addItem(faultItems[i]);
	}
}

void MyFaultTree::setHeight()
{
	//int height = 60 + 40 * items.size();
	//this->setGeometry(20, 0, 300, height); //显示树状导航一级节点大小
}

MyFaultTree::~MyFaultTree()
{

}

void MyFaultTree::selectItem(QString text)
{
	for (auto& item : items)
	{
		bool selected = false; //取反
		if (text == item.tab->getName())//（子参数在这里更改）
		{
			//选中
			selected = true; //取反
			emit onFaultNodeClick(item.tab->getName(), item.m_deviceID);
		}
		else
		{
			selected = false;
		}
		item.is_selected = selected;
		item.tab->selectItem(selected);
	}
}
void MyFaultTree::addToolItem(ItemStruct oneFault)
{
	addItem(oneFault);
	selectItem(oneFault.Name);
}

void MyFaultTree::EditToolItem(ItemStruct oneFault)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].tab->getName() == oneFault.Name)
		{
			items[i].m_deviceID = oneFault.deviceID;
			items[i].deviceParamInfoID = oneFault.deviceParamInfoID;
			items[i].responseCommandID = oneFault.responseCommandID;

			items[i].FaultCommandID = oneFault.FaultCommandID;
			items[i].type = oneFault.type;

			//selectItem(items[i].tab->getName());
			break;
		}
	}

}

void MyFaultTree::DelToolItem(ItemStruct oneFault)
{
	//从vector中删除指定元素 
	for (vector<ToolItem>::iterator iter = items.begin(); iter != items.end(); iter++)
	{
		if (iter->tab->getName() == oneFault.Name)
		{
			layout->removeWidget(iter->tab);
			layout->removeWidget(iter->border);
			items.erase(iter);
			break;
		}
	}

	if (items.size() > 0)
	{
		selectItem(items[0].tab->getName());//选中第一项
	}
}


void MyFaultTree::addItem(ItemStruct oneFault)
{
	ToolItem item;
	item.is_selected = false;
	item.tab = new MyButton(oneFault.Name, this);
	item.tab->setMinimumSize(300, 39);
	connect(item.tab, &MyButton::onClick, this, &MyFaultTree::onClick);
	item.border = new QLabel(this);
	item.border->setFixedHeight(1);
	item.border->setStyleSheet(QStringLiteral("background-color:#F6F45F8; border:0px;"));

	item.type = oneFault.type;
	item.FaultCommandID = oneFault.FaultCommandID;
	item.responseCommandID = oneFault.responseCommandID;
	item.deviceParamInfoID = oneFault.deviceParamInfoID;

	item.m_deviceID = oneFault.deviceID;
	item.createTime = oneFault.createTime;
	item.lastUpdateTime = oneFault.lastUpdateTime;

	item.itemIndex = m_index;

	layout->insertWidget(m_index++, item.tab);
	layout->insertWidget(m_index++, item.border);
	items.push_back(item);
}


void MyFaultTree::onClick(QObject* obj)
{
	MyButton* my_button = static_cast<MyButton*>(obj);
#pragma region 单选
	for (auto& item : items)
	{
		bool selected = false; //取反
		if (my_button == item.tab)
		{
			//选中
			selected = true; //取反
			emit onFaultNodeClick(item.tab->getName(), item.m_deviceID);
		}
		else
		{
			selected = false;
		}
		item.is_selected = selected;
		item.tab->selectItem(selected);
	}

#pragma endregion
}
