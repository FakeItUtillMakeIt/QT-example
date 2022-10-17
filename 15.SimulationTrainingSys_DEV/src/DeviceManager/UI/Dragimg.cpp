#include "Dragimg.h"
#include<qdebug.h>
Dragimg::Dragimg():editflag(false)
{



}
Dragimg::Dragimg(QWidget* parent):QLabel(parent),editflag(false)
{



 }
Dragimg::~Dragimg()
{
}


void Dragimg::mousePressEvent(QMouseEvent* me)
{

	if (me->button() == Qt::LeftButton)
	{
		if (editflag)
		{
			this->mainUI->ui.attr_wid->setGeometry(1300, 0, 330, 130);
			this->mainUI->ui.attr_wid->show();
		}
		this->mainUI->ui.height_line->setDisabled(false);
		this->mainUI->ui.width_line->setDisabled(false);
		this->mainUI->curComp = this;
		this->mainUI->curCompimg = this;
		this->mainUI->compType = 1;

		this->mainUI->ui.width_line->setText(QString::number(this->width()));
		this->mainUI->ui.height_line->setText(QString::number(this->height()));
		this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
		this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
		rage_x = me->pos().x() ;
		rage_y = me->pos().y();
	
		
	}

}
void Dragimg::mouseReleaseEvent(QMouseEvent* me)
{

}
void Dragimg::mouseDoubleClickEvent(QMouseEvent* me)
{
	if (me->button() == Qt::LeftButton)
	{
		if (editflag) {
			QString file = QFileDialog::getOpenFileName(
				this,
				"Select one or more files to open",
				"",
				"ImageFile(*.png *.jpg *.jpeg)");
			if (!file.isEmpty())
			{
				QPixmap np = QPixmap(file);
				this->clear();
				this->setPixmap(np);
				this->mainUI->baseimg = np;
				this->setGeometry(this->pos().x(), this->pos().y(), np.width(), np.height());
				this->ownimg = np;
				imgurl = file.split("/")[file.split("/").size() - 1];
			}
		}
		this->mainUI->curCompimg = this;
		}
		


}
void Dragimg::mouseMoveEvent(QMouseEvent* me)
{
	if (me->buttons()&Qt::LeftButton)
	{
		if (this->canDrag)
		{
			this->move(me->windowPos().x() - 292 - rage_x, me->windowPos().y() - 194 - rage_y);
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
		}

	}
	
}

void Dragimg::wheelEvent(QWheelEvent* qw)
{
	if (editflag) {
		if (qw->delta() > 0)
		{
			QPixmap img = this->ownimg;
			QPixmap temp = img.scaled(this->width() + 10, this->height() + 10, Qt::KeepAspectRatio);
			this->clear();
			this->setPixmap(temp);
			this->setGeometry(this->pos().x(), this->pos().y(), temp.width(), temp.height());
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));



		}
		if (qw->delta() < 0 && this->width() > 20)
		{
			QPixmap img = this->ownimg;
			QPixmap temp = img.scaled(this->width() - 10, this->height() - 10, Qt::KeepAspectRatio);
			this->clear();
			this->setPixmap(temp);
			this->setGeometry(this->pos().x(), this->pos().y(), temp.width(), temp.height());
			this->mainUI->ui.width_line->setText(QString::number(this->width()));
			this->mainUI->ui.height_line->setText(QString::number(this->height()));
			this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
			this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
			if (this->width() < 20)
			{
				QPixmap temp = img.scaledToWidth(20);
				this->clear();
				this->setPixmap(temp);
				this->setGeometry(this->pos().x(), this->pos().y(), temp.width(), temp.height());
				this->mainUI->ui.width_line->setText(QString::number(this->width()));
				this->mainUI->ui.height_line->setText(QString::number(this->height()));
				this->mainUI->ui.pos_x->setText(QString::number(this->pos().x()));
				this->mainUI->ui.pos_y->setText(QString::number(this->pos().y()));
			}

		}
		this->mainUI->curCompimg = this;
	}
}
