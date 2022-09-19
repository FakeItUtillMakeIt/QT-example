#include "Dragcomponent.h"
#include<qdebug.h>
Dragcomponent::Dragcomponent()
{
}
Dragcomponent::Dragcomponent(QWidget* parent) :QWidget(parent)
{
}
Dragcomponent::~Dragcomponent()
{
}


void Dragcomponent::mousePressEvent(QMouseEvent* me)
{
	if (me->button() == Qt::LeftButton)
	{
		this->mainUI->ui.height_line->setDisabled(false);
		this->mainUI->ui.width_line->setDisabled(false);
		this->mainUI->compType = 2;
		this->mainUI->curDcomp = this;
		this->mainUI->curComp = this;
		this->mainUI->ui.attr_wid->show();
		this->mainUI->ui.width_line->setText(QString::number(this->width()));
		this->mainUI->ui.height_line->setText(QString::number(this->height()));
		this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
		this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
		rage_x = me->pos().x();
		rage_y = me->pos().y();

	}


}
void Dragcomponent::mouseReleaseEvent(QMouseEvent* me)
{

}
void Dragcomponent::mouseDoubleClickEvent(QMouseEvent* me)
{

	if (me->button() == Qt::LeftButton)
	{
		
		this->mainUI->ui.dialog->setGeometry(this->pos().x(), this->pos().y(), 500, 140);
		this->mainUI->ui.dialog->show();
		this->mainUI->ui.dialog->raise();
	}

}
void Dragcomponent::mouseMoveEvent(QMouseEvent* me)
{
	if (me->buttons() & Qt::LeftButton)
	{
		if (this->canDrag)
		{
			this->move(me->windowPos().x() - 292-rage_x, me->windowPos().y() - 194-rage_y);
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
		}
	}

}

void Dragcomponent::wheelEvent(QWheelEvent* qw)
{
	if (qw->delta() > 0&& this->width()<500)
	{
		
		this->setGeometry(this->pos().x(), this->pos().y(), this->width() + 10, this->height());
		this->mainUI->ui.width_line->setText(QString::number(this->width()));
		this->mainUI->ui.height_line->setText(QString::number(this->height()));
		this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
		this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));



	}
	if (qw->delta() < 0 && this->width()>20)
 {
		this->setGeometry(this->pos().x(), this->pos().y(), this->width() - 10, this->height());
		this->mainUI->ui.width_line->setText(QString::number(this->width()));
		this->mainUI->ui.height_line->setText(QString::number(this->height()));
		this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
		this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));

	}
	this->mainUI->curDcomp = this;

}
