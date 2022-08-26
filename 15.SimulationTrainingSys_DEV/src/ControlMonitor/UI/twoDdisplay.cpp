#pragma execution_character_set("UTF-8")
#include "twoDdisplay.h"
#include"textcontent.h"
#include "../../VLCVideo/vlcplayer.h"
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
#include<QDesktopServices>
#include<QMessageBox>
#include<Windows.h>
#include "../../ControlMonitor/UI/Module.h"
#include "../../ControlMonitor/UI/Component.h"








twoDdisplay::twoDdisplay(QWidget* parent)
	: QWidget(parent)
	, exepath(QCoreApplication::applicationDirPath())
	, module_thumbnail(nullptr)
	, deletewid(nullptr)
	, e_aflag(true)
	, verticalSpacer(nullptr)
	,editflag(false)
	, isOpen(false)
	, cur_module(1)
    
	

{
	ui.setupUi(this);

	//dataload();



	//QString img1 = QCoreApplication::applicationDirPath() + "/gz-x.png";


//预览内容载入
	

	//ui.imagebtn_1->setIcon(QIcon(img1));
	//ui.imagebtn_1->setIconSize(QSize(220, 140));

	//黑色遮罩背景初始化
	zhezhao_global = new QWidget(this);
	zhezhao_global->setGeometry(0, 0, 1920, 794);
	zhezhao_global->setStyleSheet("background-color:rgba(0,0,0,0.3)");
	zhezhao_global->hide();
	ui.contentEdit->setParent(zhezhao_global);
	ui.content_prew->setParent(zhezhao_global);
	ui.dialog->setParent(ui.stackedWidget);
	ui.contentEdit->hide();
	//ui.content_prew->hide();
	zhezhao_global->hide();
	ui.content_prew->hide();
	//ui.contentEdit->show();

	ui.modulea_edialog->setGeometry(780, 70, 361, 450);
	ui.delete_dialog->setGeometry(768, 70, 384, 241);
	ui.delete_dialog->setParent(zhezhao_global);
	ui.modulea_edialog->setParent(zhezhao_global);

	ui.imgprew_editbtn->hide();
	ui.save_edit->hide();


	ui.delete_dialog->hide();
	ui.modulea_edialog->hide();
	ui.namewarning->hide();
	ui.imgwarning->hide();



//文件上传相关功能
	ui.uploadimgwid1->hide();
	ui.uploadimgwid2->hide();
	ui.uploadimgwid3->hide();
	ui.uploadimgwid4->hide();
	ui.uploadimgwid5->hide();
	ui.uploadimgwid6->hide();
	ui.uploadedvideo1->hide();
	ui.uploadedvideo2->hide();
	ui.uploadedvideo3->hide();
	ui.uploadedvideo4->hide();
	ui.uploadedvideo5->hide();
	ui.uploadedvideo6->hide();
	ui.fileuploadwid1->hide();
	ui.fileuploadwid2->hide();
	ui.fileuploadwid3->hide();
	ui.fileuploadwid4->hide();
	ui.fileuploadwid5->hide();
	ui.fileuploadwid6->hide();

	connect(ui.imguploadbtn, &QPushButton::clicked, this, [=]() {
		QStringList files = QFileDialog::getOpenFileNames(
			this,
			"Select one or more files to open",
			"",
			"ImageFile(*.png *.jpg *.jpeg)");
	
		if (uploaded_img.size() >= 6||files.size()>6|| (uploaded_img.size()+ files.size())>6)
		{
			QMessageBox::critical(NULL, "错误", "最多只能上传6张图片！");
		}
		
		else 
		{
			for (int i = 0; i < files.size(); i++)
			  {
				
				  QFile* f = new QFile(files[i]);
				  int filesize = (int)(f->size() / (1024 * 1024));

				  if (filesize >= 1)
				{
					QMessageBox::critical(NULL, "错误", "单张图片大小不能超过1M！");
					continue;
				}
				  uploaded_img.append(files[i]);

 
			  }

			if (!uploaded_img.isEmpty())
			{
				for (int i = 1; i < uploaded_img.size() + 1; i++)
				{
					QString imagewidname = "uploadimgwid" + QString::number(i);
					QString imagename = "uploadedimg" + QString::number(i);
					QWidget* wid = ui.imgeditwid->findChild<QWidget*>(imagewidname);
					QLabel* img = ui.imgeditwid->findChild<QLabel*>(imagename);
					QPixmap map =QPixmap(uploaded_img[i - 1]);
				
					if (map.height() > 100 || map.width() > 100)
					{
						QPixmap temp = map.scaled(100, 100, Qt::KeepAspectRatio);
						img->setPixmap(temp);
					}
					else {
					    img->setPixmap(map);
					}
				
					wid->show();
				}

			}
		}
	
		});

	connect(ui.imguploadresetbtn, &QPushButton::clicked, this, [=](){
		for (int i = 1; i <7; i++)
		{
			QString imagewidname = "uploadimgwid" + QString::number(i);
			QString imagename = "uploadedimg" + QString::number(i);
			QWidget* wid = ui.imgeditwid->findChild<QWidget*>(imagewidname);
			QLabel* img = ui.imgeditwid->findChild<QLabel*>(imagename);
			img->clear();
			wid->hide();
		}
		uploaded_img.clear();
		
		});
	
	connect(ui.videouploadbtn, &QPushButton::clicked, this, [this]() {
		QStringList files = QFileDialog::getOpenFileNames(
			this,
			"Select one or more files to open",
			"",
			"VideoFile(*.mp4 *.jpg *.jpeg)");


		if (uploaded_video.size() >= 6 || files.size() > 6 || (uploaded_video.size() + files.size()) > 6)
		{
			QMessageBox::critical(NULL, "错误", "最多只能上传6个视频！");
		}

		else
		{
			for (int i = 0; i < files.size(); i++)
			{

				QFile* f = new QFile(files[i]);
				int filesize = (int)(f->size() / (1024 * 1024));

				if (filesize >= 1024)
				{
					QMessageBox::critical(NULL, "错误", "单个视频不能超过1G！");
					continue;
				}
				uploaded_video.append(files[i]);


			}

			if (!uploaded_video.isEmpty())
			{
				for (int i = 1; i < uploaded_video.size() + 1; i++)
				{
					QString videowidname = "uploadedvideo" + QString::number(i);
					QWidget* wid = ui.videoeditwid->findChild<QWidget*>(videowidname);
					wid->show();
				}

			}
		}

		});

	connect(ui.videouploadresetbtn, &QPushButton::clicked, this, [this]() {
		
	
			for (int i = 1; i < uploaded_video.size() + 1; i++)
			{
				QString videowidname = "uploadedvideo" + QString::number(i);
				QWidget* wid = ui.videoeditwid->findChild<QWidget*>(videowidname);
				wid->hide();
			}

		
			uploaded_video.clear();
		});

	connect(ui.fileuoloadbtn, &QPushButton::clicked, this, [this]() {
		QStringList files = QFileDialog::getOpenFileNames(
			this,
			"Select one or more files to open",
			"",
			"Files(*.pdf *.doc *.docx *.ppt *.pptx *.xls *.xlsx *.txt)");


		if (uploaded_file.size() >= 6 || files.size() > 6 || (uploaded_file.size() + files.size()) > 6)
		{
			QMessageBox::critical(NULL, "错误", "最多只能上传6个文件！");
		}

		else
		{
			for (int i = 0; i < files.size(); i++)
			{

				QFile* f = new QFile(files[i]);
				int filesize = (int)(f->size() / (1024 * 1024));

				if (filesize >= 100)
				{
					QMessageBox::critical(NULL, "错误", "单个文件不能超过100M！");
					continue;
				}
				uploaded_file.append(files[i]);


			}

			if (!uploaded_file.isEmpty())
			{
				for (int i = 1; i < uploaded_file.size() + 1; i++)
				{
					QString filewidname = "fileuploadwid" + QString::number(i);
					QWidget* wid = ui.fileeditwid->findChild<QWidget*>(filewidname);
					QFile* f = new QFile(uploaded_file[i-1]);
					QString filename = "fileuploadname" + QString::number(i);
					QLabel* namelabel = ui.fileeditwid->findChild<QLabel*>(filename);
					namelabel->setText(f->fileName().split(".")[f->fileName().split(".").size()-1].append("文件"));
					wid->show();
				}

			}
		}

		});
	
	connect(ui.fileuploadresetbtn, &QPushButton::clicked, this, [this]() {
		for (int i = 1; i < uploaded_file.size() + 1; i++)
		{
			QString filewidname = "fileuploadwid" + QString::number(i);
			QWidget* wid = ui.fileeditwid->findChild<QWidget*>(filewidname);
			QString filename = "fileuploadname" + QString::number(i);
			QLabel* namelabel = ui.fileeditwid->findChild<QLabel*>(filename);
			namelabel->setText("");
			wid->hide();
		}

		uploaded_file.clear();
		
		});

	connect(ui.contentEdit_close, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.contentEdit->hide();
		
		});
	connect(ui.contentEdit_cancel, &QPushButton::clicked, this, [this]() {
		zhezhao_global->hide();
		ui.contentEdit->hide();

		});
	connect(ui.contentEdit_ok, &QPushButton::clicked, this, [this]() {
		if (uploaded_img.size() == 0)
		{
			QMessageBox::critical(NULL, "错误", "必须上传一张图片");
		}
		else {

			zhezhao_global->hide();
			ui.contentEdit->hide();
		}


		});

	connect(ui.save_edit, &QPushButton::clicked, this, [this]() {
		for (int i = 0; i < uploaded_img.size(); i++)
		{
			QFile* f = new QFile(uploaded_img[i]);
			QString destpath = CMutils::destimg_output(uploaded_img[i], i);

			f->copy(uploaded_img[i], destpath);
			uploaded_img[i] = destpath;


		}
		ui.vicontent->setStyleSheet("border-image:url(" + uploaded_img[0] + ");");
		for (int i = 0; i < uploaded_video.size(); i++)
		{
			QFile* f = new QFile(uploaded_video[i]);
			QString destpath = CMutils::destvideo_output(uploaded_video[i], i);
			f->copy(uploaded_video[i], destpath);
			uploaded_video[i] = destpath;

		}
		for (int i = 0; i < uploaded_file.size(); i++)
		{
			QFile* f = new QFile(uploaded_file[i]);
			QString destpath = CMutils::destfile_output(uploaded_file[i], i);
			f->copy(uploaded_file[i], destpath);
			uploaded_file[i] = destpath;
		}

		curwid->findChild<QPushButton*>(curobjectname + "btn")->setText(ui.dialogtitle->text());
	
		for (int i = 0; i < module_list.size(); i++)
		{
			if (module_list[i]->id = cur_module)
			{
				for (int j = 0; j < module_list[i]->m_Component.size(); j++)
				{
					if (module_list[i]->m_Component[j]->m_objname == curobjectname)
					{
						module_list[i]->m_Component[j]->imglist.clear();
						module_list[i]->m_Component[j]->imglist=uploaded_img;
						module_list[i]->m_Component[j]->videolist.clear();
						module_list[i]->m_Component[j]->videolist = uploaded_video;
						module_list[i]->m_Component[j]->filelist.clear();
						module_list[i]->m_Component[j]->filelist = uploaded_file;
						module_list[i]->m_Component[j]->textContent = ui.textcontent->toPlainText();
						module_list[i]->m_Component[j]->m_Compname = ui.dialogtitle->text();
		
					}

				}
			}

		}


		});
	

	//临时需求
	connect(ui.jzxqs, &QPushButton::clicked, this, [this]() {
	/*	VlcPlayer * m_pVlcPlayer = new VlcPlayer(0, 0, "", 20);
		m_pVlcPlayer->setParent(ui.testwid);
		QString temp = QCoreApplication::applicationDirPath() + "/video/tempjzx.mp4";
		QString path = temp.replace("/", "\\\\");
		m_pVlcPlayer->PlayVideo(path.toStdString());*/
		QString url = QCoreApplication::applicationDirPath() + "/video/tempjzx.mp4";
		QDesktopServices::openUrl(QUrl(url));

		});




