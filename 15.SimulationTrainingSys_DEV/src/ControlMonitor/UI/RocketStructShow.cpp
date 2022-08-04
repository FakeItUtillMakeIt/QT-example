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
    //全箭初始化
    ui.lb_weiduan->hide(); 
    ui.lb_up->hide();
    pb_level1enginer = false;
    pb_level1enginer_2 = false;
    pb_weiduan = false;
    pb_weiduan_2 = false;  
    pb_up = false;
    pb_up_2 = false;
     
    //控制舱
    ui.lb_control->hide();
    ui.lb_receiver->hide();
    pb_control = false;
    pb_control_2 = false;
    pb_receiver = false;
    pb_receiver_2 = false;

    if (m_pVlcPlayer)
    {
        m_pVlcPlayer->close();
        delete m_pVlcPlayer;
        m_pVlcPlayer = nullptr;
    }
}


void RocketStructShow::ShowOne()
{
    InitNav();
    ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p1.png")));
     
    ui.one->hide();
    ui.two->hide();
    ui.three->hide();
    ui.four->hide();
    ui.one->show();
     
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
}
void RocketStructShow::ShowTwo()
{
    InitNav();
    ui.label_two->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p2.png")));
     
    ui.one->hide();
    ui.two->hide();
    ui.three->hide();
    ui.four->hide();
    ui.two->show();

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
}
void RocketStructShow::ShowThree()
{
    InitNav();
    ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p3.png")));
     
    ui.one->hide();
    ui.two->hide();
    ui.three->hide();
    ui.four->hide();

    ui.three->show();
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
}
void RocketStructShow::ShowFour() 
{
    InitNav();
    ui.label_four->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p4.png")));
     
    ui.one->hide();
    ui.two->hide();
    ui.three->hide();
    ui.four->hide();

    ui.four->show();
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
}
 
void RocketStructShow::Init()
{
    connect(ui.pushButton, &QPushButton::clicked, this, &RocketStructShow::ShowOne);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &RocketStructShow::ShowTwo);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &RocketStructShow::ShowThree);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &RocketStructShow::ShowFour);

    connect(ui.pb_s_up, &QPushButton::clicked, this, &RocketStructShow::ShowTwo);
    connect(ui.pb_s_control, &QPushButton::clicked, this, &RocketStructShow::ShowThree);
    connect(ui.pb_s_weiduan, &QPushButton::clicked, this, &RocketStructShow::ShowFour);

#pragma region 全箭

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

        if (pb_up_2)
        {
            pb_up_2 = false;
            ui.lb_up->hide();
            pb_up = true;
            ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click3.png")));
            return;
        }

        if (pb_up)
        {
            pb_up = false;
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
    connect(ui.pb_up, &QPushButton::clicked, this, [this]() {
        InitNav();
        pb_up = true;
        ui.label->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click3.png")));
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

    connect(ui.pb_up_2, &QPushButton::clicked, this, [this]() {
        pb_up_2 = true;
        ui.lb_up->show();
        });

#pragma endregion

#pragma region 控制舱
    connect(ui.pb_all_three, &QPushButton::clicked, this, [this]() {
        if (pb_control_2)
        {
            pb_control_2 = false;
            ui.lb_control->hide();
            pb_control = true;
            ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click4.png")));
            return;
        }

        if (pb_control)
        {
            pb_control = false;
            ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p3.png")));
            return;
        }

        if (pb_receiver_2)
        {
            pb_receiver_2 = false;
            ui.lb_receiver->hide();
            pb_receiver = true;
            ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click5.png")));
            return;
        }

        if (pb_receiver)
        {
            pb_receiver = false;
            ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p3.png")));
            return;
        }

        ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/p3.png")));
        });

 
    connect(ui.pb_control, &QPushButton::clicked, this, [this]() {
        InitNav();
        pb_control = true;
        ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click4.png")));
        });
    connect(ui.pb_receiver, &QPushButton::clicked, this, [this]() {
        InitNav();
        pb_receiver = true;
        ui.label_three->setPixmap(QPixmap(QString::fromUtf8(":/images/RocketStruct/click5.png")));
        });
 
    connect(ui.pb_control_2, &QPushButton::clicked, this, [this]() {
        pb_control_2 = true;
        ui.lb_control->show();
        });

    connect(ui.pb_receiver_2, &QPushButton::clicked, this, [this]() {
        pb_receiver_2 = true;
        ui.lb_receiver->show();
        }); 

#pragma endregion 

    ShowOne();
}