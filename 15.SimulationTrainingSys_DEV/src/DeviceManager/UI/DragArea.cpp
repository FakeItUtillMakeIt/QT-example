#include "DragArea.h"
#include<qdebug.h>
DragArea::DragArea() :QLabel()
, color_num(0)
{
	colors.append("white");
	colors.append("black");
	colors.append("red");
	colors.append("blue");
	colors.append("green");
	colors.append("pruple");
	colors.append("yellow");
	colors.append("aqua");
	colors.append("fuchsia");
	colors.append("gray");
	colors.append("lime");
	colors.append("maroon");
	colors.append("navy");
	colors.append("olive");
	colors.append("silver");
	colors.append("teal");


}
DragArea::DragArea(QWidget* parent) :QLabel(parent)
, color_num(0)

{
	colors.append("white");
	colors.append("black");
	colors.append("red");
	colors.append("blue");
	colors.append("green");
	colors.append("pruple");
	colors.append("yellow");
	colors.append("aqua");
	colors.append("fuchsia");
	colors.append("gray");
	colors.append("lime");
	colors.append("maroon");
	colors.append("navy");
	colors.append("olive");
	colors.append("silver");
	colors.append("teal");


}
DragArea::~DragArea()
{
}


void DragArea::mousePressEvent(QMouseEvent* me)
{

	if (me->button() == Qt::LeftButton)
	{
		this->mainUI->ui.height_line->setDisabled(false);
		this->mainUI->ui.width_line->setDisabled(false);
		this->mainUI->curComp = this;
		this->mainUI->compType = 5;
		this->mainUI->curArea = this;
		this->mainUI->ui.attr_wid->show();
		this->mainUI->ui.width_line->setText(QString::number(this->width()));
		this->mainUI->ui.height_line->setText(QString::number(this->height()));
		this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
		this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
		rage_x = me->pos().x();
		rage_y = me->pos().y();

	}
	if (me->button() == Qt::RightButton)
	{
		

		
		color_num++;
		if (color_num == 16)
		{
			color_num = 0;
		}
		this->setStyleSheet("border:2px dashed " + colors[color_num] + ";");
		this->mainUI->curArea = this;

	}

}
void DragArea::mouseReleaseEvent(QMouseEvent* me)
{

}
void DragArea::mouseDoubleClickEvent(QMouseEvent* me)
{
	if (me->button() == Qt::LeftButton)
	{
		this->mainUI->zhezhao_global->show();
		this->mainUI->ui.area_bindwid->show();
	}
	this->mainUI->curArea = this;
}
void DragArea::mouseMoveEvent(QMouseEvent* me)
{
	if (me->buttons() & Qt::LeftButton)
	{
		if (this->canDrag)
		{
			this->move(me->windowPos().x() - 292 - rage_x, me->windowPos().y() - 194 - rage_y);
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
		}

	}

}

void DragArea::wheelEvent(QWheelEvent* qw)
{
	if (qw->delta() > 0)
	{

		this->setGeometry(this->pos().x(), this->pos().y(), this->width() + 10, this->height()+10);
		this->mainUI->ui.width_line->setText(QString::number(this->width() ));
		this->mainUI->ui.height_line->setText(QString::number(this->height()));
		this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
		this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));



	}
	if (qw->delta() < 0 && (this->width() > 20||this->height()>20))
	{
		this->setGeometry(this->pos().x(), this->pos().y(), this->width() - 10, this->height()-10);
		this->mainUI->ui.width_line->setText(QString::number(this->width() ));
		this->mainUI->ui.height_line->setText(QString::number(this->height()));
		this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
		this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
		if (this->width() < 20)
		{
			this->setGeometry(this->pos().x(), this->pos().y(), 20, this->height());
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));

		}
		if (this->height() < 20)
		{
			this->setGeometry(this->pos().x(), this->pos().y(), this->width(), 20);
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));

		}

	}
	this->mainUI->curArea = this;
}