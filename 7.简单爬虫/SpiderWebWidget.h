#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SpiderWebWidget.h"
#include "SpiderWebScript.h"
#include <QtWidgets>


class SpiderWebWidget : public QMainWindow
{
    Q_OBJECT

public:
    SpiderWebWidget(QWidget *parent = Q_NULLPTR);

private:
    Ui::SpiderWebWidgetClass ui;

    SpiderWebScript* spider;

private:
    QLabel* webLabel;
    QLineEdit* webUrlInput;
    QPushButton* webClickBtn;
    QPlainTextEdit* webRetInfo;
    QGridLayout* windowLayout;

private:
    void click_slot();
};
