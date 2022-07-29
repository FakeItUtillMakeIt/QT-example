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
    void ShowOne();//��ʾȫ��
    void ShowTwo();//��ʾ���漶
    void ShowThree();//��ʾ���Ʋ�
    void ShowFour();//��ʾβ��

    //ȫ��
    bool pb_level1enginer;//һ��������
    bool pb_level1enginer_2;
    bool pb_weiduan;//wβ��
    bool pb_weiduan_2;
    bool pb_up;//�����˿ط�����
    bool pb_up_2;
     
    //���Ʋ�
    bool pb_control;//�ۿػ�
    bool pb_control_2;
    bool pb_receiver;//��ȫָ����ջ�
    bool pb_receiver_2;
     
    VlcPlayer* m_pVlcPlayer; 
};