//内容预览切换
	connect(ui.displayimage, &QPushButton::clicked, this, [this]() {

		ui.displayimage->setIcon(QIcon(":/twoDdisplay/image_select"));
		ui.displayimage->setIconSize(QSize(24, 24));
		
		ui.displayvideo->setIcon(QIcon(":/twoDdisplay/video_default"));
		ui.displayvideo->setIconSize(QSize(24, 24));
		
		ui.displayfile->setIcon(QIcon(":/twoDdisplay/file_default"));
		ui.displayfile->setIconSize(QSize(24, 24));

		ui.stackedWidget_2->setCurrentIndex(0);

		
		});


	connect(ui.displayvideo, &QPushButton::clicked, this, [this]() {
		ui.displayimage->setIcon(QIcon(":/twoDdisplay/image_default"));
		ui.displayimage->setIconSize(QSize(24, 24));

		ui.displayvideo->setIcon(QIcon(":/twoDdisplay/video_select"));
		ui.displayvideo->setIconSize(QSize(24, 24));

		ui.displayfile->setIcon(QIcon(":/twoDdisplay/file_default"));
		ui.displayfile->setIconSize(QSize(24, 24));
		ui.stackedWidget_2->setCurrentIndex(1);
		});
	connect(ui.displayfile, &QPushButton::clicked, this, [this]() {

		ui.displayimage->setIcon(QIcon(":/twoDdisplay/image_default"));
		ui.displayimage->setIconSize(QSize(24, 24));

		ui.displayvideo->setIcon(QIcon(":/twoDdisplay/video_default"));
		ui.displayvideo->setIconSize(QSize(24, 24));

		ui.displayfile->setIcon(QIcon(":/twoDdisplay/file_select"));
		ui.displayfile->setIconSize(QSize(24, 24));
		ui.stackedWidget_2->setCurrentIndex(2);
		});



	  
























		


