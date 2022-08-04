#pragma once
#include <QObject>
#include "ui_CenterOperate.h"

class CenterOperate : public QWidget
{
    Q_OBJECT

public:
    CenterOperate(QWidget *parent = Q_NULLPTR);

private:
    Ui::CenterOperate ui;
    void Init();
};
