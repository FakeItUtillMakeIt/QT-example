#include "ListWidgetView.h"

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
	drag->exec(Qt::MoveAction);
}
