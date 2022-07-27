#include "RocketStructShow.h" 
RocketStructShow::RocketStructShow(QWidget *parent)
    : QWidget(parent)
    , m_pVlcPlayer(nullptr)
{
    ui.setupUi(parent);   //
    Init();
} 

void RocketStructShow::InitNav()
{
    ui.lb_weiduan->hide(); 
    pb_level1enginer = false;
    pb_level1enginer_2 = false;
    pb_weiduan = false;
    pb_weiduan_2 = false;  

    if (m_pVlcPlayer)
    {
        m_pVlcPlayer->close();
        delete m_pVlcPlayer;
        m_pVlcPlayer = nullptr;
    }
}

void RocketStructShow::Init()
{ 
    InitNav();
    connect(ui.pb_all, &QPushButton::clicked, this, [this]() { 
        if (pb_level1enginer_2)
        {
            pb_level1enginer_2 = false;
            if (m_pVlcPlayer)
            {
                m_pVlcPlayer->close();
                delete m_pVlcPlayer;
                m_pVlcPlayer = nullptr;
            }
            pb_level1enginer = true;
            ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click1.png")));
            return;
        }

        if (pb_level1enginer)
        {
            pb_level1enginer = false;
            ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p1.png")));
            return;
        }

        if (pb_weiduan_2)
        {
            pb_weiduan_2 = false;
            ui.lb_weiduan->hide();
            pb_weiduan = true;
            ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click2.png")));
            return;
        }

        if (pb_weiduan)
        {
            pb_weiduan = false;
            ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p1.png")));
            return;
        }

        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p1.png")));
    });

    connect(ui.pb_level1enginer, &QPushButton::clicked, this, [this]() {
        InitNav(); 
        pb_level1enginer = true;
        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click1.png")));
    });
    connect(ui.pb_weiduan, &QPushButton::clicked, this, [this]() {
        InitNav();
        pb_weiduan = true;
        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click2.png")));
    });

    connect(ui.pb_level1enginer_2, &QPushButton::clicked, this, [this]() {
        pb_level1enginer_2 = true;  
        m_pVlcPlayer = new VlcPlayer(0, 0, "", 20);
        m_pVlcPlayer->setGeometry(QRect(424, 220, 800, 600)); 
        QString temp = QCoreApplication::applicationDirPath() + "/video/Level1Engine.mp4";
        QString path = temp.replace("/", "\\\\");
        m_pVlcPlayer->PlayVideo(path.toStdString());
    });
    connect(ui.pb_weiduan_2, &QPushButton::clicked, this, [this]() {
        pb_weiduan_2 = true;
        ui.lb_weiduan->show();
    });

    connect(ui.pushButton, &QPushButton::clicked, this, [this] (){
        InitNav();
        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p1.png")));
        ui.pushButton->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav1-2.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_2->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav2-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_3->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav3-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_4->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav4-1.png);\n"
            "   border:0px;\n"
            "}")); 
    });

    connect(ui.pushButton_2, &QPushButton::clicked, this, [this]() {
        InitNav();
        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p2.png")));
        ui.pushButton->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav1-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_2->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav2-2.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_3->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav3-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_4->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav4-1.png);\n"
            "   border:0px;\n"
            "}"));


        });

    connect(ui.pushButton_3, &QPushButton::clicked, this, [this]() {
        InitNav();
        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p3.png")));
        ui.pushButton->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav1-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_2->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav2-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_3->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav3-2.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_4->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav4-1.png);\n"
            "   border:0px;\n"
            "}"));


        });

    connect(ui.pushButton_4, &QPushButton::clicked, this, [this]() {
        InitNav();
        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p4.png")));
        ui.pushButton->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav1-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_2->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav2-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_3->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav3-1.png);\n"
            "   border:0px;\n"
            "}"));

        ui.pushButton_4->setStyleSheet(QLatin1String("QPushButton {\n"
            "	background-color:transparent;\n"
            "	background-image: url(:/images/RocketStruct/nav4-2.png);\n"
            "   border:0px;\n"
            "}"));  
        });
}