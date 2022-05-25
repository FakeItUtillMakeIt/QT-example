#pragma once

#ifndef SIMULATION_SEND_SERIAL_PORT_H
#define SIMULATION_SEND_SERIAL_PORT_H


#include <QtWidgets/QMainWindow>
#include "ui_SimulationSendSerialPort.h"
#include <QtWidgets>
#include <QSerialPortInfo>
#include "MasterThread.h"

class SimulationSendSerialPort : public QMainWindow
{
    Q_OBJECT

public:
    SimulationSendSerialPort(QWidget *parent = Q_NULLPTR);
private slots:
	void transaction();
	void showResponse(const QString& s);
	void processError(const QString& s);
	void processTimeout(const QString& s);

private:
	void setControlsEnabled(bool enable);

private:
    Ui::SimulationSendSerialPortClass ui;

	int m_transactionCount = 0;
	QLabel* m_serialPortLabel = nullptr;
	QComboBox* m_serialPortComboBox = nullptr;
	QLabel* m_waitResponseLabel = nullptr;
	QSpinBox* m_waitResponseSpinBox = nullptr;
	QLabel* m_requestLabel = nullptr;
	QLineEdit* m_requestLineEdit = nullptr;
	QLabel* m_trafficLabel = nullptr;
	QLabel* m_statusLabel = nullptr;
	QPushButton* m_runButton = nullptr;

	MasterThread m_thread;
};


#endif