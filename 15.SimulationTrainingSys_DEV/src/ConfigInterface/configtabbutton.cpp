#include "configtabbutton.h"
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include "configscene.h"

ConfigNameSpaceStart

ConfigTabButton::ConfigTabButton(const QString& text, QWidget* parent ):QPushButton(text,parent)
{
    setAcceptDrops(true);
}
ConfigTabButton::ConfigTabButton(QWidget* parent):QPushButton(parent)
{
    setAcceptDrops(true);
}

void ConfigTabButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
               startPos = e->pos();

    QPushButton::mousePressEvent(e);
}

void ConfigTabButton::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
           int distance = (e->pos() - startPos).manhattanLength();
           if (distance >= QApplication::startDragDistance())
               handleDragEvent();
        }

    QWidget::mouseMoveEvent(e);
}

void ConfigTabButton::dropEvent(QDropEvent *event)
{
    QByteArray  sceneaddrstr = event->mimeData()->data("sceneaddr");
    unsigned long long  sceneaddr = sceneaddrstr.toULongLong();
    ConfigScene* swapscene = (ConfigScene*)sceneaddr;
    ConfigScene* oldscene = configscene;

    if (configscene && swapscene)
        configscene->ButtonSwap(swapscene);

    XmlStore::SwapScenePosition(oldscene, swapscene);
}

void ConfigTabButton::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
     update();
}

void ConfigTabButton::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void ConfigTabButton::dragEnterEvent(QDragEnterEvent *event)
{
    ControlRole  ctrlrole = (ControlRole)event->mimeData()->data("operation").toInt();
    if (ctrlrole == cTabMove)
    {
        event->accept();
        return;
    }
    event->ignore();
}
void ConfigTabButton::handleDragEvent()
{
    if (configscene == nullptr) return;
    QDrag *dg=new QDrag(this);
    //将需要拖动的数据放入QMimedata对象中，该对象用于保存需要传递的数据，数据的内
    //容完全由程序员自行设定。通常为界面上所选择内容。
    QMimeData *md=new QMimeData;
    md->setData("operation",QByteArray::number(cTabMove));    			//这是md中存储的内容(即拖放时传递的数据)。
    unsigned long long addr = (unsigned long long)configscene;
    md->setData("sceneaddr", QByteArray::number(addr));    			//这是md中存储的内容(即拖放时传递的数据)。
    dg->setMimeData(md);   			//步骤1：设置拖动的数据。该函数会获得md的所有权。
    dg->setPixmap(this->grab());
    dg->setHotSpot(QPoint(this->width()/2,this->height()/2));
    dg->exec(Qt::CopyAction | Qt::MoveAction);

}
ConfigNameSpaceEnd