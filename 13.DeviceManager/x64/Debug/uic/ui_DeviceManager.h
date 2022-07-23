/********************************************************************************
** Form generated from reading UI file 'DeviceManager.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICEMANAGER_H
#define UI_DEVICEMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DeviceManagerClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *vl_UI;
    QWidget *top_wgt;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QLabel *title;
    QSpacerItem *horizontalSpacer;
    QWidget *right_wgt;
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_4;
    QGridLayout *gridLayout;
    QPushButton *pb_min;
    QPushButton *pb_close;
    QWidget *widget_5;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pb_min_1;
    QLabel *lb_user;
    QWidget *center_wgt;
    QWidget *top_wgt_2;
    QWidget *wgt_status_left;
    QLabel *lb_flag_5;

    void setupUi(QMainWindow *DeviceManagerClass)
    {
        if (DeviceManagerClass->objectName().isEmpty())
            DeviceManagerClass->setObjectName(QStringLiteral("DeviceManagerClass"));
        DeviceManagerClass->resize(1024, 771);
        centralWidget = new QWidget(DeviceManagerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        vl_UI = new QVBoxLayout(centralWidget);
        vl_UI->setSpacing(0);
        vl_UI->setContentsMargins(11, 11, 11, 11);
        vl_UI->setObjectName(QStringLiteral("vl_UI"));
        vl_UI->setContentsMargins(5, 5, 5, 5);
        top_wgt = new QWidget(centralWidget);
        top_wgt->setObjectName(QStringLiteral("top_wgt"));
        top_wgt->setMinimumSize(QSize(1014, 120));
        top_wgt->setMaximumSize(QSize(16777215, 120));
        top_wgt->setStyleSheet(QStringLiteral("border-image:url(\":/DeviceManager/topbar\")"));
        horizontalLayout = new QHBoxLayout(top_wgt);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(top_wgt);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(600, 0));
        widget->setAutoFillBackground(false);
        widget->setStyleSheet(QStringLiteral("border-image:\"transpant\""));
        title = new QLabel(widget);
        title->setObjectName(QStringLiteral("title"));
        title->setGeometry(QRect(200, 40, 291, 41));
        title->setStyleSheet(QStringLiteral("border-image:\"transpant\";font-size:36px;color:white"));
        title->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(widget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        right_wgt = new QWidget(top_wgt);
        right_wgt->setObjectName(QStringLiteral("right_wgt"));
        right_wgt->setMinimumSize(QSize(120, 80));
        right_wgt->setMaximumSize(QSize(16777215, 16777215));
        right_wgt->setStyleSheet(QStringLiteral("border-image:\"transpant\""));
        verticalLayout = new QVBoxLayout(right_wgt);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(right_wgt);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        gridLayout = new QGridLayout(widget_4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pb_min = new QPushButton(widget_4);
        pb_min->setObjectName(QStringLiteral("pb_min"));
        pb_min->setMinimumSize(QSize(30, 30));
        pb_min->setMaximumSize(QSize(30, 30));
        pb_min->setCursor(QCursor(Qt::PointingHandCursor));
        pb_min->setStyleSheet(QString::fromUtf8("QPushButton:hover{\n"
"	background-color:transparent;\n"
"	background-image:url(:/DeviceManager/images/\346\234\200\345\260\217\345\214\226-\346\202\254\346\265\256.png);\n"
"    border:0px;}\n"
"QPushButton:pressed{\n"
"	background-color:transparent; \n"
"    background-image: url(:/DeviceManager/images/\346\234\200\345\260\217\345\214\226-\347\202\271\345\207\273.png);\n"
"    border:0px;}\n"
"QPushButton {\n"
"	background-color:transparent;\n"
"	background-image: url(:/DeviceManager/images/\346\234\200\345\260\217\345\214\226-\351\273\230\350\256\244.png);\n"
"    border:0px;\n"
"}\n"
"\n"
""));

        gridLayout->addWidget(pb_min, 0, 2, 1, 1);

        pb_close = new QPushButton(widget_4);
        pb_close->setObjectName(QStringLiteral("pb_close"));
        pb_close->setMinimumSize(QSize(30, 30));
        pb_close->setMaximumSize(QSize(30, 30));
        pb_close->setCursor(QCursor(Qt::PointingHandCursor));
        pb_close->setStyleSheet(QString::fromUtf8("QPushButton:hover{\n"
"	background-color:transparent;\n"
"	background-image: url(:/DeviceManager/images/\345\205\263\351\227\255-\346\202\254\346\265\256.png);\n"
"    border:0px;}\n"
"QPushButton:pressed{\n"
"	background-color:transparent; \n"
"	background-image: url(:/DeviceManager/images/\345\205\263\351\227\255-\347\202\271\345\207\273.png);\n"
"    border:0px;}\n"
"QPushButton {\n"
"	background-color:transparent;\n"
"	background-image: url(:/DeviceManager/images/\345\205\263\351\227\255-\351\273\230\350\256\244.png);\n"
"    border:0px;\n"
"}\n"
"\n"
""));

        gridLayout->addWidget(pb_close, 0, 3, 1, 1);

        widget_5 = new QWidget(widget_4);
        widget_5->setObjectName(QStringLiteral("widget_5"));

        gridLayout->addWidget(widget_5, 0, 0, 1, 1);


        verticalLayout_2->addWidget(widget_4);

        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pb_min_1 = new QPushButton(widget_3);
        pb_min_1->setObjectName(QStringLiteral("pb_min_1"));
        pb_min_1->setMinimumSize(QSize(30, 30));
        pb_min_1->setMaximumSize(QSize(30, 30));
        pb_min_1->setCursor(QCursor(Qt::PointingHandCursor));
        pb_min_1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color:transparent;\n"
"	background-image: url(:/DeviceManager/images/\347\224\250\346\210\2671.png);\n"
"    border:0px;\n"
"}\n"
"\n"
""));

        horizontalLayout_2->addWidget(pb_min_1);

        lb_user = new QLabel(widget_3);
        lb_user->setObjectName(QStringLiteral("lb_user"));
        lb_user->setStyleSheet(QLatin1String("color:white;font-size:14px;font-family:\"Microsoft Yahei\"\n"
""));
        lb_user->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(lb_user);


        verticalLayout_2->addWidget(widget_3);


        verticalLayout->addWidget(widget_2);


        horizontalLayout->addWidget(right_wgt);


        vl_UI->addWidget(top_wgt);

        center_wgt = new QWidget(centralWidget);
        center_wgt->setObjectName(QStringLiteral("center_wgt"));
        center_wgt->setStyleSheet(QStringLiteral("background-color: rgb(170, 170, 255);"));

        vl_UI->addWidget(center_wgt);

        top_wgt_2 = new QWidget(centralWidget);
        top_wgt_2->setObjectName(QStringLiteral("top_wgt_2"));
        top_wgt_2->setMinimumSize(QSize(1014, 31));
        top_wgt_2->setMaximumSize(QSize(16777215, 31));
        top_wgt_2->setStyleSheet(QLatin1String(" \n"
"background-color: rgb(85, 170, 255);"));
        wgt_status_left = new QWidget(top_wgt_2);
        wgt_status_left->setObjectName(QStringLiteral("wgt_status_left"));
        wgt_status_left->setGeometry(QRect(0, 0, 790, 31));
        wgt_status_left->setMinimumSize(QSize(790, 24));
        lb_flag_5 = new QLabel(wgt_status_left);
        lb_flag_5->setObjectName(QStringLiteral("lb_flag_5"));
        lb_flag_5->setGeometry(QRect(10, 4, 24, 24));
        lb_flag_5->setStyleSheet(QStringLiteral(""));
        lb_flag_5->setPixmap(QPixmap(QString::fromUtf8(":/DeviceManager/images/icon_info.png")));

        vl_UI->addWidget(top_wgt_2);

        DeviceManagerClass->setCentralWidget(centralWidget);

        retranslateUi(DeviceManagerClass);

        QMetaObject::connectSlotsByName(DeviceManagerClass);
    } // setupUi

    void retranslateUi(QMainWindow *DeviceManagerClass)
    {
        DeviceManagerClass->setWindowTitle(QApplication::translate("DeviceManagerClass", "DeviceManager", Q_NULLPTR));
        title->setText(QApplication::translate("DeviceManagerClass", "\350\256\276\345\244\207\347\256\241\347\220\206\350\275\257\344\273\266", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        pb_min->setToolTip(QApplication::translate("DeviceManagerClass", "\346\234\200\345\260\217\345\214\226", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pb_min->setText(QString());
#ifndef QT_NO_TOOLTIP
        pb_close->setToolTip(QApplication::translate("DeviceManagerClass", "\345\205\263\351\227\255", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pb_close->setText(QString());
#ifndef QT_NO_TOOLTIP
        pb_min_1->setToolTip(QApplication::translate("DeviceManagerClass", "\345\275\223\345\211\215\347\224\250\346\210\267", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pb_min_1->setText(QString());
        lb_user->setText(QApplication::translate("DeviceManagerClass", "admin", Q_NULLPTR));
        lb_flag_5->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DeviceManagerClass: public Ui_DeviceManagerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICEMANAGER_H
