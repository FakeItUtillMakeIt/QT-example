#include "CustomFlowEditWidget.h"

using namespace CUSTOM_FLOW_EDIT_WIDGET;


CustomFlowEditWidget::CustomFlowEditWidget(QWidget *parent)
	: QWidget(parent)
{


	QGridLayout* leftLayout=new QGridLayout;
	QHBoxLayout* leftTopLayout = new QHBoxLayout;
	QGridLayout* rightLayout=new QGridLayout;
	QHBoxLayout* boxLayout = new QHBoxLayout;

	QHBoxLayout* bottomLayout = new QHBoxLayout;

	leftSearchInput = new QLineEdit;
	leftSearchButton = new QPushButton;
	leftSearchButton->setIcon(QIcon(":/ControlMonitor/icon/find.png"));
	leftAddFlowButton = new QPushButton;
	leftAddFlowButton->setIcon(QIcon(":/ControlMonitor/icon/+hao.png"));

	leftProjectFlowList = new QListWidget;
	leftSearchInput->setMaximumWidth(80);
	leftSearchButton->setMaximumWidth(20);
	leftAddFlowButton->setMaximumWidth(20);
	leftProjectFlowList->setMaximumWidth(120);
	leftProjectFlowList->setStyleSheet("* {background:rgb(220,220,220);border:0}");


	leftTopLayout->addWidget(leftSearchInput);
	leftTopLayout->addWidget(leftSearchButton);
	leftTopLayout->addWidget(leftAddFlowButton);

	leftLayout->addLayout(leftTopLayout,0,0);
	leftLayout->addWidget(leftProjectFlowList);

	rightFlowNameLabel = new QLabel(QString::fromLocal8Bit("xxx��Ŀ����"));
	rightFlowEditButton = new QPushButton(QString::fromLocal8Bit("�༭"));
	rightFlowDeleteButton = new QPushButton(QString::fromLocal8Bit("ɾ��"));
	rightFlowUpdateButton = new QPushButton;
	rightFlowUpdateButton->setIcon(QIcon(":/ControlMonitor/icon/update.png"));
	rightFlowUpdateButton->setMaximumWidth(20);

	rightFlowTable = new CustomTableWidget;
	rightFlowTable->setMinimumWidth(600);
	rightFlowTable->setMinimumHeight(400);
	
	rightLayout->addWidget(rightFlowNameLabel, 0, 0,1,4);
	rightLayout->addWidget(rightFlowEditButton, 0, 4, 1, 1);
	rightLayout->addWidget(rightFlowDeleteButton, 0, 5, 1, 1);
	rightLayout->addWidget(rightFlowUpdateButton, 0, 6, 1, 1);

	rightLayout->addWidget(rightFlowTable, 1, 0, 1, 7);

	bottomCancleButton = new QPushButton(QString::fromLocal8Bit("ȡ��"));
	bottomOKButton = new QPushButton(QString::fromLocal8Bit("ȷ��"));

	bottomLayout->addStretch();
	bottomLayout->addWidget(bottomCancleButton);
	bottomLayout->addWidget(bottomOKButton);

	boxLayout->addLayout(leftLayout);
	boxLayout->addLayout(rightLayout);
	QVBoxLayout* totalLayout = new QVBoxLayout;
	totalLayout->addLayout(boxLayout);
	totalLayout->addLayout(bottomLayout);

	this->setLayout(totalLayout);
	initConnection();

	tableColumnKey << QString::fromLocal8Bit("���") << QString::fromLocal8Bit("����")
		<< QString::fromLocal8Bit("����") << QString::fromLocal8Bit("����")
		<< QString::fromLocal8Bit("��ע");

	setStylize();
}



CustomFlowEditWidget::~CustomFlowEditWidget()
{
}

