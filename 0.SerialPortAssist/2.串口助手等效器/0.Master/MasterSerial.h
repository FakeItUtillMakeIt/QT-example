#pragma once

#ifndef MASTER_SERIAL_H
#define MASTER_SERIAL_H



#include <QtWidgets/QMainWindow>
#include "ui_MasterSerial.h"

#include <QString>
#include <QGridLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QMessageBox>

#include "BaseLuanchCommand.h"

class MasterSerial : public QMainWindow
{
    Q_OBJECT

public:
    MasterSerial(QWidget *parent = Q_NULLPTR);

private:
    Ui::MasterSerialClass ui;

    QLabel* m_portLabel = nullptr;
    QComboBox* m_portComboBox = nullptr;
    QLabel* m_sendLabel = nullptr;
    QPlainTextEdit* m_sendPlainText = nullptr;
    QLabel* m_recvLabel = nullptr;
    QPlainTextEdit* m_recvPlainText = nullptr;
    QPushButton* m_btnSend = nullptr;
    QComboBox* m_commandComboBox = nullptr;
    QPushButton* m_openPort = nullptr;

private:
    QSerialPort serial;
    QByteArray m_sendData;

    BaseRequestText allCmd;
};


#endif