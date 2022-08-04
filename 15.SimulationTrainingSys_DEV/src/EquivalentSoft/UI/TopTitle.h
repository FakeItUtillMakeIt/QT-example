#pragma once
#include <QObject>
#include "ui_TopTitle.h"
#include "../AppCache.h"
class TopTitle : public QWidget
{
    Q_OBJECT

public:
    TopTitle(QWidget *parent = Q_NULLPTR);

private:
    Ui::TopTitle ui; 
    AppCache* m_app;
    void Init(); 
signals:
    void CloseWindow();
    void ShowMinimized();
};
