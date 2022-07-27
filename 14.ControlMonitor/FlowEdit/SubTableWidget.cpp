#include "SubTableWidget.h"

using namespace SUB_TABLE_WIDGET;
/**
    @brief  ��table���캯��
    @retval  - 
**/
SubTableWidget::SubTableWidget() {

	row = 0, column = 0;

	popWidget = new QWidget;
	QVBoxLayout* boxLayout = new QVBoxLayout;
	frontInsertCell = new QPushButton(QString::fromLocal8Bit("��ǰ����"));
	backInsertCell = new QPushButton(QString::fromLocal8Bit("������"));
	removeCell = new QPushButton(QString::fromLocal8Bit("ɾ��"));
	frontInsertCell->setStyleSheet("* {color:black;border:none;}");
	backInsertCell->setStyleSheet("* {color:black;border:none;}");
	removeCell->setStyleSheet("* {color:red;border:none;}");
	boxLayout->addWidget(frontInsertCell);
	boxLayout->addWidget(backInsertCell);
	boxLayout->addWidget(removeCell);

	popWidget->setLayout(boxLayout);

	popWidget->setWindowFlags(Qt::Popup);

	connect(this, &SubTableWidget::cellClicked, this, &SubTableWidget::rightButtonClicked);
	connect(this, &SubTableWidget::cellDoubleClicked, this, &SubTableWidget::doubleClickTable);

	connect(frontInsertCell, &QPushButton::clicked, this, [=]() {
		this->insertRow(row);

		this->setItem(row, 0, new QTableWidgetItem);
		emit opOver(cursorPos);
		popWidget->hide();
		});


	connect(backInsertCell, &QPushButton::clicked, this, [=]() {
		this->insertRow(row + 1);
		
		this->setItem(row + 1, 0, new QTableWidgetItem);
		emit opOver(cursorPos);
		popWidget->hide();
		});

	connect(removeCell, &QPushButton::clicked, this, [=]() {

		this->removeRow(row);
		emit opOver(cursorPos);
		popWidget->hide();
		});
}

void SubTableWidget::rightButtonClicked(int row, int column) {
	if (row == -1 || column == -1 )
	{
		return;
	}

	popWidget->move(cursorPos);
	popWidget->raise();
	popWidget->show();

	row = row;
	column = column;

}
void SubTableWidget::doubleClickTable(int row, int column) {
	this->editItem(this->item(row, column));
}


void SubTableWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::RightButton)
	{
		auto a = this->indexAt(event->pos());
		if (!a.isValid())
			return;
		auto b = this->itemFromIndex(a);

		//emit mouseRightClicked(event->globalPos());
		cursorPos = event->globalPos();
		emit cellClicked(b->row(), b->column());

	}
}

void SubTableWidget::mouseDoubleClickEvent(QMouseEvent* event) {
	auto a = this->indexAt(event->pos());
	if (!a.isValid())
		return;
	auto b = this->itemFromIndex(a);
	emit cellDoubleClicked(b->row(), b->column());
}


