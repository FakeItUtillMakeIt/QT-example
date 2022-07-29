#include "log.h"
#include "DirOperation.h"
#include <windows.h>
#include <wchar.h>

void LogInit(char* arg, string rootdir)
{
	// Initialize Google's logging library.    
	google::InitGoogleLogging(arg);
	//string logDir = DirOperation::ConcatePath(rootdir,"log");
	string logDir = rootdir + "/log";
	DirOperation::CreateDir(logDir);
	// PRINT("LogDir: " + logDir);
	google::SetLogDestination(google::GLOG_INFO, DirOperation::ConcatePath(logDir, "/Info").c_str());
	google::SetLogDestination(google::GLOG_WARNING, DirOperation::ConcatePath(logDir, "/Warning").c_str());
	google::SetLogDestination(google::GLOG_ERROR, DirOperation::ConcatePath(logDir, "/Error").c_str());
	google::SetLogDestination(google::GLOG_FATAL, DirOperation::ConcatePath(logDir, "/Fatal").c_str());
	// 设置日志路径  INFO WARNING ERROR FATAL    
	// ...    
	//google::SetLogDestination(google::GLOG_ERROR, "stderr");

	google::SetStderrLogging(google::GLOG_WARNING);//配置输出到标准错误输出的最低日记级别

												   // google::LogToStderr();//只输出到标准错误输出
	LOG(INFO) << "Log Init. Found " << google::COUNTER << endl;
}