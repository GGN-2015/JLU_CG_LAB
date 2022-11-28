
// MFCDemoView.cpp : CMFCDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCDemo.h"
#endif

#include "MFCDemoDoc.h"
#include "MFCDemoView.h"

//【8】设置颜色
#include "SetColor.h"
//【8】

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDemoView

IMPLEMENT_DYNCREATE(CMFCDemoView, CView)

BEGIN_MESSAGE_MAP(CMFCDemoView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_LINE, &CMFCDemoView::OnLine)
	ON_COMMAND(ID_RECTANGLE, &CMFCDemoView::OnRectangle)
	ON_COMMAND(ID_SETCOLOR, &CMFCDemoView::OnSetcolor)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCDemoView 构造/析构

CMFCDemoView::CMFCDemoView()
{
	// TODO:  在此处添加构造代码
	//【5】用鼠标绘制直线段
	//设置鼠标左键初始没有按下
	m_LButtonDown = false;
	//【5】

	//【6】用鼠标同时绘制直线段和矩形
	//程序初始状态不绘制任何图元
	type = 0;
	//【6】

	//【8】设置颜色
	r = 0; g = 0; b = 0;
	//【8】
}

CMFCDemoView::~CMFCDemoView()
{
}

BOOL CMFCDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCDemoView 绘制

void CMFCDemoView::OnDraw(CDC* pDC)
{
	CMFCDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	/*【1】设备环境对象以及绘图函数
	//初始绘图演示
	pDC->MoveTo(10, 10);
	pDC->LineTo(100, 100);
	pDC->LineTo(200, 100);
	ReleaseDC(pDC);
	【1】*/

	/*【2】获得设备环境对象的方法
	//获得设备环境对象
	pDC->LineTo(100, 100);
	CDC* pDC2 = this->GetDC();
	pDC2->LineTo(100, 200);
	CClientDC dc(this);
	dc.LineTo(100, 300);
	dc.LineTo(300, 300);
	ReleaseDC(pDC);
	ReleaseDC(pDC2);
	【2】*/

	/*【3】画笔和画刷的使用
	CPen pen;
	CPen* oldpen;
	pen.CreatePen(PS_SOLID, 2,RGB(0,0,255));
	oldpen = pDC->SelectObject(&pen);
	pDC->MoveTo(100,100);
	pDC->LineTo(100,200);
	pDC->SelectObject(oldpen);

	CClientDC dc(this);
	CPen pen2;
	pen2.CreatePen(PS_SOLID,8,RGB(255,0,0));
	CBrush brush;
	CBrush* oldbrush;
	brush.CreateSolidBrush(RGB(0,255,0));
	oldpen = dc.SelectObject(&pen2);
	oldbrush=dc.SelectObject(&brush);
	dc.Rectangle(200,200,400,400);
	dc.SelectObject(oldpen);
	dc.SelectObject(oldbrush);

	ReleaseDC(pDC);//释放设备环境对象
	//释放画笔、画刷资源
	pen.DeleteObject();
	pen2.DeleteObject();
	brush.DeleteObject();
	【3】*/

	/*【4】实现图形学算法，利用算法完成绘图
	//使用DDALine函数画线
	this->DDALine(pDC, 100, 100, 300, 200, RGB(255, 0, 0));
	ReleaseDC(pDC);
	【4】*/

	/*【7】部分：实现重画
	//CDC* pDC = this->GetDC();
	for (int i = 0; i<objList.GetSize(); i++){
		MapObj obj = objList.GetAt(i);
		if (obj.type == 1){
			DDALine(pDC, obj.p1.x, obj.p1.y, obj.p2.x, obj.p2.y, RGB(0, 0, 0));
		}
		else if (obj.type == 2){
			DDALine(pDC, obj.p1.x, obj.p1.y, obj.p2.x, obj.p1.y, RGB(0, 0, 0));
			DDALine(pDC, obj.p2.x, obj.p1.y, obj.p2.x, obj.p2.y, RGB(0, 0, 0));
			DDALine(pDC, obj.p2.x, obj.p2.y, obj.p1.x, obj.p2.y, RGB(0, 0, 0));
			DDALine(pDC, obj.p1.x, obj.p2.y, obj.p1.x, obj.p1.y, RGB(0, 0, 0));
		}
	}
	ReleaseDC(pDC);
	【7】*/

	//【8】设置颜色
	//CDC* pDC = this->GetDC();
	for (int i = 0; i<objList.GetSize(); i++){
		MapObj obj = objList.GetAt(i);
		if (obj.type == 1){
			DDALine(pDC, obj.p1.x, obj.p1.y, obj.p2.x, obj.p2.y, obj.color);
		}
		else if (obj.type == 2){
			DDALine(pDC, obj.p1.x, obj.p1.y, obj.p2.x, obj.p1.y, obj.color);
			DDALine(pDC, obj.p2.x, obj.p1.y, obj.p2.x, obj.p2.y, obj.color);
			DDALine(pDC, obj.p2.x, obj.p2.y, obj.p1.x, obj.p2.y, obj.color);
			DDALine(pDC, obj.p1.x, obj.p2.y, obj.p1.x, obj.p1.y, obj.color);
		}
	}
	ReleaseDC(pDC);
	//【8】
}


