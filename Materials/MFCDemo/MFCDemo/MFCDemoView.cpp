
// MFCDemoView.cpp : CMFCDemoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCDemo.h"
#endif

#include "MFCDemoDoc.h"
#include "MFCDemoView.h"

//��8��������ɫ
#include "SetColor.h"
//��8��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCDemoView

IMPLEMENT_DYNCREATE(CMFCDemoView, CView)

BEGIN_MESSAGE_MAP(CMFCDemoView, CView)
	// ��׼��ӡ����
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

// CMFCDemoView ����/����

CMFCDemoView::CMFCDemoView()
{
	// TODO:  �ڴ˴���ӹ������
	//��5����������ֱ�߶�
	//������������ʼû�а���
	m_LButtonDown = false;
	//��5��

	//��6�������ͬʱ����ֱ�߶κ;���
	//�����ʼ״̬�������κ�ͼԪ
	type = 0;
	//��6��

	//��8��������ɫ
	r = 0; g = 0; b = 0;
	//��8��
}

CMFCDemoView::~CMFCDemoView()
{
}

BOOL CMFCDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFCDemoView ����

void CMFCDemoView::OnDraw(CDC* pDC)
{
	CMFCDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	/*��1���豸���������Լ���ͼ����
	//��ʼ��ͼ��ʾ
	pDC->MoveTo(10, 10);
	pDC->LineTo(100, 100);
	pDC->LineTo(200, 100);
	ReleaseDC(pDC);
	��1��*/

	/*��2������豸��������ķ���
	//����豸��������
	pDC->LineTo(100, 100);
	CDC* pDC2 = this->GetDC();
	pDC2->LineTo(100, 200);
	CClientDC dc(this);
	dc.LineTo(100, 300);
	dc.LineTo(300, 300);
	ReleaseDC(pDC);
	ReleaseDC(pDC2);
	��2��*/

	/*��3�����ʺͻ�ˢ��ʹ��
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

	ReleaseDC(pDC);//�ͷ��豸��������
	//�ͷŻ��ʡ���ˢ��Դ
	pen.DeleteObject();
	pen2.DeleteObject();
	brush.DeleteObject();
	��3��*/

	/*��4��ʵ��ͼ��ѧ�㷨�������㷨��ɻ�ͼ
	//ʹ��DDALine��������
	this->DDALine(pDC, 100, 100, 300, 200, RGB(255, 0, 0));
	ReleaseDC(pDC);
	��4��*/

	/*��7�����֣�ʵ���ػ�
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
	��7��*/

	//��8��������ɫ
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
	//��8��
}


// CMFCDemoView ��ӡ

BOOL CMFCDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFCDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFCDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CMFCDemoView ���

#ifdef _DEBUG
void CMFCDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCDemoDoc* CMFCDemoView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDemoDoc)));
	return (CMFCDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCDemoView ��Ϣ�������

/*
��4��ʵ��ͼ��ѧ�㷨�������㷨��ɻ�ͼ
DDALine������ʵ����DDAֱ�߶�ɨ��ת���㷨
����pDC���豸��������ָ��
����x1��y1��x2��y2��ֱ�߶������˵������
����color��ֱ�߶ε���ɫ
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

//������������Ϣ�Ĵ�����
void CMFCDemoView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*��5����������ֱ�߶�
	this->SetCapture();//��׽���
	//���ÿ�ʼ�����ֹ�㣬��ʱΪͬһ��
	m_StartPoint = point;
	m_EndPoint = point;
	m_LButtonDown = true;//��������������
	��5��*/

	//��6�������ͬʱ����ֱ�߶κ;���
	if (type == 1 || type == 2){
		this->SetCapture();//��׽���
		//���ÿ�ʼ�����ֹ�㣬��ʱΪͬһ��
		m_StartPoint = point;
		m_EndPoint = point;
		m_LButtonDown = true;//��������������
	}
	//��6��

	CView::OnLButtonDown(nFlags, point);
}

//������̧����Ϣ������
void CMFCDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*��5����������ֱ�߶�
	ReleaseCapture();//�ͷ����
	m_LButtonDown = false;
	��5��*/

	/*��6�������ͬʱ����ֱ�߶κ;���
	if (type == 1 || type == 2){
		ReleaseCapture();//�ͷ����
		m_LButtonDown = false;
	}
	��6��*/

	/*��7��ʵ���ػ�
	if (type == 1 || type == 2){
		MapObj obj;
		obj.type = type;
		obj.p1 = m_StartPoint;
		obj.p2 = m_EndPoint;
		objList.Add(obj);
		ReleaseCapture();//�ͷ����
		m_LButtonDown = false;
	}
	��7��*/

	//��8��������ɫ
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
		ReleaseCapture();//�ͷ����
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
		ReleaseCapture();//�ͷ����
		m_LButtonDown = false;
	}
	//��8��

	CView::OnLButtonUp(nFlags, point);
}

