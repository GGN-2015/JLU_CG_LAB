#pragma once
//【7】实现重画
class MapObj
{
public:
	//【7】实现重画
	CPoint p1, p2;//图形的控制点
	int type;//图形的种类
	//【7】

	//【8】设置颜色
	COLORREF color;
	//【8】
public:
	MapObj();
	virtual ~MapObj();
};

