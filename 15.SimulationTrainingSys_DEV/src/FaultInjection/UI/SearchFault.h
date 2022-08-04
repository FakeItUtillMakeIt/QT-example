#pragma once

#include <QWidget>
#include<QEvent>
#include<QPainter> 
#include <QMouseEvent>
#include <qmath.h>
#include "ui_SearchFault.h"
#include <QScrollArea>
#include "../Controls/MyFaultTree.h"

class SearchFault : public QWidget
{
	Q_OBJECT

public:
	SearchFault(QWidget *parent = nullptr);
	~SearchFault();


	void SearchFault::ShowSearchFaults(vector<ItemStruct> Items);

protected:

	QPoint mLastMousePosition;
	bool mMoving;
	void paintEvent(QPaintEvent* event);
	void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;


private:
	Ui::SearchFaultClass ui;
	QScrollArea* m_scrollSearchFault;

	MyFaultTree* m_searchTree;
	QHBoxLayout* m_searchLayout;

	void Init();

};
