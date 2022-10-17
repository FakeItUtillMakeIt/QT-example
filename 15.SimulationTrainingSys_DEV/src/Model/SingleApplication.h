#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QtNetwork/QLocalServer>

class SingleApplication : public QApplication
{
    Q_OBJECT
public:
    SingleApplication(int& argc, char* argv[]);
    bool isRunning();                           // 是否已有实例在运行
    QWidget* w;                                   //MainWindow指针

    //bool isAppStart;

private slots:
    //有新连接时触发
    void NewLocalConnection();
   

private:
    //初始化本地连接
    void InitLocalConnection();
    //创建服务端
    void NewLocalServer();
    //激活窗口
    void ActivateWindow();


    QLocalServer* localserver;        //是否已有实例在运行
    QString       serverName;           //本地socket Server
    bool          isRunnings;               //服务名称
};

#endif // SINGLEAPPLICATION_H

