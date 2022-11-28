
// MFCDemoView.h : CMFCDemoView ��Ľӿ�
//

#pragma once

//��7��ʵ���ػ�
#include <afxtempl.h>
#include "MapObj.h"
//��7��

class CMFCDemoView : public CView
{
protected: // �������л�����
	CMFCDemoView();
	DECLARE_DYNCREATE(CMFCDemoView)

// ����
public:
	CMFCDemoDoc* GetDocument() const;
	//��5����������ֱ�߶�
	CPoint m_StartPoint, m_EndPoint;//��Ƥ�ߵ������˵�
	bool m_LButtonDown;//�ж��������Ƿ���
	//��5��

	//��6�������ͬʱ����ֱ�߶κ;���
	int type;//ѡ����Ƶ�ͼԪ����
	//��6��

	//��7��ʵ���ػ�
	CArray<MapObj, MapObj> objList;//ͼԪ�б�
	//��7��

	//��8��������ɫ
	int r, g, b;
	//��8��

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMFCDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // MFCDemoView.cpp �еĵ��԰汾
inline CMFCDemoDoc* CMFCDemoView::GetDocument() const
   { return reinterpret_cast<CMFCDemoDoc*>(m_pDocument); }
#endif

