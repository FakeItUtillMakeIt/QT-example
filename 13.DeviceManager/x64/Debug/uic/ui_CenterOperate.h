/********************************************************************************
** Form generated from reading UI file 'CenterOperate.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CENTEROPERATE_H
#define UI_CENTEROPERATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CenterOperate
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QListWidget *listWidget;

    void setupUi(QWidget *CenterOperate)
    {
        if (CenterOperate->objectName().isEmpty())
            CenterOperate->setObjectName(QStringLiteral("CenterOperate"));
        CenterOperate->resize(1023, 654);
        CenterOperate->setStyleSheet(QStringLiteral("background-color: rgb(250, 250, 250);"));
        gridLayout = new QGridLayout(CenterOperate);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new QWidget(CenterOperate);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));

        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);


        gridLayout->addWidget(widget, 0, 1, 1, 1);

        listWidget = new QListWidget(CenterOperate);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setMaximumSize(QSize(200, 16777215));

        gridLayout->addWidget(listWidget, 0, 0, 1, 1);


        retranslateUi(CenterOperate);

        QMetaObject::connectSlotsByName(CenterOperate);
    } // setupUi

    void retranslateUi(QWidget *CenterOperate)
    {
        CenterOperate->setWindowTitle(QApplication::translate("CenterOperate", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CenterOperate: public Ui_CenterOperate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CENTEROPERATE_H
