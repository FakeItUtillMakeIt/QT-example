#pragma once

#include <string>
using namespace std;

#include "glog/logging.h"

/**
* @brief ��ʼ��google log
* @param[in] arg ��ִ�г����ļ�����
* @param[in] logdir ��־����·��
* @return void
*/
void LogInit(char* arg, string logdir);