void CustomFlowEditWidget::setStylize() {
	leftSearchButton->setStyleSheet("background:rgb(30, 144, 255); ");
	leftAddFlowButton->setStyleSheet("background:rgb(30, 144, 255); ");
	rightFlowEditButton->setStyleSheet("background:rgb(30, 144, 255); ");
	rightFlowDeleteButton->setStyleSheet("background:rgb(30, 144, 255); ");
	rightFlowUpdateButton->setStyleSheet("background:rgb(30, 144, 255); ");
	bottomOKButton->setStyleSheet("background:rgb(30, 144, 255); ");
	bottomCancleButton->setStyleSheet("color:red; ");
	bottomCancleButton->hide();
	bottomOKButton->hide();
	this->setStyleSheet("* {background:rgb(245,245,245);}");
}

void CustomFlowEditWidget::initConnection() {
	connect(leftSearchButton, &QPushButton::clicked, this, [=]() {
		
		});

	connect(leftAddFlowButton, &QPushButton::clicked, this, &CustomFlowEditWidget::addNewFlow);

	connect(leftProjectFlowList, &QListWidget::itemPressed, this, &CustomFlowEditWidget::loadFlowTable);

	connect(rightFlowTable, &CustomTableWidget::mouseRightClicked, this, &CustomFlowEditWidget::tableRightMouseClick);

	connect(rightFlowTable, &CustomTableWidget::cellClicked, this, &CustomFlowEditWidget::tableCellClick);

	connect(rightFlowEditButton, &QPushButton::clicked, this, &CustomFlowEditWidget::tableEditClick);

	connect(rightFlowTable, &CustomTableWidget::cellDoubleClicked, this, &CustomFlowEditWidget::doubleClickTable);

	connect(bottomOKButton, &QPushButton::clicked, this, &CustomFlowEditWidget::okButtonClick);

	connect(bottomCancleButton, &QPushButton::clicked, this, &CustomFlowEditWidget::cancelButtonClick);
}

/**
    @brief ��������̱�
**/
void CustomFlowEditWidget::addNewFlow() {

	QWidget* newFlowName = new QWidget();
	newFlowName->setWindowTitle(QString::fromLocal8Bit("���������"));
	QLabel* label = new QLabel(QString::fromLocal8Bit("��������"));
	QLineEdit* lineEdit = new QLineEdit;
	QPushButton* okButton = new QPushButton(QString::fromLocal8Bit("ȷ��"));
	QHBoxLayout* newFlowLayout = new QHBoxLayout;
	newFlowLayout->addWidget(label);
	newFlowLayout->addWidget(lineEdit);
	newFlowLayout->addWidget(okButton);
	newFlowName->setLayout(newFlowLayout);
	//newFlowName->setWindowFlags(Qt::SubWindow);
	newFlowName->raise();
	newFlowName->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
	newFlowName->show();

	connect(okButton, &QPushButton::clicked, this, [=]() {
		if (lineEdit->text().isEmpty())
		{
			newFlowName->deleteLater();
			return;
		}
		leftProjectFlowList->addItem(new QListWidgetItem(lineEdit->text()));
		newFlowName->deleteLater();

		});

	//auto dingCheckExcel = new QXlsx::Document();

}

/**
    @brief �������������
**/
void CustomFlowEditWidget::loadFlowTable() {
	rightFlowTable->horizontalHeader()->hide();
	rightFlowTable->verticalHeader()->hide();
	rightFlowTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	rightFlowTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	rightFlowTable->setColumnCount(5);
	rightFlowTable->setRowCount(1);
	//rowHeight = rightFlowTable->height() / 6;
	rightFlowTable->setRowHeight(0, rightFlowTable->height() / 6);
	
	int i = 0;
	for (QString key:tableColumnKey)
	{
		QTableWidgetItem* item = new QTableWidgetItem(key);
		QFont font;
		font.setBold(true);
		item->setFont(font);
		item->setFlags(Qt::NoItemFlags);
		columnWidth = rightFlowTable->width() / 5;
		rightFlowTable->setColumnWidth(i, rightFlowTable->width() / 5);
		
		rightFlowTable->setItem(0, i++, item);
	}
}


void CustomFlowEditWidget::tableRightMouseClick(QPoint pos) {
	cursorPos = pos;

}

