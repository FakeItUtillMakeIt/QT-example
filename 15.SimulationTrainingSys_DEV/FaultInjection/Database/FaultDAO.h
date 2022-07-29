#pragma once

#include <string>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Model/Fault.h"
#include "mysql.h" 

//#include "FaultFrameInfo.h"

namespace DataBase
{ 
	class FaultDAO
	{
	public:
		FaultDAO(OutputPath* path);
		~FaultDAO(); 
		bool exec_sql(string sql); 
		bool connected();
		bool connect();
		bool test();
		bool getFault();  


		bool GetAllRocketInfoFrame();

		bool GetAllCommandInfoFrame();
		bool InsertCommandInfo(FaultCommandInfo* fameInfo);
		bool DelCommandInfoParam(int code);

		bool GetAllCommandParamInfoFrame();
		bool InsertCommandParamInfo(FaultCommandParamInfo* fameInfo);
		bool DelCommandParamInfoParam(int code);

		int GetNowId(string sql);

	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}
