#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QPainter>
#include <QMouseEvent>
#include <QToolTip>
#include <QApplication>

/**

	@class   ItemDelegate
	@brief   仅供优化后的参数和指令新增编辑comboBox
	@details ~

**/
class ItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	ItemDelegate(QObject* parent = nullptr);
	virtual ~ItemDelegate() {}
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

signals:
	void deleteItem(const QModelIndex& index);
};

#endif // ITEMDELEGATE_H
