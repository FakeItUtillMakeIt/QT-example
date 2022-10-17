#include"Dragline.h"

#include<qdebug.h>
Dragline::Dragline():QLabel(),
color_num(0),color("white"),editflag(false)
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
Dragline::Dragline(QWidget* parent) :QLabel(parent)
,color_num(0), color("white"),editflag(false)
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
Dragline::~Dragline()
{
}


void Dragline::mousePressEvent(QMouseEvent* me)
{
	if (me->button() == Qt::LeftButton)
	{
		
		this->mainUI->ui.height_line->setDisabled(false);
		this->mainUI->ui.width_line->setDisabled(false);
		if (editflag)
		{

			this->mainUI->ui.attr_wid->setGeometry(1300, 0, 330, 130);
			this->mainUI->ui.attr_wid->show();
		}
		
		if (this->linetype == 0)
		{
			
			this->mainUI->compType = 3;
			this->mainUI->curComp = this;
			this->mainUI->curLine = this;


			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
			this->mainUI->ui.height_line->setDisabled(true);
		}
		if (this->linetype == 1)
		{

			if (editflag)
			{

			//	this->mainUI->ui.attr_wid->setGeometry(1300, 0, 330, 130);
				this->mainUI->ui.attr_wid->show();
			}
			this->mainUI->compType = 4;
			this->mainUI->curComp = this;
			this->mainUI->curLine = this;

			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
			this->mainUI->ui.width_line->setDisabled(true);
		}
		rage_x = me->pos().x();
		rage_y = me->pos().y();
		
	}
	if (me->button() == Qt::RightButton)
	{
		color_num++;
		if (this->linetype == 0)
		{
			this->setStyleSheet("border-top:2px dashed " + colors[color_num] + ";");

		
			if (color_num == 15)
			{
				color_num = 0;
			}
		}
		if (this->linetype == 1)
		{
		
			this->setStyleSheet("border-left:2px dashed " + colors[color_num] + ";");

			if (color_num == 15)
			{
				color_num = 0;
			}
		}
		this->color = colors[color_num];
		this->mainUI->curLine = this;

	}



}
void Dragline::mouseReleaseEvent(QMouseEvent* me)
{


}
void Dragline::mouseDoubleClickEvent(QMouseEvent* me)
{



}
void Dragline::mouseMoveEvent(QMouseEvent* me)
{
	if (me->buttons() & Qt::LeftButton) {
	
		if (this->canDrag)
		{
			if (this->linetype == 0)
			{
				this->move(me->windowPos().x() - 292 - rage_x, me->windowPos().y() - 194);
				this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
				this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
			}

			if (this->linetype == 1)
			{
				this->move(me->windowPos().x() - 292, me->windowPos().y() - 192 -rage_y);
				this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
				this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
			}

		}
	
	}
	
}

void Dragline::wheelEvent(QWheelEvent* qw)
{
	if (editflag) {
		if (qw->delta() > 0 && this->width() < 1600 && this->linetype == 0)
		{

			this->setGeometry(this->pos().x(), this->pos().y(), this->width() + 10, this->height());
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));



		}
		if (qw->delta() < 0 && this->width() > 20 && this->linetype == 0)
		{
			this->setGeometry(this->pos().x(), this->pos().y(), this->width() - 10, this->height());
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));

		}
		if (qw->delta() > 0 && this->height() < 1600 && this->linetype == 1)
		{

			this->setGeometry(this->pos().x(), this->pos().y(), this->width(), this->height() + 10);
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));



		}
		if (qw->delta() < 0 && this->height() > 20 && this->linetype == 1)
		{
			this->setGeometry(this->pos().x(), this->pos().y(), this->width(), this->height() - 10);
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));

		}
		this->mainUI->curLine = this;
	}
}
