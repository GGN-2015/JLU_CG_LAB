
// CG00612View.cpp: CCG00612View 类的实现
//

#include "framework.h"
#include "pch.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG00612.h"
#endif

#include "CG00612Doc.h"
#include "CG00612View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCG00612View

IMPLEMENT_DYNCREATE(CCG00612View, CView)

BEGIN_MESSAGE_MAP(CCG00612View, CView)
// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_SETNODE, &CCG00612View::OnSetnode)
ON_COMMAND(ID_DELETENODE, &CCG00612View::OnDeletenode)
END_MESSAGE_MAP()

// CCG00612View 构造/析构

CCG00612View::CCG00612View() noexcept {
  // TODO: 在此处添加构造代码
}

CCG00612View::~CCG00612View() {}

BOOL CCG00612View::PreCreateWindow(CREATESTRUCT& cs) {
  // TODO: 在此处通过修改
  //  CREATESTRUCT cs 来修改窗口类或样式

  return CView::PreCreateWindow(cs);
}

// CCG00612View 绘图

void CCG00612View::OnDraw(CDC* /*pDC*/) {
  CCG00612Doc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc) return;

  // TODO: 在此处为本机数据添加绘制代码
  MyFunc_ShowAllItem();
}

// CCG00612View 打印

BOOL CCG00612View::OnPreparePrinting(CPrintInfo* pInfo) {
  // 默认准备
  return DoPreparePrinting(pInfo);
}

void CCG00612View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加额外的打印前进行的初始化过程
}

void CCG00612View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加打印后进行的清理过程
}

// CCG00612View 诊断

#ifdef _DEBUG
void CCG00612View::AssertValid() const { CView::AssertValid(); }

void CCG00612View::Dump(CDumpContext& dc) const { CView::Dump(dc); }

CCG00612Doc* CCG00612View::GetDocument() const  // 非调试版本是内联的
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG00612Doc)));
  return (CCG00612Doc*)m_pDocument;
}
#endif  //_DEBUG

// CCG00612View 消息处理程序

double CCG00612View::MyStaticFunc_GetPointDistance(CPoint p1, CPoint p2) {
#define sqr(x) ((double)(x) * (x))
  double ans = sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
  return ans;
#undef sqr
}

void CCG00612View::MyStaticFunc_Warning(const char* n_File, int n_Line,
                                        const char* n_Msg) {
  fprintf(stderr, "Warning: %s\n - line %d: %s\n", n_File, n_Line, n_Msg);
  fflush(stderr);
}

int CCG00612View::MyFunc_GetCursorOnNodeId(CPoint n_CursorPosition) {
  for (auto& pr : m_NodeMap) {
    const auto& nodePos = pr.second;
    if (MyStaticFunc_GetPointDistance(nodePos, n_CursorPosition) <=
        NODE_RADIUS) {
      return pr.first;
    }
  }
  assert(m_NodeMap.count(-1) == 0);
  return -1;
}

bool CCG00612View::MyFunc_CheckCursorOnNode(CPoint n_CursorPosition) {
  return MyFunc_GetCursorOnNodeId(n_CursorPosition) != -1;
}

void CCG00612View::MyFunc_TestAndDeleteNode(int nodeId) {
  if (m_NodeMap.count(nodeId) > 0) {
    auto nodePoint = m_NodeMap[nodeId];
    if (nodePoint.x <= 0 &&
        nodePoint.y <= 0) { /* 将结点移动到左上角之外就会被删除 */

      m_NodeMap.erase(nodeId);
    }
  }
}

void CCG00612View::MyFunc_AddNewNodeAt(CPoint point) {
  fprintf(stderr, "[CCG00612View::MyFunc_AddNewNodeAt] point = {%d,%d}\n",
          point.x, point.y);
  int nodeId = ++m_MaxNodeId;
  m_NodeMap[nodeId] = point;
}

void CCG00612View::MyFunc_PutDownMovingNode() {
  if (m_NodeMap.count(m_PickUpNodeId) > 0) {
    MyFunc_TestAndDeleteNode(m_PickUpNodeId);
    m_PickUpNodeId = -1;
  }
}

