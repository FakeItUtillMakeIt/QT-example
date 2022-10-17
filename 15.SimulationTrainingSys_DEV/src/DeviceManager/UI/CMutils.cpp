#include"CMutils.h"
#include<QVBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<qdebug.h>


QString CMutils::thumnailpath_output(QString origin)
	{
	    QString filename = origin.split("/")[origin.split("/").size() - 1];
		QString destpath = QCoreApplication::applicationDirPath() + "/image/thumbnail/" + filename;
		return destpath;
	}
QString CMutils::destimg_output(QString origin)
{
	QString filename = origin.split("/")[origin.split("/").size() - 1];
	QString destpath = QCoreApplication::applicationDirPath() + "/image/prewimg/"+ filename;
	return destpath;
}
QString CMutils::destvideo_output(QString origin)
{
	QString vname = origin.split("/")[origin.split("/").size() - 1];
	QString destpath = QCoreApplication::applicationDirPath() + "/video/" + vname ;
	return destpath;
}
QString CMutils::destfile_output(QString origin)
{

	QString fname = origin.split("/")[origin.split("/").size() - 1];
	QString destpath = QCoreApplication::applicationDirPath() + "/files/" + fname ;
	return destpath;
}

QString CMutils::shortname(QString str,QString postfix)
{
	QString shortname;
	shortname = str;
	int weight = 0;
	int count = 0;
	for (int i = 0; i < str.length(); i++)
	{
		QString temp(str[i]);
		if (weight >= 32)
		{
			shortname= str.mid(0, count);
			shortname.append("...");

			break;
		}

		if (temp.contains(QRegExp("[\\x4e00-\\x9fa5]+")))
		{
			weight = weight + 2;
		}
		else {
			weight++;
		}
		qDebug()<<weight;
		count++;

	}
	
	return shortname;
}
QString CMutils::upload_shortname(QString str)
{
	QString shortname;
	shortname = str;
	int weight = 0;
	int count = 0;

	for (int i = 0; i < str.length(); i++)
	{
		QString temp(str[i]);
		if (weight >= 15)
		{
			shortname = str.mid(0, count);
			shortname.append(".");

			break;
		}

		if (temp.contains(QRegExp("[\\x4e00-\\x9fa5]+")))
		{
			weight = weight + 2;
		}
		else {
			weight++;
		}

		count++;

	}

	return shortname;
}
