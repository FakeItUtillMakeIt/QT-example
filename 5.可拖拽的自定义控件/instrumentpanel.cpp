#include "instrumentpanel.h"

InstrumentPanel::InstrumentPanel(QWidget *parent) :
    QWidget(parent)
{

    this->m_refSize=200;

    m_updateTimer=new QTimer(this);

    m_updateTimer->setInterval(50);//定时器刷新时间

    m_startAngle=0;
    m_endAngle=0;
    m_foreColor=Qt::green;
    m_backColor=Qt::black;
    m_scaleMajor=10;//刻度
    m_scaleMinor=5;//每个刻度分为5个分度
    m_minValue=0;
    m_maxValue=180;
    m_numbericIndicitorEnabled=true;
    m_units="km/h";
    m_title="时速表";
    m_precision=0;
    m_value=0;

    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(updateAngle()));

    //this->setWindowFlag(Qt::FramelessWindowHint);//无窗体
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明
    this->resize(400,400);

    m_updateTimer->start();
}


void InstrumentPanel::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//使用反锯齿
    painter.translate(width()/2,height()/2);//设置坐标原点
    float scale=qMin(width(),height());
    //设置控件缩放比例和先后顺序
    painter.scale(scale/200.0,scale/200.0);


    drawCrown(painter);// 表盘框

    drawScaleNum(painter);//刻度值
    drawScale(painter);// 刻度线
    drawTile(painter);//单位

    drawNumericValue(painter);//数字显示
    drawIndicator(painter);// 表针

}


void InstrumentPanel::thresholdManager(){

}


void InstrumentPanel::drawCrown(QPainter& painter){
    painter.save();
    int radius=100;
    QLinearGradient lg1(0,-radius,0,radius);

    lg1.setColorAt(0,Qt::white);
    lg1.setColorAt(1,Qt::gray);

    painter.setBrush(lg1);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(-radius,-radius,radius<<1,radius<<1);
    painter.setBrush(m_backColor=Qt::black);
    painter.drawEllipse(-92,-92,184,184);
    painter.restore();

}

//在分好的 刻度中 画刻度线
void InstrumentPanel::drawScale(QPainter& painter)
{
    painter.save();
    painter.rotate(m_startAngle);//函数实现角度的旋转
    int steps=(m_scaleMajor*m_scaleMinor);//10*5
    double angleStep=(360.0-m_startAngle-m_endAngle)/steps;//7.2 °

    QPen pen;
    pen.setColor(Qt::green);
    for (int i=0;i<=steps;i++) {
        if(i%m_scaleMinor==0){// 大刻度 加粗
            pen.setWidth(1);
            painter.setPen(pen);
            painter.drawLine(0,62,0,72);//两个坐标点

        }
        else{
            pen.setWidth(0);
            painter.setPen(pen);
            painter.drawLine(0,67,0,72);
        }
        painter.rotate(angleStep);// 坐标系在当前基础上旋转
    }
    painter.restore();
}

//画刻度值
void InstrumentPanel::drawScaleNum(QPainter& painter){
    painter.save();
    painter.setPen(m_foreColor);
    double startRad=m_startAngle*(3.14/180);//转换为  弧度
    double deltaRad=(360-m_startAngle-m_endAngle)*(3.14/180)/m_scaleMajor;
    double sina,cosa;
    int x,y;
    QFontMetricsF fm(this->font());
    double w,h,tmpVal;
    QString str;
    for (int i = 0; i <= m_scaleMajor; i++)
       {
           sina = sin(startRad - i * deltaRad);
           cosa = cos(startRad - i * deltaRad);

           tmpVal = 1.0 * i *((m_maxValue - m_minValue) / m_scaleMajor) + m_minValue;
          // tmpVal = 50;
           str = QString( "%1" ).arg(tmpVal);  //%1作为占位符   arg()函数比起 sprintf()来是类型安全的
           w = fm.size(Qt::TextSingleLine,str).width();
           h = fm.size(Qt::TextSingleLine,str).height();
           x = 82 * cosa - w / 2;
           y = -82 * sina + h / 4;
           painter.drawText(x, y, str); //函数的前两个参数是显示的坐标位置，后一个是显示的内容，是字符类型

       }


    painter.restore();
}

//显示单位 与数值
void InstrumentPanel::drawNumericValue(QPainter& painter){
    QString str=QString("%1 %2").arg(m_value,0,'f',m_precision).arg(m_units);
    QFontMetricsF fm(this->font());
    double w=fm.size(Qt::TextSingleLine,str).width();
    painter.setPen(m_foreColor);
    painter.drawText(-w/2,42,str);
}


void InstrumentPanel::drawIndicator(QPainter& painter){
    painter.save();
    static const QPointF points[3]={
        QPointF(0.0,2),
        QPointF(0.0,-2),
        QPointF(60.0,0)
    };

    //   画指针
    double degRotate=m_startAngle+m_value*2;
    painter.rotate(degRotate);// 顺时针 旋转坐标 系统
    QRadialGradient haloGradient(0,0,60,0,0);//   圆心  半径
    haloGradient.setColorAt(0,QColor(250,250,250));
    haloGradient.setColorAt(1,QColor(55,155,155));
    painter.setPen(Qt::green);//定义线条 颜色，设置线条颜色
    painter.setBrush(haloGradient);//刷子定义 形状如何填满，填充后的颜色
    painter.drawConvexPolygon(points,3);

    painter.restore();

    painter.save();
    //  画中心点
    QColor niceBlue(150,150,150);
    QConicalGradient coneGradient(0,0,-90.0);// 角度渐变
    coneGradient.setColorAt(0,Qt::darkGray);
    coneGradient.setColorAt(0.2,niceBlue);
    coneGradient.setColorAt(0.5,Qt::white);
    coneGradient.setColorAt(1.0,Qt::darkGray);

    painter.setPen(Qt::green);//没有 线，填满没有 边界
    painter.setBrush(coneGradient);
    painter.drawEllipse(QPoint(0,0),5,5);

    painter.restore();

}

void InstrumentPanel::drawTile(QPainter& painter){
    painter.save();
    painter.setPen(m_foreColor);
    QString str(m_title);
    QFontMetrics fm(this->font());
    double w=fm.size(Qt::TextSingleLine,str).width();
    painter.drawText(-w/2,-30,str);
    painter.restore();

}
void InstrumentPanel::drawBackground(QPainter& painter){
    painter.save();
    painter.setBackground(QBrush(Qt::black));
    painter.restore();

}

void InstrumentPanel::updateAngle(){
    m_value+=1;

    if(m_value>180){
        m_value=0;
    }

    update();//刷新控件，会调用paintevent
}


InstrumentPanel::~InstrumentPanel(){

}
