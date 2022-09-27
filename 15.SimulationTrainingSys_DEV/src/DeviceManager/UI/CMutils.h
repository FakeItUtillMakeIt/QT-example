#pragma once
#include<QString>
#include<QFileDialog>
#include<QCoreApplication>
#include<QDateTime>

#include"twoDdisplay.h"

namespace CMutils {
	QString destfilename_output(QString origin);
	QString destimg_output(QString origin);
	QString destvideo_output(QString origin);
	QString destfile_output(QString origin);
	QString shortname(QString str, QString postfix);
	QString upload_shortname(QString str);

}