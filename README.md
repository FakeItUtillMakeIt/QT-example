 **https://zhuanlan.zhihu.com/p/368984870** 

1. QDrag 支持基于mime的拖放数据传输
2. QDragEnterEvent 当拖放操作进入小部件时发送至小部件的事件
3. QDragLeaveEvent 当拖放操作离开小部件时发送至小部件的事件
4. QDragMoveEvent 在执行拖放操作时发送的事件
5. QDropEvent 拖放操作完成时发送的事件

- 启用小部件拖动最简单的方法：实现mousePressEvent
- 确认是否应该拖动，并构造一个拖动对象来处理该操作，实现dragMoveEvent
- 通常用于通知qt小部件接受的数据类型，实现dragEnterEvent
- dropEvent用于解包丢弃的数据，并以适合您的应用程序的方式对其进行处理。

# QT-example
QT-example应用程序
# YOLOV5-Window
## 1.需要下载opencv4.5+
  下载地址：https://opencv.org/releases/
## 2.需要QT动态库，版本5.14.2
  下载地址：https://pan.baidu.com/s/1hP0AUQtyDqOijYXYYUJpWQ 
提取码：22jg
## 3.程序运行截图

   <img align="center" src="./app.png">

# 综控机测试软件 


    
        
          
    

        
    
    @@ -18,4 +19,5 @@ QT-example应用程序
  
  地址：https://github.com/FakeItUtillMakeIt/QT-example/tree/main/GeneralCTrlMacTest
## 1.需要NI设备支持
## 2.需要串口支持
## 3.程序运行截图

  <img align="center" src="./aoo.png">
