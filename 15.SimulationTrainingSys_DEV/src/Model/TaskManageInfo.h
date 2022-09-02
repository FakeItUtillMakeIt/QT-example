#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;


class TaskManageInfo
{
public:
	TaskManageInfo() {};
	~TaskManageInfo() {};

	int m_id;
	int m_rocketId;
	string m_taskName;
	string m_roketSoftName;
	string m_createTime;
	string m_lastUpdateTime;

private:

};
