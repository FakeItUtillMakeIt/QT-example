#include "twoDdisplay.h"
#include"textcontent.h"
#include<QFontDatabase>
#include<QFont>
#include<QList>
#include<qdebug.h>
#include<QPoint>
#include<qmenu.h>
#include<qaction.h>
#include< QFileDialog>
#include<QStringList>
#include<QDateTime>
#include<QString>
#include<QTimer>
#include<QSpacerItem>






twoDdisplay::twoDdisplay(QWidget* parent)
	: QWidget(parent)
	, exepath(QCoreApplication::applicationDirPath() + "/image/")
	, module_thumbnail(nullptr)
	, deletewid(nullptr)
	, e_aflag(true)
	, verticalSpacer(nullptr)
	,editflag(false)
    
	

{
	ui.setupUi(this);
	ui.thumbnail_prew->setPixmap(exepath + "defaultimg.png");
	ui.thumbnail_prew->setScaledContents(true);
		



	connect(ui.editbtn, &QPushButton::clicked, this, [this]() {
		if (editflag==false)
		{
			ui.imgprew_editbtn1->show();
			ui.imgprew_editbtn2->show();
			ui.imgprew_editbtn3->show();
			ui.imgprew_editbtn4->show();
			ui.editbtn->setIcon((QIcon(":/twoDdisplay/noedit")));
			ui.editbtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			ui.editbtn->setIconSize(QSize(40, 40));
			editflag = true;
			ui.textcontent1->setReadOnly(false);
			ui.textcontent2->setReadOnly(false);
			ui.textcontent3->setReadOnly(false);
			ui.textcontent4->setReadOnly(false);
			ui.dialogtitle1->setReadOnly(false);
			ui.dialogtitle2->setReadOnly(false);
			ui.dialogtitle3->setReadOnly(false);
			ui.dialogtitle4->setReadOnly(false);
		}
		else {
			ui.imgprew_editbtn1->hide();
			ui.imgprew_editbtn2->hide();
			ui.imgprew_editbtn3->hide();
			ui.imgprew_editbtn4->hide();

			ui.editbtn->setIcon((QIcon(":/twoDdisplay/edit")));
			ui.editbtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			ui.editbtn->setIconSize(QSize(40, 40));
			editflag = false;
			ui.textcontent1->setReadOnly(true);
			ui.textcontent2->setReadOnly(true);
			ui.textcontent3->setReadOnly(true);
			ui.textcontent4->setReadOnly(true);
			ui.dialogtitle1->setReadOnly(true);
			ui.dialogtitle2->setReadOnly(true);
			ui.dialogtitle3->setReadOnly(true);
			ui.dialogtitle4->setReadOnly(true);
		}
		
		});
	connect(ui.imgprew_editbtn1, &QPushButton::clicked, this, [this]() {
	
		zhezhao_global->show();
		ui.imageEdit->show();
		ui.imgprew_reset->clicked();

		});
	connect(ui.imgprew_editbtn2, &QPushButton::clicked, this, [this]() {
	
		zhezhao_global->show();
		ui.imageEdit->show();
		ui.imgprew_reset->clicked();


		});
	connect(ui.imgprew_editbtn3, &QPushButton::clicked, this, [this]() {
	
		zhezhao_global->show();
		ui.imageEdit->show();
		ui.imgprew_reset->clicked();


		});
	connect(ui.imgprew_editbtn4, &QPushButton::clicked, this, [this]() {
	
		zhezhao_global->show();
		ui.imageEdit->show();
		ui.imgprew_reset->clicked();


		});
	


	module_displaynum=ui.leftscrollpart->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly).size();
	

	connect(ui.imgprewedit_close, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.imageEdit->hide();
		});
	connect(ui.imgprewedit_cancel, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.imageEdit->hide();
		});
	connect(ui.imgprewedit_ok, &QPushButton::clicked, this, [this]() {
	

		
		
		

		zhezhao_global->hide();
		ui.imageEdit->hide();
		});



	connect(ui.imgprew_reset, &QPushButton::clicked, this, [this]() {
		QList<QWidget*> list=ui.imgprew_wid->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
		for (int i = 0; i < list.size(); i++)
		{

			ui.imgsprew_ho->removeWidget(list[i]);
			delete list[i];
		}
		imgprewlist.clear();
		ui.imgupload_num->setText("0");
		ui.thumbnail_prew->setPixmap(exepath + "defaultimg.png");
		ui.thumbnail_prew->setScaledContents(true);
	});
	connect(ui.imgprew_upload, &QPushButton::clicked, this, [this]() {
		QStringList files = QFileDialog::getOpenFileNames(
			this,
			"Select one or more files to open",
			"",
			"ImageFile(*.png *.jpg *.jpeg *.bmp *.Tiff)");




			for (int i=0; i < files.size(); i++)
			{
				imgprewlist.append(files[i]);
			}
			
			

		if (!imgprewlist.isEmpty()&& !files.isEmpty())
		{

			QList<QWidget*> list = ui.imgprew_wid->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly);
			for (int i = 0; i < list.size(); i++)
			{
				
				ui.imgsprew_ho->removeWidget(list[i]);
				delete list[i];
			}
			ui.imgsprew_ho->removeItem(verticalSpacer);
			delete verticalSpacer;
			verticalSpacer = nullptr;

			int row = (int)(imgprewlist.size() / 5);
			int more = imgprewlist.size() % 5;
			if (row>0)
			{
			
	
				if (more == 0)
				{
			
					for (int i = 0; i < row; i++)
					{
						QWidget* tempwid = new QWidget();
						QHBoxLayout* tempho = new QHBoxLayout(tempwid);
						tempho->setSpacing(35);
						tempho->setContentsMargins(35, 0, 35, 0);
						for (int j = 0; j < 5; j++)
						{
							QPushButton* btn = new QPushButton(tempwid);
							btn->setStyleSheet("border:0;outline:0;");
							btn->setMinimumSize(QSize(100, 100));
							btn->setMaximumSize(QSize(100, 100));
							btn->setIcon((QIcon(imgprewlist[i * 5 + j])));
							btn->setObjectName(imgprewlist[i * 5 + j]);
							btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
							btn->setIconSize(QSize(98, 98));
							tempho->addWidget(btn);
						}
						ui.imgsprew_ho->addWidget(tempwid);
					}
				}
				else {
					QWidget* wid = new QWidget(ui.imgprew_wid);
					QHBoxLayout* bar_ho = new QHBoxLayout(wid);
					for (int i = 0; i < row; i++)
					{
						QWidget* tempwid = new QWidget();
						QHBoxLayout* tempho = new QHBoxLayout(tempwid);
						tempho->setSpacing(35);
						tempho->setContentsMargins(35, 0, 35, 0);
						for (int j = 0; j < 5; j++)
						{
							QPushButton* btn = new QPushButton(tempwid);
							btn->setStyleSheet("border:0;outline:0;");
							btn->setMinimumSize(QSize(100, 100));
							btn->setMaximumSize(QSize(100, 100));
							btn->setIcon((QIcon(imgprewlist[i * 5 + j])));
							btn->setObjectName(imgprewlist[i * 5 + j]);
							btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
							btn->setIconSize(QSize(98, 98));
							tempho->addWidget(btn);
						}
						ui.imgsprew_ho->addWidget(tempwid);
					}
					for (int i = 0; i < more; i++)
					{
		
						bar_ho->setSpacing(35);
						bar_ho->setContentsMargins(35, 0, 0, 0);
						QPushButton* btn = new QPushButton(wid);
						btn->setStyleSheet("border:0;outline:0;");
						btn->setMinimumSize(QSize(100, 100));
						btn->setMaximumSize(QSize(100, 100));
						btn->setIcon((QIcon(imgprewlist[row * 5 + i])));
						btn->setObjectName(imgprewlist[row * 5 + i]);
						btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
						btn->setIconSize(QSize(98, 98));
						//				connect(btn, &QPushButton::clicked, this, [=]() {});
						bar_ho->addWidget(btn);

					}
					ui.imgsprew_ho->addWidget(wid);
					QSpacerItem* horizontalSpacer = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
					bar_ho->addItem(horizontalSpacer);
				}

	
	
			

				if (verticalSpacer == nullptr)
				{
					verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
					ui.imgsprew_ho->addItem(verticalSpacer);
				}
			}
			if (row == 0 && more > 0)
			{
				QWidget* wid = new QWidget();
				QHBoxLayout* bar_ho = new QHBoxLayout(wid);
				bar_ho->setSpacing(35);
				bar_ho->setContentsMargins(35, 0, 0, 0);
				for (int i = 0; i < more; i++)
				{
					QPushButton* btn = new QPushButton(wid);
					btn->setStyleSheet("border:0;outline:0;");
					btn->setMinimumSize(QSize(100, 100));
					btn->setMaximumSize(QSize(100, 100));
					btn->setIcon((QIcon(imgprewlist[i])));
					btn->setObjectName(imgprewlist[i]);
					btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
					btn->setIconSize(QSize(98, 98));
					bar_ho->addWidget(btn);
				}
				QSpacerItem* horizontalSpacer = new QSpacerItem(35, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

				bar_ho->addItem(horizontalSpacer);
				ui.imgsprew_ho->addWidget(wid);

				if (verticalSpacer == nullptr)
				{
					verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
					ui.imgsprew_ho->addItem(verticalSpacer);
				}
			

				


			}
			ui.imgupload_num->setText(QString::number(imgprewlist.size()));
			QList<QPushButton*> btnlist = ui.imgprew_wid->findChildren<QPushButton*>();
	
			for (int m = 0; m < btnlist.size(); m++)
			{


				btnlist[m]->setCursor(Qt::PointingHandCursor);
				if (btnlist.size() > 5)
				{
					ui.thumbnail_prew->setPixmap(btnlist[more]->objectName());
				}
				else {
					ui.thumbnail_prew->setPixmap(btnlist[0]->objectName());
				}
				
				connect(btnlist[m], &QPushButton::clicked, this, [=]() {
					
					
					for (int j = 0; j < btnlist.size();j++)
					{
						btnlist[j]->setStyleSheet("border:0;outline:0;}");
						
					}

					btnlist[m]->setStyleSheet("border:0;outline:0;border:2px dashed rgb(0,159,251);}");

					ui.thumbnail_prew->setPixmap(btnlist[m]->objectName());
					});

			}
		}

	
	
	
		}
		
		
		
		);


	
	testbool = true;
	QTimer* flushtest = new QTimer(this);
	connect(flushtest, &QTimer::timeout, this, [this]() {
	
		if (testbool)
		{
		
			ui.flashrect->hide();
			testbool = false;
		}
		else {
	
			ui.flashrect->show();
			testbool = true;
		}
		
		});
	flushtest->start(500);
	//黑色遮罩背景初始化
	zhezhao_global = new QWidget(this);
	zhezhao_global->setGeometry(0, 0, 1920, 794);
	zhezhao_global->setStyleSheet("background-color:rgba(0,0,0,0.3)");
	zhezhao_global->hide();
	ui.modulea_edialog->setGeometry(780, 70, 361,450);
	ui.delete_dialog->setGeometry(768, 70, 384, 241);
	ui.delete_dialog->setParent(zhezhao_global);
	ui.modulea_edialog->setParent(zhezhao_global);
	ui.imageEdit->setParent(zhezhao_global);
	ui.imgprew_editbtn1->hide();
	ui.imgprew_editbtn2->hide();
	ui.imgprew_editbtn3->hide();
	ui.imgprew_editbtn4->hide();
	ui.imageEdit->hide();
	ui.delete_dialog->hide();
	ui.modulea_edialog->hide();
	ui.namewarning->hide();
	ui.imgwarning->hide();


	//初始化载入数字
	QList<QWidget*> list=ui.leftscrollpart->findChildren<QWidget*>(QString(),Qt::FindDirectChildrenOnly);
	module_basenum=list[list.size() - 1]->objectName().split("_")[1].toInt();
	module_basenum++;
	

	//删除模型
	connect(ui.moduleclosebtn_1, &QPushButton::clicked, this, [this]() {
		zhezhao_global->show();
		ui.delete_dialog->show();
		});
	connect(ui.moduleclosebtn_2, &QPushButton::clicked, this, [this]() {
		zhezhao_global->show();
		ui.delete_dialog->show();
		});
	connect(ui.moduleclosebtn_3, &QPushButton::clicked, this, [this]() {
		zhezhao_global->show();
		ui.delete_dialog->show();
		});
	connect(ui.moduleclosebtn_4, &QPushButton::clicked, this, [this]() {
		zhezhao_global->show();
		ui.delete_dialog->show();
		});

	connect(ui.deleteOK, &QPushButton::clicked, this, [this]() {
		if (deletewid != nullptr)
		{
			delete deletewid;
			zhezhao_global->hide();
			ui.delete_dialog->hide();
			deletewid = nullptr;
			curlabel = ui.thumbnail_1;
			ui.thumbnail_1->setStyleSheet("border-image:url(\":/twoDdisplay/select\");");
			ui.thumbnailbtn_1->clicked();
		}
		zhezhao_global->hide();
		ui.delete_dialog->hide();
		ui.stackedWidget->setCurrentIndex(0);
		});
	connect(ui.deleteCancel, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.delete_dialog->hide();
		});
	connect(ui.deletecolsebtn, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.delete_dialog->hide();
		});
	
	//新增模型
	connect(ui.module_add, &QPushButton::clicked, this, [this]() {   
		    e_aflag = true;
	
			ui.e_aname->setText(QString::fromLocal8Bit("新建模型"));
			ui.modulename_edit->setText("");
			zhezhao_global->show();
			ui.modulea_edialog->show();
			ui.namewarning->hide();
			ui.imgwarning->hide();
			ui.module_thumnail->setStyleSheet("border-image:url(:/twoDdisplay/defaultimg);");
			ui.module_thumnail->setPixmap(QPixmap(""));
		


});
	connect(ui.imgupload,&QPushButton::clicked, this, [this]() {
		QString filepath=QFileDialog::getOpenFileName(this,"选择一张图片","","ImageFile (*.png *.jpg *.bpm *.tiff *.jpeg)");	
		QString destpath=CMutils::destfilename_output(filepath);
		thumbnail_originpath = filepath;
		thumbnail_exepath = destpath;
	//	file.copy(filepath, destpath);
		QString str = QString("border-image:url(%1)").arg(filepath);
		if (filepath!="")
		{
			ui.module_thumnail->setPixmap(QPixmap(filepath));
			module_thumbnail = new QFile(filepath);
			mudule_thumbnailpath = destpath;
			ui.module_thumnail->setStyleSheet("");

		}
		
	

		});
	connect(ui.xj_ok, &QPushButton::clicked, this, [this]() {
		ui.modulea_edialog->show();
		if (e_aflag)
		{
			if (ui.modulename_edit->text().isEmpty())
			{
				ui.namewarning->show();
			}
			if (module_thumbnail == nullptr)
			{
				ui.imgwarning->show();
			}
			if (module_thumbnail != nullptr && !ui.modulename_edit->text().isEmpty())
			{

				QFile* temp = module_thumbnail;

				temp->copy(temp->fileName(), mudule_thumbnailpath);

				QWidget* singleModule = new QWidget(ui.leftscrollpart);
				QString objname = "singleModue_";
				objname.append(QString::number(module_basenum));
				singleModule->setObjectName(objname);
				singleModule->setMinimumSize(QSize(272, 187));
				singleModule->setMaximumSize(QSize(272, 187));

				QVBoxLayout* layout = new QVBoxLayout(singleModule);
				layout->setSpacing(0);
				layout->setContentsMargins(11, 11, 11, 11);
				layout->setContentsMargins(0, 0, 0, 0);


				QWidget* moduletop = new QWidget(singleModule);
				QString objname1 = "moduletop_";
				objname1.append(QString::number(module_basenum));
				moduletop->setObjectName(objname1);
				moduletop->setMinimumSize(QSize(0, 36));
				moduletop->setMaximumSize(QSize(16777215, 36));
				moduletop->setStyleSheet("#" + objname1 + "{background-color:rgba(255,255,255,0.6);}");

				QLabel* modulelogo = new QLabel(moduletop);

				QString objname2 = "modulelogo_";
				objname2.append(QString::number(module_basenum));
				modulelogo->setObjectName(objname2);
				modulelogo->setGeometry(QRect(20, 9, 20, 20));
				modulelogo->setPixmap(QPixmap(QString::fromUtf8(":/twoDdisplay/lefticon")));
				modulelogo->setAlignment(Qt::AlignCenter);


				QLabel* modulename = new QLabel(moduletop);
				QString objname3 = "modulelogo_";
				objname3.append(QString::number(module_basenum));
				modulename->setObjectName(objname3);
				modulename->setText(ui.modulename_edit->text());
				modulename->setGeometry(QRect(52, 0, 160, 36));
				modulename->setFont(font);
				modulename->setStyleSheet(QLatin1String("color: rgba(72,72,72,0.8500);font-size:16px;\n"
					""));


				QPushButton* moduleeditbtn = new QPushButton(moduletop);
				QString objname4 = "moduleeditbtn_";
				objname4.append(QString::number(module_basenum));
				moduleeditbtn->setObjectName(objname4);
				moduleeditbtn->setGeometry(QRect(220, 5, 20, 20));
				moduleeditbtn->setMinimumSize(QSize(20, 20));
				moduleeditbtn->setMaximumSize(QSize(20, 20));
				moduleeditbtn->setCursor(QCursor(Qt::PointingHandCursor));
				moduleeditbtn->setStyleSheet(QLatin1String("\n"
					"\n"
					"QPushButton {\n"
					"	background-color:transparent;\n"
					"	border-image: url(:/twoDdisplay/editmodule);\n"
					"    border:0px;\n"
					"}"));
				moduleeditbtn->setIconSize(QSize(20, 20));
				moduleeditbtn->setFlat(false);
				connect(moduleeditbtn, &QPushButton::clicked, this, [=]() {
					e_aflag = false;
					ui.namewarning->hide();
					ui.imgwarning->hide();
					zhezhao_global->show();
					ui.modulea_edialog->show();
					ui.module_thumnail->setStyleSheet("");
					ui.e_aname->setText(QString::fromLocal8Bit("编辑模型"));
					QString module_name = moduleeditbtn->parent()->findChildren<QLabel*>()[1]->text();
					ui.modulename_edit->setText(module_name);
					editwid = (QWidget*)moduleeditbtn->parent()->parent();
					ui.module_thumnail->setPixmap(*(moduleeditbtn->parent()->parent()->findChildren<QLabel*>()[2])->pixmap());

					});

				QPushButton* moduleclosebtn = new QPushButton(moduletop);
				connect(moduleclosebtn, &QPushButton::clicked, this, [=]() {
					zhezhao_global->show();
					ui.delete_dialog->show();
					deletewid = singleModule;
					});

				QString objname5 = "moduleclosebtn_";
				objname5.append(QString::number(module_basenum));
				moduleclosebtn->setObjectName(objname5);
				moduleclosebtn->setGeometry(QRect(240, 0, 30, 30));
				moduleclosebtn->setMinimumSize(QSize(30, 30));
				moduleclosebtn->setMaximumSize(QSize(30, 30));
				moduleclosebtn->setCursor(QCursor(Qt::PointingHandCursor));
				moduleclosebtn->setStyleSheet(QLatin1String("QPushButton:hover{\n"
					"	background-color:transparent;\n"
					"	border-image: url(:/twoDdisplay/closehover);\n"
					"    border:0px;}\n"
					"\n"
					"QPushButton {\n"
					"	background-color:transparent;\n"
					"	border-image: url(:/twoDdisplay/close);\n"
					"    border:0px;\n"
					"}"));
				moduleclosebtn->setIconSize(QSize(20, 20));
				moduleclosebtn->setFlat(false);

				layout->addWidget(moduletop);

				QWidget* thumbnailW = new QWidget(singleModule);
				QString objname6 = "thumbnailW_";
				objname6.append(QString::number(module_basenum));
				thumbnailW->setObjectName(objname6);


				QLabel* thumbnail = new QLabel(thumbnailW);
				QString objname7 = "thumbnail_";
				objname7.append(QString::number(module_basenum));
				thumbnail->setObjectName(objname7);
				thumbnail->setGeometry(QRect(0, 0, 272, 150));
				thumbnail->setStyleSheet("border-image:url(\":/twoDdisplay/select\")");
				thumbnail->setPixmap(QPixmap(mudule_thumbnailpath));
				//thumbnail->setScaledContents(true);
				thumbnail->setAlignment(Qt::AlignCenter);


				QPushButton* thumbnailbtn = new QPushButton(thumbnailW);
				QString objname8 = "thumbnail_";
				objname8.append(QString::number(module_basenum));
				thumbnailbtn->setObjectName(objname8);
				thumbnailbtn->setGeometry(QRect(0, 0, 272, 150));
				thumbnailbtn->setCursor(QCursor(Qt::PointingHandCursor));
				layout->addWidget(thumbnailW);
				ui.leftpart_ho->addWidget(singleModule);
				QWidget* wid = new QWidget();
				QString newobjectname = "page_" + QString::number(module_basenum);
				wid->setObjectName(newobjectname);
				ui.stackedWidget->addWidget(wid);


				connect(thumbnailbtn, &QPushButton::clicked, this, [=]() {
					if (curlabel != nullptr && curlabel != thumbnail)
					{
						thumbnail->setStyleSheet("border-image:url(\":/twoDdisplay/select\")");
						curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
					}
					curlabel = thumbnail;
					QString pagename = thumbnail->objectName().replace("thumbnail", "page");
					QWidget* temp = ui.stackedWidget->findChild<QWidget*>(pagename);
					ui.stackedWidget->setCurrentWidget(ui.stackedWidget->findChild<QWidget*>(pagename));
					});


				singleModule->show();
				module_basenum++;
				module_displaynum++;
				curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
				curlabel = thumbnail;
				ui.stackedWidget->setCurrentWidget(ui.stackedWidget->findChild<QWidget*>(newobjectname));
				zhezhao_global->hide();
				ui.modulea_edialog->hide();
			}
		}
		else {
		zhezhao_global->hide();
		ui.modulea_edialog->hide();
		ui.modulename_edit->text();
		ui.module_thumnail->pixmap();
		QLabel* mname=editwid->findChildren<QLabel*>()[1];
		QLabel* mthumbnail=editwid->findChildren<QLabel*>()[2];
	//	mname->setText(ui.modulename_edit->text());
		//mthumbnail->setPixmap(*ui.module_thumnail->pixmap());

}
	



		});
	connect(ui.xj_cancel, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.modulea_edialog->hide();
		});
	connect(ui.ea_close, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.modulea_edialog->hide();
		});
	//编辑模型
	connect(ui.moduleeditbtn_1, &QPushButton::clicked, this, [this]() {
		e_aflag = false;
		ui.namewarning->hide();
		ui.imgwarning->hide();
		zhezhao_global->show();
		ui.modulea_edialog->show();
		ui.module_thumnail->setStyleSheet("");
		ui.e_aname->setText(QString::fromLocal8Bit("编辑模型"));
		QString module_name=ui.moduleeditbtn_1->parent()->findChildren<QLabel*>()[1]->text();
		ui.modulename_edit->setText(module_name);
		ui.module_thumnail->setPixmap(*(ui.moduleeditbtn_1->parent()->parent()->findChildren<QLabel*>()[2])->pixmap());
		editwid = (QWidget*)ui.moduleeditbtn_1->parent()->parent();

		});
	connect(ui.moduleeditbtn_2, &QPushButton::clicked, this, [this]() {
		e_aflag = false;
		ui.namewarning->hide();
		ui.imgwarning->hide();
		zhezhao_global->show();
		ui.modulea_edialog->show();
		ui.module_thumnail->setStyleSheet("");
		ui.e_aname->setText(QString::fromLocal8Bit("编辑模型"));
		QString module_name = ui.moduleeditbtn_2->parent()->findChildren<QLabel*>()[1]->text();
		ui.modulename_edit->setText(module_name);
		editwid = (QWidget*)ui.moduleeditbtn_2->parent()->parent();
		ui.module_thumnail->setPixmap(*(ui.moduleeditbtn_2->parent()->parent()->findChildren<QLabel*>()[2])->pixmap());


		});
	connect(ui.moduleeditbtn_3, &QPushButton::clicked, this, [this]() {
		    e_aflag = false;
			ui.namewarning->hide();
			ui.imgwarning->hide();
			zhezhao_global->show();
			ui.modulea_edialog->show();
			ui.module_thumnail->setStyleSheet("");
			ui.e_aname->setText(QString::fromLocal8Bit("编辑模型"));
			QString module_name = ui.moduleeditbtn_3->parent()->findChildren<QLabel*>()[1]->text();
			ui.modulename_edit->setText(module_name);
			editwid = (QWidget*)ui.moduleeditbtn_3->parent()->parent();
			ui.module_thumnail->setPixmap(*(ui.moduleeditbtn_3->parent()->parent()->findChildren<QLabel*>()[2])->pixmap());


		    });
	connect(ui.moduleeditbtn_4, &QPushButton::clicked, this, [this]() {
		        e_aflag = false;
				ui.namewarning->hide();
				ui.imgwarning->hide();
				zhezhao_global->show();
				ui.modulea_edialog->show();
				ui.module_thumnail->setStyleSheet("");
				ui.e_aname->setText(QString::fromLocal8Bit("编辑模型"));
				QString module_name = ui.moduleeditbtn_4->parent()->findChildren<QLabel*>()[1]->text();
				ui.modulename_edit->setText(module_name);
				editwid = (QWidget*)ui.moduleeditbtn_4->parent()->parent();
				ui.module_thumnail->setPixmap(*(ui.moduleeditbtn_4->parent()->parent()->findChildren<QLabel*>()[2])->pixmap());


			});






	//加载字体
	int id = QFontDatabase::addApplicationFont(":/ControlMonitor/word/word.ttf");
	QStringList s = QFontDatabase::applicationFontFamilies(id);
	font.setFamily(s[0]);
    ui.title->setFont(font);
	ui.modulename_1->setFont(font);
	ui.modulename_2->setFont(font);
	ui.modulename_3->setFont(font);
	ui.modulename_4->setFont(font);
	ui.dialogtitle1->setFont(font);
	ui.imageview_title->setFont(font);
	ui.dialogtitle2->setFont(font);
	ui.imageview_title2->setFont(font);
	ui.dialogtitle3->setFont(font);
	ui.imageview_title3->setFont(font);
	ui.dialogtitle4->setFont(font);
	ui.imageview_title4->setFont(font);
	ui.tx->setFont(font);
	ui.dmydz->setFont(font);
	ui.fwq_2->setFont(font);
	ui.qdjsj->setFont(font);

	curlabel = nullptr;
	curwid = nullptr;
	curobjectname = "null";
	imgnum = 0;
	ui.zhezhao->hide();
	ui.zhezhao2->hide();
	ui.zhezhao3->hide();
	ui.zhezhao4->hide();
	ui.dialog->hide();
	ui.dialog2->hide();
	ui.dialog3->hide();
	ui.dialog4->hide();
	ui.textcontent1->setReadOnly(true);
	ui.textcontent2->setReadOnly(true);
	ui.textcontent3->setReadOnly(true);
	ui.textcontent4->setReadOnly(true);
	ui.dialogtitle1->setReadOnly(true);
	ui.dialogtitle2->setReadOnly(true);
	ui.dialogtitle3->setReadOnly(true);
	ui.dialogtitle4->setReadOnly(true);


	
	//加载左侧点击事件效果
	if (contentnum != 0)
	{
		curlabel = ui.thumbnail_1;
	}

		connect(ui.thumbnailbtn_1, &QPushButton::clicked, this, [this]() {

		if (curlabel != nullptr && curlabel!=ui.thumbnail_1)
		{
			ui.thumbnail_1->setStyleSheet("border-image:url(\":/twoDdisplay/select\");");
			curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
		}
		curlabel = ui.thumbnail_1;
		ui.stackedWidget->setCurrentIndex(0);
		});

	connect(ui.thumbnailbtn_2, &QPushButton::clicked, this, [this]() {

		if (curlabel != nullptr && curlabel != ui.thumbnail_2)
		{
			ui.thumbnail_2->setStyleSheet("border-image:url(\":/twoDdisplay/select\");");
			curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
		}
		curlabel = ui.thumbnail_2;
		ui.stackedWidget->setCurrentIndex(1);
		});
	connect(ui.thumbnailbtn_3, &QPushButton::clicked, this, [this]() {

		if (curlabel != nullptr && curlabel != ui.thumbnail_3)
		{
			ui.thumbnail_3->setStyleSheet("border-image:url(\":/twoDdisplay/select\")");
			curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
		}
		curlabel = ui.thumbnail_3;
		ui.stackedWidget->setCurrentIndex(2);
		});
	connect(ui.thumbnailbtn_4, &QPushButton::clicked, this, [this]() {

		if (curlabel != nullptr && curlabel != ui.thumbnail_4)
		{
			ui.thumbnail_4->setStyleSheet("border-image:url(\":/twoDdisplay/select\")");
			curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
		}
		curlabel = ui.thumbnail_4;
		ui.stackedWidget->setCurrentIndex(3);
		});

	//全箭切换功能
	connect(ui.smjjump, &QPushButton::clicked, this, [this]() {
		ui.thumbnailbtn_2->clicked();
		});
	connect(ui.kzcjump, &QPushButton::clicked, this, [this]() {
		ui.thumbnailbtn_3->clicked();
		});
	connect(ui.wdjump, &QPushButton::clicked, this, [this]() {
		ui.thumbnailbtn_4->clicked();
		});

	// 全箭模型按钮绑定
	    connect(ui.dialogclose, &QPushButton::clicked, this, [this]() {
			ui.dialog->hide();					
		});
	
		QList<QPushButton*> btnlist = ui.contentwid1->findChildren<QPushButton*>();
	
		for (int i = 0; i < btnlist.size(); i++)
		{
	
			if (btnlist[i]->objectName() != "dialogclose" && btnlist[i]->objectName() != "imageview_close")
			{
				btnlist[i]->setFont(font);
				btnlist[i]->setStyleSheet("font-size:16px;border-image:url(:/twoDdisplay/btnbg);color:rgb(98, 109, 125);");
				connect(btnlist[i], &QPushButton::clicked, this, [=]() {
				
				    curwid = btnlist[i]->parentWidget();
					QPoint pt = curwid->pos();
	
					int x = pt.x();
					int y = pt.y();

					if (x > 1200)
					{
						ui.dialog->setGeometry(x-280, y, 400, 140);
					}
					else {
						ui.dialog->setGeometry(x, y, 400, 140);
					}
					if (btnlist[i]->objectName() == "smjzkbtn")
					{
						curobjectname = "smjzkbtn";
						ui.textcontent1->setText(smjzktext);
						ui.vicontent1->setStyleSheet("border-image:url("+exepath+"/displayimg/smjzk-x.png)");	
					}
					if (btnlist[i]->objectName() == "rwzhbtn")
					{
						curobjectname = "rwzhbtn";
						ui.textcontent1->setText(rwzhtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/rwzh-x.png)");
					}
					if (btnlist[i]->objectName() == "yctxbtn")
					{
						curobjectname = "yctxbtn";
						ui.textcontent1->setText(yctxtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/yctx-x.png)");
					}

					if (btnlist[i]->objectName() == "kzcbtn")
					{	
						curobjectname = "kzcbtn";
						ui.textcontent1->setText(kzctext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/kzc-x.png)");
					}
					if (btnlist[i]->objectName() == "smjgkbtn")
					{
						curobjectname = "smjgkbtn";
						ui.textcontent1->setText(smjgktext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/smjgk-x.png)");
					}
					if (btnlist[i]->objectName() == "sjfdjbtn")
					{
						curobjectname = "sjfdjbtn";
						ui.textcontent1->setText(sjfdjtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/sjfdj-x.png)");
					}
					if (btnlist[i]->objectName() == "esjjjbtn")
					{
						curobjectname = "esjjjbtn";
						ui.textcontent1->setText(esjjjtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/esjjj-x.png)");
					}
					if (btnlist[i]->objectName() == "ejfdjbtn")
					{
						curobjectname = "ejfdjbtn";
						ui.textcontent1->setText(ejfdjtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/ejfdj-x.png)");
					}
					if (btnlist[i]->objectName() == "yejjjbtn")
					{
						curobjectname = "yejjjbtn";
						ui.textcontent1->setText(yejjjtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/yejjj-x.png)");
					}
					if (btnlist[i]->objectName() == "yjfdjbtn")
					{
						curobjectname = "yjfdjbtn";
						ui.textcontent1->setText(yjfdjtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/yjfdj-x.png)");
					}

					if (btnlist[i]->objectName() == "yjwdbtn")
					{
						curobjectname = "yjwdbtn";
						ui.textcontent1->setText(yjwdtext);
						ui.vicontent1->setStyleSheet("border-image:url(" + exepath + "/displayimg/yjwd-x.png)");
					}
					
				
			
					
					ui.dialogtitle1->setText(btnlist[i]->text());
					ui.dialog->show();
					ui.dialog->raise();

					});
			}
		}


		connect(ui.vicontent1, &QPushButton::clicked, this, [=]() {
		
			if (curobjectname == "smjzkbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/smjzk-d.jpg");

			}
			if (curobjectname == "rwzhbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/rwzh-d.jpeg");
			}
			if (curobjectname == "yctxbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/yctx-d.jpg");
			}
			if (curobjectname == "kzcbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				qDebug() << imglist.size();
				imglist.append(exepath + "/displayimg/kzc-d.png");
			}
			if (curobjectname == "smjgkbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/smjgk-d.png");
			}
			if (curobjectname == "sjfdjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/sjfdj-d.png");
			}
			if (curobjectname == "esjjjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/esjjj-d.png");
			}
			if (curobjectname == "ejfdjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/ejfdj-d.png");
			}
			if (curobjectname == "yejjjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/yejjj-d.png");
			}
			if (curobjectname == "yjfdjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/yjfdj-d.png");
			}
			if (curobjectname == "yjwdbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/yjwd-d.png");

			}
			ui.imageview_title->setText(curwid->findChild<QPushButton*>()->text());
			ui.zhezhao->show();
			ui.zhezhao->raise();
			ui.zhezhao->setGeometry(0, 0, 1630, 750);
			QPixmap tmp = QPixmap(imglist[0]);
			int width = 0;
			int height = 0;
			ui.imageview_label->setScaledContents(true);
			if (tmp.height() > 600)
			{
	
				width = 600 * tmp.width() / tmp.height();
				height = 600;
			}
			else {
				width = tmp.size().width();
				height = tmp.size().height();	
			}
			ui.imageview_label->setPixmap(tmp);
			int x = (int)(1630 - width - 80) / 2;
			int y = (int)(750 - height - 80) / 2;


			ui.imageview->setGeometry(x,y,width + 80, height + 80);
			

			});
			connect(ui.imgleft, &QPushButton::clicked, this, [this]() {
				if (imgnum > 0)
				{
					imgnum--;
					QPixmap tmp = QPixmap(imglist[imgnum]);
					int width = 0;
					int height = 0;
					ui.imageview_label->setScaledContents(true);
					if (tmp.height() > 600)
					{
						width = 600 * tmp.width() / tmp.height();
						height = 600;
					}
					else {
						width = tmp.size().width();
						height = tmp.size().height();
					}
					ui.imageview_label->setPixmap(tmp);
					int x = (int)(1630 - width - 80) / 2;
					int y = (int)(750 - height - 80) / 2;
					ui.imageview->setGeometry(x, y, width + 80, height + 80);
				}
			

				});
			connect(ui.imgright, &QPushButton::clicked, this, [this]() {
				if (imgnum <imglist.size()-1)
				{
					imgnum++;
					QPixmap tmp = QPixmap(imglist[imgnum]);
					int width = 0;
					int height = 0;
					ui.imageview_label->setScaledContents(true);
					if (tmp.height() > 600)
					{

						width = 600 * tmp.width() / tmp.height();
						height = 600;
					}
					else {
						width = tmp.size().width();
						height = tmp.size().height();
					}
					ui.imageview_label->setPixmap(tmp);
					int x = (int)(1630 - width - 80) / 2;
					int y = (int)(750 - height - 80) / 2;
					ui.imageview->setGeometry(x, y, width + 80, height + 80);
				}

				});
	
		connect(ui.imageview_close, &QPushButton::clicked, this, [this]() {
			ui.zhezhao->hide();

			});











		// 上面级模型按钮绑定
		connect(ui.dialogclose2, &QPushButton::clicked, this, [this]() {
			ui.dialog2->hide();
			});

		QList<QPushButton*> btnlist2 = ui.contentwid2->findChildren<QPushButton*>();

		for (int i = 0; i < btnlist2.size(); i++)
		{
			if (btnlist2[i]->objectName() != "dialogclose2")
			{
				btnlist2[i]->setFont(font);
				btnlist2[i]->setStyleSheet("font-size:16px;border-image:url(:/twoDdisplay/btnbg);color:rgb(98, 109, 125);");
				connect(btnlist2[i], &QPushButton::clicked, this, [=]() {
			
					curwid = btnlist2[i]->parentWidget();
					QPoint pt = curwid->pos();
					int x = pt.x();
					int y = pt.y();

					if (x > 1200)
					{
						ui.dialog2->setGeometry(x - 280, y, 400, 140);
					}
					else {
						ui.dialog2->setGeometry(x, y, 400, 140);
					}

					if (btnlist2[i]->objectName() == "zlzbtn")
					{
						curobjectname = "zlzbtn";
						ui.textcontent2->setText(zlztext);
						ui.vicontent2->setStyleSheet("border-image:url(" + exepath + "/displayimg/zlz-x.png)");
					}
					if (btnlist2[i]->objectName() == "zkfdjbtn")
					{
						curobjectname = "zkfdjbtn";
						ui.textcontent2->setText(smjzktext);
						ui.vicontent2->setStyleSheet("border-image:url(" + exepath + "/displayimg/smjzk-d.jpg)");
					}
					if (btnlist2[i]->objectName() == "rwzhbtn2")
					{
						curobjectname = "rwzhbtn2";
						ui.textcontent2->setText(rwzhtext);
						ui.vicontent2->setStyleSheet("border-image:url(" + exepath + "/displayimg/rwzh-x.png)");
					}
					if (btnlist2[i]->objectName() == "gkfdjbtn")
					{
						curobjectname = "gkfdjbtn";
						ui.textcontent2->setText(smjgktext);
						ui.vicontent2->setStyleSheet("border-image:url(" + exepath + "/displayimg/smjgk-d.png)");
					}
				
					ui.dialogtitle2->setText(btnlist2[i]->text());
					ui.dialog2->show();
					ui.dialog2->raise();
					ui.imageview_title->setText(curwid->findChild<QPushButton*>()->text());
			
					});
			}
		}



		connect(ui.vicontent2, &QPushButton::clicked, this, [=]() {
			if (curobjectname == "zlzbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/zlz-d.png");

			}
			if (curobjectname == "rwzhbtn2")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/rwzh-d.jpeg");

			}
			if (curobjectname == "gkfdjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/smjgk-d.png");

			}
			if (curobjectname == "zkfdjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/smjzk-d.jpg");
	

			}
			ui.imageview_title2->setText(curwid->findChild<QPushButton*>()->text());
			ui.zhezhao2->show();
			ui.zhezhao2->raise();
			ui.zhezhao2->setGeometry(0, 0, 1630, 750);
			QPixmap tmp2 = QPixmap(imglist[0]);
			int width2 = 0;
			int height2 = 0;
			ui.imageview_label2->setScaledContents(true);
			if (tmp2.height() > 600)
			{
				width2 = 600 * tmp2.width() / tmp2.height();
				height2 = 600;
			}
			else {
				width2 = tmp2.size().width();
				height2 = tmp2.size().height();
			}
			ui.imageview_label2->setPixmap(tmp2);
			int x = (int)(1630 - width2 - 80) / 2;
			int y = (int)(750 - height2 - 80) / 2;
			ui.imageview2->setGeometry(x, y, width2 + 80, height2 + 80);

			});

		connect(ui.imgleft2, &QPushButton::clicked, this, [this]() {
			if (imgnum > 0)
			{
				imgnum--;
				QPixmap tmp2 = QPixmap(imglist[imgnum]);
				int width2 = 0;
				int height2 = 0;
				ui.imageview_label2->setScaledContents(true);
				if (tmp2.height() > 600)
				{
					width2 = 600 * tmp2.width() / tmp2.height();
					height2 = 600;
				}
				else {
					width2 = tmp2.size().width();
					height2 = tmp2.size().height();
				}
				ui.imageview_label2->setPixmap(tmp2);
				int x = (int)(1630 - width2 - 80) / 2;
				int y = (int)(750 - height2 - 80) / 2;
				ui.imageview2->setGeometry(x, y, width2 + 80, height2 + 80);
			}


			});
		connect(ui.imgright2, &QPushButton::clicked, this, [this]() {
			if (imgnum < imglist.size() - 1)
			{
				imgnum++;
				QPixmap tmp2 = QPixmap(imglist[imgnum]);
				int width2 = 0;
				int height2 = 0;
				ui.imageview_label2->setScaledContents(true);
				if (tmp2.height() > 600)
				{
					width2 = 600 * tmp2.width() / tmp2.height();
					height2 = 600;
				}
				else {
					width2 = tmp2.size().width();
					height2 = tmp2.size().height();
				}
				ui.imageview_label2->setPixmap(tmp2);
				int x = (int)(1630 - width2 - 80) / 2;
				int y = (int)(750 - height2 - 80) / 2;
				ui.imageview2->setGeometry(x, y, width2 + 80, height2 + 80);
			}

			});

		connect(ui.dialogclose2, &QPushButton::clicked, this, [this]() {
			ui.dialog2->hide();
			});
		connect(ui.imageview_close2, &QPushButton::clicked, this, [this]() {
			ui.zhezhao2->hide();

			});

























		// 控制舱模型按钮绑定
		connect(ui.dialogclose3, &QPushButton::clicked, this, [this]() {
			ui.dialog3->hide();
			});

		QList<QPushButton*> btnlist3 = ui.contentwid3->findChildren<QPushButton*>();

		for (int i = 0; i < btnlist3.size(); i++)
		{
			if (btnlist3[i]->objectName() != "dialogclose3")
			{
				btnlist3[i]->setFont(font);
				btnlist3[i]->setStyleSheet("font-size:16px;border-image:url(:/twoDdisplay/btnbg);color:rgb(98, 109, 125);");
				connect(btnlist3[i], &QPushButton::clicked, this, [=]() {
					curwid = btnlist3[i]->parentWidget();
					QPoint pt = curwid->pos();
					int x = pt.x();
					int y = pt.y();

					if (x > 1200)
					{
						ui.dialog3->setGeometry(x - 280, y, 400, 140);
					}
					else {
						ui.dialog3->setGeometry(x, y, 400, 140);
					}

					if (btnlist3[i]->objectName() == "gzbtn")
					{
						curobjectname = "gzbtn";
						ui.textcontent3->setText(gztext);
						ui.vicontent3->setStyleSheet("border-image:url(" + exepath + "/displayimg/gz-x.png)");
					}
					if (btnlist3[i]->objectName() == "mcxsydjbtn")
					{
						curobjectname = "mcxsydjbtn";
						ui.textcontent3->setText(mcxsydjtext);
						ui.vicontent3->setStyleSheet("border-image:url(" + exepath + "/displayimg/mcxsydj-x.png)");
					}
					if (btnlist3[i]->objectName() == "aqzljsjbtn")
					{
						curobjectname = "aqzljsjbtn";
						ui.textcontent3->setText(mcxsydjtext);
						ui.vicontent3->setStyleSheet("border-image:url(" + exepath + "/displayimg/aqzljsj-x.png)");
					}
					if (btnlist3[i]->objectName() == "dzhbtn")
					{
						curobjectname = "dzhbtn";
						ui.textcontent3->setText(mcxsydjtext);
						ui.vicontent3->setStyleSheet("border-image:url(" + exepath + "/displayimg/dzh-x.png)");
					}
					if (btnlist3[i]->objectName() == "zkjbtn")
					{
						curobjectname = "zkjbtn";
						ui.textcontent3->setText(mcxsydjtext);
						ui.vicontent3->setStyleSheet("border-image:url(" + exepath + "/displayimg/zkj-x.png)");
					}
					if (btnlist3[i]->objectName() == "spqdbtn")
					{
						curobjectname = "spqdbtn";
						ui.textcontent3->setText(mcxsydjtext);
						ui.vicontent3->setStyleSheet("border-image:url(" + exepath + "/displayimg/spqd-x.png)");
					}
					if (btnlist3[i]->objectName() == "zdcbtn")
					{
						curobjectname = "zdcbtn";
						ui.textcontent3->setText(mcxsydjtext);
						ui.vicontent3->setStyleSheet("border-image:url(" + exepath + "/displayimg/zdc-x.png)");
					}
		
					ui.dialogtitle3->setText(btnlist3[i]->text());
					ui.dialog3->show();
					ui.dialog3->raise();

					});
			}
		}

		connect(ui.vicontent3, &QPushButton::clicked, this, [=]() {
			if (curobjectname == "gzbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/gz-d.jpg");

			}
			if (curobjectname == "mcxsydjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/mcxsydj-d.jpg");

			}
			if (curobjectname == "aqzljsjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/aqzljsj-d.png");

			}
			if (curobjectname == "dzhbtn")
			{
				
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/dzh-d.jpg");

			}
			if (curobjectname == "zkjbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/zkj-d1.jpg");
				imglist.append(exepath + "/displayimg/zkj-d2.png");
			}
			if (curobjectname == "spqdbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/spqd-d.jpg");

			}
			if (curobjectname == "zdcbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/zdc-d.png");

			}
			ui.imageview_title3->setText(curwid->findChild<QPushButton*>()->text());
			ui.zhezhao3->show();
			ui.zhezhao3->raise();
			ui.zhezhao3->setGeometry(0, 0, 1630, 750);
			QPixmap tmp3 = QPixmap(imglist[0]);
			int width3 = 0;
			int height3 = 0;
			ui.imageview_label3->setScaledContents(true);
			if (tmp3.height() > 600)
			{
				width3 = 600 * tmp3.width() / tmp3.height();
				height3 = 600;
			}
			else {
				width3 = tmp3.size().width();
				height3 = tmp3.size().height();
			}
			ui.imageview_label3->setPixmap(tmp3);
			int x = (int)(1630 - width3 - 80) / 2;
			int y = (int)(750 - height3 - 80) / 2;
			ui.imageview3->setGeometry(x, y, width3 + 80, height3 + 80);

			});

		connect(ui.imgleft3, &QPushButton::clicked, this, [this]() {
			if (imgnum > 0)
			{
				imgnum--;
				QPixmap tmp3 = QPixmap(imglist[imgnum]);
				int width3 = 0;
				int height3 = 0;
				ui.imageview_label3->setScaledContents(true);
				if (tmp3.height() > 600)
				{
		
					width3 = 600 * tmp3.width() / tmp3.height();
					height3 = 600;
				}
				else {
					width3 = tmp3.size().width();
					height3 = tmp3.size().height();
				}
				ui.imageview_label3->setPixmap(tmp3);
				int x = (int)(1630 - width3 - 80) / 2;
				int y = (int)(750 - height3 - 80) / 2;
				ui.imageview3->setGeometry(x, y, width3 + 80, height3 + 80);
			}


			});
		connect(ui.imgright3, &QPushButton::clicked, this, [this]() {
			if (imgnum < imglist.size() - 1)
			{
				imgnum++;
				QPixmap tmp3 = QPixmap(imglist[imgnum]);
				int width3 = 0;
				int height3 = 0;
				ui.imageview_label3->setScaledContents(true);
				if (tmp3.height() > 600)
				{
					width3 = 600 * tmp3.width() / tmp3.height();
					height3 = 600;
				}
				else {
					width3 = tmp3.size().width();
					height3 = tmp3.size().height();
				}
				ui.imageview_label3->setPixmap(tmp3);
				int x = (int)(1630 - width3 - 80) / 2;
				int y = (int)(750 - height3 - 80) / 2;
				ui.imageview3->setGeometry(x, y, width3 + 80, height3 + 80);
			}

			});

		connect(ui.dialogclose3, &QPushButton::clicked, this, [this]() {
			ui.dialog3->hide();
			});
		connect(ui.imageview_close3, &QPushButton::clicked, this, [this]() {
			ui.zhezhao3->hide();

			});

















		// 尾段模型按钮绑定
		connect(ui.dialogclose4, &QPushButton::clicked, this, [this]() {
			ui.dialog4->hide();
			});

		QList<QPushButton*> btnlist4 = ui.contentwid4->findChildren<QPushButton*>();

		for (int i = 0; i < btnlist4.size(); i++)
		{
			if (btnlist4[i]->objectName() != "dialogclose4")
			{
				btnlist4[i]->setFont(font);
				btnlist4[i]->setStyleSheet("font-size:16px;border-image:url(:/twoDdisplay/btnbg);color:rgb(98, 109, 125);");
				connect(btnlist4[i], &QPushButton::clicked, this, [=]() {
					curwid = btnlist4[i]->parentWidget();
					QPoint pt = curwid->pos();
					int x = pt.x();
					int y = pt.y();

					if (x > 1200)
					{
						ui.dialog4->setGeometry(x - 280, y, 400, 140);
					}
					else {
						ui.dialog4->setGeometry(x, y, 400, 140);
					}
					if (btnlist4[i]->objectName() == "kqdbtn")
					{
						curobjectname = "kqdbtn";
						ui.textcontent4->setText(kqdtext);
						ui.vicontent4->setStyleSheet("border-image:url(" + exepath + "/displayimg/kqd-x.png)");
					}
					if (btnlist4[i]->objectName() == "sfrdcbtn")
					{
						curobjectname = "sfrdcbtn";
						ui.textcontent4->setText(sfrdctext);
						ui.vicontent4->setStyleSheet("border-image:url(" + exepath + "/displayimg/sfrdc-x.png)");
					}
					if (btnlist4[i]->objectName() == "rqdbtn")
					{
						curobjectname = "rqdbtn";
						ui.textcontent4->setText(rqdtext);
						ui.vicontent4->setStyleSheet("border-image:url(" + exepath + "/displayimg/rqd-x.png)");
					}
					if (btnlist4[i]->objectName() == "sfzdqbtn")
					{
						curobjectname = "sfzdqbtn";
						ui.textcontent4->setText(sfzdqtext);
						ui.vicontent4->setStyleSheet("border-image:url(" + exepath + "/displayimg/sfzdq-x.png)");
					}
					if (btnlist4[i]->objectName() == "sfkzqbtn")
					{
						curobjectname = "sfkzqbtn";
						ui.textcontent4->setText(sfkzqtext);
						ui.vicontent4->setStyleSheet("border-image:url(" + exepath + "/displayimg/sfkzq-x.png)");
					}
			
					ui.dialogtitle4->setText(btnlist4[i]->text());
					ui.dialog4->show();
					ui.dialog4->raise();

					});
			}
		}

		connect(ui.vicontent4, &QPushButton::clicked, this, [=]() {
		
			if (curobjectname == "kqdbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/kqd-d.jpg");

			}
			if (curobjectname == "sfrdcbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/sfrdc-d.jpg");

			}
			if (curobjectname == "rqdbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/rqd-d1.jpg");
				imglist.append(exepath + "/displayimg/rqd-d2.jpg");

			}
			if (curobjectname == "sfzdqbtn")
			{

				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/sfzdq-d1.jpg");
				imglist.append(exepath + "/displayimg/sfzdq-d2.jpg");

			}
			if (curobjectname == "sfkzqbtn")
			{
				foreach(auto item, imglist)
				{
					imglist.removeOne(item);
				}
				imglist.append(exepath + "/displayimg/sfkzq-d1.jpg");
				imglist.append(exepath + "/displayimg/sfkzq-d2.jpg");
			}
	
			ui.imageview_title4->setText(curwid->findChild<QPushButton*>()->text());
			ui.zhezhao4->show();
			ui.zhezhao4->raise();
			ui.zhezhao4->setGeometry(0, 0, 1630, 750);
			QPixmap tmp4 = QPixmap(imglist[0]);
			int width4 = 0;
			int height4 = 0;
			ui.imageview_label4->setScaledContents(true);
			if (tmp4.height() > 600)
			{
				width4 = 600 * tmp4.width() / tmp4.height();
				height4 = 600;
			}
			else {
				width4 = tmp4.size().width();
				height4 = tmp4.size().height();
			}
			ui.imageview_label4->setPixmap(tmp4);
			int x = (int)(1630 - width4 - 80) / 2;
			int y = (int)(750 - height4 - 80) / 2;
			ui.imageview4->setGeometry(x, y, width4 + 80, height4 + 80);

			});
	


			connect(ui.imgleft4, &QPushButton::clicked, this, [this]() {
				if (imgnum > 0)
				{
					imgnum--;
					QPixmap tmp4 = QPixmap(imglist[imgnum]);
					int width4 = 0;
					int height4 = 0;
					ui.imageview_label4->setScaledContents(true);
					if (tmp4.height() > 600)
					{

						width4 = 600 * tmp4.width() / tmp4.height();
						height4 = 600;
					}
					else {
						width4 = tmp4.size().width();
						height4 = tmp4.size().height();
					}
					ui.imageview_label4->setPixmap(tmp4);
					int x = (int)(1630 - width4 - 80) / 2;
					int y = (int)(750 - height4 - 80) / 2;
					ui.imageview4->setGeometry(x, y, width4 + 80, height4 + 80);
				}


				});
			connect(ui.imgright4, &QPushButton::clicked, this, [this]() {
				if (imgnum < imglist.size() - 1)
				{
					imgnum++;
					QPixmap tmp4 = QPixmap(imglist[imgnum]);
					int width4 = 0;
					int height4 = 0;
					ui.imageview_label4->setScaledContents(true);
					if (tmp4.height() > 600)
					{
						width4 = 600 * tmp4.width() / tmp4.height();
						height4 = 600;
					}
					else {
						width4 = tmp4.size().width();
						height4 = tmp4.size().height();
					}
					ui.imageview_label4->setPixmap(tmp4);
					int x = (int)(1630 - width4 - 80) / 2;
					int y = (int)(750 - height4 - 80) / 2;
					ui.imageview4->setGeometry(x, y, width4 + 80, height4 + 80);
				}

				});

			connect(ui.dialogclose4, &QPushButton::clicked, this, [this]() {
				ui.dialog4->hide();
				});
			connect(ui.imageview_close4, &QPushButton::clicked, this, [this]() {
				ui.zhezhao4->hide();

				});


}

twoDdisplay::~twoDdisplay()
{}





