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
/// 导入参数
/// </summary>
void ImportComPramData::AddPramComDatas(int rocketID)
{


	//读取excel文件
	QString readFile = QFileDialog::getOpenFileName(nullptr, QStringLiteral("选择Excel文件"), "", tr("Exel file(*.xls *.xlsx)"));
	int row_count, col_count;
	QStringList str;

#if 0
	if (!readFile.isEmpty())
	{
		QAxObject excel("Excel.Application");
		excel.setProperty("Visible", false); //不显示Excel界面，如果为true会看到启动的Excel界面  
		QAxObject* work_books = excel.querySubObject("WorkBooks");
		work_books->dynamicCall("Open (const QString&)", readFile);//打开指定文件
		QAxObject* work_book = excel.querySubObject("ActiveWorkBook");
		QAxObject* work_sheets = work_book->querySubObject("Sheets");  //获取工作表，Sheets也可换用WorkSheets
		int sheet_count = work_sheets->property("Count").toInt();  //获取工作表数目

		if (sheet_count > 0)
		{
			QAxObject* work_sheet = work_book->querySubObject("Sheets(int)", 1); //表格sheet，参数 "1" 代表第1个sheet
			QAxObject* used_range = work_sheet->querySubObject("UsedRange");
			QAxObject* rows = used_range->querySubObject("Rows");
			QAxObject* colums = used_range->querySubObject("Columns");
			row_count = rows->property("Count").toInt();  //获取行数
			col_count = colums->property("Count").toInt(); //获取列数
			//QString txt = work_sheet->querySubObject("Cells(int,int)", i, 1)->property("Value").toString(); //获取单元格内容
			for (int i = 1; i <= row_count; i++)
			{
				for (int j = 1; j <= col_count; j++)
				{
					QString cell = work_sheet->querySubObject("Cells(int,int)", i, j)->property("Value").toString(); //获取表格内容
					str.append(cell);
					qDebug() << cell << endl;
				}

			}

			work_book->dynamicCall("Close()", false);  //关闭文件
			excel.dynamicCall("Quit()");  //退出
		}
	}
	else
	{
		//QMessageBox::warning(this, "提示", "文件路径为空！");
	}

#endif


	//QString filename = readFile;
	//QFile f(filename);

	//if (f.exists())
	//{
	//	int flag = 0;

	//	QAxObject excel("Excel.Application"); // 连接excel控件
	//	excel.setProperty("Visible", false); // 不显示窗体

	//	QAxObject* workbooks = excel.querySubObject("WorkBooks"); //获取工作簿集合

	//	workbooks->dynamicCall("Open(const QString&)", filename); // 打开文件

	//	QAxObject* workbook = excel.querySubObject("ActiveWorkBook");
	//	//QAxObject* sheets = workbook->querySubObject("Sheets");
	//	QAxObject* sheet = workbook->querySubObject("Sheets(int)", 1);
	//	QAxObject* range = sheet->querySubObject("UsedRange");
	//	QAxObject* rows = range->querySubObject("Rows");
	//	QAxObject* columns = range->querySubObject("Columns");

	//	// 注意行、列中多余的（如：列标题）数据的排除
	//	int rowStart = range->property("Row").toInt();
	//	int columnStart = range->property("Column").toInt();

	//	int nRow = rows->property("Count").toInt();
	//	int nColumn = columns->property("Count").toInt();

	//	// 实测上面获取到的列数会不正确，因此需要自己重新判断

	//// 打印每行的第2列的元素
	//	for (int i = rowStart; i <= nRow; i++)
	//	{
	//		QAxObject* cell = range->querySubObject("Cells(int,int)", i, 2);
	//		QString strValue = cell->property("Value").toString();
	//		if (strValue != "")
	//		{
	//			qDebug() << strValue;
	//		}

	//	}

	//	workbook->dynamicCall("Close(Boolen)", false); // 关闭文件
	//	excel.dynamicCall("Quit(void)");              // 退出

	//}

}
