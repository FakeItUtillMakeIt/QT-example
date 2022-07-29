#include "UI/FaultInjection.h"
#include <QtWidgets/QApplication>
#include <QProcess>
#include "AppCache.h"
#include "../Public/log.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// 只允许打开一个软件
	QProcess process;
	process.start("tasklist");
	if (process.waitForFinished()) {
		//得到进程列表
		QString result = QString::fromLocal8Bit(process.readAllStandardOutput());
		if (result.count("MyFramework", Qt::CaseInsensitive) >= 2) {
			QMessageBox::critical(qApp->activeWindow(), QObject::tr("警告"), QObject::tr("主软件已启动!"));
			app.exit();
			return 0;
		}
	}
	else {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("警告"), QObject::tr("软件打开任务管理器失败，无法识别是否存在运行实例!"));
		app.exit();
		return 0;
	}

	// 初始化软件缓存，指定EXE所在位置，同时创建日志文件夹
	if (!AppCache::instance()->SetExePath(app.applicationDirPath())) {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("警告"), QObject::tr("创建日志文件夹失败!"));
		app.exit();
	}
	AppCache::instance()->argc = &argc;
	AppCache::instance()->argv = &argv;
	//初始化日志
	LogInit(argv[0], app.applicationDirPath().toStdString());
	LOG(INFO) << "程序开始启动... " << endl;

	//读取配置文件
	if (!AppCache::instance()->ReadConfig()) {
		QMessageBox::critical(qApp->activeWindow(), QObject::tr("错误"), QObject::tr("读取配置文件时出错，详细信息请查看日志。"));
		app.exit();
	}
    FaultInjection w;
    w.show();
    return app.exec();
}
