#include "ParamEquivalent.h"

ParamEquivalent::ParamEquivalent(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initSignalSlot_fun();
}

bool ParamEquivalent::initSignalSlot_fun() {

    connect(ui.selectPluginFile, &QPushButton::clicked, this, &ParamEquivalent::pluginFile_btn);
    connect(ui.addWidget, &QPushButton::clicked, this, &ParamEquivalent::addNewPage_btn);
    connect(ui.deleteWidget, &QPushButton::clicked, this, &ParamEquivalent::deletePage_btn);
    connect(ui.saveWidget, &QPushButton::clicked, this, &ParamEquivalent::savePage_btn);

    return true;
}

void ParamEquivalent::pluginFile_btn() {
    QStringList filtersFormat;
    filtersFormat << "dynamic library (*.dll)"
        << "static library (*.lib)";
    QFileDialog* fileDiag = new QFileDialog(this);
    fileDiag->setNameFilters(filtersFormat);

    QStringList selectFileName;
    if (fileDiag->exec())
    {
        selectFileName=fileDiag->selectedFiles();
    }
    //½âÎödll

}

void ParamEquivalent::addNewPage_btn() {
    auto pageNumber=ui.tabWidget->count();
    ui.tabWidget->insertTab(pageNumber, new QWidget, QIcon(), QString::fromLocal8Bit("Page") + QString::number(pageNumber+1));
}
void ParamEquivalent::deletePage_btn() {
    auto currentPageNumber = ui.tabWidget->currentIndex();
    ui.tabWidget->removeTab(currentPageNumber);
}
void ParamEquivalent::savePage_btn() {

}