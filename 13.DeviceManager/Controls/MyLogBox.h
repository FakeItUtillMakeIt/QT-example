#pragma once
#include <QObject>
#include <QTextBrowser>
#include<QEvent>
#include <QMouseEvent>
 
  
class MyLogBox : public QTextBrowser
{
	Q_OBJECT 
public:
    MyLogBox(QWidget* parent = Q_NULLPTR);
    ~MyLogBox(); 
private :
    //void enterEvent(QEvent *e) override;//鼠标进入事件
    void leaveEvent(QEvent *e) override;//鼠标离开事件
signals:
    void leaveInfo(QObject* obj);	
};
