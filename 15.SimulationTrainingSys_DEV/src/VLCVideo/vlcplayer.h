#ifndef VLCPLAYER_H
#define VLCPLAYER_H
#include <vlc/vlc.h>
#include <vlc/libvlc.h>
#include <map>
#include <string>
#include <QApplication> 
#include <QPainter>
#include <QMouseEvent>
#include <QWidget> 
//#include "../Network/PeerInfo.h"
using namespace std;

extern "C"
{
    #include <Windows.h>
    #include <WinUser.h>
}
 
class VlcPlayer : public QWidget
{
    Q_OBJECT
public:
    VlcPlayer(int offX, int offY, string url, int , QWidget* parent = Q_NULLPTR);
    ~VlcPlayer();
public:
      /*!
           初始化
           @return 成功返回true，否则false
       */
       bool InitPlayer(); 
       void StopVideo(); 
       void pause();
       QPoint m_point;  
       int PlayVideo(string path);
private:
       libvlc_instance_t *m_instace = nullptr;
       libvlc_media_player_t *m_pPlayer = nullptr;
       libvlc_media_t* pMedia = nullptr;
       string net_url; 
       int m_bufferTime; //缓冲时间
       HWND m_handle; 

       QPoint mLastMousePosition;
       bool mMoving;
       bool isMax; 
       // Event handlers 
       void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
       void mouseReleaseEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
       void mouseMoveEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
       void mouseDoubleClickEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
};

#endif //VLCPLAYER_H
