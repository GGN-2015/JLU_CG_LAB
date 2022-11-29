
// CG10612View.cpp: CCG10612View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG10612.h"
#endif

#include "CG10612Doc.h"
#include "CG10612View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCG10612View

IMPLEMENT_DYNCREATE(CCG10612View, CView)

BEGIN_MESSAGE_MAP(CCG10612View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCG10612View 构造/析构

CCG10612View::CCG10612View() noexcept
{
	// TODO: 在此处添加构造代码

}

CCG10612View::~CCG10612View()
{
}

BOOL CCG10612View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCG10612View 绘图

void CCG10612View::OnDraw(CDC* /*pDC*/)
{
	CCG10612Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCG10612View 打印

BOOL CCG10612View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCG10612View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCG10612View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCG10612View 诊断

#ifdef _DEBUG
void CCG10612View::AssertValid() const
{
	CView::AssertValid();
}

void CCG10612View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCG10612Doc* CCG10612View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG10612Doc)));
	return (CCG10612Doc*)m_pDocument;
}
#endif //_DEBUG


// CCG10612View 消息处理程序
