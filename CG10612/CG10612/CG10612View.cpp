
// CG10612View.cpp: CCG10612View 类的实现
//

#include "framework.h"
#include "pch.h"
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
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_SETNODE, &CCG10612View::OnSetnode)
ON_COMMAND(ID_DELETENODE, &CCG10612View::OnDeletenode)
END_MESSAGE_MAP()

// CCG10612View 构造/析构

CCG10612View::CCG10612View() noexcept {
  // TODO: 在此处添加构造代码
}

CCG10612View::~CCG10612View() {}

BOOL CCG10612View::PreCreateWindow(CREATESTRUCT& cs) {
  // TODO: 在此处通过修改
  //  CREATESTRUCT cs 来修改窗口类或样式

  return CView::PreCreateWindow(cs);
}

// CCG10612View 绘图

void CCG10612View::OnDraw(CDC* /*pDC*/) {
  CCG10612Doc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc) return;

  // TODO: 在此处为本机数据添加绘制代码
  MyFunc_ShowAllItem();
}

// CCG10612View 打印

BOOL CCG10612View::OnPreparePrinting(CPrintInfo* pInfo) {
  // 默认准备
  return DoPreparePrinting(pInfo);
}

void CCG10612View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加额外的打印前进行的初始化过程
}

void CCG10612View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加打印后进行的清理过程
}

// CCG10612View 诊断

#ifdef _DEBUG
void CCG10612View::AssertValid() const { CView::AssertValid(); }

void CCG10612View::Dump(CDumpContext& dc) const { CView::Dump(dc); }

CCG10612Doc* CCG10612View::GetDocument() const  // 非调试版本是内联的
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG10612Doc)));
  return (CCG10612Doc*)m_pDocument;
}
#endif  //_DEBUG

// CCG10612View 消息处理程序

double CCG10612View::MyStaticFunc_GetPointDistance(CPoint p1, CPoint p2) {
#define sqr(x) ((double)(x) * (x))
  double ans = sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
  return ans;
#undef sqr
}

void CCG10612View::MyStaticFunc_Warning(const char* n_File, int n_Line,
                                        const char* n_Msg) {
  fprintf(stderr, "Warning: %s\n - line %d: %s\n", n_File, n_Line, n_Msg);
  fflush(stderr);
}

int CCG10612View::MyFunc_GetCursorOnNodeId(CPoint n_CursorPosition) {
  for (auto& pr : m_NodeMap) {
    const auto& nodePos = pr.second;
    if (MyStaticFunc_GetPointDistance(nodePos.pos, n_CursorPosition) <=
        NODE_RADIUS) {
      return pr.first;
    }
  }
  assert(m_NodeMap.count(UNDEFINED) == 0);
  return UNDEFINED;
}

bool CCG10612View::MyFunc_CheckCursorOnNode(CPoint n_CursorPosition) {
  return MyFunc_GetCursorOnNodeId(n_CursorPosition) != UNDEFINED;
}

void CCG10612View::MyFunc_TestAndDeleteNode(int nodeId) {
  if (m_NodeMap.count(nodeId) > 0) {
    auto nodePoint = m_NodeMap[nodeId];
    if (nodePoint.pos.x <= 0 &&
        nodePoint.pos.y <= 0) { /* 将结点移动到左上角之外就会被删除 */

      m_NodeMap.erase(nodeId);
    }
  }
}

void CCG10612View::MyFunc_AddNewNodeAt(CPoint point) {
  fprintf(stderr, "[CCG10612View::MyFunc_AddNewNodeAt] point = {%d,%d}\n",
          point.x, point.y);
  int nodeId = ++m_MaxNodeId;
  CMyNode tmp;
  tmp.nodeId = nodeId;
  tmp.pos = point;
  tmp.tag = UNDEFINED; /* undefined. */
  m_NodeMap[nodeId] = tmp;
}

void CCG10612View::MyFunc_PutDownMovingNode() {
  if (m_NodeMap.count(m_PickUpNodeId) > 0) {
    MyFunc_TestAndDeleteNode(m_PickUpNodeId);
    m_PickUpNodeId = UNDEFINED;
  }
}

