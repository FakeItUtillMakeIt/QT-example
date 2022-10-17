#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H


#define  COMBOXCOL 1

#include <QObject>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QPainter>
#include <QMouseEvent>
#include <QToolTip>
#include <QApplication>

class Delegate;
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


/**

	@class   Delegate
	@brief
	@details ~

**/
class Delegate : public QItemDelegate
{
	Q_OBJECT

public:
	Delegate(QObject* parent = nullptr);
	~Delegate();

	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(QWidget* editor, QAbstractItemModel* model,
		const QModelIndex& index) const;

};

#endif 



