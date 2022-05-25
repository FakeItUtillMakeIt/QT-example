#pragma once
#ifndef SLAVE_SERIAL_H
#define SLAVE_SERIAL_H


#include <QtWidgets/QMainWindow>
#include "ui_SlaveSerial.h"
#include <QtWidgets>
#include <QString>
#include <QGridLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QLabel>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QPushButton>

#include "RocketGroundBaseProtol.h"

class SlaveSerial : public QMainWindow
{
    Q_OBJECT

public:
    SlaveSerial(QWidget *parent = Q_NULLPTR);

private:
    Ui::SlaveSerialClass ui;

	QLabel* m_portLabel = nullptr;
	QComboBox* m_portComboBox = nullptr;
	QLabel* m_sendLabel = nullptr;
	QPlainTextEdit* m_sendPlainText = nullptr;
	QLabel* m_recvLabel = nullptr;
	QPlainTextEdit* m_recvPlainText = nullptr;
	QPushButton* m_btnSend = nullptr;
	QPushButton* m_openPort = nullptr;

private:
	QSerialPort serial;
	QByteArray m_sendData;
};


#endif