void CustomFlowEditWidget::tableCellClick(int row,int column) {
	
	if (row==-1 || column==-1)
	{
		return;
	}
	
	if (rightFlowTable->item(0, column)->text().contains(tableColumnKey[column]))
	{
		QWidget* widget = new QWidget;
		QVBoxLayout* boxLayout = new QVBoxLayout;
		QPushButton* frontInsertCell = new QPushButton(QString::fromLocal8Bit("��ǰ����")+ tableColumnKey[column]);
		QPushButton* backInsertCell = new QPushButton(QString::fromLocal8Bit("������") + tableColumnKey[column]);
		QPushButton* removeCell = new QPushButton(QString::fromLocal8Bit("ɾ��") + tableColumnKey[column]);
		frontInsertCell->setStyleSheet("* {color:black;border:none;}");
		backInsertCell->setStyleSheet("* {color:black;border:none;}");
		removeCell->setStyleSheet("* {color:red;border:none;}");
		boxLayout->addWidget(frontInsertCell);
		boxLayout->addWidget(backInsertCell);
		boxLayout->addWidget(removeCell);

		widget->setLayout(boxLayout);

		widget->setWindowFlags(Qt::Popup);
		widget->raise();
		widget->setFixedWidth(rightFlowTable->columnWidth(column));
		
		widget->move(cursorPos.x(),cursorPos.y());
		widget->show();
		//��ǰ����
		connect(frontInsertCell, &QPushButton::clicked, this, [=]() {
			if (row==0)
			{
				return;
			}


#ifdef _USE_TABLE_ENBLED_
			if (column == 0 || column == 1)
			{
				rightFlowTable->insertRow(row);
				rowMaxRowCount[row ] = 2;
			}
			else
			{
				
				SubTableWidget* subTable = new SubTableWidget();

				//subTable->setEditTriggers(QAbstractItemView::CurrentChanged);
				subTable->setRowCount(2);
				subTable->setColumnCount(1);
				subTable->horizontalHeader()->hide();
				subTable->verticalHeader()->hide();
				rightFlowTable->setCellWidget(row, column, subTable);
				rightFlowTable->setRowHeight(row, rowHeight * rowMaxRowCount[row] / 2);
				subTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
				subTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
				for (int i = 0; i < subTable->rowCount(); i++)
				{
					subTable->setRowHeight(i, rowHeight* rowMaxRowCount[row] / 2 / 2);
					subTable->setColumnWidth(0, columnWidth);
					QTableWidgetItem* item = new QTableWidgetItem;

					//item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
					subTable->setItem(i, 0, item);
				}

				connect(subTable, &SubTableWidget::opOver, this, &CustomFlowEditWidget::adjustCellHeight);

			}

#endif

#ifndef _USE_SPAN_

			if (column == 2 || column == 3)
			{
				for (int c = 0; c < rightFlowTable->columnCount(); c++)
				{
					if (column == c)
					{
						continue;
					}
					rightFlowTable->setSpan(row, c, 2, 1);
				}

			}

#endif // _USE_SPAN_

			});

		//������
		connect(backInsertCell, &QPushButton::clicked, this, [=]() {
			
			//ʹ�õ�Ԫ������widgetʵ�ֵ�Ԫ������ɾ������
#ifdef _USE_TABLE_ENBLED_
			if (column == 0 || column == 1)
			{
				rightFlowTable->insertRow(row + 1);
				rowMaxRowCount[row + 1] = 2;
			}
			else
			{
				if (row==0)
				{
					return;
				}
				SubTableWidget* subTable = new SubTableWidget();
		
				//subTable->setEditTriggers(QAbstractItemView::CurrentChanged);
				subTable->setRowCount(2);
				subTable->setColumnCount(1);
				subTable->horizontalHeader()->hide();
				subTable->verticalHeader()->hide();
				rightFlowTable->setCellWidget(row, column, subTable);
				rightFlowTable->setRowHeight(row, rowHeight* rowMaxRowCount[row] /2);
				subTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
				subTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
				for (int i = 0; i < subTable->rowCount(); i++)
				{
					subTable->setRowHeight(i, rowHeight* rowMaxRowCount[row] / 2 / 2);
					subTable->setColumnWidth(0, columnWidth);
					QTableWidgetItem* item = new QTableWidgetItem;
					
					//item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
					subTable->setItem(i, 0, item);
				}

				connect(subTable, &SubTableWidget::opOver, this, &CustomFlowEditWidget::adjustCellHeight);

			}
			
#endif
			for(int i = 0; i < rightFlowTable->columnCount(); i++)
			{
				QTableWidgetItem* item = new QTableWidgetItem;
				rightFlowTable->setItem(row + 1, i, item);

			}

			for (int r = 1; r < rightFlowTable->rowCount(); r++)
			{
				rightFlowTable->item(r, 0)->setText(QString::number(r));
			}
			widget->deleteLater();

			//ʹ�úϲ���ֵ�Ԫ��ʵ�ֲ����ɾ������
#ifndef _USE_SPAN_
			
			if (column == 2 || column == 3 || column==4)
			{
				mergeCell[column].push_back(row);
				mergeCell[column].push_back(row + 1);
				rowInclude[row].push_back(row);
				rowInclude[row].push_back(row + 1);
				for (int c = 0; c < rightFlowTable->columnCount(); c++)
				{
					if (!mergeCell.keys().contains(c))
					{
						rightFlowTable->setSpan(row, c, 2, 1);
					}

				}

			}
			

#endif // _USE_SPAN_
			
			});

		connect(removeCell, &QPushButton::clicked, this, [=]() {
			if (row==0)
			{
				return;
			}
			for (int i = 0; i < rightFlowTable->columnCount(); i++)
			{
				rightFlowTable->takeItem(row, i);
			}
			if(column==1)
				rightFlowTable->removeRow(row);
			//rightFlowTable->removeCellWidget(row, column);

			for (int r = 1; r < rightFlowTable->rowCount(); r++)
			{
				if(rightFlowTable->item(r,0))
					rightFlowTable->item(r, 0)->setText(QString::number(r));
			}
			widget->deleteLater();
			});
	}
}

