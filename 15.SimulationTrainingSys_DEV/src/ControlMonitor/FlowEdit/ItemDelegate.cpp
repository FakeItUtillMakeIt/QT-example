#include "ItemDelegate.h"
#include <QComboBox>

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


Delegate::Delegate(QObject* parent)
	: QItemDelegate(parent)
{

}

Delegate::~Delegate()
{

}
void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	QItemDelegate::paint(painter, option, index);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	return QItemDelegate::sizeHint(option, index);
}

QWidget* Delegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
	if (index.isValid() && index.column() == COMBOXCOL)
	{
		QComboBox* editor = new QComboBox(parent);
		editor->setEditable(true);
		editor->installEventFilter(const_cast<Delegate*>(this));
		return editor;
	}
	else
	{
		return QItemDelegate::createEditor(parent, option, index);
	}
}

void Delegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	if (index.isValid() && index.column() == COMBOXCOL)
	{
		QString value = index.model()->data(index, Qt::DisplayRole).toString();
		QComboBox* combox = static_cast<QComboBox*>(editor);
		FlowInfoConfig2DB::getInstance()->customReadTableInfo(QString("SELECT\
				command_info.id,\
				command_info.`name`,\
				command_info.createTime,\
				command_info.lastUpdateTime\
				FROM\
				command_info\
				WHERE\
				command_info.type = 1 AND\
				command_info.rocket_id = %1").arg(AppCache::instance()->m_CurrentRocketType->m_id));

		for (auto ele : FlowInfoConfig2DB::getInstance()->customReadInfoMap)
		{
			combox->addItem(QString::fromStdString(ele.second[1]), ele.first);
		}

		combox->setCurrentText(value);

	}
	else
	{
		QItemDelegate::setEditorData(editor, index);
	}
}

void Delegate::setModelData(QWidget* editor, QAbstractItemModel* model,
	const QModelIndex& index) const
{
	if (index.isValid() && index.column() == COMBOXCOL)
	{
		QComboBox* combox = static_cast<QComboBox*>(editor);
		model->setData(index, combox->currentText(), Qt::DisplayRole);
		model->setData(index, combox->currentData(), Qt::UserRole);
	}
	else
	{
		QItemDelegate::setModelData(editor, model, index);
	}
}



