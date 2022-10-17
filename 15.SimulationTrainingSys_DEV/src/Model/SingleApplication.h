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
    bool isRunning();                           // �Ƿ�����ʵ��������
    QWidget* w;                                   //MainWindowָ��

    //bool isAppStart;

private slots:
    //��������ʱ����
    void NewLocalConnection();
   

private:
    //��ʼ����������
    void InitLocalConnection();
    //���������
    void NewLocalServer();
    //�����
    void ActivateWindow();


    QLocalServer* localserver;        //�Ƿ�����ʵ��������
    QString       serverName;           //����socket Server
    bool          isRunnings;               //��������
};

#endif // SINGLEAPPLICATION_H

