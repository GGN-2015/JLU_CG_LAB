# JLU_CG_LAB
 JLU 计算机图形学实验

作者：GGN\_2015

环境：Visual Studio 2022 - MFC - C ++ v143

## CG0-

实现了控制点的增加删除和移动，是所有其他项目的基础。



## CG1-

利用控制点绘制圆和椭圆。

遇到的问题：起初的时候选择画笔 CPen 怎么也不能修改绘制的颜色，后来发现是 CreateCompatibleBitmap 时使用了错误的 dc 作为参数。