//编辑功能启动
	connect(ui.editbtn, &QPushButton::clicked, this, [this]() {
		if (editflag==false)
		{
			ui.imgprew_editbtn->show();
			ui.save_edit->show();
			ui.editbtn->setIcon((QIcon(":/twoDdisplay/noedit")));
			ui.editbtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			ui.editbtn->setIconSize(QSize(40, 40));
			editflag = true;
			ui.textcontent->setReadOnly(false);
			ui.dialogtitle->setReadOnly(false);

		}
		else {
			ui.imgprew_editbtn->hide();
			ui.save_edit->hide();


			ui.editbtn->setIcon((QIcon(":/twoDdisplay/edit")));
			ui.editbtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			ui.editbtn->setIconSize(QSize(40, 40));
			editflag = false;
			ui.textcontent->setReadOnly(true);
			ui.dialogtitle->setReadOnly(true);

		}
		
		});
	connect(ui.imgprew_editbtn, &QPushButton::clicked, this, [this]() {
	
		zhezhao_global->show();
		ui.contentEdit->show();

		//清空ui内容
		ui.uploadimgwid1->hide();
		ui.uploadimgwid2->hide();
		ui.uploadimgwid3->hide();
		ui.uploadimgwid4->hide();
		ui.uploadimgwid5->hide();
		ui.uploadimgwid6->hide();
		ui.uploadedvideo1->hide();
		ui.uploadedvideo2->hide();
		ui.uploadedvideo3->hide();
		ui.uploadedvideo4->hide();
		ui.uploadedvideo5->hide();
		ui.uploadedvideo6->hide();
		ui.fileuploadwid1->hide();
		ui.fileuploadwid2->hide();
		ui.fileuploadwid3->hide();
		ui.fileuploadwid4->hide();
		ui.fileuploadwid5->hide();
		ui.fileuploadwid6->hide();


		if (!uploaded_img.isEmpty())
		{
			for (int i = 1; i < uploaded_img.size() + 1; i++)
			{
				QString imagewidname = "uploadimgwid" + QString::number(i);
				QString imagename = "uploadedimg" + QString::number(i);
				QWidget* wid = ui.imgeditwid->findChild<QWidget*>(imagewidname);
				QLabel* img = ui.imgeditwid->findChild<QLabel*>(imagename);
				QPixmap map = QPixmap(uploaded_img[i - 1]);

				if (map.height() > 100 || map.width() > 100)
				{
					QPixmap temp = map.scaled(100, 100, Qt::KeepAspectRatio);
					img->setPixmap(temp);
				}
				else {
					img->setPixmap(map);
				}

				wid->show();
			}

		}
		if (!uploaded_video.isEmpty())
		{
			for (int i = 1; i < uploaded_video.size() + 1; i++)
			{
				QString videowidname = "uploadedvideo" + QString::number(i);
				QWidget* wid = ui.videoeditwid->findChild<QWidget*>(videowidname);
				wid->show();
			}

		}
		if (!uploaded_file.isEmpty())
		{
			for (int i = 1; i < uploaded_file.size() + 1; i++)
			{
				QString filewidname = "fileuploadwid" + QString::number(i);
				QWidget* wid = ui.fileeditwid->findChild<QWidget*>(filewidname);
				QFile* f = new QFile(uploaded_file[i - 1]);
				QString filename = "fileuploadname" + QString::number(i);
				QLabel* namelabel = ui.fileeditwid->findChild<QLabel*>(filename);
				namelabel->setText(f->fileName().split(".")[f->fileName().split(".").size() - 1].append("文件"));
				wid->show();
			}

		}
	

		});

	
	
	//展示模型的数量

	module_displaynum=ui.leftscrollpart->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly).size();
	



		
		
		







	//矩形框闪烁动画

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
				QFile* f=new QFile(thumbnail_originpath);
				f->copy(thumbnail_originpath, thumbnail_exepath);
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
					ui.dialog->hide();
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
		ui.dialog->hide();
		QLabel* mname=editwid->findChildren<QLabel*>()[1];
		QLabel* mthumbnail=editwid->findChildren<QLabel*>()[2];
	mname->setText(ui.modulename_edit->text());
		mthumbnail->setPixmap(*ui.module_thumnail->pixmap());

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
	ui.dialogtitle->setFont(font);
	ui.tx->setFont(font);
	ui.dmydz->setFont(font);
	ui.fwq_2->setFont(font);
	ui.qdjsj->setFont(font);

	curlabel = nullptr;
	curwid = nullptr;
	curobjectname = "null";
	imgnum = 0;

	ui.dialog->hide();

	ui.textcontent->setReadOnly(true);
	ui.dialogtitle->setReadOnly(true);



	
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
		cur_module = 1;
		ui.dialog->hide();
		});

	connect(ui.thumbnailbtn_2, &QPushButton::clicked, this, [this]() {

		if (curlabel != nullptr && curlabel != ui.thumbnail_2)
		{
			ui.thumbnail_2->setStyleSheet("border-image:url(\":/twoDdisplay/select\");");
			curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
		}
		curlabel = ui.thumbnail_2;
		ui.stackedWidget->setCurrentIndex(1);
		cur_module = 2;
		ui.dialog->hide();
		});
	connect(ui.thumbnailbtn_3, &QPushButton::clicked, this, [this]() {

		if (curlabel != nullptr && curlabel != ui.thumbnail_3)
		{
			ui.thumbnail_3->setStyleSheet("border-image:url(\":/twoDdisplay/select\")");
			curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
		}
		curlabel = ui.thumbnail_3;
		ui.stackedWidget->setCurrentIndex(2);
		cur_module = 3;
		ui.dialog->hide();
		});
	connect(ui.thumbnailbtn_4, &QPushButton::clicked, this, [this]() {

		if (curlabel != nullptr && curlabel != ui.thumbnail_4)
		{
			ui.thumbnail_4->setStyleSheet("border-image:url(\":/twoDdisplay/select\")");
			curlabel->setStyleSheet("border-image:url(\"\");background-color:rgba(255,255,255,0.3);");
		}
		curlabel = ui.thumbnail_4;
		ui.stackedWidget->setCurrentIndex(3);
		cur_module = 4;
		ui.dialog->hide();
		});









	//全箭区域切换功能
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
					
						curobjectname = "smjzk";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "smjzk")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url("+ module_list[0]->m_Component[j]->imglist[0]+");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
						
	
					}
					if (btnlist[i]->objectName() == "rwzhbtn")
					{
						curobjectname = "rwzh";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "rwzh")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}

					}
					if (btnlist[i]->objectName() == "yctxbtn")
					{
						curobjectname = "yctx";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "yctx")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}

					if (btnlist[i]->objectName() == "kzcbtn")
					{	
						curobjectname = "kzc";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "kzc")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist[i]->objectName() == "smjgkbtn")
					{
						curobjectname = "smjgk";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "smjgk")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist[i]->objectName() == "sjfdjbtn")
					{
						curobjectname = "sjfdj";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "sjfdj")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}

					}
					if (btnlist[i]->objectName() == "esjjjbtn")
					{
						curobjectname = "esjjj";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "esjjj")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}

					}
					if (btnlist[i]->objectName() == "ejfdjbtn")
					{
						curobjectname = "ejfdj";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "ejfdj")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
		
					}
					if (btnlist[i]->objectName() == "yejjjbtn")
					{
						curobjectname = "yejjj";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "yejjj")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}

					}
					if (btnlist[i]->objectName() == "yjfdjbtn")
					{
						curobjectname = "yjfdj";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "yjfdj")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}

					}

					if (btnlist[i]->objectName() == "yjwdbtn")
					{
						curobjectname = "yjwd";
						for (int j = 0; j < module_list[0]->m_Component.size(); j++)
						{
							if (module_list[0]->m_Component[j]->m_objname == "yjwd")
							{
								ui.textcontent->setText(module_list[0]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[0]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
				
					}
				
					ui.dialogtitle->setText(btnlist[i]->text());
					ui.dialog->show();
					ui.dialog->raise();

					});
			}
		}

