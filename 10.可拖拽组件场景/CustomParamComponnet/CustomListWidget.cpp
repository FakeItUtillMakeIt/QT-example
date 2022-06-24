#include "CustomListWidget.h"

using namespace CUSTOM_LIST_WIDGET;

/**
    @brief CustomListWidget object constructor
    @param parent - ���ʼ�������캯��
**/
CustomListWidget::CustomListWidget(QWidget* parent) :QListWidget(parent) {

	selectIndex = -1;
	selectElement = false;

	setIconSize(QSize(150, 150));
	setViewMode(QListView::IconMode);
	setResizeMode(QListView::Adjust);
	setMovement(QListView::Static);
	//setFlow(QListView::TopToBottom);
	setWrapping(true);
	setModelColumn(0);
	setLayoutMode(QListView::SinglePass);
	setGridSize(QSize(80, 80));

	this->setStyleSheet("* {background-color:0f3000}");
}

CustomListWidget::~CustomListWidget() {

}

/**
    @brief 
    @param event - 
**/
void CustomListWidget::mouseMoveEvent(QMouseEvent* event) {
	if (!selectElement)
	{
		return;
	}

	if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
	{
		return;
	}

	QDrag* drag = new QDrag(this);
	QMimeData* mimeData = new QMimeData;
	drag->setMimeData(mimeData);
	QByteArray dataByte;
	//dataByte.setNum(selectIndex);
	QDataStream dataStream(&dataByte, QIODevice::WriteOnly);

	dataStream << selectIndex << QPoint(event->pos() - dragStartPosition);

	mimeData->setData("selectIndex", dataByte);
	

	QPixmap tmpPixmap = this->currentItem()->icon().pixmap(0);
	drag->setPixmap(tmpPixmap);
	drag->setHotSpot(event->pos());

	drag->exec();

	//event->accept();
	//drag->setDragCursor(tmpPixmap,Qt::CopyAction);
}

/**
    @brief 
    @param event - 
**/
void CustomListWidget::mousePressEvent(QMouseEvent* event) {
	
	selectIndex = row(itemAt(event->pos()));
	dragStartPosition = event->pos();
	if (event->button() == Qt::LeftButton)
	{
		selectElement = true;
		
	}



}

/**
    @brief 
    @param event - 
**/
void CustomListWidget::mouseReleaseEvent(QMouseEvent* event) {
	selectElement = false;
	selectIndex = -1;
}


void CustomListWidget::dragMoveEvent(QDragMoveEvent* event) {
	event->acceptProposedAction();
}

void CustomListWidget::dragEnterEvent(QDragEnterEvent* event) {

	event->acceptProposedAction();

}