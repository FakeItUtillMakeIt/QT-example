#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
class dragablewid : public QWidget
{
	Q_OBJECT

public:
	dragablewid(QWidget* parent = nullptr);
	~dragablewid();
protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);


private:


};
