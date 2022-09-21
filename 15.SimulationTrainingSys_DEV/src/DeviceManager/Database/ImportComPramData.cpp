//#pragma execution_character_set("utf-8")

#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
//#include <QtWidgets/qfiledialog.h>
//#include <qfiledialog.h>
//#include <QtWidgets/qmessagebox.h>

//#include <ActiveQt/qaxobject.h>
#include "ImportComPramData.h"
#include "xlsxdocument.h"


#define MAXCODE 65535
ImportComPramData::ImportComPramData(QObject* parent)
	: QObject(parent)
	, m_EXcelDAO(nullptr)
	, m_rocketId(-1)
{
	m_app = AppCache::instance();

	m_EXcelDAO = new ImpotEXcelDAO(m_app->m_outputPath);
}

ImportComPramData::~ImportComPramData()
{

}

/// <summary>
/// 导入参数 V2
/// </summary>
void ImportComPramData::AddPramComDatas(int rocketID, QString readFile)
{
	qDebug() << "ImportComPramData：" << QThread::currentThreadId() << QThread::currentThread();
	//excel V1.00
#if 0
	AddPramComDatasV1(rocketID);

	//excel V2.00
#else
	int row_count, col_count;
	QStringList str;
	QString Qstr;

	QXlsx::Document* m_xlsx = nullptr;
	if (!m_xlsx)
	{
		m_xlsx = new QXlsx::Document(readFile, this);// 打开EXCEL_NAME文件，将所有数据读取到内存中，然后关闭excel文件
	}
	if (m_xlsx->load())//判断文件是否打开成功
	{
		qInfo() << "excel打开成功!";
	}
	else
	{
		Qstr = "文件打开失败！";
		emit ImportResult(Qstr);
		return;
	}

	//获取当前火箭名称数据库信息
	QVariant rocketName = m_xlsx->read(1, 2);//得到火箭名称
	m_EXcelDAO->Init(rocketName.toString().toStdString());

	//获取火箭id
	m_rocketId = m_EXcelDAO->GetRocketID();
	if (m_rocketId != rocketID)
	{
		Qstr = "导入火箭型号不匹配！";
		emit ImportResult(Qstr);
		return;
	}

	bool isExit = false;
	QVariant value;
	int ParamTableId = -1;//当前参数的参数表id
	int ParamId = -1;//当前参数的id
	int deviceId = -1;

	int cmdTableId = -1;//当前指令的参数表id
	int cmdBackId = -1;
	int cmdId = -1;
	int rowLen = m_xlsx->dimension().rowCount();           // 获取最大行数
	int columnLen = m_xlsx->dimension().columnCount();     // 获取最大列数
	int paramCount = 0;//写入参数成功计数
	int cmdCount = 0;
	bool result = false;
	PCTable oneTable = { 0,m_rocketId,"" };
	for (int i = 1; i <= rowLen; i++)                       // 遍历每一行
	{
		if (i < 4) continue;

		isExit = false;

		//参数操作 ()
		value = m_xlsx->read(i, 4);//目前只判断了参数表是否有效，有必要的话每个数据都判断一边
		if (value.isValid())
		{
			oneTable.m_tableName = value.toString().toStdString();
			ParamTableId = m_EXcelDAO->ParamtableIsExist(oneTable);
			if (ParamTableId != -1)
			{
				//参数操作
				ParamInfo* oneParam = new ParamInfo();//没有使用到重复的变量，只new一个就行
				oneParam->m_name = m_xlsx->read(i, 1).toString().toStdString();//参数名称
				oneParam->m_type = m_xlsx->read(i, 2).toInt();//类型
				oneParam->m_unit = m_xlsx->read(i, 3).toString().toStdString();//单位
				ParamId = m_EXcelDAO->NowRocketParamExist(oneParam, isExit);//判断对应的火箭是否绑定该参数 //如果参数存在则不写入，但是要继续后面的指令写入
				if (ParamId != -1 && isExit == false)
				{
					//参数和参数表关联操作

					oneParam->m_rocketid = m_rocketId;
					oneParam->m_paramtableid = ParamTableId;
					oneParam->m_id = ParamId;
					result = m_EXcelDAO->InsertParamParamtable(oneParam);

					if (result == true)
					{
						//paramCount++;
						//m_EXcelDAO->GetAllParamInfo();//刷新参数表，后续查重使用
					}

					//参数和设备关联操作
					//判断设备是否存在
					DeviceInfo* oneDevice = new DeviceInfo();
					oneDevice->m_rocketid = m_rocketId;
					oneDevice->m_name = m_xlsx->read(i, 5).toString().toStdString();//设备名称
					oneDevice->m_isVirtual = m_xlsx->read(i, 6).toInt();//类型
					deviceId = m_EXcelDAO->DeviceIsExist(oneDevice);
					if (deviceId != -1)
					{
						//设备-参数关联
						oneParam->m_deviceid = deviceId;
						result = m_EXcelDAO->InsertParamDevice(oneParam);

						if (result == true)
						{
							paramCount++;
						}
					}

				}
			}
		}


		//指令操作
		value = m_xlsx->read(i, 9);
		if (value.isValid())
		{
			oneTable.m_tableName = value.toString().toStdString();
			cmdTableId = m_EXcelDAO->CommandtableIsExist(oneTable);
			if (cmdTableId == -1)
			{
				continue;
			}

			//指令操作 （先写回令，再写指令）
			//写回令
			Command* oneCmdBack = new Command();
			oneCmdBack->m_iBackId = 0;
			oneCmdBack->m_sName = m_xlsx->read(i, 8).toString().toStdString() + "回令";//回令名称
			oneCmdBack->m_iRocketId = m_rocketId;
			oneCmdBack->m_iType = 2;//回令
			oneCmdBack->m_iCode = NewCode();
			cmdBackId = m_EXcelDAO->NowCommandBackExist(oneCmdBack, isExit);
			if (cmdBackId == -1 || isExit == true)//回令存在，指令肯定存在所以直接continue
			{
				continue;
			}

			//写指令
			Command* oneCmd = new Command();
			oneCmd->m_iBackId = cmdBackId;
			oneCmd->m_sName = m_xlsx->read(i, 8).toString().toStdString();//指令名称
			oneCmd->m_iRocketId = m_rocketId;
			oneCmd->m_iType = 1;//指令
			oneCmd->m_iCode = NewCode();
			cmdId = m_EXcelDAO->NowCommandExist(oneCmd);
			if (cmdId == -1)
			{
				continue;
			}

			//指令和指令表关联操作
			oneCmd->m_commandTableId = cmdTableId;
			oneCmd->m_id = cmdId;
			result = m_EXcelDAO->InsertCommandCommandtable(oneCmd);

			if (result == true)
			{
				cmdCount++;
				//m_EXcelDAO->GetCommandInfo();//刷新指令表，后续查重使用
			}

		}

	}


	Qstr = QString("成功导入参数：%1条；指令：%2条").arg(paramCount).arg(cmdCount);
	//QMessageBox::information(nullptr, "信息", QString("成功导入参数：%1条；指令：%2条").arg(paramCount).arg(cmdCount), "确定");
	
	emit ImportResult(Qstr);
	return;

#endif
}

