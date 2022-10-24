#include "configdefine.h"
#include "QUuid"
#include <QDebug>
#include <qgridlayout.h>
#include <qlabel.h>
#include <QLineEdit>
#include <qpushbutton.h>
#include <qdialog.h>
#include <QDesktopWidget>
#include <qapplication.h>
#include <qspinbox.h>
ConfigNameSpaceStart


QString  get_uuid()
{
    QString uuid =  QUuid::createUuid().toString();
    qDebug() << "preuuid:" << uuid;
    uuid.remove('{').remove('}');
    return uuid;
}
QString getXmlGroupName(ControlType ctrtype)
{
    switch (ctrtype)
    {
    case cConfigButton:
        return "Buttons";
    case cConfigPairLabel:
        return "PairLabels";
    case cConfigGroup:
        return "Groups";
    case cConfigCurve:
        return "Curves";
    default:
        return "null";
    }
}
ConfigValue::ConfigValue(void *ivalue, ConfigValueType ivaluetype, QString ivaluename, int iuplimit, int idownlimit)
{
    valueid = get_uuid();
    value  = ivalue;
    valuetype = ivaluetype;
    valuename    = ivaluename;
    uplimit = iuplimit;
    downlimit = idownlimit;
}

QString ConfigValue::getStrValue()
{
    QString result;
    switch (valuetype) {
        case eConfigInt:
        {
         result  = QString::number(*(int*)value);
        }
       break;
    case eConfigDataSource:
    case eConfigCommand:
    case eConfigString:
    case eConfigStyleInfo:
        {
        result  = QString::fromLocal8Bit((char*)value);

        }
        break;
    case eConfigMultiDataSource:
       {
        result =  ((QList<QString>*)value)->join(',');
       }
    break;
    default:
        break;
    }
    return result;

}

void ConfigValue::SetWidthStrValue(QString strvalue)
{
    if(strvalue == "")
        return;
    char tmpSrc[MaximumPathLength];
    char* tmpAim = nullptr;
    int  tmpint,tmplen;
    QStringList  strlist;
    switch (valuetype) {
        case eConfigInt:
        {
         (*(int*)value)  = strvalue.toInt();
        }
       break;
        case eConfigDataSource:
        case eConfigCommand:
        case eConfigStyleInfo:
        case eConfigString:
        {
            tmpAim  = (char*)value;
            memset(tmpAim,0,MaximumPathLength);
            memcpy(tmpAim,strvalue.toLocal8Bit().data(),strlen(strvalue.toLocal8Bit().data()));

        }
        break;
        case eConfigMultiDataSource:
        {
             strlist =  strvalue.split(",");
             for(auto str: strlist)
                ((QList<QString>*)value)->append(str);
        }
        break;
    
    default:
        break;
    }
}


bool  ConfigTools::GetInputDouble(QString  hint,double & value)
{
    QDialog  dlg;
    bool  bret  = false;
    dlg.setWindowTitle(hint);
    dlg.setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    dlg.setWindowIcon(QIcon(":/EquivalentSoft/images/任务模拟64_64.ico"));
    QPushButton  okbtn("确认");
    QPushButton  cancelbtn("取消");
    connect(&okbtn, &QPushButton::clicked, [&dlg]() { dlg.accept(); });
    connect(&cancelbtn, &QPushButton::clicked, [&dlg]() { dlg.reject(); });
    QVBoxLayout  vlayout;
    QHBoxLayout  hlayout;
    QDoubleSpinBox  spinbox;
    spinbox.setRange(-1000000,1000000);
    spinbox.setValue(value);
    vlayout.addWidget(&spinbox);
    hlayout.addWidget(&okbtn);
    hlayout.addWidget(&cancelbtn);
    vlayout.addLayout(&hlayout);
    dlg.setLayout(&vlayout);
    QDesktopWidget* desktop = QApplication::desktop();
    int  iret = dlg.exec();
    dlg.move((desktop->width() - dlg.width()) / 2, (desktop->height() - dlg.height()) / 2);
    if (iret == QDialog::Rejected)
        return false;
    value = spinbox.value();
    return  true;
}
bool  ConfigTools::GetInputString(QString  hint, QString& value)
{
    QDialog  dlg;
    bool  bret = false;
    dlg.setWindowTitle(hint);
    dlg.setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    dlg.setWindowIcon(QIcon(":/EquivalentSoft/images/任务模拟64_64.ico"));
    QPushButton  okbtn("确认");
    QPushButton  cancelbtn("取消");
    connect(&okbtn, &QPushButton::clicked, [&dlg]() { dlg.accept(); });
    connect(&cancelbtn, &QPushButton::clicked, [&dlg]() { dlg.reject(); });
    QVBoxLayout  vlayout;
    QHBoxLayout  hlayout;
    QLineEdit  lineEdit;
    lineEdit.setText(value);
    vlayout.addWidget(&lineEdit);
    hlayout.addWidget(&okbtn);
    hlayout.addWidget(&cancelbtn);
    vlayout.addLayout(&hlayout);
    dlg.setLayout(&vlayout);
    QDesktopWidget* desktop = QApplication::desktop();
    int  iret = dlg.exec();
    dlg.move((desktop->width() - dlg.width()) / 2, (desktop->height() - dlg.height()) / 2);
    if (iret == QDialog::Rejected)
        return false;
    value = lineEdit.text();
    return  true;
}
bool  ConfigTools::GetInputInt(QString  hint, int& value)
{
    QDialog  dlg;
    bool  bret = false;
    dlg.setWindowTitle(hint);
    dlg.setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    dlg.setWindowIcon(QIcon(":/EquivalentSoft/images/任务模拟64_64.ico"));
    QPushButton  okbtn("确认");
    QPushButton  cancelbtn("取消");
    connect(&okbtn, &QPushButton::clicked, [&dlg]() { dlg.accept(); });
    connect(&cancelbtn, &QPushButton::clicked, [&dlg]() { dlg.reject(); });
    QVBoxLayout  vlayout;
    QHBoxLayout  hlayout;
    QSpinBox  spinbox;
    spinbox.setValue(value);
    vlayout.addWidget(&spinbox);
    hlayout.addWidget(&okbtn);
    hlayout.addWidget(&cancelbtn);
    vlayout.addLayout(&hlayout);
    dlg.setLayout(&vlayout);
    QDesktopWidget* desktop = QApplication::desktop();
    int  iret = dlg.exec();
    dlg.move((desktop->width() - dlg.width()) / 2, (desktop->height() - dlg.height()) / 2);
    if (iret == QDialog::Rejected)
        return false;
    value = spinbox.value();
    return  true;
}

ConfigNameSpaceEnd
