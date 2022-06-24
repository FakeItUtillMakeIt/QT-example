#include "CustomParamComponent.h"

using namespace CUSTOM_PARAM_COMPONENT;
using namespace MYSQL_OP;


/**
    @brief  ���캯��
    @param  parent - 
    @retval        - 
**/
CustomParamComponent::CustomParamComponent(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    

    /*���ô������ϲ�*/
	/*this->setWindowFlags(Qt::FramelessWindowHint |
		Qt::WindowStaysOnTopHint |
		Qt::Tool| Qt::WindowSystemMenuHint |
		Qt::WindowCloseButtonHint|
		Qt::MacWindowToolBarButtonHint|
		Qt::WindowContextHelpButtonHint
		);*/
    ui.mainToolBar->deleteLater();
    ui.statusBar->deleteLater();

    QLabel* labelComponnet = new QLabel(QString::fromLocal8Bit("�������"));
    
    //labelComponnet->setPixmap((QPixmap("img/groupbox.png").scaled(QSize(20,20))));
    labelComponnet->setAlignment(Qt::AlignCenter);
    labelComponnet->setMargin(4);
    
    labelComponnet->setStyleSheet("* {background-color:0f0000}");

	dropWidget = new DROP_WIDGET::DropWidget(this);

	dropWidget->setMinimumWidth(600);
    customListWidget = new CUSTOM_LIST_WIDGET::CustomListWidget(this);

    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/label.png").scaled(QSize(60,60))), QString::fromLocal8Bit("��ǩ")));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/button.png").scaled(QSize(60, 60))), QString::fromLocal8Bit("��ť")));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/plot.png").scaled(QSize(60, 60))), QString::fromLocal8Bit("�ź�ͼ")));
    customListWidget->addItem(new QListWidgetItem(QIcon(QPixmap("img/groupbox.png").scaled(QSize(60, 60))), QString::fromLocal8Bit("����")));

    customListWidget->setFixedWidth(90);
    customListWidget->setMinimumHeight(400);

    
    QVBoxLayout* listLayout = new QVBoxLayout();
    listLayout->addWidget(labelComponnet);
    listLayout->addWidget(customListWidget);

    windowLayout->addWidget(dropWidget,0,1);
    //windowLayout->addWidget(customListWidget, 1, 0);
    windowLayout->addLayout(listLayout, 0, 0);
    QPushButton* okButton = new QPushButton(QString::fromLocal8Bit("ȷ�ϲ���"));
    windowLayout->addWidget(okButton, 1, 1);
    connect(okButton, &QPushButton::clicked, this,&CustomParamComponent::saveDropWidgetComponent);
    
    ui.centralWidget->setLayout(windowLayout);

    
}

    
CustomParamComponent::~CustomParamComponent() {

    QFrame* attributeWidget = new QFrame;
    attributeWidget->setFixedWidth(200);

}

/**
    @brief ��д���ƺ���
    @param event - 
**/
void CustomParamComponent::paintEvent(QPaintEvent* event) {

}

/**
    @brief �������Դ���
    @param widget - 
**/
void CustomParamComponent::displayAttributeWindow(QWidget& widget) {
   

    QGridLayout* windowLay = static_cast<QGridLayout*>(this->layout());
    if (windowLay->itemAtPosition(0, 2))
    {
        auto itemPos = windowLay->itemAtPosition(0, 2)->widget();
        itemPos->hide();
        windowLay->removeWidget(itemPos);

    }
    widget.setStyleSheet("* {background-color:#f0ffff }");
	windowLay->addWidget(&widget, 0, 2);
	widget.show();

}

/**
    @brief ����ȷ�ϲ���֮��Ӧ�ý�dropWidgetҳ��ӵ�е����пؼ����͡�λ�á��󶨲����������ݿ�
**/
void CustomParamComponent::saveDropWidgetComponent() {

    MysqlOP* mysqlOp = MysqlOP::getInstance();
    bool ret = mysqlOp->connectMysql("127.0.0.1", "root", "123456", "simulatedtraining", 3306);

    if (!ret)
    {
        qDebug() << "�����ݿ�ʧ��";
        return;
    }

    QString writeString = "INSERT INTO `simulatedtraining`.`compgraphic`(`componnetName`, `posX`, `posY`, `bindParam`) VALUES ";// ('button', 0, 0, 'command')";
   // mysqlOp->writeMysql(writeString.toStdString());


    for each (QObject * c in dropWidget->children())
    {
        
        QWidget* w = static_cast<QWidget*>(c);
        qDebug() << c->objectName()<<w->pos();
        QString bindParam = "";
        if (c->objectName()=="button")
        {
            CUSTOM_PUSHBUTTON::CustomPushButton* buttonTmp = static_cast<CUSTOM_PUSHBUTTON::CustomPushButton*>(c);
            bindParam =QString::number( buttonTmp->getBindParamIndex());
        }
		if (c->objectName() == "label")
		{
			CUSTOM_LABEL::CustomLabel* buttonTmp = static_cast<CUSTOM_LABEL::CustomLabel*>(c);
			bindParam = QString::number(buttonTmp->getBindParamIndex());
		}

        QString objectString("INSERT INTO `simulatedtraining`.`compgraphic`(`componnetName`, `posX`, `posY`, `bindParam`) VALUES ('%1', %2, %3, '%4');");

        QString fin=objectString.arg(c->objectName()).arg(w->x()).arg(w->y()).arg(bindParam);

        qDebug() << fin;
        mysqlOp->writeMysql(fin.toStdString());
    }


}