/// <summary>
/// 导入参数 V1版本
/// </summary>
void ImportComPramData::AddPramComDatasV1(int rocketID)
{
	//读取excel文件
	QString readFile = QFileDialog::getOpenFileName(nullptr, QStringLiteral("选择Excel文件"), "", tr("Exel file(*.xls *.xlsx)"));
	if (readFile == "")
	{
		return;
	}

	int row_count, col_count;
	QStringList str;

	QXlsx::Document* m_xlsx = nullptr;
	if (!m_xlsx)
	{
		m_xlsx = new QXlsx::Document(readFile, this);// 打开EXCEL_NAME文件，将所有数据读取到内存中，然后关闭excel文件
	}
	if (m_xlsx->load())//判断文件是否打开成功
	{
		qInfo() << "excel打开成功!";
	}
	else
	{
		QMessageBox::warning(nullptr, "提示", "文件打开失败！", "确定");
		return;
	}

	//获取当前火箭名称数据库信息
	QVariant rocketName = m_xlsx->read(1, 2);//得到火箭名称
	m_EXcelDAO->Init(rocketName.toString().toStdString());

	//获取火箭id
	m_rocketId = m_EXcelDAO->GetRocketID();
	if (m_rocketId != rocketID)
	{
		QMessageBox::warning(nullptr, "提示", "请导入对应火箭型号的数据！", "确定");
		return;
	}

	bool isExit = false;
	QVariant value;
	int ParamTableId = -1;//当前参数的参数表id
	int ParamId = -1;//当前参数的id
	int deviceId = -1;

	int cmdTableId = -1;//当前指令的参数表id
	int cmdBackId = -1;
	int cmdId = -1;
	int rowLen = m_xlsx->dimension().rowCount();           // 获取最大行数
	int columnLen = m_xlsx->dimension().columnCount();     // 获取最大列数
	int paramCount = 0;//写入参数成功计数
	int cmdCount = 0;
	bool result = false;
	PCTable oneTable = { 0,m_rocketId,"" };
	for (int i = 1; i <= rowLen; i++)                       // 遍历每一行
	{
		if (i < 4) continue;

		isExit = false;

		//参数操作 ()
		value = m_xlsx->read(i, 2);//目前只判断了参数表是否有效，有必要的话每个数据都判断一边
		if (value.isValid())
		{
			oneTable.m_tableName = value.toString().toStdString();
			ParamTableId = m_EXcelDAO->ParamtableIsExist(oneTable);
			if (ParamTableId != -1)
			{
				//参数操作
				ParamInfo* oneParam = new ParamInfo();
				oneParam->m_name = m_xlsx->read(i, 1).toString().toStdString();//参数名称
				oneParam->m_type = m_xlsx->read(i, 3).toInt();//类型
				oneParam->m_unit = m_xlsx->read(i, 4).toString().toStdString();//单位
				ParamId = m_EXcelDAO->NowRocketParamExist(oneParam, isExit);//判断对应的火箭是否绑定该参数 //如果参数存在则不写入，但是要继续后面的指令写入
				if (ParamId != -1 && isExit == false)
				{
					//参数和参数表关联操作

					oneParam->m_rocketid = m_rocketId;
					oneParam->m_paramtableid = ParamTableId;
					oneParam->m_id = ParamId;
					result = m_EXcelDAO->InsertParamParamtable(oneParam);

					if (result == true)
					{
						//paramCount++;
						//m_EXcelDAO->GetAllParamInfo();//刷新参数表，后续查重使用
					}

#if 0
					//参数和设备关联操作
					//判断设备是否存在
					DeviceInfo* oneDevice = new DeviceInfo();
					oneDevice->m_rocketid = m_rocketId;
					oneDevice->m_name = m_xlsx->read(i, 5).toString().toStdString();//名称
					oneDevice->m_isVirtual = m_xlsx->read(i, 6).toInt();//类型
					deviceId = m_EXcelDAO->DeviceIsExist(oneDevice);
					if (deviceId != -1)
					{
						//设备-参数关联
						oneParam->m_deviceid = deviceId;
						result = m_EXcelDAO->InsertParamDevice(oneParam);

						if (result == true)
						{
							paramCount++;
						}
					}
#endif
				}
			}
		}


		//指令操作
		value = m_xlsx->read(i, 7);
		if (value.isValid())
		{
			oneTable.m_tableName = value.toString().toStdString();
			cmdTableId = m_EXcelDAO->CommandtableIsExist(oneTable);
			if (cmdTableId == -1)
			{
				continue;
			}

			//指令操作 （先写回令，再写指令）
			//写回令
			Command* oneCmdBack = new Command();
			oneCmdBack->m_iBackId = 0;
			oneCmdBack->m_sName = m_xlsx->read(i, 6).toString().toStdString() + "回令";//回令名称
			oneCmdBack->m_iRocketId = m_rocketId;
			oneCmdBack->m_iType = 2;//回令
			oneCmdBack->m_iCode = NewCode();
			cmdBackId = m_EXcelDAO->NowCommandBackExist(oneCmdBack, isExit);
			if (cmdBackId == -1 || isExit == true)//回令存在，指令肯定存在所以直接continue
			{
				continue;
			}

			//写指令
			Command* oneCmd = new Command();
			oneCmd->m_iBackId = cmdBackId;
			oneCmd->m_sName = m_xlsx->read(i, 6).toString().toStdString();//指令名称
			oneCmd->m_iRocketId = m_rocketId;
			oneCmd->m_iType = 1;//指令
			oneCmd->m_iCode = NewCode();
			cmdId = m_EXcelDAO->NowCommandExist(oneCmd);
			if (cmdId == -1)
			{
				continue;
			}

			//指令和指令表关联操作
			oneCmd->m_commandTableId = cmdTableId;
			oneCmd->m_id = cmdId;
			result = m_EXcelDAO->InsertCommandCommandtable(oneCmd);

			if (result == true)
			{
				cmdCount++;
				//m_EXcelDAO->GetCommandInfo();//刷新指令表，后续查重使用
			}

		}

	}
	//QString str = QString("成功导入参数：%1条；指令：%2条").arg(paramCount).arg(cmdCount);
	QMessageBox::information(nullptr, "信息", QString("成功导入参数：%1条；指令：%2条").arg(paramCount).arg(cmdCount), "确定");
	return;
}




/// <summary>
/// 获取唯一的code
/// </summary>
/// <returns></returns>
int ImportComPramData::NewCode()
{
	int newcode = 0;
	vector<int> code;

	for (auto item : m_EXcelDAO->m_allCommadI)
	{
		code.push_back(item.second->m_iCode);
	}

	while (true)
	{
		newcode = rand() % MAXCODE;
		if (std::find(code.begin(), code.end(), newcode) == code.end())
		{
			break;
		}
	}

	return newcode;
}


//string ImportComPramData::QStr2Str(const QString qStr)
//{
//	QByteArray data = qStr.toLocal8Bit();
//	return string(data);
//}