void CCG00612View::MyFunc_ShowAllItem() {
  CDC* pDC = this->GetDC();
  CRect zcRect;
  this->GetClientRect(&zcRect);

  /* 内存双缓冲 */
  CDC dc;
  dc.CreateCompatibleDC(pDC);
  CBitmap memBmp;
  memBmp.CreateCompatibleBitmap(&dc, zcRect.Width(), zcRect.Height());
  CBitmap* OldBmp = dc.SelectObject(&memBmp);

  /* 使用背景颜色清空屏幕 */
  CRect rect;
  GetClientRect(rect);
  CBrush BkBrush;
  BkBrush.CreateSolidBrush(BACKGROUND_COLOR);
  dc.FillRect(rect, &BkBrush);

  // TODO: 显示所有对象

  MyFunc_ShowAllNode(&dc); /* 绘制所有结点 */

  /* 最后要记得显示到屏幕上 */
  pDC->BitBlt(rect.left, rect.top, zcRect.Width(), zcRect.Height(), &dc, 0, 0,
              SRCCOPY);
  ReleaseDC(pDC); /* 记得 release */
}

void CCG00612View::MyFunc_ShowAllNode(CDC* pDC) {
  for (auto ptr = m_NodeMap.rbegin(); ptr != m_NodeMap.rend(); ptr++) {
    auto nodePoint = ptr->second;

    RECT rect;
    rect.left = nodePoint.x - NODE_RADIUS;
    rect.right = nodePoint.x + NODE_RADIUS;
    rect.top = nodePoint.y - NODE_RADIUS;
    rect.bottom = nodePoint.y + NODE_RADIUS;

    pDC->Ellipse(&rect);
  }
}

void CCG00612View::OnLButtonDown(UINT nFlags, CPoint point) {
  SetCapture(); /* 跟踪鼠标位置 */
  m_LButtonDown = true;

  switch (m_State) {
    case STATE_FREE: /* 自由状态，点击鼠标左键按下 */
      if (MyFunc_CheckCursorOnNode(point)) { /* 如果按到了结点 */
        m_PickUpNodeId = MyFunc_GetCursorOnNodeId(point);
        m_State = STATE_MOVENODE; /* 切换到状态 MOVE */
      }
    case STATE_SETNODE:
      /* 左键按下时不要响应，左键抬起时进行响应 */
      break;
    case STATE_MOVENODE:
      MyWarning("[m_State: STATE_MOVENODE] LButtonDown.");
      break;
    case STATE_DELETENODE:
      /* 左键按下时不响应，左键抬起时响应 */
      break;
      // TODO: 填充其他的自动机切换
  }
  CView::OnLButtonDown(nFlags, point);
}

void CCG00612View::OnLButtonUp(UINT nFlags, CPoint point) {
  ReleaseCapture();
  m_LButtonDown = false;

  switch (m_State) {
    case STATE_FREE:
      break;
    case STATE_SETNODE: /* 每次放下一个点就重新恢复自由状态 */
      MyFunc_AddNewNodeAt(point);
      MyFunc_ShowAllItem();
      m_State = STATE_FREE;
      break;
    case STATE_MOVENODE: /* 放下结点 */
      MyFunc_PutDownMovingNode();
      m_State = STATE_FREE;
      break;
    case STATE_DELETENODE:
      if (MyFunc_CheckCursorOnNode(point)) {
        int nodeId = MyFunc_GetCursorOnNodeId(point);
        m_NodeMap.erase(nodeId);
      }
      m_State = STATE_FREE;
      MyFunc_ShowAllItem();
      break;
      // TODO: 鼠标抬起事件
  }
  CView::OnLButtonUp(nFlags, point);
}

void CCG00612View::OnMouseMove(UINT nFlags, CPoint point) {
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  CPoint oldCursorPos = m_CursorPos;
  m_CursorPos = point;

  switch (m_State) {
    case STATE_FREE:
      break;
    case STATE_SETNODE:
      break;
    case STATE_MOVENODE:
      m_NodeMap[m_PickUpNodeId] = point;
      MyFunc_ShowAllItem();
      break;
    case STATE_DELETENODE:
      break;
      // TODO: MouseMove 事件处理
  }
  CView::OnMouseMove(nFlags, point);
}

void CCG00612View::OnSetnode() {
  /* 点击了工具栏上的 SetNode 按钮 */
  m_State = STATE_SETNODE;
}

void CCG00612View::OnDeletenode() { m_State = STATE_DELETENODE; }
