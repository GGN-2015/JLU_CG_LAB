
// CG10612View.h: CCG10612View 类的接口
//

#pragma once


class CCG10612View : public CView
{
protected: // 仅从序列化创建
	CCG10612View() noexcept;
	DECLARE_DYNCREATE(CCG10612View)

// 特性
public:
	CCG10612Doc* GetDocument() const;

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
	virtual ~CCG10612View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CG10612View.cpp 中的调试版本
inline CCG10612Doc* CCG10612View::GetDocument() const
   { return reinterpret_cast<CCG10612Doc*>(m_pDocument); }
#endif

