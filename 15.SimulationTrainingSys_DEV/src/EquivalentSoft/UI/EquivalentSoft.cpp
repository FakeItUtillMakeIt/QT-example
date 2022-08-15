#include "EquivalentSoft.h"
#include <QDateTime>
#include <QPainter>
#include <qmath.h>
#include <QFont>
#include <QtGui/qfontdatabase.h>
#include<QTime>
#include<QTimer>
#include<QLabel>
#include<QDebug>
#include<QHBoxLayout>
#include<qlistview.h>
#include "../../ConfigInterface/configinterface.h"
#include "../../ConfigInterface/xmlstore.h"
#include "../../ConfigInterface/configglobal.h"

#define LABEL_LEFT(name) leftlabel##name
#define LABEL_RIGHT(name) rightlabel##name
#define DXQ_CLOSE(name) dxqclose##name
#define DXQ_NAME(name) dxqname##name
#define SINGLE_NAME(name) singledxq##name
#define HO_NAME(name) inner_ho##name
using namespace std;


static CenterOperate* gCenterOperate = nullptr;

void processCMD(int cmd, ConfigNameSpace::ConfigButton* btn)
{
	if (!gCenterOperate)
	{
		QMessageBox::warning(nullptr, ("����"), ("�����ӿ�δ��ʼ��"));
		return;
	}
	gCenterOperate->sendCMDFromInterface(cmd, btn);
}


EquivalentSoft::EquivalentSoft(QWidget* parent)
    : QMainWindow(parent)
	, m_pCenterOperate(nullptr)
	, m_pUserDAO(nullptr)
	, m_pDeviceDAO(nullptr)
	, m_pCommandDAO(nullptr)
	, m_pRocketDataDAO(nullptr)
	, tb_show(nullptr)
	, m_myInfoTip(nullptr)
	, m_isMax(false)
	, m_pReceiveRocketData(nullptr)
{
    ui.setupUi(this);
	m_app = AppCache::instance();
	//ui.lb_title->setText(m_app->m_soft->GetName());
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);//ȥ��������
	//this->setWindowTitle(m_app->m_soft->GetName());
    setAttribute(Qt::WA_TranslucentBackground, true);
	ui.comboBox->setView(new  QListView());
	
    Init(); 
	CreatConfigInterface();

}
void EquivalentSoft::CreatConfigInterface()
{  
	gCenterOperate = m_pCenterOperate;
	ConfigNameSpace::ConfigGlobal::cmdhandler = processCMD;

	QList<QPushButton* >  functionBtnList;
	functionBtnList.push_back(ui.configedit);
	functionBtnList.push_back(ui.configstyle);
	functionBtnList.push_back(ui.configsave);
	functionBtnList.push_back(ui.configcurve);
	functionBtnList.push_back(ui.configlabel);
	functionBtnList.push_back(ui.confitbtn);
	functionBtnList.push_back(ui.configgroup);
	functionBtnList.push_back(ui.configdelete);

	//	m_allDeviceParam = m_app->m_allDeviceParam;
	ConfigNameSpace::ConfigGlobal::m_allDeviceParamPtr = &m_app->m_allDeviceParam;
	ConfigNameSpace::ConfigGlobal::m_allCommadPrt = &m_app->m_allCommad;
	ConfigNameSpace::ConfigGlobal::m_allFaultCommnd = &m_app->m_allFaultCommnd;

	ConfigNameSpace::XmlStore::InitSceneFile();
	QString exepath = QApplication::applicationDirPath();
	QList<ConfigNameSpace::SceneInfo>  sceneinfolist;
	int result = ConfigNameSpace::XmlStore::ReadSceneFile(exepath.toStdString() + "/scene/scenes.xml", sceneinfolist);
	//��ʽ������
	ConfigNameSpace::StyleManager* stylemanager = new   ConfigNameSpace::StyleManager;
	ConfigNameSpace::ConfigGlobal::gstylemanager = stylemanager;
	//   //���ֹ�����
	ConfigNameSpace::MoveAbleFrame::CreateControl();
	ConfigNameSpace::ConfigInterface* configinterface = new  ConfigNameSpace::ConfigInterface;
	ConfigNameSpace::ConfigGlobal::gconfiginterface = configinterface;
	configinterface->firstinitFromOut(functionBtnList);
	configinterface->RestoreSceneFromXml(sceneinfolist);
	for (auto ele : configinterface->autoCreateBtn)
	{
		AutoAddDxq(ele.first, ele.second);
	}

	QHBoxLayout* hlayout = new QHBoxLayout;
	hlayout->addWidget(configinterface);
	ui.display_wgt->setLayout(hlayout);
	hlayout->setMargin(0); 
}
void EquivalentSoft::InitFrame()
{
	//����Э��֡
	m_app->m_CurrentRocketDataFrame = m_app->m_RocketDataFrame[m_app->m_CurrentRocketType->m_id]; 
	m_pReceiveRocketData = new ReceiveRocketData(); 
}
 