// CMFCDemoView 打印

BOOL CMFCDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CMFCDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CMFCDemoView 诊断

#ifdef _DEBUG
void CMFCDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCDemoDoc* CMFCDemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDemoDoc)));
	return (CMFCDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCDemoView 消息处理程序

/*
【4】实现图形学算法，利用算法完成绘图
DDALine函数，实现了DDA直线段扫描转换算法
参数pDC：设备环境对象指针
参数x1，y1，x2，y2：直线段两个端点的坐标
参数color：直线段的颜色
*/
void CMFCDemoView::DDALine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	double dx, dy, e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx /= e;
	dy /= e;
	x = x1;
	y = y1;
	for (int i = 1; i <= e; i++)
	{
		pDC->SetPixel((int)(x + 0.5), (int)(y + 0.5), color);
		x += dx;
		y += dy;
	}
}

//鼠标左键按下消息的处理函数
void CMFCDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*【5】用鼠标绘制直线段
	this->SetCapture();//捕捉鼠标
	//设置开始点和终止点，此时为同一点
	m_StartPoint = point;
	m_EndPoint = point;
	m_LButtonDown = true;//设置鼠标左键按下
	【5】*/

	//【6】用鼠标同时绘制直线段和矩形
	if (type == 1 || type == 2){
		this->SetCapture();//捕捉鼠标
		//设置开始点和终止点，此时为同一点
		m_StartPoint = point;
		m_EndPoint = point;
		m_LButtonDown = true;//设置鼠标左键按下
	}
	//【6】

	CView::OnLButtonDown(nFlags, point);
}

//鼠标左键抬起消息处理函数
void CMFCDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*【5】用鼠标绘制直线段
	ReleaseCapture();//释放鼠标
	m_LButtonDown = false;
	【5】*/

	/*【6】用鼠标同时绘制直线段和矩形
	if (type == 1 || type == 2){
		ReleaseCapture();//释放鼠标
		m_LButtonDown = false;
	}
	【6】*/

	/*【7】实现重画
	if (type == 1 || type == 2){
		MapObj obj;
		obj.type = type;
		obj.p1 = m_StartPoint;
		obj.p2 = m_EndPoint;
		objList.Add(obj);
		ReleaseCapture();//释放鼠标
		m_LButtonDown = false;
	}
	【7】*/

	//【8】设置颜色
	if (type == 1){
		CDC* dc = this->GetDC();
		DDALine(dc, m_StartPoint.x, m_StartPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(r, g, b));
		this->ReleaseDC(dc);
		MapObj obj;
		obj.type = 1;
		obj.p1 = m_StartPoint;
		obj.p2 = m_EndPoint;
		obj.color = RGB(r, g, b);
		objList.Add(obj);
		ReleaseCapture();//释放鼠标
		m_LButtonDown = false;
	}
	else if (type == 2) {
		CDC* dc = this->GetDC();
		DDALine(dc, m_StartPoint.x, m_StartPoint.y, m_StartPoint.x, m_EndPoint.y, RGB(r, g, b));
		DDALine(dc, m_StartPoint.x, m_EndPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(r, g, b));
		DDALine(dc, m_EndPoint.x, m_EndPoint.y, m_EndPoint.x, m_StartPoint.y, RGB(r, g, b));
		DDALine(dc, m_EndPoint.x, m_StartPoint.y, m_StartPoint.x, m_StartPoint.y, RGB(r, g, b));
		this->ReleaseDC(dc);
		MapObj obj;
		obj.type = 2;
		obj.p1 = m_StartPoint;
		obj.p2 = m_EndPoint;
		obj.color = RGB(r, g, b);
		objList.Add(obj);
		ReleaseCapture();//释放鼠标
		m_LButtonDown = false;
	}
	//【8】

	CView::OnLButtonUp(nFlags, point);
}

