#include "ParamEquivalent.h"

ParamEquivalent::ParamEquivalent(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initSignalSlot_fun();
}
//初始化信号连接
bool ParamEquivalent::initSignalSlot_fun() {

    connect(ui.selectPluginFile, &QPushButton::clicked, this, &ParamEquivalent::pluginFile_btn);
    connect(ui.addWidget, &QPushButton::clicked, this, &ParamEquivalent::addNewPage_btn);
    connect(ui.deleteWidget, &QPushButton::clicked, this, &ParamEquivalent::deletePage_btn);
    connect(ui.saveWidget, &QPushButton::clicked, this, &ParamEquivalent::savePage_btn);

    return true;
}

void ParamEquivalent::pluginFile_btn() {
    QStringList filtersFormat;
    filtersFormat << "dynamic library (*.dll)"
        << "static library (*.lib)";
    QFileDialog* fileDiag = new QFileDialog(this);
    fileDiag->setNameFilters(filtersFormat);

    QStringList selectFileName;
    if (fileDiag->exec())
    {
        selectFileName=fileDiag->selectedFiles();
    }
    //dll

}

void ParamEquivalent::addNewPage_btn() {
    auto pageNumber=ui.tabWidget->count();
    ui.tabWidget->insertTab(pageNumber, new QWidget, QIcon(), QString::fromLocal8Bit("Page") + QString::number(pageNumber+1));
}
void ParamEquivalent::deletePage_btn() {
    auto currentPageNumber = ui.tabWidget->currentIndex();
    ui.tabWidget->removeTab(currentPageNumber);
}
void ParamEquivalent::savePage_btn() {

}

void ParamEquivalent::dragEnterEvent(QDragEnterEvent* event) {
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
void ParamEquivalent::dragMoveEvent(QDragMoveEvent* event) {
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
void ParamEquivalent::dropEvent(QDropEvent* event) {
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
void ParamEquivalent::mousePressEvent(QMouseEvent* event) {
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