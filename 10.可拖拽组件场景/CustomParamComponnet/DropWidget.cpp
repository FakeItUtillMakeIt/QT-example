#include "DropWidget.h"

using namespace DROP_WIDGET;

/**
    @brief DropWidget object constructor
    @param parent - 构造函数
**/
DropWidget::DropWidget(QWidget* parent) {

    this->setMinimumWidth(800);
    
    //this->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    this->setAcceptDrops(true);

    this->setAttribute(Qt::WA_PaintOnScreen, true);

    this->setStyleSheet("* {background-color:#f0f8ff}");

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
    @brief 重写当有控件拖入的时候的操作
    @param event - 
**/
void DropWidget::dropEvent(QDropEvent* event) {
    auto a = event->source();
	if (event->source() == this)
	{
		event->setDropAction(Qt::MoveAction);
		event->ignore();
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
            dropObject->resize(QSize(80, 20));
            break;
        case TYPE_BUTTON:
            dropObject = new CUSTOM_PUSHBUTTON::CustomPushButton("button",this);
            dropObject->resize(QSize(80, 20));
            break;
        case TYPE_PLOT:
            dropObject = new CUSTOM_CURVE_PLOT::CustomCurvePlot(this);
            dropObject->resize(QSize(200, 200));
            break;
		case TYPE_BOX:
			dropObject = new CUSTOM_GROUP_BOX::CustomGroupBox(this);
            dropObject->resize(QSize(200, 80));
			break;
        default:
            break;
        }

#ifdef ONLY_CONTROL_COMPARAM
		/*2022/05/30*/
            
		CUSTOM_GROUP_BOX::CustomGroupBox* dropObject = nullptr;
        CUSTOM_GROUP_BOX_WIDGET::CustomGroupBoxWidget* boxW = new CUSTOM_GROUP_BOX_WIDGET::CustomGroupBoxWidget(this);

		//根据拖拽类型创建实例
		switch (dropType)
		{
		case TYPE_LABEL:
			dropObject = boxW->getGroupWidget(CUSTOM_GROUP_BOX_WIDGET::CUSTOM_GROUP_WIDGET_TYPE::LABEL_GROUP);

            break;
		case TYPE_BUTTON:
			//dropObject = new CUSTOM_PUSHBUTTON::CustomPushButton("button", this);
			break;
		case TYPE_PLOT:
            //dropObject = new CUSTOM_PLOT::CustomPlot(this);
            dropObject = boxW->getGroupWidget(CUSTOM_GROUP_BOX_WIDGET::CUSTOM_GROUP_WIDGET_TYPE::CURVE_PLOT);
			break;
        default:
            break;
        }
#endif // ONLY_CONTROL_COMPARAM

/*2022/05/30*/
        
#ifdef CONTROL_COMPARAM_AND_GROUPBOX

        if (!dropObject)
			{
            return;
        }

        dropObject->move(event->pos() - offset);
		dropObject->show();
		dropObject->setAttribute(Qt::WA_DeleteOnClose);
          
        /*QGroupBox* groupBox = new QGroupBox(this);
        QVBoxLayout* lay = new QVBoxLayout(groupBox);
        lay->addWidget(dropObject);

        groupBox->setTitle("222");
        groupBox->move(event->pos() - offset);

        groupBox->showNormal();*/

#endif // CONTROL_COMPARAM_AND_GROUPBOX
			
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
    
}

