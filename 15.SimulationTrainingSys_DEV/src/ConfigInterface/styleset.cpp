#include "booldialog.h"
#include "styleset.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QFontDialog>
#include <QColorDialog>
#include "StyleGroup.h"
#include "stylebutton.h"
#include "stylelabel.h"
#include "stylecurve.h"
#include "stylepairlabel.h"
#include "configglobal.h"

ConfigNameSpaceStart
StyleSet::StyleSet(QWidget* parent):QWidget(parent)
{
    initUi();
}

void StyleSet::initUi()
{
    stackwidget = new QStackedWidget;
    QHBoxLayout* hlayout = new QHBoxLayout;
    hlayout->addWidget(stackwidget);
    labelSetEntry  = new QWidget();
    buttonSetEntry = new QWidget();
    grouoSetEntry = new QWidget();
    curveSetEntry = new QWidget();
    pairLabelSetEntry = new QWidget();
    stackwidget->addWidget(labelSetEntry);
    stackwidget->addWidget(buttonSetEntry);
    stackwidget->addWidget(grouoSetEntry);
    stackwidget->addWidget(curveSetEntry);
    stackwidget->addWidget(pairLabelSetEntry);

    initlabelSetEntry();
    initbuttonSetEntry();
    initgroupSetEntry();
    initcurveSetEntry();
    initPairLabelSetEntry();
    setLayout(hlayout);
    initwidget = new QWidget;
    stackwidget->addWidget(initwidget);
    stackwidget->setCurrentWidget(initwidget);

   // stackwidget->setCurrentWidget(pairLabelSetEntry);

}
void StyleSet::initStackWidget()
{

    stackwidget->setCurrentWidget(initwidget);

}
void StyleSet::initbuttonSetEntry()
{
    QVBoxLayout* vlayout = new QVBoxLayout;
    addTextEdit("设置类型名称",vlayout,StyleButton::Button_Style_Name,cConfigButton);
    addIconSel("请选择背景图片",vlayout,StyleButton::Button_Common_Style,cConfigButton);
    addIconSel("请选择鼠标经过背景图片",vlayout,StyleButton::Button_Hoverd_Style,cConfigButton);
    addIconSel("请选择鼠标按下背景图片",vlayout,StyleButton::Button_Pressed_Style,cConfigButton);
    addIconSel("请选择命令成功背景图片",vlayout,StyleButton::Button_Success_Style,cConfigButton);
    addIconSel("请选择命令失败背景图片",vlayout,StyleButton::Button_Failed_Style,cConfigButton);
    addIconSel("请选择按钮禁止时背景图片",vlayout,StyleButton::Button_Diable_Style,cConfigButton);
    addFontSel("请选择字体",vlayout,StyleButton::Button_Font_Style,cConfigButton);
    addTextColorSel("请选择字体颜色",vlayout,StyleButton::Button_Text_Color,cConfigButton);

    vlayout->addStretch();
    buttonSetEntry->setLayout(vlayout);
}
void StyleSet::initlabelSetEntry()
{
    QVBoxLayout* vlayout = new QVBoxLayout;
    addTextEdit("设置类型名称",vlayout,StyleLabel::Label_Style_Name,cConfigLabel);
    addIconSel("请选择背景图片",vlayout,StyleLabel::Label_Common_Style,cConfigLabel);
    addFontSel("请选择字体",vlayout,StyleLabel::Label_Font_Style,cConfigLabel);
    addTextColorSel("请选择字体颜色",vlayout,StyleLabel::Label_Text_Color,cConfigLabel);
    vlayout->addStretch();
    labelSetEntry->setLayout(vlayout);
}
void StyleSet::initgroupSetEntry()
{
    QVBoxLayout* vlayout = new QVBoxLayout;
    addTextEdit("设置分组名称",vlayout,StyleGroup::Group_Style_Name,cConfigGroup);
    addIconSel("请选择背景图片",vlayout,StyleGroup::Group_Common_Style,cConfigGroup);
    addIconSel("标题背景图片",vlayout,StyleGroup::Group_Title_Style,cConfigGroup);
    addIconSel("标题LOGO图片",vlayout,StyleGroup::Group_Icon_Style,cConfigGroup);

    //addIconSel("标题文字背景图片",vlayout,StyleGroup::Group_Title_Text_Style,cConfigGroup);
    addFontSel("请选择标题字体",vlayout,StyleGroup::Group_Font_Style,cConfigGroup);
    addTextColorSel("请选择标题字体颜色",vlayout,StyleGroup::Group_Text_Color,cConfigGroup);
    vlayout->addStretch();
    grouoSetEntry->setLayout(vlayout);
}

