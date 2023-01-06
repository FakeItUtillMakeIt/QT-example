#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWebChannel>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QWebEngineView>

#include <QMessageBox>
#include <QKeyEvent>

#include "jscommwithqt.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#define BUILD_DIR "G:\\BaiduNetdiskDownload\\bmap_offline_demo\\"

enum COMM_CMD{
    SEARCH_CITY=0,

    ADD_MARKER=1,
    ADD_LINE=2,
    PLAY_LINE=3,

    EXIT_OP=99
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initConnect();

private:
    QWebChannel* channel;
    QWebEngineView* view;
    JsCommWithQt* m_jsCommWithQt=nullptr;

    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
signals:
    void sendText(const QString &text);

public slots:
    void sendCity();

    void sendData1();
    void sendData2();
    void sendData3();

    void exitMapEdit();

    void testComm(QString text);
};
#endif // MAINWINDOW_H