//鼠标移动消息处理函数
void CMFCDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*【5】用鼠标绘制直线段
	if (m_LButtonDown){
	CDC* pDC = this->GetDC();//构造设备环境对象
	pDC->SetROP2(R2_NOT);//设置绘图模式为R2_NOT
	//重新绘制前一个鼠标移动消息处理函数绘制的直线段
	//因为绘图模式的原因，结果就是擦除了该线段
	DDALine(pDC,m_StartPoint.x,m_StartPoint.y,m_EndPoint.x,m_EndPoint.y,RGB(0,0,0));
	//绘制新的直线段
	DDALine(pDC,m_StartPoint.x,m_StartPoint.y,point.x,point.y,RGB(0,0,0));
	//保存新的直线段终点
	m_EndPoint = point;
	this->ReleaseDC(pDC);
	}
	【5】*/

	//【6】用鼠标同时绘制直线段和矩形
	if (m_LButtonDown){
		CDC* pDC = this->GetDC();//构造设备环境对象
		pDC->SetROP2(R2_NOT);//设置绘图模式为R2_NOT
		if (type == 1) {
			//重新绘制前一个鼠标移动消息处理函数绘制的直线段
			//因为绘图模式的原因，结果就是擦除了该线段
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(0, 0, 0));
			//绘制新的直线段
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, point.x, point.y, RGB(0, 0, 0));
			//保存新的直线段终点
		}
		else if (type == 2) {
			//重新绘制前一个鼠标移动消息处理函数绘制的矩形
			//因为绘图模式的原因，结果就是擦除了该矩形
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, m_StartPoint.x, m_EndPoint.y, RGB(0, 0, 0));
			DDALine(pDC, m_StartPoint.x, m_EndPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(0, 0, 0));
			DDALine(pDC, m_EndPoint.x, m_EndPoint.y, m_EndPoint.x, m_StartPoint.y, RGB(0, 0, 0));
			DDALine(pDC, m_EndPoint.x, m_StartPoint.y, m_StartPoint.x, m_StartPoint.y, RGB(0, 0, 0));
			//绘制新的矩形
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, m_StartPoint.x, point.y, RGB(0, 0, 0));
			DDALine(pDC, m_StartPoint.x, point.y, point.x, point.y, RGB(0, 0, 0));
			DDALine(pDC, point.x, point.y, point.x, m_StartPoint.y, RGB(0, 0, 0));
			DDALine(pDC, point.x, m_StartPoint.y, m_StartPoint.x, m_StartPoint.y, RGB(0, 0, 0));
		}
		this->ReleaseDC(pDC);
		m_EndPoint = point;
	}
	//【6】

	CView::OnMouseMove(nFlags, point);
}

//绘制直线段的工具条按钮的处理函数
void CMFCDemoView::OnLine()
{
	// TODO:  在此添加命令处理程序代码
	//【6】用鼠标同时绘制直线段和矩形
	type = 1;//设置当前要绘制的图形为直线段
	//【6】
}

//绘制矩形的工具条按钮的处理函数
void CMFCDemoView::OnRectangle()
{
	// TODO:  在此添加命令处理程序代码
	//【6】用鼠标同时绘制直线段和矩形
	type = 2;//设置当前要绘制的图形为矩形
	//【6】
}

//设置颜色菜单项的处理函数
void CMFCDemoView::OnSetcolor()
{
	// TODO:  在此添加命令处理程序代码
	//【8】设置颜色
	SetColor s;//构造对话框类对象
	s.m_r = r; s.m_g = g; s.m_b = b;//让对话框中的编辑框里显示当前的颜色分量值
	//调用DoModal函数以模态方式显示对话框，点击OK按钮关闭对话框时，函数返回值为IDOK
	if (s.DoModal() == IDOK){
		r = s.m_r; g = s.m_g; b = s.m_b;//设置当前颜色分量值为用户设置的分量值
	}
	//【8】
}

//键盘上按键按下的消息处理函数
void CMFCDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//【9】移动图形
	if (nChar == VK_LEFT){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj obj = objList.GetAt(i);
			obj.p1.x -= 1;
			obj.p2.x -= 1;
			objList.SetAt(i, obj);
		}
		this->Invalidate(true);//引起主动视图重画
	}
	if (nChar == VK_RIGHT){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj obj = objList.GetAt(i);
			obj.p1.x += 1;
			obj.p2.x += 1;
			objList.SetAt(i, obj);
		}
		this->Invalidate(true);
	}
	if (nChar == VK_UP){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj obj = objList.GetAt(i);
			obj.p1.y -= 1;
			obj.p2.y -= 1;
			objList.SetAt(i, obj);
		}
		this->Invalidate(true);
	}
	if (nChar == VK_DOWN){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj obj = objList.GetAt(i);
			obj.p1.y += 1;
			obj.p2.y += 1;
			objList.SetAt(i, obj);
		}
		this->Invalidate(true);
	}
	//【9】

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