void EquivalentSoft::Init()
{  
	//m_pCenterOperate = new CenterOperate(ui.center_wgt); 
	//gCenterOperate = m_pCenterOperate;
	QWidget* tmpwidget = new QWidget();
	m_pCenterOperate = new CenterOperate(tmpwidget);
	ConfigNameSpace::ConfigGlobal::cmdhandler = processCMD;
    connect(ui.pb_close, &QPushButton::clicked, this, &EquivalentSoft::CloseWindow);
    connect(ui.pb_min, &QPushButton::clicked, this, &EquivalentSoft::ShowMinimized);
	//connect(ui.pb_resize, &QPushButton::clicked, this, &EquivalentSoft::changeResize);
	// 
		//��������
	int id = QFontDatabase::addApplicationFont(":/EquivalentSoft/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	QFont f;
	f.setFamily(s[0]);
	ui.softwarename->setFont(f);
	ui.rtlabel->setFont(f);
	ui.ctlabel->setFont(f);
	ui.ftlabel->setFont(f);
	ui.rokect_type->setFont(f);
	ui.curtime->setFont(f);
	ui.time->setFont(f);
	m_app->rokecttype = ui.rokect_type;
	//ui.rokect_type->setText(m_app->m_soft->GetType());
	
	//����ʱ��
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timecount()));
	timer->start(1000);
	timer->setTimerType(Qt::PreciseTimer);
	baseTime = QTime::currentTime();

	//��������
	hbj = new QWidget(this);
	hbj->setGeometry(0, 0, 1920, 1080);
	hbj->setStyleSheet("background-color:rgba(0,0,0,0.2)");
	ui.xj_dxqdialog->setParent(hbj);
	ui.xj_dxqdialog->setGeometry(768, 420, 384, 240);
	hbj->hide();

	hbj2 = new QWidget(this);
	hbj2->setGeometry(0, 0, 1920, 1080);
	hbj2->setStyleSheet("background-color:rgba(0,0,0,0.2)");
	ui.sc_dxqdialog->setParent(hbj2);
	ui.sc_dxqdialog->setGeometry(768, 420, 384, 240);
	hbj2->hide();
	//��Ч����زۺ�����
	connect(ui.add, SIGNAL(clicked()), this, SLOT(adddxq()));
	connect(ui.xj_cancel, SIGNAL(clicked()), this, SLOT(closehbj()));
	connect(ui.xj_close, SIGNAL(clicked()), this, SLOT(closehbj()));
	connect(ui.xj_ok, SIGNAL(clicked()), this, SLOT(adddxq_ok()));
	connect(ui.delete_OK, SIGNAL(clicked()), this, SLOT(delete_ok()));
	connect(ui.delete_cancel, SIGNAL(clicked()), this, SLOT(closehbj2()));
	//��Ч���༭
	connect(ui.edit, SIGNAL(clicked()), this, SLOT(editdxq()));
	 
	//��ʼ��������Ϣ��ʾ��
	m_myInfoTip = new MyInfoTip(ui.wgt_status_left);
	//ui.wgt_status_left->setStyleSheet("background-color:red");
	connect(m_myInfoTip, &MyInfoTip::hoverInfo, this, [this]() {
		tb_show->raise();
		tb_show->show();
		
	});
	if (tb_show != nullptr)
	{
		delete tb_show;
	}
	tb_show = new MyLogBox(ui.centralWidget);
	connect(tb_show, &MyLogBox::leaveInfo, this, [this]() {
		tb_show->hide();
	});
	tb_show->hide(); 
	changeResize();

	displayStatuInfo("ϵͳ��ʼ���ػ������ݣ�"); 
	//���ػ�������
	m_pUserDAO = new DataBase::UserDAO(m_app->m_outputPath);
	if (!m_pUserDAO->getUser())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("�����û�������ϣ�"); 

	//���ػ�������
	m_pDeviceDAO = new DataBase::DeviceDAO(m_app->m_outputPath);
	if (!m_pDeviceDAO->getDevice())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("�����豸����������ϣ�");

	if (!m_pDeviceDAO->getDeviceParam())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("�����豸����������ϣ�");

	m_pCommandDAO = new DataBase::CommandDAO(m_app->m_outputPath);
	if (!m_pCommandDAO->getCommand())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("����ָ��������ϣ�");
	if (!m_pCommandDAO->getCommandParam())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("����ָ�����������ϣ�");
	if (!m_pCommandDAO->getFaultCommand())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("���ع�����Ϣ������ϣ�");
	m_pRocketDataDAO = new DataBase::RocketDataDAO(m_app->m_outputPath);
	if (!m_pRocketDataDAO->getRocketData())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("���ؼ���ң������֡Э��������ϣ�");
	if (!m_pRocketDataDAO->getRocketParam())
	{
		QString info = "�������ݿ�����ʧ�ܣ��������ݿ������ļ�";
		displayStatuInfo(info, true);
		return;
	}
	displayStatuInfo("����֡Э�����������ϣ�"); 
	//Э��֡����
	for (auto item : m_app->m_RocketDataFrame)
	{
		item.second->sortParams();
	}
	//InitFrame();
	displayStatuInfo("ϵͳ������ϣ�");

}
//��ӵ�Ч���������
void EquivalentSoft::adddxq()
{
	
	hbj->show();
	
}
//��ӵ�Ч���ɹ�������
void EquivalentSoft::adddxq_ok()
{
	count++;
	QWidget*SINGLE_NAME(count) = new QWidget();
	QString count_str = QString::number(count);
	QString s1 = "single_dxq";
	QString single = s1.append(count_str);
	SINGLE_NAME(count)->setObjectName(single);
	SINGLE_NAME(count)->setMinimumSize(QSize(172, 24));
	SINGLE_NAME(count)->setMaximumSize(QSize(172, 24));

	QString s2 = "inner_ho";
	QString ho = s2.append(count_str);
	QHBoxLayout* HO_NAME(count) = new QHBoxLayout(SINGLE_NAME(count));
	HO_NAME(count)->setSpacing(0);
	HO_NAME(count)->setContentsMargins(11, 11, 11, 11);
	HO_NAME(count)->setObjectName(ho);
	HO_NAME(count)->setContentsMargins(0, 0, 0, 0);

	QLabel* LABEL_LEFT(count) = new QLabel();
	QString s3 = "leftlabel";
	QString left = s3.append(count_str);
	LABEL_LEFT(count)->setObjectName(left);
	LABEL_LEFT(count)->setMinimumSize(QSize(4, 0));
	LABEL_LEFT(count)->setMaximumSize(QSize(4, 16777215));
	LABEL_LEFT(count)->setPixmap(QPixmap(QString::fromUtf8(":/EquivalentSoft/rectleft")));
	LABEL_LEFT(count)->setAlignment(Qt::AlignCenter);
	HO_NAME(count)->addWidget(LABEL_LEFT(count));

	QPushButton* DXQ_NAME(count) = new QPushButton();
	QString s4 = "dxqname";
	QString dxqname = s4.append(count_str);


	DXQ_NAME(count)->setObjectName(dxqname);
	DXQ_NAME(count)->setText(ui.comboBox->currentText());
	DXQ_NAME(count)->setMinimumSize(QSize(140, 24));
	DXQ_NAME(count)->setMaximumSize(QSize(140, 24));
	DXQ_NAME(count)->setStyleSheet(QLatin1String("\n"
		"QPushButton{\n"
		"color:rgb(180,180,180);\n"
		"border:none;\n"
		"}\n"
		"QPushButton:hover{\n"
		"color:black;\n"
		"border:none;\n"
		"background-color:rgb(242,247,252)\n"
		"}\n"
		""));
	DXQ_NAME(count)->setCursor(QCursor(Qt::PointingHandCursor));
	connect(DXQ_NAME(count), SIGNAL(clicked()), this, SLOT(selected()));
	
	HO_NAME(count)->addWidget(DXQ_NAME(count));

	QLabel* LABEL_RIGHT(count) = new QLabel();
	QString s5 = "rightlabel";
	QString right = s5.append(count_str);
	LABEL_RIGHT(count)->setObjectName(right);
	LABEL_RIGHT(count)->setMinimumSize(QSize(4, 0));
	LABEL_RIGHT(count)->setMaximumSize(QSize(4, 16777215));
	LABEL_RIGHT(count)->setPixmap(QPixmap(QString::fromUtf8(":/EquivalentSoft/rectright")));
	LABEL_RIGHT(count)->setAlignment(Qt::AlignCenter);
	HO_NAME(count)->addWidget(LABEL_RIGHT(count));

	QPushButton*DXQ_CLOSE(count) = new QPushButton();
	QString s = "dxqCLOSE";
	QString dxqclose = s.append(count_str);
	connect(DXQ_CLOSE(count), SIGNAL(clicked()), this, SLOT(deletedxq()));

	DXQ_CLOSE(count)->setObjectName(dxqclose);
	DXQ_CLOSE(count)->setMinimumSize(QSize(24, 24));
	DXQ_CLOSE(count)->setMaximumSize(QSize(24, 24));
	DXQ_CLOSE(count)->setCursor(QCursor(Qt::PointingHandCursor));
	DXQ_CLOSE(count)->setStyleSheet(QString::fromUtf8("QPushButton:hover{\n"
		"	background-color:transparent;\n"
		"	border-image: url(:/EquivalentSoft/images/\345\205\263\351\227\255-\346\202\254\346\265\256.png);\n"
		"    border:0px;}\n"
		"QPushButton:pressed{\n"
		"	background-color:transparent; \n"
		"	border-image: url(:/EquivalentSoft/images/\345\205\263\351\227\255-\347\202\271\345\207\273.png);\n"
		"    border:0px;}\n"
		"QPushButton {\n"
		"	background-color:transparent;\n"
		"	border-image: url(:/EquivalentSoft/images/\345\205\263\351\227\255-\351\273\230\350\256\244.png);\n"
		"    border:0px;\n"
		"}\n"
		"\n"
		""));

	HO_NAME(count)->addWidget(DXQ_CLOSE(count));


	ui.dxqbar_ho->addWidget(SINGLE_NAME(count));
	SINGLE_NAME(count)->show();
	cur = SINGLE_NAME(count);
	QList<QPushButton*> list = ui.dxqbar->findChildren<QPushButton*>();
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i]->width() == 140)
		{
			list[i]->setStyleSheet(QLatin1String("\n"
				"QPushButton{\n"
				"color:rgb(180,180,180);\n"
				"border:none;\n"
				"}\n"
				"QPushButton:hover{\n"
				"color:black;\n"
				"border:none;\n"
				"background-color:rgb(242,247,252)\n"
				"}\n"
				""));
		}

	}	
	DXQ_NAME(count)->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252)}");	
	hbj->hide();

	ConfigNameSpace::ConfigGlobal::gconfiginterface->AddSceneFromOut(ui.comboBox->currentText(), DXQ_NAME(count), DXQ_CLOSE(count));
}

