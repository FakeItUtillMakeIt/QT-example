#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TcpCommTest.h"
#include "TcpServer.h"
#include "TcpClient.h"
#include <QThread>

class TcpCommTest : public QMainWindow
{
    Q_OBJECT

public:
    TcpCommTest(QWidget *parent = Q_NULLPTR);

private:
    QThread* m_thread;
    TcpServer* server_window;
    TcpClient* client_window;

    Ui::TcpCommTestClass ui;

private slots:
    void btnOk_slot();
};
