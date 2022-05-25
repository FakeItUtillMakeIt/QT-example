#include "ListWidgetView.h"


ListWidgetView::ListWidgetView() {
	indexofItem = -1;

	QListWidgetItem* item = new QListWidgetItem(QIcon(), "1");
	item->setData(Qt::UserRole, QVariant());
	this->addItem(item);

	this->setDragEnabled(true);
	this->setAcceptDrops(true);
	this->setDropIndicatorShown(true);
	this->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(this, &ListWidgetView::itemPressed, this, &ListWidgetView::dealPressEvent);
	
}

void ListWidgetView::startDrag(Qt::DropActions supportedActions) {


	QListWidgetItem* item = currentItem();
	QDrag* drag = new QDrag(this);
	QMimeData* mimeData = new QMimeData;

	QByteArray itemData;
	
	QPixmap p1(QSize(50, 50));
	p1.fill(Qt::black);
	mimeData->setText(item->text());
	mimeData->setProperty("TYPE", QVariant("MODELS"));
	mimeData->setProperty("MODEL_TYPE", QVariant("QLABEL"));

	drag->setMimeData(mimeData);
	drag->setPixmap(p1);
	drag->exec(Qt::CopyAction);
}

void ListWidgetView::dealPressEvent(QListWidgetItem* item) {

	indexofItem = row(item);
	this->addItem(new QListWidgetItem(QString::number(indexofItem+1)));
	emit emitdrag(indexofItem);

}

void ListWidgetView::emitdrag(int) {

}