void EquivalentSoft::AutoAddDxq(QPushButton* selbtn,QPushButton* delbtn)
{
	count++;
	QWidget* SINGLE_NAME(count) = new QWidget();
	QString count_str = QString::number(count);
	QString s1 = "single_dxq";
	QString single = s1.append(count_str);
	SINGLE_NAME(count)->setObjectName(single);
	SINGLE_NAME(count)->setMinimumSize(QSize(172, 24));
	SINGLE_NAME(count)->setMaximumSize(QSize(172, 24));

	QString s2 = "inner_ho";
	QString ho = s2.append(count_str);
	QHBoxLayout* HO_NAME(count) = new QHBoxLayout(SINGLE_NAME(count));
	HO_NAME(count)->setSpacing(0);
	HO_NAME(count)->setContentsMargins(11, 11, 11, 11);
	HO_NAME(count)->setObjectName(ho);
	HO_NAME(count)->setContentsMargins(0, 0, 0, 0);

	QLabel* LABEL_LEFT(count) = new QLabel();
	QString s3 = "leftlabel";
	QString left = s3.append(count_str);
	LABEL_LEFT(count)->setObjectName(left);
	LABEL_LEFT(count)->setMinimumSize(QSize(4, 0));
	LABEL_LEFT(count)->setMaximumSize(QSize(4, 16777215));
	LABEL_LEFT(count)->setPixmap(QPixmap(QString::fromUtf8(":/EquivalentSoft/rectleft")));
	LABEL_LEFT(count)->setAlignment(Qt::AlignCenter);
	HO_NAME(count)->addWidget(LABEL_LEFT(count));

	QPushButton* DXQ_NAME(count) = selbtn;
	QString s4 = "dxqname";
	QString dxqname = s4.append(count_str);


	DXQ_NAME(count)->setObjectName(dxqname);
	//DXQ_NAME(count)->setText(ui.comboBox->currentText());
	DXQ_NAME(count)->setMinimumSize(QSize(140, 24));
	DXQ_NAME(count)->setMaximumSize(QSize(140, 24));
	DXQ_NAME(count)->setStyleSheet(QLatin1String("\n"
		"QPushButton{\n"
		"color:rgb(180,180,180);\n"
		"border:none;\n"
		"}\n"
		"QPushButton:hover{\n"
		"color:black;\n"
		"border:none;\n"
		"background-color:rgb(242,247,252)\n"
		"}\n"
		""));
	DXQ_NAME(count)->setCursor(QCursor(Qt::PointingHandCursor));
	connect(DXQ_NAME(count), SIGNAL(clicked()), this, SLOT(selected()));

	HO_NAME(count)->addWidget(DXQ_NAME(count));

	QLabel* LABEL_RIGHT(count) = new QLabel();
	QString s5 = "rightlabel";
	QString right = s5.append(count_str);
	LABEL_RIGHT(count)->setObjectName(right);
	LABEL_RIGHT(count)->setMinimumSize(QSize(4, 0));
	LABEL_RIGHT(count)->setMaximumSize(QSize(4, 16777215));
	LABEL_RIGHT(count)->setPixmap(QPixmap(QString::fromUtf8(":/EquivalentSoft/rectright")));
	LABEL_RIGHT(count)->setAlignment(Qt::AlignCenter);
	HO_NAME(count)->addWidget(LABEL_RIGHT(count));

	QPushButton* DXQ_CLOSE(count) = delbtn;
	QString s = "dxqCLOSE";
	QString dxqclose = s.append(count_str);
	connect(DXQ_CLOSE(count), SIGNAL(clicked()), this, SLOT(deletedxq()));

	DXQ_CLOSE(count)->setObjectName(dxqclose);
	DXQ_CLOSE(count)->setMinimumSize(QSize(24, 24));
	DXQ_CLOSE(count)->setMaximumSize(QSize(24, 24));
	DXQ_CLOSE(count)->setCursor(QCursor(Qt::PointingHandCursor));
	DXQ_CLOSE(count)->setStyleSheet(QString::fromUtf8("QPushButton:hover{\n"
		"	background-color:transparent;\n"
		"	border-image: url(:/EquivalentSoft/images/\345\205\263\351\227\255-\346\202\254\346\265\256.png);\n"
		"    border:0px;}\n"
		"QPushButton:pressed{\n"
		"	background-color:transparent; \n"
		"	border-image: url(:/EquivalentSoft/images/\345\205\263\351\227\255-\347\202\271\345\207\273.png);\n"
		"    border:0px;}\n"
		"QPushButton {\n"
		"	background-color:transparent;\n"
		"	border-image: url(:/EquivalentSoft/images/\345\205\263\351\227\255-\351\273\230\350\256\244.png);\n"
		"    border:0px;\n"
		"}\n"
		"\n"
		""));

	HO_NAME(count)->addWidget(DXQ_CLOSE(count));


	ui.dxqbar_ho->addWidget(SINGLE_NAME(count));
	SINGLE_NAME(count)->show();
	cur = SINGLE_NAME(count);
	QList<QPushButton*> list = ui.dxqbar->findChildren<QPushButton*>();
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i]->width() == 140)
		{
			list[i]->setStyleSheet(QLatin1String("\n"
				"QPushButton{\n"
				"color:rgb(180,180,180);\n"
				"border:none;\n"
				"}\n"
				"QPushButton:hover{\n"
				"color:black;\n"
				"border:none;\n"
				"background-color:rgb(242,247,252)\n"
				"}\n"
				""));
		}

	}
	DXQ_NAME(count)->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252)}");
	hbj->hide();
}

