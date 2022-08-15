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
