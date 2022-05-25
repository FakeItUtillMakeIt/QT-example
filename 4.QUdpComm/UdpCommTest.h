#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UdpCommTest.h"
#include "UdpServer.h"
#include "UdpClient.h"

class UdpCommTest : public QMainWindow
{
    Q_OBJECT

public:
    UdpCommTest(QWidget *parent = Q_NULLPTR);

private:
    Ui::UdpCommTestClass ui;

private slots:
    void btn_OK_slot();
};