//������ѡ��Ч��
void EquivalentSoft::selected()
{
	QPushButton* selectbutton = (QPushButton*)sender();
	QList<QPushButton*> list = ui.dxqbar->findChildren<QPushButton*>();
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i]->width() == 140)
		{
			list[i]->setStyleSheet(QLatin1String("\n"
				"QPushButton{\n"
				"color:rgb(180,180,180);\n"
				"border:none;\n"
				"}\n"
				"QPushButton:hover{\n"
				"color:black;\n"
				"border:none;\n"
				"background-color:rgb(242,247,252)\n"
				"}\n"
				""));
		}
	
	}
	selectbutton->setStyleSheet("QPushButton{color:rgb(0,0,0);border:none;background-color:rgb(242,247,252)}");
	cur = selectbutton->parentWidget();
	QString m = "��չʾ����-�༭�����������ϽǾ���Сͼ��";
	QString n = cur->objectName();
	n.append(m);
   // ui.test->setText(n.append(m));

	ConfigNameSpace::ConfigGlobal::gconfiginterface->ChangeSceneByBtnFromOut(selectbutton);

}
//��������
void EquivalentSoft::closehbj()
{
	hbj->hide();
}
//��������
void EquivalentSoft::closehbj2()
{
	hbj2->hide();
}
//ɾ����ʵ����
void EquivalentSoft::deletedxq()
{
	btnToDelete = (QPushButton*)sender();
	cur = btnToDelete->parentWidget();
	hbj2->show();
}
//ɾ���ɹ�
void EquivalentSoft::delete_ok()
{
	int result = 0;
	if (btnToDelete)
	{
		result = ConfigNameSpace::ConfigGlobal::gconfiginterface->DeleteSceneByBtnFromOut(btnToDelete);
		btnToDelete = nullptr;
		cur->deleteLater();
		cur = nullptr;
		hbj2->hide();
		if(result)
		QMessageBox::warning(nullptr, tr("����"), tr("ɾ���ڵ�ʧ��%1").arg(result));

	}
	
}
//����༭����
void EquivalentSoft::editdxq()

