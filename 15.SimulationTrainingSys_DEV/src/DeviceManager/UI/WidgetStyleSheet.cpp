#include "WidgetStyleSheet.h"

WidgetStyleSheet* WidgetStyleSheet::instance = nullptr;

void WidgetStyleSheet::styleSheetInit() {
	//ºÍµ×²¿×´Ì¬À¸ÑÕÉ«±£³ÖÒ»ÖÂ
	backGround = "\
		%1{ background-color:rgb(0,250,250); \
		";

	widgetStyleSheet = "\
		%1{ background-color:rgb(85,170,255); \
			width:150px; \
			height:50px; \
			border-width:4px; \
			border-color:rgb(30,144,255);\
			border-radius:2px; \
			font:bold 14px; \
			padding:10px; }\
		";

	listStyleSheet = "\
		%1{ background-color:rgb(240,240,240); \
			width:150px; \
			height:50px; \
			border-width:4px; \
			border-color:rgb(100,100,100);\
			border-radius:2px; \
			font:bold 14px; \
			padding:10px; }\
		";

	buttonStyleSheet = "\
		%1:pressed{ background-color:rgb(240,255,255);\
			color:black; \
			border-style:inset; }\
		%1:flat{ background-color:rgb(240,255,255);\
			color:black;\
			border:none;}\
		%1:release{ background-color:rgb(85,170,255); \
			border-style:outset; \
			color:black}\
		";

	coverStyleSheet = "\
		%1:hover{ background-color:rgb(85,170,255); \
			border-style:inset; }\
		%1:itemClicked{ background-color:green; \
			border-style:inset; }\
		%1:release{ background-color:rgb(85,170,255); \
			border-style:outset; }\
		";

	itemStyleSheet = "\
		%1::item{ background-color:rgb(80,170,255);\
			icon:':icon/icon/squarebl.png';} \
		%1::item:hover{ background-color:rgb(240,240,240);\
			padding-top:-2px;padding-bottom:-1px;} \
			icon:':icon/icon/squareWl.png';\
		%1::item:selected{ background:rgb(240,255,255);\
			color:black;\
			padding-top:-2px;padding-bottom:-1px;padding-right:20px;} \
			icon:':icon/icon/squareWl.png';\
		";

	listItemStyleSheet = "\
		%1::item{ background-color:rgb(240,240,240);\
			} \
		%1::item:hover{ background-color:rgb(80,170,255);\
			padding-top:-2px;padding-bottom:-1px;} \
		%1::item:selected{ background:rgb(80,170,255);\
			color:black;\
			padding-top:-2px;padding-bottom:-1px;padding-right:20px;} \
		";

	lineEditStyleSheet = "\
		%1{ background-color:rgb(235,235,235); \
			width:150px; \
			height:20px; \
			border-width:1px; \
			border-color:rgb(100,100,100);\
			border-radius:10px; \
			font:bold 12px Î¢ÈíÑÅºÚ; \
			padding:5px; }\
		";

	comboxStyleSheet = "\
		%1{ background-color:rgb(235,235,235); \
			width:150px; \
			height:20px; \
			border-width:1px; \
			border-color:rgb(100,100,100);\
			border-radius:10px; \
			font:bold 12px Î¢ÈíÑÅºÚ; \
			padding:10px; }\
		";

	labelStyleSheet = "\
		%1{ background-color:rgb(250,250,250); \
			width:150px; \
			height:20px; \
			border-width:1px; \
			border-color:rgb(100,100,100);\
			font: 12px Î¢ÈíÑÅºÚ; \
			padding:10px; }\
		";

	iconButtonStyleSheet = "\
		%1{ background-color:rgb(250,250,250); \
			width:12px; \
			height:12px; \
			border-width:0px; \
			border-radius:5px;\
			border-color:rgb(240,100,100);\
			padding:10px; \
			font: 12px Î¢ÈíÑÅºÚ;}\
		";
	insertButtonStyleSheet = "\
		%1{ color:white;background-color:rgb(24, 144, 255); \
			width:36px; \
			height:12px; \
			border-width:0px; \
			border-radius:5px;\
			font: 12px Î¢ÈíÑÅºÚ; \
			padding:10px; }\
		";
	deleteButtonStyleSheet = "\
		%1{ color:red;background-color:rgb(245,245,245);\
			width:36px; \
			height:12px; \
			border-width:0px; \
			border-radius:5px;\
			font:12px bold; \
			padding:10px; }\
		";

	listItemStyleSheet = "\
		%1::item{ background-color:rgb(240,240,240);\
			} \
		%1::item:hover{ background-color:rgb(80,170,255);\
			padding-top:-2px;padding-bottom:-1px;} \
		%1::item:selected{ background:rgb(80,170,255);\
			color:black;\
			padding-top:-2px;padding-bottom:-1px;padding-right:20px;} \
		";

	deviceManageListSS = QString("\
				QListWidget{background-color:rgba(249,249,249,1);border:0;margin:10px 4px 4px 10px;}\
				QListWidget::item{ border-radius:3px;image:url(%1);min-height:36px;image-position: left ;font: 14px Î¢ÈíÑÅºÚ;background-color:rgba(249,249,249,1);color:black;margin:10px 4px 4px 0px;}\
				QListWidget::item:hover{ image:url(%2);min-height:36px;image-position: left;background-color:rgba(73,136,250,1);color:white;padding-top:-2px;padding-bottom:-1px;} \
				QListWidget::item:selected{ image:url(%2);min-height:36px;image-position: left;background-color:rgba(73,136,250,1);color:white;;padding-top:-2px;padding-bottom:-1px;padding-right:20px;} \
		");

	lineEditStyleSheet = "\
		%1{ background-color:gainsboro; \
			width:150px; \
			height:20px; \
			border-width:1px; \
			border-color:aliceblue;\
			border-radius:5px; \
			font:bold 12px Î¢ÈíÑÅºÚ; \
			padding:6px; }\
		";

	comboxStyleSheet = "\
		%1{ background-color:rgb(235,235,235); \
			width:150px; \
			height:20px; \
			border-width:1px; \
			border-color:rgb(100,100,100);\
			border-radius:10px; \
			font:bold 12px Î¢ÈíÑÅºÚ; \
			padding:10px; }\
		";

	labelStyleSheet = "\
		%1{ background-color:rgb(250,250,250); \
			width:150px; \
			height:20px; \
			border-width:1px; \
			border-color:rgb(100,100,100);\
			font:bold 12px Î¢ÈíÑÅºÚ; \
			padding:10px; }\
		";

	iconButtonStyleSheet = "\
		%1{ background-color:rgb(250,250,250); \
			width:12px; \
			height:12px; \
			border-width:0px; \
			border-radius:5px;\
			border-color:rgb(240,100,100);\
			padding:10px; }\
		";
	insertButtonStyleSheet = "\
		%1{ background-color:rgb(24, 144, 255); \
			width:36px; \
			height:12px; \
			border-width:0px; \
			border-radius:5px;\
			font: 12px Î¢ÈíÑÅºÚ; \
			padding:10px; }\
		";
	deleteButtonStyleSheet = "\
		%1{ background-color:rgb(245,245,245); \
			width:36px; \
			height:12px; \
			border-width:0px; \
			border-radius:5px;\
			font: 12px Î¢ÈíÑÅºÚ; \
			padding:10px; }\
		";

	infoConfigLabelStyleSheet = "\
		%1{ background-color:rgb(255,255,255); \
			color:black;\
			width:60px; \
			height:30px; \
			font: 12px Î¢ÈíÑÅºÚ;}\
		";
	infoConfigLineEditStyleSheet = "\
		%1{ background-color:rgb(255,255,255); \
			width:160px; \
			height:30px; \
			color:black;\
			font: 12px Î¢ÈíÑÅºÚ;\
			border-style:solid;\
			border-width:1px; \
			border-color:gray;\
			border-radius:4px;}\
		";
	infoConfigPushButtonStyleSheet = "\
		%1{background-color:lightgray;\
			width:80px; \
			height:30px; \
			color:black;\
			font: 12px Î¢ÈíÑÅºÚ;\
			border-style:solid;\
			border-width:1px; \
			border-color:lightgray;\
			border-radius:4px; }\
		%1:pressed{ background-color:dodgerblue;\
			color:black; \
			border-style:inset; }\
		%1:flat{ background-color:dodgerblue;\
			color:black;\
			border:none;}\
		%1:release{ background-color:rgb(85,170,255); \
			border-style:outset; \
			color:black}\
		";


	infoConfigComboBoxStyleSheet = "\
		%1{\
		\
		width:150px;\
		height:25px;\
		;}\
		";;

	infoConfigFrameStyleSheet = "\
		%1{backgroud-color:gray;\
		border-color:gray;}\
		";
}