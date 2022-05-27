#pragma once

#ifndef CUSTOM_COMBINED_WIDGET_H
#define CUSTOM_COMBINED_WIDGET_H


#include <qwidget.h>


namespace CUSTOM_COMBINED_WIDGET {
    /**

        @class   CustomCombinedWidget
        @brief   组合控件  
        @details ~控件可以手动添加删除组合参数

    **/
    class CustomCombinedWidget :
        public QWidget
    {
        Q_OBJECT

    public:
        CustomCombinedWidget(QWidget* parent = nullptr);
        ~CustomCombinedWidget();

    };

    

}



#endif