{
	qDebug() << cur;
	if (cur==nullptr)
	{
		QMessageBox::warning(this, "waring", "��Ҫѡ��һ����Ч�����ܱ༭", (QMessageBox::Ok | QMessageBox::Cancel));
		
	}
	else {
	
		QString m = "�ı༭����";
		QString n = cur->objectName();
		n.append(m);
	//	ui.test->setText(n.append(m));
	}

}
//ͷ��ʱ���趨����
void EquivalentSoft::timeupdate()
{

	QDateTime time = QDateTime::currentDateTime();
	ui.curtime->setText(time.toString("yyyy-MM-dd hh:mm:ss"));
}
void EquivalentSoft::timecount()
{

	QTime current = QTime::currentTime();//��ȡϵͳ��ǰʱ��
	int t = this->baseTime.msecsTo(current);  //���������ʱ��֮��
	QTime showtime(0, 0, 0, 0);//��ʼʱ��
	showtime = showtime.addMSecs(t);//����tms
	QString str = showtime.toString("hh:mm:ss"); //ת��Ϊstring����
	ui.time->setText(str);
}
void EquivalentSoft::CloseWindow()
{
    this->close();
}
void EquivalentSoft::ShowMinimized()
{
    this->showMinimized();
}

void EquivalentSoft::displayStatuInfo(QString info, bool is_alarm)
{
	m_myInfoTip->setLabelText(is_alarm, info);
	LOG(INFO) << info.toStdString();
	QString temp_str = QString("%1:%2\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(info);

	tb_show->insertPlainText(temp_str);
	tb_show->moveCursor(QTextCursor::Start);//ʼ�ն�Ϊ�ڵ�һ��
	QTextCursor cursor = tb_show->textCursor();
	cursor.select(QTextCursor::LineUnderCursor);   //ѡ��Ҫ��ɫ������
	QTextCharFormat fmt;
	fmt.setForeground(is_alarm ? QColor(Qt::red) : QColor(Qt::black));
	cursor.mergeCharFormat(fmt);    //�����ı���ʽ
	cursor.clearSelection(); //����ѡ��
	//cursor.movePosition(QTextCursor::EndOfLine);  //cursor��anchor������ĩβ  
}


void EquivalentSoft::changeResize()
{
	m_isMax = !m_isMax;
	QString hoverMark;
	QString pressMark;
	QString mark;

	if (m_isMax)
	{
		mark = "��С-Ĭ��.png";
		pressMark = "��С-���.png";
		hoverMark = "��С-����.png";
		//ui.pb_resize->setToolTip("���»�ԭ");
		ui.vl_UI->setContentsMargins(0, 0, 0, 0); //ȥ���������߽�
		this->move(this->pos() + (QPoint(1, 1))); //���������Ҫ 
		showMaximized();
		tb_show->setGeometry(QRect(6, ui.center_wgt->height() - 158, ui.center_wgt->width() - 12, 300));
 
	}
	else
	{
		mark = "�Ŵ�-Ĭ��.png";
		pressMark = "�Ŵ�-���.png";
		hoverMark = "�Ŵ�-����.png";
		//ui.pb_resize->setToolTip("���");
		showNormal();
		ui.vl_UI->setContentsMargins(5, 5, 5, 5);//�����������߽�
		tb_show->setGeometry(QRect(8, ui.center_wgt->height() - 127, ui.center_wgt->width() - 17, 200));
 
	}

	//ui.lb_title->setGeometry(QRect((ui.wgt_title_middle->width() - 250) / 2, 5, 250, 16));

	/*ui.pb_resize->setStyleSheet(QString("QPushButton:hover{\n"
		"background-color:transparent;\n"
		"background-image:url(:/EquivalentSoft/images/%1);\n"
		"border:0px;}\n"
		"QPushButton:pressed{\n"
		"background-color:transparent;\n"
		"background-image:url(:/EquivalentSoft/images/%2);\n"
		"border:0px;}\n"
		"QPushButton {\n"
		"background-color:transparent;\n"
		"background-image: url(:/EquivalentSoft/images/%3);\n"
		"border:0px;\n"
		"}").arg(hoverMark).arg(pressMark).arg(mark)); */
}

void EquivalentSoft::paintEvent(QPaintEvent* event)
{
	int border_width = 5;
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(border_width, border_width, this->width() - border_width * 2, this->height() - border_width * 2);

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.fillPath(path, QBrush(Qt::white));
	QColor color(0, 0, 0, 250);
	for (int i = 0; i < border_width; i++)
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);
		path.addRect(border_width - i, border_width - i, this->width() - (border_width - i) * 2, this->height() - (border_width - i) * 2);
		color.setAlpha(125 - qSqrt(i) * 50);
		painter.setPen(color);
		painter.drawPath(path);
	}
}

void EquivalentSoft::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier)
	{
		qDebug() << "~~EquivalentSoft offset outrange" <<  " event->globalPos()" << event->globalPos() << " screnn:" << event->screenPos() << " windowspos:" << event->windowPos() << " mLastMousePosition" << mLastMousePosition;;;

		mMoving = true;
		
		mLastMousePosition = event->globalPos();
	}
}

void EquivalentSoft::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons().testFlag(Qt::LeftButton) && mMoving)
	{
		setCursor(Qt::OpenHandCursor);
		QPoint offset = event->globalPos() - mLastMousePosition;

		if (abs(offset.x()) > 50 || abs(offset.y()) > 50)
		{

			return;
		}

		this->move(this->pos() + (offset));
		mLastMousePosition = event->globalPos();
	}
}

void EquivalentSoft::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		mMoving = false;
		setCursor(Qt::ArrowCursor);
	}
}

void EquivalentSoft::mouseDoubleClickEvent(QMouseEvent* event) {
	//if (Qt::LeftButton == event->button())
	//	changeResize();//�˴��������/��ԭ��ť�����
	//	event->ignore();
}