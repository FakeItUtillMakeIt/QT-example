#include "DropWidget.h"

DropWidget::DropWidget(){

	this->setMinimumHeight(200);
	this->setMinimumWidth(200);
	this->setBackgroundRole(QPalette::Light);
}

DropWidget::~DropWidget() {

}


void DropWidget::dragEnterEvent(QDragEnterEvent* event) {
	//先查看拖拽到场景的物体是否是模型
	const QMimeData* data = event->mimeData();
	QVariant type = data->property("TYPE");
	if (type == QVariant("MODLES"))
	{
		QVariant type1 = data->property("MODEL_TYPE");
		event->accept();
	}
	else
	{
		event->ignore();
	}

}
void DropWidget::dragLeaveEvent(QDragLeaveEvent* event) {


}
void DropWidget::dragMoveEvent(QDragMoveEvent* event) {
	if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else {
		event->ignore();
	}

}
void DropWidget::dropEvent(QDropEvent* event) {
	if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
		QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QPixmap pixmap;
		QPoint offset;
		dataStream >> pixmap >> offset;

		QLabel* newIcon = new QLabel(this);
		newIcon->setPixmap(pixmap);
		newIcon->move(event->pos() - offset);
		newIcon->show();
		newIcon->setAttribute(Qt::WA_DeleteOnClose);

		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else {
		event->ignore();
	}

}