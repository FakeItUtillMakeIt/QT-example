#include "Draglabel.h"
#include<QWidget>
#include<qdebug.h>
Draglabel::Draglabel(QWidget* parent) :QLabel(parent)
{
}
Draglabel::~Draglabel()
{
}
void Draglabel::dragEnterEvent(QDragEnterEvent* Qd)
{


}
void Draglabel::dropEvent(QDropEvent* Qe)
{


}

void Draglabel::mousePressEvent(QMouseEvent* me)
{
	if (me->button() == Qt::LeftButton)
	{
		this->dropwid->show();
		this->dropwid->setGeometry(0, 0, 1630, 854);

		QDrag* drag = new QDrag(this);
		QMimeData* data = new QMimeData();
		drag->setMimeData(data);

		if (this->label_type == 1)
		{

			data->setData("labeltype", "1");
			drag->setPixmap(QPixmap(":/twoDdisplay/defaultimg"));
			drag->setHotSpot(QPoint(130, 75));
		}
		if (this->label_type == 2)
		{
			data->setData("labeltype", "2");
			drag->setPixmap(QPixmap(":/twoDdisplay/dcomp"));
			drag->setHotSpot(QPoint(60, 20));
		}
		if (this->label_type == 3)
		{
			data->setData("labeltype", "3");
			drag->setPixmap(QPixmap(":/twoDdisplay/dlineh"));
			drag->setHotSpot(QPoint(100, 0));
		}
		if (this->label_type == 4)
		{
			data->setData("labeltype", "4");
			drag->setPixmap(QPixmap(":/twoDdisplay/dlinev"));
			drag->setHotSpot(QPoint(0, 100));
		}

		if (this->label_type == 5)
		{
			data->setData("labeltype", "5");
			drag->setPixmap(QPixmap(":/twoDdisplay/gotoarea"));
			drag->setHotSpot(QPoint(50, 50));
		}


		drag->exec();
	}
	

}

void Draglabel::mouseMoveEvent(QMouseEvent* me)
{




}
