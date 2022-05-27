#include "DropWidget.h"

using namespace DROP_WIDGET;

DropWidget::DropWidget(QWidget* parent) {

    this->setMinimumWidth(800);

    //this->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    this->setAcceptDrops(true);

    this->setAttribute(Qt::WA_PaintOnScreen, true);

}

DropWidget::~DropWidget() {

}

/**
    @brief
    @param event -
**/
void DropWidget::dragEnterEvent(QDragEnterEvent* event) {
    auto a = event->mimeData()->formats();
    if (event->mimeData()->hasFormat("application/x-dnditemdata") || event->mimeData()->hasFormat("selectIndex"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }

    }
    else
    {
        event->ignore();
    }
}

/**
    @brief
    @param event -
**/
void DropWidget::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasFormat("application/x-dnditemdata") || event->mimeData()->hasFormat("selectIndex"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {

            event->acceptProposedAction();

        }

    }
    else
    {
        event->ignore();
    }
}

/**
    @brief
    @param event -
**/
void DropWidget::dropEvent(QDropEvent* event) {

	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
        return;
	}
	else
	{

		event->acceptProposedAction();

	}
    //放下时应该获得实例类型
    if (event->mimeData()->hasFormat("selectIndex") || event->mimeData()->hasFormat("application/x- dnditemdata"))
    {
        QByteArray itemData = event->mimeData()->data("selectIndex");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        //获取传送的信息流
        int dropType = itemData.toInt();
        QPoint offset;
        dataStream >> dropType >> offset;

        QWidget* dropObject = nullptr;

        //根据拖拽类型创建实例
        switch (dropType)
        {
        case TYPE_LABEL:
            dropObject = new CUSTOM_LABEL::CustomLabel("label",this);

            break;
        case TYPE_BUTTON:
            dropObject = new CUSTOM_PUSHBUTTON::CustomPushButton("button",this);
            break;
        case TYPE_PLOT:
            //dropObject = new CUSTOM_PLOT::CustomPlot(this);
            dropObject = new CUSTOM_CURVE_PLOT::CustomCurvePlot(this);
            break;
        default:
            break;
        }



        if (dropObject)
        {

			dropObject->resize(QSize(80, 80));
			dropObject->move(event->pos() - offset);
			dropObject->show();
			dropObject->setAttribute(Qt::WA_DeleteOnClose);

			/*QGroupBox* groupBox = new QGroupBox(this);
			QVBoxLayout* lay = new QVBoxLayout(groupBox);
            lay->addWidget(dropObject);
           
            groupBox->setTitle("222");
            groupBox->move(event->pos() - offset);
            
            groupBox->showNormal();*/
        }

        
    }
    else
    {
        event->ignore();
    }

    this->update();
}

/**
    @brief
    @param event -
**/
void DropWidget::mousePressEvent(QMouseEvent* event) {
    auto posWidget = (childAt(event->pos()));
    if (!posWidget)
    {
        return;
    }

    //单点下时显示属性栏

}

