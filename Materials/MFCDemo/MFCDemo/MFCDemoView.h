
// MFCDemoView.h : CMFCDemoView 类的接口
//

#pragma once

//【7】实现重画
#include <afxtempl.h>
#include "MapObj.h"
//【7】

class CMFCDemoView : public CView
{
protected: // 仅从序列化创建
	CMFCDemoView();
	DECLARE_DYNCREATE(CMFCDemoView)

// 特性
public:
	CMFCDemoDoc* GetDocument() const;
	//【5】用鼠标绘制直线段
	CPoint m_StartPoint, m_EndPoint;//橡皮线的两个端点
	bool m_LButtonDown;//判断鼠标左键是否按下
	//【5】

	//【6】用鼠标同时绘制直线段和矩形
	int type;//选择绘制的图元类型
	//【6】

	//【7】实现重画
	CArray<MapObj, MapObj> objList;//图元列表
	//【7】

	//【8】设置颜色
	int r, g, b;
	//【8】

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLine();
	afx_msg void OnRectangle();
	afx_msg void OnSetcolor();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MFCDemoView.cpp 中的调试版本
inline CMFCDemoDoc* CMFCDemoView::GetDocument() const
   { return reinterpret_cast<CMFCDemoDoc*>(m_pDocument); }
#endif