//点击图片进入详情预览后相关操作
connect(ui.vicontent, &QPushButton::clicked, this, [=]() {
			zhezhao_global->show();
			ui.content_prew->show();
			ui.displayimage->clicked();

			for (int i = 0; i < module_list[cur_module-1]->m_Component.size(); i++)
			{
				if (module_list[cur_module - 1]->m_Component[i]->m_objname == curobjectname)
				{
					prew_img = module_list[cur_module - 1]->m_Component[i]->imglist;
					prew_video = module_list[cur_module - 1]->m_Component[i]->videolist;
					prew_file = module_list[cur_module - 1]->m_Component[i]->filelist;
				}
			}
		
//清空里面内容
			for (int i = 0; i < 6; i++)
			{
				ui.image_page->findChildren<QPushButton*>()[i]->hide();
				ui.image_page->findChildren<QLabel*>()[i]->hide();
				QPushButton* imgbtn = ui.image_page->findChild<QPushButton*>("imagebtn_" + QString::number(i + 1));
				disconnect(imgbtn, &QPushButton::clicked, 0, 0);


				ui.video_page->findChildren<QPushButton*>()[i]->hide();
				ui.video_page->findChildren<QLabel*>()[i]->hide();
				QPushButton* video = ui.video_page->findChild<QPushButton*>("videobtn" + QString::number(i + 1));
				disconnect(video, &QPushButton::clicked, 0, 0);

				ui.filepage->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)[i]->hide();
				QPushButton* filebtn = ui.filepage->findChild<QPushButton*>("filebtn" + QString::number(i + 1));
				disconnect(filebtn, &QPushButton::clicked, 0, 0);
	
	
			}
			for (int i = 0; i < prew_img.size(); i++)
			{

				QPushButton* btn = ui.image_page->findChild<QPushButton*>("imagebtn_" + QString::number(i + 1));
				QLabel* picname = ui.image_page->findChild<QLabel*>("imgname" + QString::number(i + 1));
				btn->setIcon(QIcon(prew_img[i]));
				btn->setIconSize(QSize(220, 140));
				btn->show();
				picname->show();
				connect(btn, &QPushButton::clicked, this, [=]() {
					prewimg_index = btn->objectName().split("_")[1].toInt() - 1;
					ui.stackedWidget_2->setCurrentIndex(3);
					QPixmap map = QPixmap(prew_img[prewimg_index]);
					ui.bigimg_prew->setMaximumSize(750, 420);
					if (map.height() > 420 || map.width() > 750)
					{
						QPixmap temp = map.scaled(750, 420, Qt::KeepAspectRatio);
						ui.bigimg_prew->setPixmap(temp);
					}
					else {
						ui.bigimg_prew->setPixmap(map);
					}
					});
			}			
			for (int i = 0; i < prew_video.size(); i++)
			{
				QPushButton* video = ui.video_page->findChild<QPushButton*>("videobtn" + QString::number(i + 1));
				QLabel* videoname = ui.video_page->findChild<QLabel*>("videoname" + QString::number(i + 1));
			
				connect(video, &QPushButton::clicked, this, [=]() {
					prewvideo_index = video->objectName().split("n")[1].toInt() - 1;				
					QDesktopServices::openUrl(QUrl(prew_video[prewvideo_index]));
					});
				video->show();
				videoname->show();

			}
			for (int i = 0; i < prew_file.size(); i++)
			{
			
				QWidget* filewid = ui.filepage->findChild<QWidget*>("filewid" + QString::number(i + 1));
				QPushButton* filebtn = ui.filepage->findChild<QPushButton*>("filebtn" + QString::number(i + 1));
				connect(filebtn, &QPushButton::clicked, this, [=]() {
					prewfile_index = filebtn->objectName().split("n")[1].toInt() - 1;

					QDesktopServices::openUrl(QUrl(prew_file[prewfile_index]));
					});
				filewid->show();

			}
			});



