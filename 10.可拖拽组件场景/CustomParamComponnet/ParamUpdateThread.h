#pragma once
#ifndef PRARM_UPDATE_THREAD_H
#define PRARM_UPDATE_THREAD_H


#include <qobject.h>
#include <QThread>

class ParamUpdateThread :
    public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    ParamUpdateThread(QWidget* widget);
    ~ParamUpdateThread();

public slots:
    
signals:


};

#endif