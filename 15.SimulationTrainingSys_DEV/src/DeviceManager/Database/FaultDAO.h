#pragma once

#include <string>
using namespace std;

#include "../AppCache.h"
#include "../../Public/log.h"
#include "../../Model/Fault.h"
#include "mysql.h" 

#include "../../Public/Utils.h"
//#include "../../Model/Command.h"

namespace DataBaseF
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

		bool GetFaultCommand();
		bool GetAllCommandInfoFrame();
		bool InsertCommandInfo(FaultCommandInfo* fameInfo);
		bool DelCommandInfoParam(int id);
		bool DelCommandInfoParam(string name);

		bool GetCommandCode(vector<int>*);
		int GetNewCode();

		bool GetAllCommandParamInfoFrame();
		bool InsertCommandParamInfo(FaultCommandParamInfo* fameInfo);
		bool DelCommandParamInfoParam(int code);

		int GetNowId(string tabname);
		int GetNowId2(string tableName);


		
	private:
		OutputPath* m_path;
		MYSQL my_connection;
		bool is_connected;
		AppCache* m_app;
	};
}