//预览图片切换

connect(ui.imgleft_arr, &QPushButton::clicked, this, [=]() {

				if (prewimg_index == 0)
				{

					prewimg_index = prew_img.size()-1;

					QPixmap map = QPixmap(prew_img[prewimg_index]);
					if (map.height() > 420 || map.width() > 750)
					{
						QPixmap temp = map.scaled(750, 420, Qt::KeepAspectRatio);
						ui.bigimg_prew->setPixmap(temp);
					}
					else { ui.bigimg_prew->setPixmap(map); }
			
				}
				else {
					prewimg_index--;
					QPixmap map = QPixmap(prew_img[prewimg_index]);
					if (map.height() > 420 || map.width() > 750)
					{
						QPixmap temp = map.scaled(750, 420, Qt::KeepAspectRatio);
						ui.bigimg_prew->setPixmap(temp);
					}
					else { ui.bigimg_prew->setPixmap(map); }
				}

	
				ui.bigimg_prew->setMaximumSize(750, 420);
			
				});
connect(ui.imgright_arr, &QPushButton::clicked, this, [=]() {
				if (prewimg_index == prew_img.size() - 1)
				{
	
					prewimg_index =0;
					QPixmap map = QPixmap(prew_img[prewimg_index]);
					if (map.height() > 420 || map.width() > 750)
					{
						QPixmap temp = map.scaled(750, 420, Qt::KeepAspectRatio);
						ui.bigimg_prew->setPixmap(temp);
					}
					else { ui.bigimg_prew->setPixmap(map); }
				}
				else {
					prewimg_index++;
					QPixmap map = QPixmap(prew_img[prewimg_index]);
					if (map.height() > 420 || map.width() > 750)
					{
						QPixmap temp = map.scaled(750, 420, Qt::KeepAspectRatio);
						ui.bigimg_prew->setPixmap(temp);
					}
					else { ui.bigimg_prew->setPixmap(map); }
				}


				ui.bigimg_prew->setMaximumSize(750, 420);

				});


//关闭预览界面
connect(ui.cotentprew_close, &QPushButton::clicked, this, [this]()
	           {
				zhezhao_global->hide();
				ui.content_prew->hide();
				});









