#pragma once
#include <QtWidgets>
#include <qtablewidget.h>


class SubTableWidget :
    public QTableWidget
{

    Q_OBJECT
public:
    SubTableWidget();
    
private:
    QPoint cursorPos;
    QWidget* popWidget;

    QPushButton* frontInsertCell;
    QPushButton* backInsertCell;
    QPushButton* removeCell;

    int row, column;

private:
	void mousePressEvent(QMouseEvent* event);
	void mouseDoubleClickEvent(QMouseEvent* event);

private slots:
    void rightButtonClicked(int row, int column);
    void doubleClickTable(int row, int column);

signals:
	void mouseRightClicked(QPoint);
    void opOver(QPoint);

};

