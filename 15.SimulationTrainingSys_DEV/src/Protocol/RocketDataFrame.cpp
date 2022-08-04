#include "RocketDataFrame.h"

RocketDataFrame::RocketDataFrame(QObject* parent)
	: m_Head(FrameHead())
	, m_error_info("")
	, QObject(parent)
{
}

RocketDataFrame::RocketDataFrame(RocketDataFrame& frame, QObject* parent)
	: QObject(parent)
{ 
	this->m_Head = frame.head();
	for (auto a : frame.params())
	{
		this->m_vParams.push_back(a);
	}
}

RocketDataFrame& RocketDataFrame::operator= (RocketDataFrame& frame)
{ 
	this->m_Head = frame.head();
	for (auto a : frame.params())
	{
		this->m_vParams.push_back(a);
	}
	return *this;
}

RocketDataFrame::~RocketDataFrame()
{
}

FrameHead& RocketDataFrame::head()
{
	return m_Head;
}


vector<RocketDataParam>& RocketDataFrame::params()
{
	return m_vParams;
}


unsigned short RocketDataFrame::Size()
{
	return (unsigned short)(FRAMEHEAD_LENGTH + m_vParams.size() * PARAM_LENGTH);
}

bool RocketDataFrame::Serialize(unsigned char* pData, int iLength)
{
	if (m_Head.m_iFrameLen != m_vParams.size() * PARAM_LENGTH + FRAMEHEAD_LENGTH)
		return false;

	if (m_Head.m_iFrameLen > iLength)
		return false;

	m_Head.Serialize(pData);
	for (int i = 0; i < m_vParams.size(); i++)
	{
		m_vParams[i].Serialize(pData + FRAMEHEAD_LENGTH + i * PARAM_LENGTH);
	}
	//计算校验和
	*(pData + FRAMEHEAD_ADDR_CHECKSUM) = CheckSum(pData, Size());
	return true;
}

bool RocketDataFrame::DeSerialize(unsigned char* pData, int iLength)
{
	if (iLength < FRAMEHEAD_LENGTH)
	{
		m_error_info = "解析数据失败，数据长度小于信息头长度";
		return false;
	}

	unsigned short RocketDataFrameLength;
	memcpy(&RocketDataFrameLength, pData + FRAMEHEAD_ADDR_LENGTH, 2);
	if (RocketDataFrameLength != iLength)
	{
		m_error_info = "解析数据失败，数据长度字段值与实际数据长度不符";
		return false;
	}

	if (!m_Head.DeSerialize(pData))
	{
		m_error_info = m_Head.last_error_info();
		return false;
	}

	if (RocketDataFrameLength != m_Head.ParamCount() * PARAM_LENGTH + FRAMEHEAD_LENGTH)
	{
		m_error_info = "解析数据失败，帧数据长度与协议不符";
		return false;
	}
	int cycle_times = (RocketDataFrameLength - FRAMEHEAD_LENGTH) / PARAM_LENGTH;
	for (int i = 0; i < cycle_times; i++)
	{
		RocketDataParam word;
		word.DeSerialize(pData + FRAMEHEAD_LENGTH + i * PARAM_LENGTH);
		m_vParams.push_back(word);
	}

	//计算校验和
	if (CheckSum(pData, Size()) != pData[FRAMEHEAD_ADDR_CHECKSUM])
	{
		m_error_info = "解析数据失败，帧数据校验失败";
		return false;
	}
	return true;
}

unsigned char RocketDataFrame::CheckSum(unsigned char* pData, int iLength)
{
	unsigned char checkSum = 0x0;
	for (int i = 0; i < iLength; i++) {
		if (i != FRAMEHEAD_ADDR_CHECKSUM)
			checkSum += *(pData + i);
	}
	return checkSum;
}
 