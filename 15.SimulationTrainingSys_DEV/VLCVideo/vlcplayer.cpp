#include "vlcplayer.h"
#include "WinUser.h"
#include <QMessageBox>
#include <QtWidgets/QApplication>
#define WIFI_W 640
#define WIFI_H 480


VlcPlayer::VlcPlayer(int offX, int offY, string url, int bufferTime, QWidget* parent)
    : QWidget(parent)
    , net_url(url) 
    , m_bufferTime(bufferTime)
    , isMax(false)
    , m_instace(nullptr)
    , m_pPlayer(nullptr)
    , pMedia(nullptr)
{   
    m_point = QPoint(offX, offY);
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//去掉标题栏
    m_handle = (HWND)this->winId();
    //this->setAttribute(Qt::WA_NoSystemBackground, true);
    //this->setAttribute(Qt::WA_PaintOnScreen, true);
    this->setStyleSheet(QStringLiteral("background-color:#000000;"));
    //this->setGeometry(QRect(offX, offY, 944, 446));
    this->setToolTip(tr("双击窗体最大化"));
    this->setCursor(Qt::PointingHandCursor);
    
    //InitPlayer(); 
    this->show();
}
VlcPlayer::~VlcPlayer()
{
    if (nullptr != m_pPlayer) 
    {//存在内存泄漏
        StopVideo();
        //delete m_pPlayer;
        m_pPlayer = nullptr;
    }
    if (nullptr != pMedia)
    {//存在内存泄漏
        libvlc_media_release(pMedia);
        //delete pMedia;
        pMedia = nullptr;
    }
    if (nullptr != m_instace)
    {//存在内存泄漏
        libvlc_release(m_instace);
        //delete m_instace;
        m_instace = nullptr;
    }
}

int VlcPlayer::PlayVideo(string path)
{
    try
    {
        m_instace = libvlc_new(0, NULL);
        if (nullptr == m_instace)
        {
            return false;
        }
        pMedia = libvlc_media_new_path(m_instace, path.c_str());
        if (nullptr == pMedia)
        {
            return false;
        } 
        m_pPlayer = libvlc_media_player_new_from_media(pMedia);
        if (nullptr == m_pPlayer)
        {
            return false;
        }
        libvlc_media_player_set_hwnd(m_pPlayer, m_handle);
        int ret = libvlc_media_player_play(m_pPlayer);
        if (0 != ret)
        {
            StopVideo();
            return false;
        }

    }
    catch (...)
    {
    }

    return true;
}


/// 初始化vlc
bool VlcPlayer::InitPlayer()
{ 
    try
    {
        m_instace = libvlc_new(0, NULL);
        if (nullptr == m_instace)
        {
            return false;
        }
        pMedia = libvlc_media_new_location(m_instace, net_url.c_str()); 
        if (nullptr == pMedia)
        {
            return false;
        }
        string ss = ":network-caching=";
        ss.append(to_string(m_bufferTime));
        libvlc_media_add_option(pMedia, ss.c_str());//缓冲时间
        m_pPlayer = libvlc_media_player_new_from_media(pMedia);
        if (nullptr == m_pPlayer)
        {
            return false;
        }
        libvlc_media_player_set_hwnd(m_pPlayer, m_handle);
        int ret = libvlc_media_player_play(m_pPlayer);
        if (0 != ret)
        {
            StopVideo();
            return false;
        }
        
    }
    catch (...)
    {
    }
     
    return true;
}
  
/// 停止播放
void VlcPlayer::StopVideo()
{
    libvlc_media_player_stop(m_pPlayer);
    libvlc_media_player_release(m_pPlayer);
}
 
 
void VlcPlayer::pause()
{
    if (m_pPlayer) libvlc_media_player_pause(m_pPlayer);
}

void VlcPlayer::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMoving = true;
        mLastMousePosition = event->globalPos();
    }
}

void VlcPlayer::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
    {
        setCursor(Qt::OpenHandCursor);
        this->move(this->pos() + (event->globalPos() - mLastMousePosition));
        mLastMousePosition = event->globalPos();
    }
}

void VlcPlayer::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mMoving = false;
        setCursor(Qt::ArrowCursor);
    }
}

void VlcPlayer::mouseDoubleClickEvent(QMouseEvent* event) {
    if (Qt::LeftButton == event->button())
    {
        isMax = !isMax;
        if (isMax)
        {
            this->showMaximized();
            this->setToolTip(tr("双击正常窗体大小显示"));
        }
        else
        {
            this->showNormal();
            this->setToolTip(tr("双击窗体最大化"));
        }
    }

    //emit onDoubleClick();//此处调用最大化/还原按钮点击槽
    event->ignore();
}