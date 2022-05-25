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