void CCG10612View::MyFunc_ShowAllItem() {
  CDC* pDC = this->GetDC();
  CRect zcRect;
  this->GetClientRect(&zcRect);

  /* 内存双缓冲 */
  CDC dc;
  dc.CreateCompatibleDC(pDC);
  CBitmap memBmp;
  memBmp.CreateCompatibleBitmap(pDC, zcRect.Width(),
                                zcRect.Height()); /* 这里不能用 &dc */
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

void CCG10612View::MyFunc_ShowAllNode(CDC* pDC) {
  /* 绘制普通节点和选中结点时的画笔 */
  BOOL ret;
  CPen nodePen(PS_SOLID, NODE_WIDTH, NODE_COLOR);
  CPen selectedPen(PS_SOLID, NODE_WIDTH, NODE_SELECTED_COLOR);

  /* 绘制所有的结点 */
  for (auto ptr = m_NodeMap.rbegin(); ptr != m_NodeMap.rend(); ptr++) {
    const auto& nodePoint = ptr->second;

    /* 获取矩形位置 */
    RECT rect;
    nodePoint.GetRect(&rect);

    /* 选择合适的画笔 */
    CPen* oldPen =
        pDC->SelectObject(nodePoint.tag == UNDEFINED ? &nodePen : &selectedPen);

    pDC->Ellipse(&rect); /* 绘制椭圆结点 */
    pDC->SelectObject(oldPen);
  }

  /* 绘制所有的文字 */
  for (auto ptr = m_NodeMap.rbegin(); ptr != m_NodeMap.rend(); ptr++) {
    const auto& nodePoint = ptr->second;

    RECT rect;
    nodePoint.GetRect(&rect);

    if (nodePoint.tag != UNDEFINED) {
      wchar_t wbuf[30] = {};
      wsprintf(wbuf, L"%d", nodePoint.tag);
      RECT nRect = rect;
      nRect.left += 2 * NODE_RADIUS;
      nRect.top -= 2 * NODE_RADIUS;
      nRect.right += 10 * NODE_RADIUS;
      nRect.bottom += NODE_RADIUS;
      pDC->DrawText(wbuf, &nRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
  }

  nodePen.DeleteObject();
  selectedPen.DeleteObject();
}

void CCG10612View::MyFunc_ChangeStateTo(int STATE_TO) {
  m_State = STATE_TO;
  for (auto& pr : m_NodeMap) {
    auto& node = pr.second;
    node.tag = UNDEFINED; /* < 0, undefined. */
  }
}

void CCG10612View::OnLButtonDown(UINT nFlags, CPoint point) {
  SetCapture(); /* 跟踪鼠标位置 */
  m_LButtonDown = true;

  switch (m_State) {
    case STATE_FREE: /* 自由状态，点击鼠标左键按下 */
      if (MyFunc_CheckCursorOnNode(point)) { /* 如果按到了结点 */
        m_PickUpNodeId = MyFunc_GetCursorOnNodeId(point);
        MyFunc_ChangeStateTo(STATE_MOVENODE); /* 切换到状态 MOVE */
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

void CCG10612View::OnLButtonUp(UINT nFlags, CPoint point) {
  ReleaseCapture();
  m_LButtonDown = false;

  switch (m_State) {
    case STATE_FREE:
      break;
    case STATE_SETNODE: /* 每次放下一个点就重新恢复自由状态 */
      MyFunc_AddNewNodeAt(point);
      MyFunc_ShowAllItem();
      MyFunc_ChangeStateTo(STATE_FREE);
      break;
    case STATE_MOVENODE: /* 放下结点 */
      MyFunc_PutDownMovingNode();
      MyFunc_ChangeStateTo(STATE_FREE);
      break;
    case STATE_DELETENODE:
      if (MyFunc_CheckCursorOnNode(point)) {
        int nodeId = MyFunc_GetCursorOnNodeId(point);
        m_NodeMap.erase(nodeId);
      }
      MyFunc_ChangeStateTo(STATE_FREE);
      MyFunc_ShowAllItem();
      break;
      // TODO: 鼠标抬起事件
  }
  CView::OnLButtonUp(nFlags, point);
}

void CCG10612View::OnMouseMove(UINT nFlags, CPoint point) {
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  CPoint oldCursorPos = m_CursorPos;
  m_CursorPos = point;

  switch (m_State) {
    case STATE_FREE:
      break;
    case STATE_SETNODE:
      break;
    case STATE_MOVENODE:
      m_NodeMap[m_PickUpNodeId].pos = point; /* 修改结点坐标 */
      MyFunc_ShowAllItem();
      break;
    case STATE_DELETENODE:
      break;
      // TODO: MouseMove 事件处理
  }
  CView::OnMouseMove(nFlags, point);
}

void CCG10612View::OnSetnode() {
  /* 点击了工具栏上的 SetNode 按钮 */

  MyFunc_ChangeStateTo(STATE_SETNODE);
}

void CCG10612View::OnDeletenode() { MyFunc_ChangeStateTo(STATE_DELETENODE); }

void CMyNode::GetRect(RECT* rect) const {
  rect->left = pos.x - NODE_RADIUS;
  rect->right = pos.x + NODE_RADIUS;
  rect->top = pos.y - NODE_RADIUS;
  rect->bottom = pos.y + NODE_RADIUS;
}
