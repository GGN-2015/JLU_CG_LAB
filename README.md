

# JLU_CG_LAB

 JLU 计算机图形学实验

作者：GGN\_2015

环境：Visual Studio 2022 - MFC - C ++ v143

## CG0-

实现了控制点的增加删除和移动，是所有其他项目的基础。

## CG1-

利用控制点绘制圆和椭圆。

遇到的问题：起初的时候选择画笔 CPen 怎么也不能修改绘制的颜色，后来发现是 CreateCompatibleBitmap 时使用了错误的 dc 作为参数。

正确的写法：

```cpp
CDC dc;
dc.CreateCompatibleDC(pDC);
CBitmap memBmp;
memBmp.CreateCompatibleBitmap(pDC, zcRect.Width(),
                              zcRect.Height()); /* 这里不能用 &dc */
CBitmap* OldBmp = dc.SelectObject(&memBmp);
```

错误的写法：

```cpp
CDC dc;
dc.CreateCompatibleDC(pDC);
CBitmap memBmp;
memBmp.CreateCompatibleBitmap(&dc, zcRect.Width(),
							zcRect.Height()); /* 这里不能用 &dc */
CBitmap* OldBmp = dc.SelectObject(&memBmp);
```

## CG2-

多边形扫描转换的边填充算法。

遇到的问题：起初采用的算法是对每个线段 AB 考虑起始点 A 而不考虑结束点 B，发现在极值点处出现了泄露的问题，对每个极值点对应的那条扫描线多取反一次即可。

