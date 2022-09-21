#pragma once
#include<QString>
#include<QFileDialog>
#include<QCoreApplication>
#include<QDateTime>

#include"twoDdisplay.h"

namespace CMutils {
	QString destfilename_output(QString origin);
	QString destimg_output(QString origin, int bias);
	QString destvideo_output(QString origin, int bias);
	QString destfile_output(QString origin, int bias);
	QString shortname(QString str, QString postfix);
	QString upload_shortname(QString str);

}