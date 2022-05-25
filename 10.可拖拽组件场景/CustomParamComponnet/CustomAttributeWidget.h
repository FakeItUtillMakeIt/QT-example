#pragma once
#ifndef CUSTOM_ATTRIBUTE_WIDGET_H
#define CUSTOM_ATTRIBUTE_WIDGET_H


#include <qwidget.h>

/**
    @namespace CUSTOM_ATTRIBUTE_WIDGET
    @brief     自定义属性窗口类
**/
namespace CUSTOM_ATTRIBUTE_WIDGET {

    class CustomAttributeWidget :
        public QWidget
    {
    public:
        CustomAttributeWidget();
        ~CustomAttributeWidget();

    };
}



#endif