#pragma execution_character_set("utf-8")

#include "multiinputget.h"
#include "ui_multiinputget.h"
#include <QPushButton>
MultiInputGet::MultiInputGet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiInputGet)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}

MultiInputGet::~MultiInputGet()
{
    delete ui;
}

void MultiInputGet::on_buttonBox_accepted()
{
	accepted();
}

void MultiInputGet::on_buttonBox_rejected()
{
	rejected();
}

bool MultiInputGet::get_x_range(double& xmin,double& xmax)
{
	if (ui->xchecked->isChecked())
	{
		xmin = ui->xmin->text().toDouble();
		xmax = ui->xmax->text().toDouble();
		return true;
	}
	return false;
}
bool MultiInputGet::get_y_range(double& ymin, double& ymax)
{
	if (ui->ychecked->isChecked())
	{
		ymin = ui->ymin->text().toDouble();
		ymax = ui->ymax->text().toDouble();
		return true;
	}
	return false;
}
void MultiInputGet::update_range(double xmin, double xmax, double ymin, double ymax)
{
	ui->xmin->setText(QString::number(xmin,'f',3));
	ui->xmax->setText(QString::number(xmax, 'f', 3));
	ui->ymin->setText(QString::number(ymin, 'f', 3));
	ui->ymax->setText(QString::number(ymax, 'f', 3));
}