//����ƶ���Ϣ������
void CMFCDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*��5����������ֱ�߶�
	if (m_LButtonDown){
	CDC* pDC = this->GetDC();//�����豸��������
	pDC->SetROP2(R2_NOT);//���û�ͼģʽΪR2_NOT
	//���»���ǰһ������ƶ���Ϣ���������Ƶ�ֱ�߶�
	//��Ϊ��ͼģʽ��ԭ�򣬽�����ǲ����˸��߶�
	DDALine(pDC,m_StartPoint.x,m_StartPoint.y,m_EndPoint.x,m_EndPoint.y,RGB(0,0,0));
	//�����µ�ֱ�߶�
	DDALine(pDC,m_StartPoint.x,m_StartPoint.y,point.x,point.y,RGB(0,0,0));
	//�����µ�ֱ�߶��յ�
	m_EndPoint = point;
	this->ReleaseDC(pDC);
	}
	��5��*/

	//��6�������ͬʱ����ֱ�߶κ;���
	if (m_LButtonDown){
		CDC* pDC = this->GetDC();//�����豸��������
		pDC->SetROP2(R2_NOT);//���û�ͼģʽΪR2_NOT
		if (type == 1) {
			//���»���ǰһ������ƶ���Ϣ���������Ƶ�ֱ�߶�
			//��Ϊ��ͼģʽ��ԭ�򣬽�����ǲ����˸��߶�
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(0, 0, 0));
			//�����µ�ֱ�߶�
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, point.x, point.y, RGB(0, 0, 0));
			//�����µ�ֱ�߶��յ�
		}
		else if (type == 2) {
			//���»���ǰһ������ƶ���Ϣ���������Ƶľ���
			//��Ϊ��ͼģʽ��ԭ�򣬽�����ǲ����˸þ���
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, m_StartPoint.x, m_EndPoint.y, RGB(0, 0, 0));
			DDALine(pDC, m_StartPoint.x, m_EndPoint.y, m_EndPoint.x, m_EndPoint.y, RGB(0, 0, 0));
			DDALine(pDC, m_EndPoint.x, m_EndPoint.y, m_EndPoint.x, m_StartPoint.y, RGB(0, 0, 0));
			DDALine(pDC, m_EndPoint.x, m_StartPoint.y, m_StartPoint.x, m_StartPoint.y, RGB(0, 0, 0));
			//�����µľ���
			DDALine(pDC, m_StartPoint.x, m_StartPoint.y, m_StartPoint.x, point.y, RGB(0, 0, 0));
			DDALine(pDC, m_StartPoint.x, point.y, point.x, point.y, RGB(0, 0, 0));
			DDALine(pDC, point.x, point.y, point.x, m_StartPoint.y, RGB(0, 0, 0));
			DDALine(pDC, point.x, m_StartPoint.y, m_StartPoint.x, m_StartPoint.y, RGB(0, 0, 0));
		}
		this->ReleaseDC(pDC);
		m_EndPoint = point;
	}
	//��6��

	CView::OnMouseMove(nFlags, point);
}

//����ֱ�߶εĹ�������ť�Ĵ�����
void CMFCDemoView::OnLine()
{
	// TODO:  �ڴ���������������
	//��6�������ͬʱ����ֱ�߶κ;���
	type = 1;//���õ�ǰҪ���Ƶ�ͼ��Ϊֱ�߶�
	//��6��
}

//���ƾ��εĹ�������ť�Ĵ�����
void CMFCDemoView::OnRectangle()
{
	// TODO:  �ڴ���������������
	//��6�������ͬʱ����ֱ�߶κ;���
	type = 2;//���õ�ǰҪ���Ƶ�ͼ��Ϊ����
	//��6��
}

//������ɫ�˵���Ĵ�����
void CMFCDemoView::OnSetcolor()
{
	// TODO:  �ڴ���������������
	//��8��������ɫ
	SetColor s;//����Ի��������
	s.m_r = r; s.m_g = g; s.m_b = b;//�öԻ����еı༭������ʾ��ǰ����ɫ����ֵ
	//����DoModal������ģ̬��ʽ��ʾ�Ի��򣬵��OK��ť�رնԻ���ʱ����������ֵΪIDOK
	if (s.DoModal() == IDOK){
		r = s.m_r; g = s.m_g; b = s.m_b;//���õ�ǰ��ɫ����ֵΪ�û����õķ���ֵ
	}
	//��8��
}

//�����ϰ������µ���Ϣ������
void CMFCDemoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��9���ƶ�ͼ��
	if (nChar == VK_LEFT){
		for (int i = 0; i<objList.GetSize(); i++){
			MapObj obj = objList.GetAt(i);
			obj.p1.x -= 1;
			obj.p2.x -= 1;
			objList.SetAt(i, obj);
		}
		this->Invalidate(true);//����������ͼ�ػ�
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
	//��9��

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
