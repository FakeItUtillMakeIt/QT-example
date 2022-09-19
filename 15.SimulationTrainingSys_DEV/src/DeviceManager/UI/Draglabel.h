#pragma once
#include <qlabel.h>
#include<qdrag.h>
#include<qmimedata.h>
#include<QDragEnterEvent>

class Draglabel : public QLabel
{
	Q_OBJECT
public:
	int label_type;
	QWidget* dropwid;
	Draglabel(QWidget* parent);
	~Draglabel();
	void dropEvent(QDropEvent* Qe);
	void dragEnterEvent(QDragEnterEvent* Qd);
	void mousePressEvent(QMouseEvent* me);
	void mouseMoveEvent(QMouseEvent* me);

};