void CustomFlowEditWidget::tableEditClick() {
	rightFlowTable->setEditTriggers(QAbstractItemView::CurrentChanged);
	for (int r=1;r<rightFlowTable->rowCount();r++)
	{
		for (int c=1;c<rightFlowTable->columnCount();c++)
		{
			rightFlowTable->item(r, c)->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled|Qt::ItemIsSelectable);
		}
	}
}

void CustomFlowEditWidget::doubleClickTable(int row, int column) {
	rightFlowTable->editItem(rightFlowTable->item(row, column));
}


void CustomFlowEditWidget::adjustCellHeight(QPoint p) {
	auto row1 = rightFlowTable->currentRow();
	auto column1 = rightFlowTable->currentColumn();
	qDebug() << row1 << column1;
	//��֤����
	int maxEnbedRow = 1;
	for (int c=1;c<rightFlowTable->columnCount();c++)
	{
		SubTableWidget* w1 = static_cast<SubTableWidget*>(rightFlowTable->cellWidget(row1, c));
		if(w1==nullptr)
			continue;
		if (w1->rowCount() > maxEnbedRow)
			maxEnbedRow = w1->rowCount();
	}
	
	rowMaxRowCount[row1] = maxEnbedRow;
	/*if (maxEnbedRow > rowMaxRowCount[row1])
	{
		rowMaxRowCount[row1] = maxEnbedRow;
	}*/
	rightFlowTable->setRowHeight(row1, rowMaxRowCount[row1] * rowHeight / 2);
	for (int c = 1; c < rightFlowTable->columnCount(); c++)
	{
		SubTableWidget* w1 = static_cast<SubTableWidget*>(rightFlowTable->cellWidget(row1, c));
		if (w1 == nullptr)
			continue;
		for (int subRowIndex = 0; subRowIndex < w1->rowCount(); subRowIndex++)
		{
			w1->setRowHeight(subRowIndex, rowMaxRowCount[row1] * rowHeight / w1->rowCount() / 2);
		}
	}
}

//OKʱ��Ҫ����ӵ����̴洢
void CustomFlowEditWidget::okButtonClick() {


	this->deleteLater();
}

//ȡ����رմ���
void CustomFlowEditWidget::cancelButtonClick() {

	this->deleteLater();
}

