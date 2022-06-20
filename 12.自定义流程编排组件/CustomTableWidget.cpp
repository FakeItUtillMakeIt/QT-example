#include "CustomTableWidget.h"

using namespace CUSTOM_TABLE_WIDGET;

CustomTableWidget::CustomTableWidget() {

}

CustomTableWidget::~CustomTableWidget() {

}

void CustomTableWidget::mousePressEvent(QMouseEvent* event) {

	if (event->button()==Qt::RightButton)
	{
		auto a = this->indexAt(event->pos());
		if (!a.isValid())
			return;
		auto b=this->itemFromIndex(a);
		
		emit mouseRightClicked(event->globalPos());
		emit cellClicked(b->row(), b->column());

	}


}


void CustomTableWidget::mouseDoubleClickEvent(QMouseEvent* event) {

	auto a = this->indexAt(event->pos());
	if (!a.isValid())
		return;
	auto b = this->itemFromIndex(a);
	emit cellDoubleClicked(b->row(), b->column());
}