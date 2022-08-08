#include "DeviceParam.h"
#include "../DeviceManager/Database/DeviceDBConfigInfo.h"

DeviceParam::DeviceParam() :
	m_status(string("初始化")), m_iCurValue(0), m_iCurStr("")
{
	curIndex = 0;
	paramData = { 0 };

	timer = new QTimer();
	connect(timer, &QTimer::timeout, this, &DeviceParam::timerUpdateParam);
}

DeviceParam::~DeviceParam()
{
}

/**
	@brief 更新参数实时值
**/
void DeviceParam::updateParamRealVal() {
	//根据参数ID获取参数类型
	//暂时不处理离散量
	DeviceDBConfigInfo::getInstance()->readParamDB2UI();
	if (atoi(DeviceDBConfigInfo::getInstance()->paramInfo[m_subParameterId][2].c_str()) == 2)
	{
		return;
	}
	//连续量
	//根据当前状态更换实时值信息

	paramData = m_statusRealData[m_status];
	curIndex = 0;

}

/**
	@brief 定时器刷新实时值
**/
void DeviceParam::timerUpdateParam() {

	if (QString::fromStdString(m_status) == QString("初始化"))
	{
		return;
	}
	m_iCurValue = paramData[curIndex++];
	if (curIndex >= paramData.size())
	{
		curIndex = 0;

	}
	qDebug() << curIndex << m_iCurValue << timer->timerId() << m_subParameterId;
}