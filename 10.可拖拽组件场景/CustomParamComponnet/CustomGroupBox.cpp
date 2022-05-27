#include "CustomGroupBox.h"

using namespace CUSTOM_GROUP_BOX;

CustomGroupBox::CustomGroupBox(QWidget* parent/* =nullptr */) {
    
}

/**
    @brief  根据传入的类型获取group
    @param  type - 
    @retval      - 
**/
QGroupBox* CustomGroupBox::getGroupWidget(CUSTOM_GROUP_TYPE type) {
    groupBox = new QGroupBox();
    QGridLayout* boxLayout = new QGridLayout(groupBox);

   
    switch (type)
    {
    case CUSTOM_GROUP_BOX::LABEL_GROUP:
        customLabel = new CUSTOM_LABEL::CustomLabel();
        boxLayout->addWidget(customLabel);
        break;
    case CUSTOM_GROUP_BOX::CURVE_PLOT:
        customCurvePlot = new CUSTOM_CURVE_PLOT::CustomCurvePlot();
        break;
    default:
        break;
    }

 

    return groupBox;

}

/**
      @brief 更改属性栏添加按钮时的接口
      @param type -
  **/
void CustomGroupBox::addWidget2Group(CUSTOM_GROUP_TYPE type) {


}