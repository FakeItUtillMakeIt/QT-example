#ifndef JSCOMMWITHQT_H
#define JSCOMMWITHQT_H

#include <QObject>
#include <QMessageBox>
#include <QWebEngineView>

class JsCommWithQt : public QObject
{
    Q_OBJECT
public:
    explicit JsCommWithQt(QObject *parent = nullptr);

public:
    void commMsgToJS(QWebEngineView *page,const QString& msg);

signals:
    void recvMsg(QString);
public slots:
    void commFromJS(QString);

};

#endif // JSCOMMWITHQT_H
