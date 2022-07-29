#include "CustomTableWidget.h"

using namespace CUSTOM_TABLE_WIDGET;

CustomTableWidget::CustomTableWidget() {

}

CustomTableWidget::~CustomTableWidget() {

}

void CustomTableWidget::mousePressEvent(QMouseEvent* event) {

	if (event->button() == Qt::RightButton)
	{
		auto a = this->indexAt(event->pos());

		auto b = this->itemFromIndex(a);

		if (!a.isValid())
			return;

		emit mouseRightClicked(event->globalPos());
		qDebug() << a.row() << a.column();
		//emit cellClicked(b->row(), b->column());
		emit cellClicked(a.row(), a.column());
	}


}


void CustomTableWidget::mouseDoubleClickEvent(QMouseEvent* event) {

	auto a = this->indexAt(event->pos());
	if (!a.isValid())
		return;
	auto b = this->itemFromIndex(a);
	if (b == nullptr)
	{
		return;
	}
	emit cellDoubleClicked(b->row(), b->column());
}