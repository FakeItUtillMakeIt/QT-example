#ifndef INSTRUMENTPANEL_H
#define INSTRUMENTPANEL_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QColor>
#include <QPaintEvent>
#include <QPolygon>

class InstrumentPanel : public QWidget
{
    Q_OBJECT

public:
    InstrumentPanel(QWidget *parent = 0);
    ~InstrumentPanel();

private:
    float m_refSize;
    bool m_numbericIndicitorEnabled;
    QColor m_frameColor;
    QColor m_foreColor;
    QColor m_backColor;

    QString m_units;
    QString m_title;

    int m_scaleMajor;
    int m_scaleMinor;

    int m_maxValue,m_minValue;

    int m_startAngle,m_endAngle;

    double m_value;
    int m_precision;
    QTimer* m_updateTimer;

private:
    void thresholdManager();
    void paintEvent(QPaintEvent *);

    void drawCrown(QPainter&);//绘制边框
    void drawScale(QPainter&);//绘制刻度
    void drawScaleNum(QPainter&);//绘制刻度数字
    void drawNumericValue(QPainter&);//绘制速度数字
    void drawIndicator(QPainter&);//绘制速度指针
    void drawTile(QPainter&);
    void drawBackground(QPainter&);






public Q_SLOTS:
    void updateAngle();

};

#endif // INSTRUMENTPANEL_H
