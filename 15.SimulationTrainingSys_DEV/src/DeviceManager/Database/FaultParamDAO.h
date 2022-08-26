#pragma once

#include <string>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Model/Fault.h"
#include "mysql.h" 

//#include "FaultFrameInfo.h"

namespace DataBaseF
{ 
	class FaultParamDAO
	{
	public:
		FaultParamDAO(OutputPath* path);
		~FaultParamDAO(); 
		bool exec_sql(string sql); 
		bool connected();
		bool connect(); 

		bool GetAllFaultParamInfoFrames();
		bool InsertFaultParamInfo(FaultFaultParamInfo* fameInfo);
		bool DelFaultParamInfoFrame(int code);
		bool DelFaultParamInfoFrame(string name);

		bool GetAllFaultCommandInfoFrames(); 
		bool InsertFaultCommandInfoFrame(FaultFaultCommandInfo* fameInfo);
		bool DelFaultCommandInfoFrame(int code);
		bool DelFaultCommandInfoFrame(string name);


		int GetNowId(string tableName);

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}
