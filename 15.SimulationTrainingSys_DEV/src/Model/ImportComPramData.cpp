#include "ImportComPramData.h"

#include <QtCore/qtextstream.h>
#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QtWidgets/qfiledialog.h>

#include <qfiledialog.h>
//#include <ActiveQt/qaxobject.h>
#include <QtWidgets/qmessagebox.h>

//#include <QAxObject>

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

#if 0
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


	//QString filename = readFile;
	//QFile f(filename);

	//if (f.exists())
	//{
	//	int flag = 0;

	//	QAxObject excel("Excel.Application"); // ����excel�ؼ�
	//	excel.setProperty("Visible", false); // ����ʾ����

	//	QAxObject* workbooks = excel.querySubObject("WorkBooks"); //��ȡ����������

	//	workbooks->dynamicCall("Open(const QString&)", filename); // ���ļ�

	//	QAxObject* workbook = excel.querySubObject("ActiveWorkBook");
	//	//QAxObject* sheets = workbook->querySubObject("Sheets");
	//	QAxObject* sheet = workbook->querySubObject("Sheets(int)", 1);
	//	QAxObject* range = sheet->querySubObject("UsedRange");
	//	QAxObject* rows = range->querySubObject("Rows");
	//	QAxObject* columns = range->querySubObject("Columns");

	//	// ע���С����ж���ģ��磺�б��⣩���ݵ��ų�
	//	int rowStart = range->property("Row").toInt();
	//	int columnStart = range->property("Column").toInt();

	//	int nRow = rows->property("Count").toInt();
	//	int nColumn = columns->property("Count").toInt();

	//	// ʵ�������ȡ���������᲻��ȷ�������Ҫ�Լ������ж�

	//// ��ӡÿ�еĵ�2�е�Ԫ��
	//	for (int i = rowStart; i <= nRow; i++)
	//	{
	//		QAxObject* cell = range->querySubObject("Cells(int,int)", i, 2);
	//		QString strValue = cell->property("Value").toString();
	//		if (strValue != "")
	//		{
	//			qDebug() << strValue;
	//		}

	//	}

	//	workbook->dynamicCall("Close(Boolen)", false); // �ر��ļ�
	//	excel.dynamicCall("Quit(void)");              // �˳�

	//}

}
