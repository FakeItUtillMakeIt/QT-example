#pragma once
#include <QObject>
#include "ui_RocketStructShow.h"
 
#include "../../VLCVideo/vlcplayer.h"
class RocketStructShow : public QWidget
{
    Q_OBJECT

public:
    RocketStructShow(QWidget *parent = Q_NULLPTR);

private:
    Ui::RocketStructShow ui;
    void Init(); 
    void InitNav();
    bool pb_level1enginer;
    bool pb_level1enginer_2;
    bool pb_weiduan;
    bool pb_weiduan_2;
    VlcPlayer* m_pVlcPlayer; 
};
