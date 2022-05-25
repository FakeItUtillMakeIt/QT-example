#pragma once
#ifndef SERIAL_PORT_THREAD_H
#define SERIAL_PORT_THREAD_H


#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>
#include <QTime>

//typedef QMap<QString, QString> stringMap;
//qRegisterMetaType<stringMap>("stringMap");

class SerialPortThread :
    public QThread
{
    Q_OBJECT
        
public:
    explicit SerialPortThread(QObject* parent = nullptr);
    ~SerialPortThread();

    void startSerialPort(const QString& portName, int waitTimeout,const QString& response);

signals:
	void timeout(const QString& s);
	void error(const QString& s);
	void request(const QString& s);
    void request(const QByteArray& s);
    /*void timeout(const stringMap&);
    void error(const stringMap&);
    void request(const stringMap&);*/

private:
    void run() override;
    QString m_portname;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    bool m_quit = false;
    QString m_response;

private slots:
    


};

#endif
