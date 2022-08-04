#pragma once
#include <QObject>
#include <QLabel>
#include<QEvent>
#include <QMouseEvent>
 
  
class MyInfoTip : public QLabel
{
	Q_OBJECT 
public:
    MyInfoTip(QWidget* parent = Q_NULLPTR);
    ~MyInfoTip();
    void setLabelText(bool is_alarm, QString info);
private :
    void enterEvent(QEvent *e) override;//鼠标进入事件
    //void leaveEvent(QEvent *e) override;//鼠标离开事件
signals:
    void hoverInfo(QObject* obj);	
};
