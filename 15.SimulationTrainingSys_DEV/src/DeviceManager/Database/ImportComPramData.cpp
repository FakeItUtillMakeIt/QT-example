//#pragma execution_character_set("utf-8")

#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QtWidgets/qfiledialog.h>
#include <qfiledialog.h>
#include <QtWidgets/qmessagebox.h>

//#include <ActiveQt/qaxobject.h>
#include "ImportComPramData.h"
#include "xlsxdocument.h"


#define MAXCODE 65535
ImportComPramData::ImportComPramData(QObject *parent)
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
/// �������
/// </summary>
void ImportComPramData::AddPramComDatas(int rocketID)
{
	//��ȡexcel�ļ�
	QString readFile = QFileDialog::getOpenFileName(nullptr, QStringLiteral("ѡ��Excel�ļ�"), "", tr("Exel file(*.xls *.xlsx)"));
	if (readFile == "")
	{
		return;
	}
	
	int row_count, col_count;
	QStringList str;

	QXlsx::Document* m_xlsx = nullptr;
	if (!m_xlsx)
	{
		m_xlsx = new QXlsx::Document(readFile, this);// ��EXCEL_NAME�ļ������������ݶ�ȡ���ڴ��У�Ȼ��ر�excel�ļ�
	}
	if (m_xlsx->load())//�ж��ļ��Ƿ�򿪳ɹ�
	{
		qInfo() << "excel�򿪳ɹ�!";
	}
	else
	{
		QMessageBox::warning(nullptr,"��ʾ","�ļ���ʧ�ܣ�","ȷ��");
		return;
	}

	//��ȡ��ǰ����������ݿ���Ϣ
	QVariant rocketName = m_xlsx->read(1, 2);//�õ��������
	m_EXcelDAO->Init(rocketName.toString().toStdString());

	//��ȡ���id
	m_rocketId = m_EXcelDAO->GetRocketID();
	if (m_rocketId != rocketID)
	{
		QMessageBox::warning(nullptr, "��ʾ", "�뵼���Ӧ����ͺŵ����ݣ�", "ȷ��");
		return;
	}
	
	bool isExit = false;
	QVariant value;
	int ParamTableId = -1;//��ǰ�����Ĳ�����id
	int ParamId = -1;//��ǰ������id

	int cmdTableId = -1;//��ǰָ��Ĳ�����id
	int cmdBackId = -1;
	int cmdId = -1;
	int rowLen = m_xlsx->dimension().rowCount();           // ��ȡ�������
	int columnLen = m_xlsx->dimension().columnCount();     // ��ȡ�������
	int paramCount = 0;//д������ɹ�����
	int cmdCount = 0;
	bool result = false;
	PCTable oneTable = {0,m_rocketId,""};
	for (int i = 1; i <= rowLen; i++)                       // ����ÿһ��
	{
		if (i < 4) continue;

		//�������� ()
		value = m_xlsx->read(i,2);//Ŀǰֻ�ж��˲������Ƿ���Ч���б�Ҫ�Ļ�ÿ�����ݶ��ж�һ��
		if (value.isValid())
		{
			oneTable.m_tableName = value.toString().toStdString();
			ParamTableId = m_EXcelDAO->ParamtableIsExist(oneTable);
			if (ParamTableId != -1)
			{
				//��������
				ParamInfo* oneParam = new ParamInfo();
				oneParam->m_name = m_xlsx->read(i, 1).toString().toStdString();//��������
				oneParam->m_type = m_xlsx->read(i, 3).toInt();//����
				oneParam->m_unit = m_xlsx->read(i, 4).toString().toStdString();//��λ
				ParamId = m_EXcelDAO->NowRocketParamExist(oneParam, isExit);//�ж϶�Ӧ�Ļ���Ƿ�󶨸ò��� //�������������д�룬����Ҫ���������ָ��д��
				if (ParamId != -1 && isExit == false)
				{
					//�����Ͳ������������
					oneParam->m_rocketid = m_rocketId;
					oneParam->m_paramtableid = ParamTableId;
					oneParam->m_id = ParamId;
					result = m_EXcelDAO->InsertParamParamtable(oneParam);

					if (result == true)
					{
						paramCount++;
						m_EXcelDAO->GetAllParamInfo();//ˢ�²�������������ʹ��
					}
				}
			}	
		}
		

		//ָ�����
		value = m_xlsx->read(i, 7);
		if (value.isValid())
		{
			oneTable.m_tableName = value.toString().toStdString();
			cmdTableId = m_EXcelDAO->CommandtableIsExist(oneTable);
			if (cmdTableId == -1)
			{
				continue;
			}

			//ָ����� ����д�����дָ�
			//д����
			Command* oneCmd = new Command();
			oneCmd->m_iBackId = 0;
			oneCmd->m_sName = m_xlsx->read(i, 6).toString().toStdString() + "����";//��������
			oneCmd->m_iRocketId = m_rocketId;
			oneCmd->m_iType = 2;//����
			oneCmd->m_iCode = NewCode();
			cmdBackId = m_EXcelDAO->NowCommandBackExist(oneCmd, isExit);
			if (cmdBackId == -1 || isExit == true)//������ڣ�ָ��϶���������ֱ��continue
			{
				continue;
			}

			//дָ��
			oneCmd->m_iBackId = cmdBackId;
			oneCmd->m_sName = m_xlsx->read(i, 6).toString().toStdString();//ָ������
			oneCmd->m_iRocketId = m_rocketId;
			oneCmd->m_iType = 1;//ָ��
			oneCmd->m_iCode = NewCode();
			cmdId = m_EXcelDAO->NowCommandExist(oneCmd);
			if (cmdId == -1)
			{
				continue;
			}

			//ָ���ָ����������
			oneCmd->m_commandTableId = cmdTableId;
			oneCmd->m_id = cmdId;
			result = m_EXcelDAO->InsertCommandCommandtable(oneCmd);

			if (result == true)
			{
				cmdCount++;
				m_EXcelDAO->GetCommandInfo();//ˢ��ָ�����������ʹ��
			}
			
		}
		
	}
	//QString str = QString("�ɹ����������%1����ָ�%2��").arg(paramCount).arg(cmdCount);
	QMessageBox::information(nullptr,"��Ϣ", QString("�ɹ����������%1����ָ�%2��").arg(paramCount).arg(cmdCount),"ȷ��");
	return;
}

/// <summary>
/// ��ȡΨһ��code
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
		if (find(code.begin(), code.end(), newcode) == code.end())
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