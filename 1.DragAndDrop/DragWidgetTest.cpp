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

	QPushButton* pushbtn = new QPushButton(this);
	pushbtn->move(0, 20);
	pushbtn->show();
}

void DragWidgetTest::dragEnterEvent(QDragEnterEvent* event)
{
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

void DragWidgetTest::dragMoveEvent(QDragMoveEvent* event)
{
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

void DragWidgetTest::dropEvent(QDropEvent* event)
{
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

void DragWidgetTest::mousePressEvent(QMouseEvent* event)
{
	QLabel* child = static_cast<QLabel*>(childAt(event->pos()));
	if (!child)
		return;

	QPixmap pixmap = *child->pixmap();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << pixmap << QPoint(event->pos() - child->pos());

	QMimeData* mimeData = new QMimeData;
	mimeData->setData("application/x-dnditemdata", itemData);

	QDrag* drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(pixmap);
	drag->setHotSpot(event->pos() - child->pos());

	QPixmap tempPixmap = pixmap;
	QPainter painter;
	painter.begin(&tempPixmap);
	painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
	painter.end();

	child->setPixmap(tempPixmap);

	if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
		child->close();
	}
	else {
		child->show();
		child->setPixmap(pixmap);
	}
}
