#pragma once

#include <string>
using namespace std;

///帧类型
#define FRAME_TYPE_COMMAND			0x01   //  测发口令
#define FRAME_TYPE_ANSWER			0x02   //  应答口令 
#define FRAME_TYPE_YC_PARAMETER		0x03   //  遥测参数量 
#define FRAME_TYPE_UNDEFINE			0x00   //  无效帧

///信息头长度
#define FRAMEHEAD_SYNC_1	0x55
#define FRAMEHEAD_SYNC_2	0xCC
#define FRAMEHEAD_LENGTH	26

///确认帧中确认值的位置（1序）
#define FRAMEHEAD_ADDR_SYNC     0 
#define FRAMEHEAD_ADDR_LENGTH   2
#define FRAMEHEAD_ADDR_TYPE     4  
#define FRAMEHEAD_ADDR_YEAR     5
#define FRAMEHEAD_ADDR_MONTH    7
#define FRAMEHEAD_ADDR_DAY      8
#define FRAMEHEAD_ADDR_TIME     9 
#define FRAMEHEAD_ADDR_COUNT    17
#define FRAMEHEAD_ADDR_PARAM_NUM 21 
#define FRAMEHEAD_ADDR_CHECKSUM 25
  
class RocketDataFrame;
///
///信息头定义
///
class FrameHead
{
public:
	FrameHead();
	FrameHead(const FrameHead& head);
	FrameHead& operator=(const FrameHead& head);
	~FrameHead();

	///生成确认成功的帧头字节序列
	static void GetFailConfirmFrame(unsigned char* const buff);

	///生成确认失败的帧头字节序列
	static void GetSuccessConfirmFrame(unsigned char* const buff);
	///返回帧头长度
	static int Size();

	void UpdateTime();

	unsigned char FrameSync1() const		{ return m_iFrameSync1; }
	unsigned char FrameSync2() const		{ return m_iFrameSync2; } 
	unsigned short FrameLen() const			{ return m_iFrameLen; }
	unsigned char  FrameType() const		{ return m_iFrameType; } 
	unsigned short Year() const				{ return m_iYear; }
	unsigned char Month() const				{ return m_iMonth; }
	unsigned char Day() const				{ return m_iDay; }
	unsigned long long Time() const			{ return m_ullTime; }
	unsigned long  FrameCount() const		{ return m_iFrameCount; }
	unsigned short ParamCount() const		{ return m_iParamCount; }
	unsigned char CheckSum() const			{ return m_iCheckSum; }

	void setFrameSync1(unsigned char FrameSync1)		{ m_iFrameSync1 = FrameSync1; }
	void setFrameSync2(unsigned char FrameSync2)		{ m_iFrameSync2 = FrameSync2; }
	void setFrameLen(unsigned short FrameLen)			{ m_iFrameLen = FrameLen; }
	void setFrameType(unsigned char   FrameType)		{ m_iFrameType = FrameType; } 
	void setYear(unsigned short Year)					{ m_iYear = Year; }
	void setMonth(unsigned char Month)					{ m_iMonth = Month; }
	void setDay(unsigned char Day)						{ m_iDay = Day; }
	void setTime(unsigned long long Time)				{ m_ullTime = Time; }
	void setFrameCount(unsigned long  FrameCount)		{ m_iFrameCount = FrameCount; }
	void setParamCount(unsigned short  ParamCount)		{ m_iParamCount = ParamCount; }

private:
	friend class RocketDataFrame;

	string& last_error_info() { return m_error_info; }

	///将缓冲区中的字符序列反序列化到信息头各字段
	bool DeSerialize(unsigned char* const buff);
    ///将信息头各字段序列化到缓冲区
	bool Serialize(unsigned char* const buff);

	///帧头同步字节1
	unsigned char m_iFrameSync1;
	///帧头同步字节2
	unsigned char m_iFrameSync2; 
    ///帧长度
	unsigned short m_iFrameLen;       
    ///帧类型
	unsigned char   m_iFrameType;
	///年
	unsigned short m_iYear;
	///月
	unsigned char m_iMonth;
	///日
	unsigned char m_iDay;
	///时间，相对于当日零时的纳秒数
	unsigned long long m_ullTime;
    ///帧计数
	unsigned long  m_iFrameCount; 
    ///参数个数
	unsigned short m_iParamCount;
	///校验和
	unsigned char m_iCheckSum;
	///内部异常信息
	string m_error_info;
};
