#include "ItemDelegate.h"

ItemDelegate::ItemDelegate(QObject* parent) :
	QStyledItemDelegate(parent)
{

}

/**
	@brief 重写View的显示内容
	@param painter -
	@param option  -
	@param index   -
**/
void ItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	QStyleOptionViewItem viewOption(option);
	if (viewOption.state & QStyle::State_HasFocus) {
		viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
	}
	QStyledItemDelegate::paint(painter, viewOption, index);
	int height = (viewOption.rect.height() - 15) / 2;
	QPixmap pixmap = QPixmap(":/DeviceManager/icon/dlt.png");
	QRect decorationRect = QRect(viewOption.rect.left() + viewOption.rect.width() - 30, viewOption.rect.top() + height, 20, 20);
	painter->drawPixmap(decorationRect, pixmap);
}

/**
	@brief  编辑事件响应
	@param  event  -
	@param  model  -
	@param  option -
	@param  index  -
	@retval        -
**/
bool ItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) {
	int height = (option.rect.height() - 15) / 2;
	QRect decroationRect = QRect(option.rect.left() + option.rect.width() - 30,
		option.rect.top() + height, 20, 20);
	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
	if (event->type() == QEvent::MouseButtonPress && decroationRect.contains(mouseEvent->pos())) {
		emit deleteItem(index);
	}
	if (event->type() == QEvent::MouseMove && decroationRect.contains(mouseEvent->pos())) {
		QCursor cursor(Qt::PointingHandCursor);
		QApplication::setOverrideCursor(cursor);
		QString strtext = QStringLiteral("删除该项");
		QToolTip::showText(mouseEvent->globalPos(), strtext);
	}
	else {
		QCursor cursor(Qt::ArrowCursor);
		QApplication::setOverrideCursor(cursor);
	}

	return QStyledItemDelegate::editorEvent(event, model, option, index);
}



