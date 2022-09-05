#include "ImportComPramData.h"

#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QtWidgets/qfiledialog.h>
#include <qfiledialog.h>
#include <QtWidgets/qmessagebox.h>

//#include <ActiveQt/qaxobject.h>

#include "xlsxdocument.h"


#pragma execution_character_set("utf-8")

ImportComPramData::ImportComPramData(QObject *parent)
	: QObject(parent)
{}

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

	int rowLen = m_xlsx->dimension().rowCount();           // ��ȡ�������
	int columnLen = m_xlsx->dimension().columnCount();     // ��ȡ�������
	for (int i = 1; i <= rowLen; i++)                       // ����ÿһ��
	{
		QString data = QString("��%1�У�").arg(i);
		for (int j = 1; j <= columnLen; j++)                // ����ÿһ����Ԫ��(��)
		{
#if 1
			QVariant value = m_xlsx->read(i, j);                                         // ͨ����Ԫ���кš��кŶ�ȡ����
#else
			QVariant value = m_xlsx->read(QString("%1%2").arg((char)(64 + i)).arg(j));   // ͨ����Ԫ�����ö�ȡ����
#endif
			if (!value.isNull())
			{
				data.append(value.toString()).append(" ");
			}
			else
			{
				data.append("NULL ");
			}
		}
		qInfo() << data;
	}

	//20220902������Ҫ��ɣ��õ�excel�ļ����ݣ���6�����ݿ������Эͬд�롣



#if 0 
	//qt�Ĵ������⣬ѡ����ʹ������ķ���
	if (!readFile.isEmpty())
	{
		QAxObject excel("Excel.Application");
		excel.setProperty("Visible", false); //����ʾExcel���棬���Ϊtrue�ῴ��������Excel����  
		QAxObject* work_books = excel.querySubObject("WorkBooks");
		work_books->dynamicCall("Open (const QString&)", readFile);//��ָ���ļ�
		QAxObject* work_book = excel.querySubObject("ActiveWorkBook");
		QAxObject* work_sheets = work_book->querySubObject("Sheets");  //��ȡ������SheetsҲ�ɻ���WorkSheets
		int sheet_count = work_sheets->property("Count").toInt();  //��ȡ��������Ŀ

		if (sheet_count > 0)
		{
			QAxObject* work_sheet = work_book->querySubObject("Sheets(int)", 1); //���sheet������ "1" �����1��sheet
			QAxObject* used_range = work_sheet->querySubObject("UsedRange");
			QAxObject* rows = used_range->querySubObject("Rows");
			QAxObject* colums = used_range->querySubObject("Columns");
			row_count = rows->property("Count").toInt();  //��ȡ����
			col_count = colums->property("Count").toInt(); //��ȡ����
			//QString txt = work_sheet->querySubObject("Cells(int,int)", i, 1)->property("Value").toString(); //��ȡ��Ԫ������
			for (int i = 1; i <= row_count; i++)
			{
				for (int j = 1; j <= col_count; j++)
				{
					QString cell = work_sheet->querySubObject("Cells(int,int)", i, j)->property("Value").toString(); //��ȡ�������
					str.append(cell);
					qDebug() << cell << endl;
				}

			}

			work_book->dynamicCall("Close()", false);  //�ر��ļ�
			excel.dynamicCall("Quit()");  //�˳�
		}
	}
	else
	{
		//QMessageBox::warning(this, "��ʾ", "�ļ�·��Ϊ�գ�");
	}

#endif
}
