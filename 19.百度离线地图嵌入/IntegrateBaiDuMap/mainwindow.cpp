#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_jsCommWithQt=new JsCommWithQt();

    //加载webchannel.js
    QFileInfo jsFileInfo(QDir::currentPath()+"/qwebchannel.js");
    QString path=QDir::currentPath()+"/qwebchannel.js";

    qDebug()<<path;
    if(!jsFileInfo.exists()){
        QMessageBox::warning(this,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("当前路径没有qwebchannel.js文件"));
    }

    view=new QWebEngineView();

    channel=new QWebChannel();
    channel->registerObject("jsCommWithQt",(QObject*)m_jsCommWithQt);
    QUrl url=QUrl::fromLocalFile(BUILD_DIR "demo01.html");

    view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    view->load(url);
    view->page()->setWebChannel(channel);

    ui->tab_1->setContentsMargins(0,0,0,0);
    ui->widget_2->setContentsMargins(0,0,2,0);
    ui->gridLayout_2->setSpacing(0);
    ui->gridLayout_2->setContentsMargins(0,0,0,0);

    QGridLayout* layout=new QGridLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(view);
    ui->widget_2->setLayout(layout);

    initConnect();
    //connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::sendData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect(){
    connect(ui->searchBtn,&QPushButton::clicked,this,&MainWindow::sendCity);

    connect(ui->markerBtn,&QPushButton::clicked,this,&MainWindow::sendData1);
    connect(ui->lineBtn,&QPushButton::clicked,this,&MainWindow::sendData2);
    connect(ui->playBtn,&QPushButton::clicked,this,&MainWindow::sendData3);

    connect(ui->exitBtn,&QPushButton::clicked,this,&MainWindow::exitMapEdit);
}

void MainWindow::sendCity(){

     m_jsCommWithQt->commMsgToJS(view,QString("citySearch('%1')").arg(ui->lineEdit->text()));
}

void MainWindow::sendData1(){

    m_jsCommWithQt->commMsgToJS(view,QString("execQtCommand(%1)").arg(COMM_CMD::ADD_MARKER));
}

void MainWindow::sendData2(){

    m_jsCommWithQt->commMsgToJS(view,QString("execQtCommand(%1)").arg(COMM_CMD::ADD_LINE));
}

void MainWindow::sendData3(){

    m_jsCommWithQt->commMsgToJS(view,QString("execQtCommand(%1)").arg(COMM_CMD::PLAY_LINE));
}


void MainWindow::exitMapEdit(){

    m_jsCommWithQt->commMsgToJS(view,QString("execQtCommand(%1)").arg(COMM_CMD::EXIT_OP));
}

void MainWindow::testComm(QString text){

    QMessageBox::information(nullptr,"info",text);

}


void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key()==Qt::Key_Escape){
        QMessageBox::information(nullptr,"","");
        m_jsCommWithQt->commMsgToJS(view,QString("execQtCommand(%1)").arg(COMM_CMD::EXIT_OP));
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button()==Qt::RightButton){
        QMessageBox::information(nullptr,"","");
        m_jsCommWithQt->commMsgToJS(view,QString("execQtCommand(%1)").arg(COMM_CMD::EXIT_OP));
    }
}
