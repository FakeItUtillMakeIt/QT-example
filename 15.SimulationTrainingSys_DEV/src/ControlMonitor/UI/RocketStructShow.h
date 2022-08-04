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
    void ShowOne();//显示全箭
    void ShowTwo();//显示上面级
    void ShowThree();//显示控制舱
    void ShowFour();//显示尾段

    //全箭
    bool pb_level1enginer;//一级发动机
    bool pb_level1enginer_2;
    bool pb_weiduan;//w尾段
    bool pb_weiduan_2;
    bool pb_up;//上面姿控发动机
    bool pb_up_2;
     
    //控制舱
    bool pb_control;//综控机
    bool pb_control_2;
    bool pb_receiver;//安全指令接收机
    bool pb_receiver_2;
     
    VlcPlayer* m_pVlcPlayer; 
};
