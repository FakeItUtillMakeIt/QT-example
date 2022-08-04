#pragma once

#include <string>
using namespace std;

#include "glog/logging.h"

/**
* @brief 初始化google log
* @param[in] arg 可执行程序文件名称
* @param[in] logdir 日志保存路径
* @return void
*/
void LogInit(char* arg, string logdir);
