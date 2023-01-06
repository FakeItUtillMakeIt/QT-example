#include "jscommwithqt.h"



JsCommWithQt::JsCommWithQt(QObject *parent) : QObject(parent)
{
    connect(this,&JsCommWithQt::recvMsg,this,[=](){

    });
}

void JsCommWithQt::commMsgToJS(QWebEngineView *page,const QString& msg){
    qDebug()<<"send to JS";
    page->page()->runJavaScript(msg);

}

void JsCommWithQt::commFromJS(QString text){

    QMessageBox::information(nullptr,QString::fromLocal8Bit("info"),text);

}