void StyleSet::initcurveSetEntry()
{
    QVBoxLayout* vlayout = new QVBoxLayout;

    addTextEdit("设置类型名称",vlayout,StyleCurve::Curve_Style_Name,cConfigCurve);
    addTextColorSel("请选择背景颜色",vlayout,StyleCurve::Curve_Common_Color,cConfigCurve);
    addBoolSel("是否显示标题",vlayout,StyleCurve::Curve_Title_Show,cConfigCurve);
    addFontSel("标题文字字体",vlayout,StyleCurve::Curve_Title_Font,cConfigCurve);
    addTextColorSel("标题文字颜色",vlayout,StyleCurve::Curve_Title_Color,cConfigCurve);
    vlayout->addStretch();
    curveSetEntry->setLayout(vlayout);
}

void StyleSet::initPairLabelSetEntry()
{
    QVBoxLayout* vlayout = new QVBoxLayout;
    addTextEdit("设置类型名称",vlayout,StylePairLabel::PairLabel_Style_Name,cConfigPairLabel);
    addIconSel("设置参数名称背景图片",vlayout,StylePairLabel::PairLabel_Name_Style,cConfigPairLabel);
    addIconSel("设置参数值背景图片",vlayout,StylePairLabel::PairLabel_Value_Style,cConfigPairLabel);
    addIconSel("设置参数名称异常背景图片",vlayout,StylePairLabel::PairLabel_Error_Name_Style,cConfigPairLabel);
    addIconSel("设置参数值异常背景图片",vlayout,StylePairLabel::PairLabel_Error_Value_Style,cConfigPairLabel);
    addFontSel("设置文字字体",vlayout,StylePairLabel::PairLabel_Font_Style,cConfigPairLabel);

    addTextColorSel("请选择字体颜色",vlayout,StylePairLabel::PairLabel_Text_Color,cConfigPairLabel);
    addIntSel("设置名称宽度占比",vlayout,StylePairLabel::PairLabel_Name_Strech,cConfigPairLabel);
    addIntSel("设置数值宽度占比",vlayout,StylePairLabel::PairLabel_Value_Strech,cConfigPairLabel);
    addIntSel("设置名称数值间距",vlayout,StylePairLabel::PairLabel_Space,cConfigPairLabel);

    vlayout->addStretch();
    pairLabelSetEntry->setLayout(vlayout);
}