// 上面级模型按钮绑定


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
						ui.dialog->setGeometry(x - 280, y, 400, 140);
					}
					else {
						ui.dialog->setGeometry(x, y, 400, 140);
					}

					if (btnlist2[i]->objectName() == "zlzbtn")
					{
						curobjectname = "zlz";
						for (int j = 0; j < module_list[1]->m_Component.size(); j++)
						{
							if (module_list[1]->m_Component[j]->m_objname == "zlz")
							{
								ui.textcontent->setText(module_list[1]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[1]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[1]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist2[i]->objectName() == "zkfdjbtn")
					{
						curobjectname = "zkfdj";
						for (int j = 0; j < module_list[1]->m_Component.size(); j++)
						{
							if (module_list[1]->m_Component[j]->m_objname == "zkfdj")
							{
								ui.textcontent->setText(module_list[1]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[1]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[1]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}
						}
					}
					if (btnlist2[i]->objectName() == "rwzhbtn2")
					{
						curobjectname = "rwzh";
						for (int j = 0; j < module_list[1]->m_Component.size(); j++)
						{
							if (module_list[1]->m_Component[j]->m_objname == "rwzh")
							{
								ui.textcontent->setText(module_list[1]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[1]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[1]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist2[i]->objectName() == "gkfdjbtn")
					{
						curobjectname = "gkfdj";
						for (int j = 0; j < module_list[1]->m_Component.size(); j++)
						{
							if (module_list[1]->m_Component[j]->m_objname == "gkfdj")
							{
								ui.textcontent->setText(module_list[1]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[0]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[1]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}
								}
							}
						}
					}
				
					ui.dialogtitle->setText(btnlist2[i]->text());
					ui.dialog->show();
					ui.dialog->raise();
			
					});
			}
		}

































// 控制舱模型按钮绑定
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
						ui.dialog->setGeometry(x - 280, y, 400, 140);
					}
					else {
						ui.dialog->setGeometry(x, y, 400, 140);
					}

					if (btnlist3[i]->objectName() == "gzbtn")
					{
						curobjectname = "gz";
						for (int j = 0; j < module_list[2]->m_Component.size(); j++)
						{
							if (module_list[2]->m_Component[j]->m_objname == "gz")
							{
								ui.textcontent->setText(module_list[2]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[2]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[2]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist3[i]->objectName() == "mcxsydjbtn")
					{
						curobjectname = "mcxsydj";
						for (int j = 0; j < module_list[2]->m_Component.size(); j++)
						{
							if (module_list[2]->m_Component[j]->m_objname == "mcxsydj")
							{
								ui.textcontent->setText(module_list[2]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[2]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[2]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist3[i]->objectName() == "aqzljsjbtn")
					{
						curobjectname = "aqzljsj";
						for (int j = 0; j < module_list[2]->m_Component.size(); j++)
						{
							if (module_list[2]->m_Component[j]->m_objname == "aqzljsj")
							{
								ui.textcontent->setText(module_list[2]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[2]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[2]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist3[i]->objectName() == "dzhbtn")
					{
						curobjectname = "dzh";
						for (int j = 0; j < module_list[2]->m_Component.size(); j++)
						{
							if (module_list[2]->m_Component[j]->m_objname == "dzh")
							{
								ui.textcontent->setText(module_list[2]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[2]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[2]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist3[i]->objectName() == "zkjbtn")
					{
						curobjectname = "zkj";
						for (int j = 0; j < module_list[2]->m_Component.size(); j++)
						{
							if (module_list[2]->m_Component[j]->m_objname == "zkj")
							{
								ui.textcontent->setText(module_list[2]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[2]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[2]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist3[i]->objectName() == "spqdbtn")
					{
						curobjectname = "spqd";
						for (int j = 0; j < module_list[2]->m_Component.size(); j++)
						{
							if (module_list[2]->m_Component[j]->m_objname == "spqd")
							{
								ui.textcontent->setText(module_list[2]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[2]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[2]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist3[i]->objectName() == "zdcbtn")
					{
						curobjectname = "zdc";
						for (int j = 0; j < module_list[2]->m_Component.size(); j++)
						{
							if (module_list[2]->m_Component[j]->m_objname == "zdc")
							{
								ui.textcontent->setText(module_list[2]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[2]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[2]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
		
					ui.dialogtitle->setText(btnlist3[i]->text());
					ui.dialog->show();
					ui.dialog->raise();

					});
			}
		}


		

















// 尾段模型按钮绑定
QList<QPushButton*> btnlist4 = ui.contentwid4->findChildren<QPushButton*>();
for (int i = 0; i < btnlist4.size(); i++)
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
						ui.dialog->setGeometry(x - 280, y, 400, 140);
					}
					else {
						ui.dialog->setGeometry(x, y, 400, 140);
					}
					if (btnlist4[i]->objectName() == "kqdbtn")
					{
						curobjectname = "kqd";
						for (int j = 0; j < module_list[3]->m_Component.size(); j++)
						{
							if (module_list[3]->m_Component[j]->m_objname == "kqd")
							{
								ui.textcontent->setText(module_list[3]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[3]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[3]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist4[i]->objectName() == "sfrdcbtn")
					{
						curobjectname = "sfrdc";
						for (int j = 0; j < module_list[3]->m_Component.size(); j++)
						{
							if (module_list[3]->m_Component[j]->m_objname == "sfrdc")
							{
								ui.textcontent->setText(module_list[3]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[3]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[3]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist4[i]->objectName() == "rqdbtn")
					{
						curobjectname = "rqd";
						for (int j = 0; j < module_list[3]->m_Component.size(); j++)
						{
							if (module_list[3]->m_Component[j]->m_objname == "rqd")
							{
								ui.textcontent->setText(module_list[3]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[3]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[3]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist4[i]->objectName() == "sfzdqbtn")
					{
						curobjectname = "sfzdq";
						for (int j = 0; j < module_list[3]->m_Component.size(); j++)
						{
							if (module_list[3]->m_Component[j]->m_objname == "sfzdq")
							{
								ui.textcontent->setText(module_list[3]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[3]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[3]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
					if (btnlist4[i]->objectName() == "sfkzqbtn")
					{
						curobjectname = "sfkzq";
						for (int j = 0; j < module_list[3]->m_Component.size(); j++)
						{
							if (module_list[3]->m_Component[j]->m_objname == "sfkzq")
							{
								ui.textcontent->setText(module_list[3]->m_Component[j]->textContent);
								ui.vicontent->setStyleSheet("border-image:url(" + module_list[3]->m_Component[j]->imglist[0] + ");");
								ui.dialogtitle->setText(module_list[3]->m_Component[j]->m_Compname);
							}
						}
						for (int i = 0; i < module_list.size(); i++)
						{
							if (module_list[i]->id = cur_module)
							{
								for (int j = 0; j < module_list[i]->m_Component.size(); j++)
								{
									if (module_list[i]->m_Component[j]->m_objname == curobjectname)
									{
										uploaded_img = module_list[i]->m_Component[j]->imglist;
										uploaded_video = module_list[i]->m_Component[j]->videolist;
										uploaded_file = module_list[i]->m_Component[j]->filelist;

									}

								}
							}

						}
					}
			
					ui.dialogtitle->setText(btnlist4[i]->text());
					ui.dialog->show();
					ui.dialog->raise();

					});
			
		}
}
// 已编辑
void twoDdisplay::dataload()
{
	QString imgpath = exepath + "/image/displayimg/";
	QString filepath = exepath + "/files/";
	QString videopath = exepath + "/video/";
	Module* mod1 = new Module();
	mod1->id = 1;
	mod1->m_moduleName = "全箭模型";
	mod1->m_thumbnail = ":/twoDdisplay/select";
	Component* comp1_1 = new Component();
	comp1_1->m_objname = "smjzk";
	comp1_1->m_Compname = "上面级资控";
	comp1_1->textContent = smjzktext;
	comp1_1->imglist.append(imgpath + "smjzk-x.png");
	comp1_1->imglist.append(imgpath + "smjzk-d.jpg");
	Component* comp1_2 = new Component();
	comp1_2->m_objname = "rwzh";
	comp1_2->m_Compname = "任务载荷";
	comp1_2->textContent = rwzhtext;
	comp1_2->imglist.append(imgpath + "rwzh-x.png");
	comp1_2->imglist.append(imgpath + "rwzh-d.jpeg");
	Component* comp1_3 = new Component();
	comp1_3->m_objname = "yctx";
	comp1_3->m_Compname = "遥测天线";
	comp1_3->textContent = yctxtext;
	comp1_3->imglist.append(imgpath + "yctx-x.png");
	comp1_3->imglist.append(imgpath + "yctx-d.jpg");
	Component* comp1_4 = new Component();
	comp1_4->m_objname = "kzc";
	comp1_4->m_Compname = "控制舱";
	comp1_4->textContent = kzctext;
	comp1_4->imglist.append(imgpath + "kzc-x.png");
	comp1_4->imglist.append(imgpath + "kzc-d.png");
	Component* comp1_5 = new Component();
	comp1_5->m_objname = "smjgk";
	comp1_5->m_Compname = "上面级轨控";
	comp1_5->textContent = smjgktext;
	comp1_5->imglist.append(imgpath + "smjgk-x.png");
	comp1_5->imglist.append(imgpath + "smjgk-d.png");
	Component* comp1_6 = new Component();
	comp1_6->m_objname = "sjfdj";
	comp1_6->m_Compname = "三级发动机";
	comp1_6->textContent = sjfdjtext;
	comp1_6->imglist.append(imgpath + "sjfdj-x.png");
	comp1_6->imglist.append(imgpath + "sjfdj-d.png");
	Component* comp1_7 = new Component();
	comp1_7->m_objname = "esjjj";
	comp1_7->m_Compname = "二三级级间";
	comp1_7->textContent = esjjjtext;
	comp1_7->imglist.append(imgpath + "esjjj-x.png");
	comp1_7->imglist.append(imgpath + "esjjj-d.png");
	Component* comp1_8 = new Component();
	comp1_8->m_objname = "ejfdj";
	comp1_8->m_Compname = "二级发动机";
	comp1_8->textContent = ejfdjtext;
	comp1_8->imglist.append(imgpath + "ejfdj-x.png");
	comp1_8->imglist.append(imgpath + "ejfdj-d.png");
	Component* comp1_9 = new Component();
	comp1_9->m_objname = "yejjj";
	comp1_9->m_Compname = "一二级级间";
	comp1_9->textContent = yejjjtext;
	comp1_9->imglist.append(imgpath + "yejjj-x.png");
	comp1_9->imglist.append(imgpath + "yejjj-d.png");
	Component* comp1_10 = new Component();
	comp1_10->m_objname = "yjfdj";
	comp1_10->m_Compname = "一级发动机";
	comp1_10->textContent = yjfdjtext;
	comp1_10->imglist.append(imgpath + "yjfdj-x.png");
	comp1_10->imglist.append(imgpath + "yjfdj-d.png");
	comp1_10->videolist.append(videopath + "Level1Engine.mp4");
	comp1_10->filelist.append(filepath + "yjfdj.txt");
	Component* comp1_11 = new Component();
	comp1_11->m_objname = "yjwd";
	comp1_11->m_Compname = "一级尾段";
	comp1_11->textContent = yjwdtext;
	comp1_11->imglist.append(imgpath + "yjwd-x.png");
	comp1_11->imglist.append(imgpath + "yjwd-d.png");

	mod1->m_Component.append(comp1_1);
	mod1->m_Component.append(comp1_2);
	mod1->m_Component.append(comp1_3);
	mod1->m_Component.append(comp1_4);
	mod1->m_Component.append(comp1_5);
	mod1->m_Component.append(comp1_6);
	mod1->m_Component.append(comp1_7);
	mod1->m_Component.append(comp1_8);
	mod1->m_Component.append(comp1_9);
	mod1->m_Component.append(comp1_10);
	mod1->m_Component.append(comp1_11);


	Module* mod2 = new Module();
	mod2->id = 2;
	mod2->m_moduleName = "上面级模型";
	mod2->m_thumbnail = ":/twoDdisplay/smj";
	Component* comp2_1 = new Component();
	comp2_1->m_objname = "zlz";
	comp2_1->m_Compname = "整流罩";
	comp2_1->textContent = zlztext;
	comp2_1->imglist.append(imgpath + "zlz-x.png");
	comp2_1->imglist.append(imgpath + "zlz-d.png");
	Component* comp2_2 = new Component();
	comp2_2->m_objname = "zkfdj";
	comp2_2->m_Compname = "姿控发动机";
	comp2_2->textContent = smjzktext;
	comp2_2->imglist.append(imgpath + "smjzk-x.png");
	comp2_2->imglist.append(imgpath + "smjzk-d.jpg");
	Component* comp2_3 = new Component();
	comp2_3->m_objname = "rwzh";
	comp2_3->m_Compname = "任务载荷";
	comp2_3->textContent = rwzhtext;
	comp2_3->imglist.append(imgpath + "rwzh-x.png");
	comp2_3->imglist.append(imgpath + "rwzh-d.jpeg");
	Component* comp2_4 = new Component();
	comp2_4->m_objname = "smjgk";
	comp2_4->m_Compname = "轨控发动机";
	comp2_4->textContent = smjgktext;
	comp2_4->imglist.append(imgpath + "smjgk-x.png");
	comp2_4->imglist.append(imgpath + "smjgk-d.png");

	mod2->m_Component.append(comp2_1);
	mod2->m_Component.append(comp2_2);
	mod2->m_Component.append(comp2_3);
	mod2->m_Component.append(comp2_4);

	Module* mod3 = new Module();
	mod3->id = 3;
	mod3->m_moduleName = "控制舱模型";
	mod3->m_thumbnail = ":/twoDdisplay/kzc";
	Component* comp3_1 = new Component();
	comp3_1->m_objname = "dzh";
	comp3_1->m_Compname = "电阻盒";
	comp3_1->textContent = dzhtext;
	comp3_1->imglist.append(imgpath + "dzh-x.png");
	comp3_1->imglist.append(imgpath + "dzh-d.jpg");
	Component* comp3_2 = new Component();
	comp3_2->m_objname = "zkj";
	comp3_2->m_Compname = "综控机";
	comp3_2->textContent = zkjtext;
	comp3_2->imglist.append(imgpath + "zkj-x.png");
	comp3_2->imglist.append(imgpath + "zkj-d1.jpg");
	comp3_2->imglist.append(imgpath + "zkj-d2.png");
	Component* comp3_3 = new Component();
	comp3_3->m_objname = "spqd";
	comp3_3->m_Compname = "射频前端";
	comp3_3->textContent = spqdtext;
	comp3_3->imglist.append(imgpath + "spqd-x.png");
	comp3_3->imglist.append(imgpath + "spqd-d.jpg");
	Component* comp3_4 = new Component();
	comp3_4->m_objname = "zdc";
	comp3_4->m_Compname = "主电池";
	comp3_4->textContent = zdctext;
	comp3_4->imglist.append(imgpath + "zdc-x.png");
	comp3_4->imglist.append(imgpath + "zdc-d.png");
	Component* comp3_5 = new Component();
	comp3_5->m_objname = "gz";
	comp3_5->m_Compname = "惯组";
	comp3_5->textContent = gztext;
	comp3_5->imglist.append(imgpath + "gz-x.png");
	comp3_5->imglist.append(imgpath + "gz-d.jpg");
	Component* comp3_6 = new Component();
	comp3_6->m_objname = "mcxsydj";
	comp3_6->m_Compname = "脉冲相叁应答机";
	comp3_6->textContent = mcxsydjtext;
	comp3_6->imglist.append(imgpath + "mcxsydj-x.png");
	comp3_6->imglist.append(imgpath + "mcxsydj-d.jpg");
	Component* comp3_7 = new Component();
	comp3_7->m_objname = "aqzljsj";
	comp3_7->m_Compname = "安全指令接收机";
	comp3_7->textContent = aqzljsjtext;
	comp3_7->imglist.append(imgpath + "aqzljsj-x.png");
	comp3_7->imglist.append(imgpath + "aqzljsj-d.png");

	mod3->m_Component.append(comp3_1);
	mod3->m_Component.append(comp3_2);
	mod3->m_Component.append(comp3_3);
	mod3->m_Component.append(comp3_4);
	mod3->m_Component.append(comp3_5);
	mod3->m_Component.append(comp3_6);
	mod3->m_Component.append(comp3_7);


	Module* mod4 = new Module();
	mod4->id = 4;
	Component* comp4_1 = new Component();
	comp4_1->m_objname = "sfzdq";
	comp4_1->m_Compname = "伺服作动器";
	comp4_1->textContent = sfzdqtext;
	comp4_1->imglist.append(imgpath + "sfzdq-x.png");
	comp4_1->imglist.append(imgpath + "sfzdq-d1.jpg");
	comp4_1->imglist.append(imgpath + "sfzdq-d2.jpg");
	Component* comp4_2 = new Component();
	comp4_2->m_objname = "sfkzq";
	comp4_2->m_Compname = "伺服控制器";
	comp4_2->textContent = sfkzqtext;
	comp4_2->imglist.append(imgpath + "sfkzq-x.png");
	comp4_2->imglist.append(imgpath + "sfkzq-d1.jpg");
	comp4_2->imglist.append(imgpath + "sfkzq-d2.jpg");
	Component* comp4_3 = new Component();
	comp4_3->m_objname = "kqd";
	comp4_3->m_Compname = "空气舵";
	comp4_3->textContent = kqdtext;
	comp4_3->imglist.append(imgpath + "kqd-x.png");
	comp4_3->imglist.append(imgpath + "kqd-d.jpg");
	Component* comp4_4 = new Component();
	comp4_4->m_objname = "sfrdc";
	comp4_4->m_Compname = "伺服热电池";
	comp4_4->textContent = sfrdctext;
	comp4_4->imglist.append(imgpath + "sfrdc-x.png");
	comp4_4->imglist.append(imgpath + "sfrdc-d.jpg");
	Component* comp4_5 = new Component();
	comp4_5->m_objname = "rqd";
	comp4_5->m_Compname = "燃气舵";
	comp4_5->textContent = rqdtext;
	comp4_5->imglist.append(imgpath + "rqd-x.png");
	comp4_5->imglist.append(imgpath + "rqd-d1.jpg");
	comp4_5->imglist.append(imgpath + "rqd-d2.jpg");

	mod4->m_Component.append(comp4_1);
	mod4->m_Component.append(comp4_2);
	mod4->m_Component.append(comp4_3);
	mod4->m_Component.append(comp4_4);
	mod4->m_Component.append(comp4_5);




	module_list.append(mod1);
	module_list.append(mod2);
	module_list.append(mod3);
	module_list.append(mod4);

}
twoDdisplay::~twoDdisplay()
{}







