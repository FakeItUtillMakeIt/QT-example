#include"CMutils.h"
#include<QVBoxLayout>
#include<QLabel>
#include<QPushButton>



QString CMutils::destfilename_output(QString origin)
	{
		QString Timestr = QString::number(QDateTime::currentDateTime().toTime_t());
		QStringList filenamelist = origin.split(".");
		QString filename = filenamelist[filenamelist.size() - 1];
		QString destpath = QCoreApplication::applicationDirPath() + "/image/thumbnail/thumbnail" + Timestr + "." + filename;
		return destpath;
	}
QString CMutils::destimg_output(QString origin, int bias)
{
	QString Timestr = QString::number(QDateTime::currentDateTime().toTime_t() + bias);
	QStringList filenamelist = origin.split(".");
	QString filename = filenamelist[filenamelist.size() - 1];
	QString destpath = QCoreApplication::applicationDirPath() + "/image/prewimg/image" + Timestr + "." + filename;
	return destpath;
}
QString CMutils::destvideo_output(QString origin, int bias)
{
	QString Timestr = QString::number(QDateTime::currentDateTime().toTime_t() + bias);
	QStringList filenamelist = origin.split(".");
	QString filename = filenamelist[filenamelist.size() - 1];
	QString destpath = QCoreApplication::applicationDirPath() + "/video/video" + Timestr + "." + filename;
	return destpath;
}
QString CMutils::destfile_output(QString origin, int bias)
{
	QString Timestr = QString::number(QDateTime::currentDateTime().toTime_t() + bias);
	QStringList filenamelist = origin.split(".");
	QString filename = filenamelist[filenamelist.size() - 1];
	QString destpath = QCoreApplication::applicationDirPath() + "/files/file" + Timestr + "." + filename;
	return destpath;
}