void StyleSet::update_property( ControlType ctrltyle, int enumvalue,QString result)
{
    if(ctrltyle == cConfigButton)
    {
         m_btnEx->m_infomap[enumvalue] = QPair<bool,QString>(true,result);
         m_btnEx->updateStyle();
    }
    else if(ctrltyle == cConfigLabel)
    {
        m_labelEx->m_infomap[enumvalue] = QPair<bool,QString>(true,result);
        m_labelEx->updateStyle();
    }
    else if(ctrltyle == cConfigGroup)
    {
        m_groupEx->m_infomap[enumvalue] = QPair<bool,QString>(true,result);
        m_groupEx->updateStyle();
    }
    else if(ctrltyle == cConfigCurve)
    {
        m_curveEx->m_infomap[enumvalue] = QPair<bool,QString>(true,result);
        m_curveEx->updateStyle();
        QList<ConfigCurve *> styleusers = m_curveEx->getStyleUsers();
        ConfigGlobal::updateCurveStyle(styleusers);
    }
    else if(ctrltyle == cConfigPairLabel)
    {
        m_pairlabelEx->m_infomap[enumvalue] = QPair<bool,QString>(true,result);
        m_pairlabelEx->updateStyle();
    }

}
void StyleSet::addTextEdit(QString title, QVBoxLayout* vlayout, int enumvalue,ControlType ctrltyle)
{
    QPushButton* inputbtn = new QPushButton(title);
    QLabel* textbrowser = new QLabel;
    textbrowser->setAlignment(Qt::AlignCenter);
    textbrowser->setMaximumHeight(50);
    connect(inputbtn,&QPushButton::clicked,[=](){
        QInputDialog inputdlg;
        inputdlg.setLabelText(title);
        inputbtn->setWindowIconText("获取输入");
        inputdlg.exec();
        QString text =  inputdlg.textValue();
        textbrowser->setText(text);
        update_property(ctrltyle,enumvalue,text);
    });
    vlayout->addWidget(inputbtn);
    vlayout->addWidget(textbrowser);
    m_tbmap[ctrltyle][enumvalue] = textbrowser;

}

void StyleSet::addIntSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle)
{
    QPushButton* inputbtn = new QPushButton(title);
    QLabel* textbrowser = new QLabel;
    textbrowser->setAlignment(Qt::AlignCenter);
    textbrowser->setMaximumHeight(50);
    connect(inputbtn,&QPushButton::clicked,[=](){
        QInputDialog inputdlg;
        inputdlg.setInputMode(QInputDialog::IntInput);
        inputdlg.setLabelText(title);
        inputbtn->setWindowIconText("获取输入");
        if(inputdlg.exec() == QInputDialog::Rejected)
            return;
        QString text =  QString::number(inputdlg.intValue());
        textbrowser->setText(text);
        update_property(ctrltyle,enumvalue,text);
    });
    vlayout->addWidget(inputbtn);
    vlayout->addWidget(textbrowser);
    m_tbmap[ctrltyle][enumvalue] = textbrowser;
}

void StyleSet::addIconSel(QString title, QVBoxLayout* vlayout, int enumvalue,ControlType ctrltyle)
{
    QPushButton* lpath = new QPushButton(title);
    QLabel* textbrowser = new QLabel;
    textbrowser->setAlignment(Qt::AlignHCenter);
    textbrowser->setMaximumHeight(50);
    connect(lpath,&QPushButton::clicked,[=](){

       QString path  =  QFileDialog::getOpenFileName(this,title);
       if(path.isNull())
       {
           textbrowser->setText("未选择");
           return;
       }
       textbrowser->setText(path);
       update_property(ctrltyle,enumvalue,path);
    });
    vlayout->addWidget(lpath);
    vlayout->addWidget(textbrowser);
    m_tbmap[ctrltyle][enumvalue] = textbrowser;
}
void StyleSet::addFontSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle)
{
    QPushButton* lpath = new QPushButton(title);
    QLabel* textbrowser = new QLabel;
    textbrowser->setAlignment(Qt::AlignHCenter);
    textbrowser->setMaximumHeight(50);
    connect(lpath,&QPushButton::clicked,[=](){

        bool ok;
        QFont font = QFontDialog::getFont(
                      &ok, QFont("Helvetica [Cronyx]", 10), this);
        if (!ok) {
            return;
        }
       QString path  =  font.toString();
       if(path.isNull())
       {
           textbrowser->setText("未选择");
           return;
       }
       textbrowser->setText(path);
       update_property(ctrltyle,enumvalue,path);
    });
    vlayout->addWidget(lpath);
    vlayout->addWidget(textbrowser);
    m_tbmap[ctrltyle][enumvalue] = textbrowser;
}

