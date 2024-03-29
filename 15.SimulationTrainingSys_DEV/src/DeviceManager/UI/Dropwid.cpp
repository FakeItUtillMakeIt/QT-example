#include "Dropwid.h"
#include"qdebug.h"


Dropwid::Dropwid()
{
}
Dropwid::Dropwid(QWidget* parent) :QWidget(parent)
{
}
Dropwid::~Dropwid()
{
}
void Dropwid::dragEnterEvent(QDragEnterEvent* Qd)
{
    Qd->accept();

}
void Dropwid::dropEvent(QDropEvent* Qe)
{
  
 
    this->hide();
    Qe->accept();
 
  
    int type = Qe->mimeData()->data("labeltype").toInt();
    if (type == 1)
    {
        this->mainUi->ui.height_line->setDisabled(false);
        this->mainUi->ui.width_line->setDisabled(false);
        Dragimg* Di = new Dragimg(this->cur_editWid);
        Di->id = this->mainUi->jsonData->base_Dimgid + 1;
        this->mainUi->jsonData->base_Dimgid++;
        Di->name = "dragimg_" + QString::number(Di->id);
        Di->type = "1";
        Di->setCursor(Qt::PointingHandCursor);
        Di->mainUI= this->mainUi;
        Di->canDrag = true;
        Di->editflag = true;
        QPixmap img = QPixmap(":/twoDdisplay/defaultimg");
        Di->mainUI->baseimg = img;
        Di->ownimg = img;
        int width=img.width();
        int height = img.height();
        Di->setScaledContents(true);
        if (width > 1600||height>850)
        {
            QPixmap temp = img.scaled(1600, 850, Qt::KeepAspectRatio);
            Di->setPixmap(temp);
            Di->setGeometry(QCursor().pos().x() - 292-0.5* temp.width(), QCursor().pos().y() - 194-0.5* temp.height(), temp.width(), temp.height());
        }
        else {
            Di->setPixmap(img);
            Di->setGeometry(QCursor().pos().x() - 292 - 0.5*img.width(), QCursor().pos().y() - 194 - 0.5*img.height(), img.width(), img.height());
        }

    
        this->mainUi->ui.width_line->setText(QString::number(Di->width()));
        this->mainUi->ui.height_line->setText(QString::number(Di->height()));
        this->mainUi->ui.pos_x->setText(QString::number(Di->pos().x()));
        this->mainUi->ui.pos_y->setText(QString::number(Di->pos().y()));
        this->mainUi->complist.append(Di);
        this->mainUi->Dimglist.append(Di);
        this->mainUi->curComp = Di;
        this->mainUi->curCompimg = Di;
        this->mainUi->compType = 1;
        Di->show();
    }
    if (type == 2)
    {
        this->mainUi->ui.height_line->setDisabled(true);
        this->mainUi->ui.width_line->setDisabled(false);
        Dragcomponent* comp = new Dragcomponent(this->cur_editWid);
        comp->id = this->mainUi->jsonData->base_Dcompid + 1;
        this->mainUi->jsonData->base_Dcompid++;
        comp->name = "dragcomp_" + QString::number(comp->id);
        comp->type = "2";
        comp->setCursor(Qt::PointingHandCursor);
        comp->mainUI = this->mainUi;
        comp->canDrag = true;
        comp->editflag = true;
        comp->setGeometry(QCursor().pos().x() - 352, QCursor().pos().y() - 216, 120, 40);
        comp->setMinimumSize(QSize(0, 40));
        comp->setMaximumSize(QSize(16777215, 40));
        QHBoxLayout* compho = new QHBoxLayout(comp);

        this->mainUi->ui.width_line->setText(QString::number(comp->width()));
        this->mainUi->ui.height_line->setText(QString::number(comp->height()));
        this->mainUi->ui.pos_x->setText(QString::number(comp->pos().x()));
        this->mainUi->ui.pos_y->setText(QString::number(comp->pos().y()));
        compho->setSpacing(0);
        compho->setContentsMargins(0, 0, 0, 0);
        QLabel* leftrect = new QLabel(comp);
        leftrect->setMinimumSize(QSize(4, 0));
        leftrect->setMaximumSize(QSize(4, 16777215));
        leftrect->setPixmap(QPixmap(QString::fromUtf8(":/twoDdisplay/rectleft")));
        compho->addWidget(leftrect);
        QLabel* btn = new QLabel(comp);
        btn->setObjectName(comp->name);
        btn->setAlignment(Qt::AlignHCenter);
        btn->setMinimumSize(QSize(0, 40));
        btn->setMaximumSize(QSize(16777215, 40));
        btn->setCursor(QCursor(Qt::PointingHandCursor));
        btn->setStyleSheet(QLatin1String("border-image:url(:/twoDdisplay/btnbg);\n"
            "color:rgb(98,109,125);font-size:16px"));
        compho->addWidget(btn);

        QLabel* rightrect = new QLabel(comp);
        rightrect->setMinimumSize(QSize(4, 0));
        rightrect->setMaximumSize(QSize(4, 16777215));
        rightrect->setPixmap(QPixmap(QString::fromUtf8(":/twoDdisplay/rectright")));
        compho->addWidget(rightrect);
        comp->show();
        this->mainUi->complist.append(comp);
        this->mainUi->Dcomplist.append(comp);
        this->mainUi->curComp = comp;
        this->mainUi->curDcomp = comp;
        this->mainUi->compType = 2;
    }
    if (type == 3)
    {
        this->mainUi->ui.height_line->setDisabled(true);
        this->mainUi->ui.width_line->setDisabled(false);
        Dragline* dl = new Dragline(this->cur_editWid);
        dl->id = this->mainUi->jsonData->base_lineid + 1;
        this->mainUi->jsonData->base_lineid++;
        dl->name = "hline_" + QString::number(dl->id);
        dl->type = "3";
        dl->linetype = 0;
        dl->mainUI = this->mainUi;
        dl->canDrag = true;
        dl->editflag = true;
        dl->setCursor(Qt::PointingHandCursor);
        dl->setStyleSheet("border-top:2px dashed white;");
        dl->setGeometry(QCursor().pos().x() - 392, QCursor().pos().y() - 194, 200, 10);
        this->mainUi->ui.width_line->setText(QString::number(dl->width()));
        this->mainUi->ui.height_line->setText(QString::number(dl->height()));
        this->mainUi->ui.pos_x->setText(QString::number(dl->pos().x()));
        this->mainUi->ui.pos_y->setText(QString::number(dl->pos().y()));
        dl->show();
        this->mainUi->complist.append(dl);
        this->mainUi->Dlinelist.append(dl);
        this->mainUi->curComp = dl;
        this->mainUi->curLine = dl;
        this->mainUi->compType = 3;
    }
    
    if (type == 4)
    {
        this->mainUi->ui.height_line->setDisabled(false);
        this->mainUi->ui.width_line->setDisabled(true);
        Dragline* dl = new Dragline(this->cur_editWid);
        dl->linetype = 1;
        dl->id = this->mainUi->jsonData->base_lineid + 1;
        this->mainUi->jsonData->base_lineid++;
        dl->name = "vline_" + QString::number(dl->id);
        dl->type = "4";
        dl->mainUI = this->mainUi;
        dl->canDrag = true;
        dl->editflag = true;
        dl->setCursor(Qt::PointingHandCursor);
        dl->setStyleSheet("border-left:2px dashed white;");
        dl->setGeometry(QCursor().pos().x() - 292, QCursor().pos().y() - 294, 10, 200);
        this->mainUi->ui.width_line->setText(QString::number(dl->width()));
        this->mainUi->ui.height_line->setText(QString::number(dl->height()));
        this->mainUi->ui.pos_x->setText(QString::number(dl->pos().x()));
        this->mainUi->ui.pos_y->setText(QString::number(dl->pos().y()));
        dl->show();
        this->mainUi->complist.append(dl);
        this->mainUi->Dlinelist.append(dl);
        this->mainUi->curComp = dl;
        this->mainUi->curLine = dl;
        this->mainUi->compType = 4;
    }
    if (type == 5)
    {
        this->mainUi->ui.height_line->setDisabled(false);
        this->mainUi->ui.width_line->setDisabled(false);

        DragArea* da = new DragArea(this->cur_editWid);
   
        da->id = this->mainUi->jsonData->base_areaid+1;
        this->mainUi->jsonData->base_areaid++;
        da->name = "area_" + QString::number(da->id);
        da->type = "5";
        da->mainUI = this->mainUi;
        da->canDrag = true;
        da->editflag = true;
        da->setCursor(Qt::PointingHandCursor);
        da->setStyleSheet("border:2px dashed white");
        da->setGeometry(QCursor().pos().x() - 342, QCursor().pos().y() - 244, 100, 100);
        this->mainUi->ui.width_line->setText(QString::number(da->width()));
        this->mainUi->ui.height_line->setText(QString::number(da->height()));
        this->mainUi->ui.pos_x->setText(QString::number(da->pos().x()));
        this->mainUi->ui.pos_y->setText(QString::number(da->pos().y()));
        da->show();
        this->mainUi->complist.append(da);
        this->mainUi->Darealist.append(da);
        this->mainUi->curComp = da;
        this->mainUi->curArea = da;
        this->mainUi->compType = 5;
    }

   
 
}


