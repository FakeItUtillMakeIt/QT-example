#include "SingleApplication.h"
#include <QtNetwork/QLocalSocket>
#include <QFileInfo>

#define TIME_OUT   500

SingleApplication::SingleApplication(int& argc, char* argv[])
    :QApplication(argc, argv),
    isRunnings(false),
    w(NULL),
    localserver(NULL)
{
    //ȡӦ�ó�������ΪlocalServer������
    serverName = QFileInfo(QApplication::applicationFilePath()).fileName();
    InitLocalConnection();
}


/*****************************************************************
* ����Ƿ�����һ��ʵ��������, true��ʵ������, falseû��ʵ������.
******************************************************************/
bool SingleApplication::isRunning()
{
    return isRunnings;
}


/*****************************************************************
* ͨ��socketͨѶʵ�ֳ���ʵ������,�������µ�����ʱ�����ú���.
******************************************************************/
void SingleApplication::NewLocalConnection()
{
    QLocalSocket* localSocket = localserver->nextPendingConnection();
    if (localSocket)
    {
        localSocket->waitForReadyRead(TIME_OUT * 2);
        delete localSocket;

        //��������,��:��ȡ��������
        ActivateWindow();
    }
}


/*****************************************************************
* ͨ��socketͨѶʵ�ֳ���ʵ�����У�
* ��ʼ���������ӣ�������Ӳ���server���򴴽��������˳�
******************************************************************/
void SingleApplication::InitLocalConnection()
{
    isRunnings = false;
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(TIME_OUT))
    {
        isRunnings = true;

        //��������,��:�������������͵������
        return;
    }

    //���Ӳ��Ϸ�����,�ʹ���һ��
    NewLocalServer();
}

/*****************************************************************
* ����localserver
******************************************************************/
void SingleApplication::NewLocalServer()
{
    localserver = new QLocalServer(this);
    connect(localserver, &QLocalServer::newConnection, this, &SingleApplication::NewLocalConnection);
    if (!localserver->listen(serverName))
    {
        // ��ʱ����ʧ��,�����ǳ������ʱ,�������̷����µ�,�Ƴ�
        if (localserver->serverError() == QAbstractSocket::AddressInUseError)
        {
            localserver->removeServer(serverName);
            localserver->listen(serverName); //���¼���
        }
    }
}

/*****************************************************************
* ����������
******************************************************************/
void SingleApplication::ActivateWindow()
{
    //��½���ڴ��ھ��˳�

    if (w)
    {
        w->raise();
        w->activateWindow();
        w->showNormal();
        

        //���������ʾԭ���ڵĴ�С������С��ǰ�Ĵ�С����windowsAPI���Ѵ��ڣ�
        //HWND hwnd = reinterpret_cast<HWND>(m->winId());
        //ShowWindow(hwnd, SW_RESTORE);
    }
}
