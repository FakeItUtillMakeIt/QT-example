#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DragWidgetTest.h"

class DragWidgetTest : public QMainWindow
{
    Q_OBJECT

public:
    DragWidgetTest(QWidget *parent = Q_NULLPTR);

	

private:
    Ui::DragWidgetTestClass ui;

protected:
	void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;
	void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
	void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

};
