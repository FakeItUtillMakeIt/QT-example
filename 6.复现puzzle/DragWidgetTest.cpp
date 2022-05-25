#include "DragWidgetTest.h"
#include <QtWidgets>


DragWidgetTest::DragWidgetTest(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);
	setMinimumSize(200, 200);
	//setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
	setAcceptDrops(true);

	QPixmap p1(QSize(50, 50));
	p1.fill(Qt::black);
	QPixmap p2(QSize(50, 50));
	p2.fill(Qt::red);
	QPixmap p3(QSize(50, 50));
	p3.fill(Qt::blue);

	QLCDNumber* pushbtn = new QLCDNumber(this);
	pushbtn->move(0, 20);
	pushbtn->show();
	pushbtn->setAttribute(Qt::WA_DeleteOnClose);
	

	QLabel* boatIcon = new QLabel(this);
	boatIcon->setPixmap(p1);
	boatIcon->move(10, 10);
	boatIcon->show();
	boatIcon->setAttribute(Qt::WA_DeleteOnClose);

	QLabel* carIcon = new QLabel(this);
	carIcon->setPixmap(p2);
	carIcon->move(100, 10);
	carIcon->show();
	carIcon->setAttribute(Qt::WA_DeleteOnClose);

	QLabel* houseIcon = new QLabel(this);
	houseIcon->setPixmap(p3);
	houseIcon->move(10, 80);
	houseIcon->show();
	houseIcon->setAttribute(Qt::WA_DeleteOnClose);

	
}
/**
    @brief 通常用于通知qt小部件接受的数据类型
    @param event 
**/
void DragWidgetTest::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()) {
		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			//event->acceptProposedAction();
			event->accept();
		}
	}
	else {
		event->ignore();
	}
}

/**
    @brief 确认是否应该拖动，并构造一个拖动对象来处理该操作
    @param event 
**/
void DragWidgetTest::dragMoveEvent(QDragMoveEvent* event)
{
	if (event->mimeData()) {
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

void DragWidgetTest::dropEvent(QDropEvent* event)
{
	QWidget* widget=QApplication::focusWidget();
	if (event->mimeData()) {
		QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QPixmap pixmap(QSize(50,50));
		QPoint offset;
		//dataStream >> pixmap >> offset;

		QLabel* newIcon = new QLabel(this);
		newIcon->setPixmap(pixmap);
		newIcon->move(event->pos() );
		newIcon->show();
		newIcon->setAttribute(Qt::WA_DeleteOnClose);

		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
			/*event->setDropAction(Qt::CopyAction);
			event->accept();*/
		}
	}
	else {
		event->ignore();
	}
}


/**
    @brief 启用小部件拖动最简单的方法：实现mousePressEvent
    @param event 
**/
void DragWidgetTest::mousePressEvent(QMouseEvent* event)
{
	QWidget* currentitem;
	if (event->button()==Qt::LeftButton)
	{
		currentitem = QApplication::focusWidget();
	}
	//QLabel* child = static_cast<QLabel*>(childAt(event->pos()));
	auto* child = static_cast<QWidget*>(childAt(event->pos()));
	if (!child)
		return;

	//QPixmap pixmap = *child->pixmap();
	QPixmap pixmap(QSize(50, 50));

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << pixmap << QPoint(event->pos() - child->pos());

	QMimeData* mimeData = new QMimeData;
	mimeData->setData("application/x-dnditemdata", itemData);

	//创建QDrag对象，并调用exec启动拖放操作
	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	drag->setHotSpot(event->pos() - child->pos());

	QPixmap tempPixmap = pixmap;
	QPainter painter;
	painter.begin(&tempPixmap);
	painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
	painter.end();

	//child->setPixmap(tempPixmap);

	if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
		child->close();
	}
	else {
		child->show();
		//child->setPixmap(pixmap);
	}
}
