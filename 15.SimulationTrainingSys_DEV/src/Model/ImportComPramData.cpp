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
/// 导入参数
/// </summary>
void ImportComPramData::AddPramComDatas(int rocketID)
{
	//读取excel文件
	QString readFile = QFileDialog::getOpenFileName(nullptr, QStringLiteral("选择Excel文件"), "", tr("Exel file(*.xls *.xlsx)"));
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
		QMessageBox::warning(nullptr,"提示","文件打开失败！","确定");
		return;
	}

	int rowLen = m_xlsx->dimension().rowCount();           // 获取最大行数
	int columnLen = m_xlsx->dimension().columnCount();     // 获取最大列数
	for (int i = 1; i <= rowLen; i++)                       // 遍历每一行
	{
		QString data = QString("第%1行：").arg(i);
		for (int j = 1; j <= columnLen; j++)                // 遍历每一个单元格(列)
		{
#if 1
			QVariant value = m_xlsx->read(i, j);                                         // 通过单元格行号、列号读取数据
#else
			QVariant value = m_xlsx->read(QString("%1%2").arg((char)(64 + i)).arg(j));   // 通过单元格引用读取数据
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

	//20220902后续需要完成，得到excel文件数据，对6个数据库表格进行协同写入。



#if 0 
	//qt的存在问题，选择了使用上面的方法
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
}
