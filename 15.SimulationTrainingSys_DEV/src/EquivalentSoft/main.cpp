#include "UI/EquivalentSoft.h"
#include "UI/EquivalenSoftLogin.h"
#include <QtWidgets/QApplication>
#include "AppCache.h"
#include "../Public/log.h"
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	// ֻ������һ������
	QProcess process;
	process.start("tasklist");
	if (process.waitForFinished()) {
		//�õ������б�
		QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
		if (result.count("EquivalentSoft", Qt::CaseInsensitive) >= 2) {
			QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("��Ч������������!"));
			app.exit();
			return 0;
		}
	}
	else {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("���������������ʧ�ܣ��޷�ʶ���Ƿ��������ʵ��!"));
		app.exit();
		return 0;
	}


	// ��ʼ���������棬ָ��EXE����λ�ã�ͬʱ������־�ļ���
	if (!AppCache::instance()->SetExePath(app.applicationDirPath())) {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("������־�ļ���ʧ��!"));
		app.exit();
	}
	AppCache::instance()->argc = &argc;
	AppCache::instance()->argv = &argv;
	//��ʼ����־
	LogInit(argv[0], app.applicationDirPath().toStdString());
	LOG(INFO) << "����ʼ����... " << endl;

	//��ȡ�����ļ�
	if (!AppCache::instance()->ReadConfig()) {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("����"), QObject::tr("��ȡ�����ļ�ʱ��������ϸ��Ϣ��鿴��־��"));
		app.exit();
	}

	EquivalentSoft w;
	w.hide();
	EquivalenSoftLogin login(&w);
	login.show();
    return app.exec();
}