void StyleSet::addTextColorSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle)
{
    QPushButton* lpath = new QPushButton(title);
    QLabel* textbrowser = new QLabel;
    textbrowser->setAlignment(Qt::AlignHCenter);
    textbrowser->setMaximumHeight(50);
    connect(lpath,&QPushButton::clicked,[=](){

       QColorDialog  dialog;
       QColor color = dialog.getColor();
       if(color.isValid() == false)
           return;
       QString colorstr = color.name(QColor::QColor::HexArgb);
       textbrowser->setText(colorstr);
       update_property(ctrltyle,enumvalue,colorstr);

    });
    vlayout->addWidget(lpath);
    vlayout->addWidget(textbrowser);
    m_tbmap[ctrltyle][enumvalue] = textbrowser;

}

void StyleSet::addBoolSel(QString title, QVBoxLayout *vlayout, int enumvalue, ControlType ctrltyle)
{
    QPushButton* lpath = new QPushButton(title);
    QLabel* textbrowser = new QLabel;
    textbrowser->setAlignment(Qt::AlignHCenter);
    textbrowser->setMaximumHeight(50);
    connect(lpath,&QPushButton::clicked,[=](){
       BoolDialog  dlg;
       dlg.setWindowTitle("请选择是否显示");
       int result =  dlg.exec();
       QString resultstr = (result == QDialog::Accepted?"是":"否");
       textbrowser->setText(resultstr);
       update_property(ctrltyle,enumvalue,resultstr);

    });
    vlayout->addWidget(lpath);
    vlayout->addWidget(textbrowser);
    m_tbmap[ctrltyle][enumvalue] = textbrowser;
}



void StyleSet::SetElement(StyleLabel* labelEx)
{\
    m_labelEx = labelEx;   
   // qDebug() <<"tbmap:" << m_tbmap;
    for(int enumi = 1; enumi < 5;enumi++)
    {
        if(m_labelEx->m_infomap.contains(enumi))
           m_tbmap[cConfigLabel][enumi]->setText(m_labelEx->m_infomap[enumi].second);
        else
        {
            m_tbmap[cConfigLabel][enumi]->clear();
        }
    }
    stackwidget->setCurrentWidget(labelSetEntry);
}

void StyleSet::SetElement(StyleButton *btnEx)
{
    m_btnEx = btnEx;
    for(int enumi = 1; enumi < 10;enumi++)
    {
        if(m_btnEx->m_infomap.contains(enumi))
           m_tbmap[cConfigButton][enumi]->setText(m_btnEx->m_infomap[enumi].second);
        else
        {
            m_tbmap[cConfigButton][enumi]->clear();
        }
    }

    stackwidget->setCurrentWidget(buttonSetEntry);
}
void StyleSet::SetElement(StyleGroup *groupEx)
{
    m_groupEx = groupEx;
    for(int enumi = 1; enumi < 7;enumi++)
    {
        if(m_groupEx->m_infomap.contains(enumi))
           m_tbmap[cConfigGroup][enumi]->setText(m_groupEx->m_infomap[enumi].second);
        else
        {
            m_tbmap[cConfigGroup][enumi]->clear();
        }
    }
    stackwidget->setCurrentWidget(grouoSetEntry);
}
void StyleSet::SetElement(StyleCurve *curveEx)
{
    m_curveEx = curveEx;
    for(int enumi = 1; enumi < 6;enumi++)
    {
        if(m_curveEx->m_infomap.contains(enumi))
           m_tbmap[cConfigCurve][enumi]->setText(m_curveEx->m_infomap[enumi].second);
        else
        {
           m_tbmap[cConfigCurve][enumi]->clear();
        }
    }
    stackwidget->setCurrentWidget(curveSetEntry);
}
void StyleSet::SetElement(StylePairLabel *pairlabelEx)
{
    m_pairlabelEx = pairlabelEx;
    for(int enumi = 1; enumi < 11;enumi++)
    {
        if(m_pairlabelEx->m_infomap.contains(enumi))
           m_tbmap[cConfigPairLabel][enumi]->setText(m_pairlabelEx->m_infomap[enumi].second);
        else
        {
           m_tbmap[cConfigPairLabel][enumi]->clear();
        }
    }
    stackwidget->setCurrentWidget(pairLabelSetEntry);
}

ConfigNameSpaceEnd
