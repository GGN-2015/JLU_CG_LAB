﻿
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
ON_COMMAND(ID_SETELLIPSE, &CCG10612View::OnSetellipse)
ON_COMMAND(ID_SETCIRCLE, &CCG10612View::OnSetcircle)
END_MESSAGE_MAP()

// CCG10612View 构造/析构

CCG10612View::CCG10612View() noexcept {
  // TODO: 在此处添加构造代码
}

CCG10612View::~CCG10612View() {}

BOOL CCG10612View::PreCreateWindow(CREATESTRUCT& cs) {
  //  CREATESTRUCT cs 来修改窗口类或样式

  return CView::PreCreateWindow(cs);
}

// CCG10612View 绘图

void CCG10612View::OnDraw(CDC* /*pDC*/) {
  CCG10612Doc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc) return;

  MyFunc_ShowAllItem();
}

// CCG10612View 打印

BOOL CCG10612View::OnPreparePrinting(CPrintInfo* pInfo) {
  // 默认准备
  return DoPreparePrinting(pInfo);
}

void CCG10612View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // 添加额外的打印前进行的初始化过程
}

void CCG10612View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // 添加打印后进行的清理过程
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

void CCG10612View::MyStaticFunc_DrawEllipse(CDC* pDC, RECT* pRect) {
  CBrush* pBrush =
      CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)); /* 空刷子 */
  CBrush* oldBrush = pDC->SelectObject(pBrush);

  pDC->Ellipse(pRect);

  pDC->SelectObject(oldBrush);
}

void CCG10612View::MyStaticFunc_DrawNode(CDC* pDC, CPoint pos) {
  RECT rect;
  rect.left = pos.x - NODE_RADIUS;
  rect.right = pos.x + NODE_RADIUS;
  rect.top = pos.y - NODE_RADIUS;
  rect.bottom = pos.y + NODE_RADIUS;

  pDC->Ellipse(&rect);
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

int CCG10612View::MyFunc_AddNewNodeAt(CPoint point) {
  fprintf(stderr, "[CCG10612View::MyFunc_AddNewNodeAt] point = {%d,%d}\n",
          point.x, point.y);
  int nodeId = ++m_MaxObjId;
  CMyNode tmp;
  tmp.nodeId = nodeId;
  tmp.pos = point;
  tmp.tag = UNDEFINED; /* undefined. */
  m_NodeMap[nodeId] = tmp;

  return nodeId;
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

  MyFunc_ShowAllCircle(&dc);  /* 绘制所有圆 */
  MyFunc_ShowAllEllipse(&dc); /* 绘制所有椭圆 */
  MyFunc_ShowAllNode(&dc);    /* 绘制所有结点 */

  /* 最后要记得显示到屏幕上 */
  pDC->BitBlt(rect.left, rect.top, zcRect.Width(), zcRect.Height(), &dc, 0, 0,
              SRCCOPY);
  ReleaseDC(pDC); /* 记得 release */
}

void CCG10612View::MyFunc_ShowAllNode(CDC* pDC) {
  MyFunc_SetTagsOnNode();

  /* 绘制普通节点和选中结点时的画笔 */
  BOOL ret;
  CPen nodePen(PS_SOLID, NODE_WIDTH, NODE_COLOR);
  CPen selectedPen(PS_SOLID, NODE_WIDTH, NODE_SELECTED_COLOR);

  /* 绘制所有的结点 */
  for (auto ptr = m_NodeMap.rbegin(); ptr != m_NodeMap.rend(); ptr++) {
    const auto& nodePoint = ptr->second;
    /* 选择合适的画笔 */
    CPen* oldPen =
        pDC->SelectObject(nodePoint.tag == UNDEFINED ? &nodePen : &selectedPen);

    MyStaticFunc_DrawNode(pDC, nodePoint.pos); /* 绘制椭圆结点 */
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
  m_NodeIdList.clear();
  m_Merge = false;
  for (auto& pr : m_NodeMap) {
    auto& node = pr.second;
    node.tag = UNDEFINED; /* < 0, undefined. */
  }
}

void CCG10612View::MyFunc_SetTagsOnNode() {
  std::map<int, int> idToTag;

  for (int i = 0; i < m_NodeIdList.size(); i += 1) {
    int id = m_NodeIdList[i];
    idToTag[id] = i;
  }

  for (auto& pr : m_NodeMap) {
    const auto& nodeId = pr.first;
    auto& node = pr.second;

    if (idToTag.count(nodeId) > 0) {
      node.tag = idToTag[nodeId];
    } else {
      node.tag = UNDEFINED;
    }
  }
}

void CCG10612View::MyFunc_ShowAllEllipse(CDC* pDC) {
  /* 找到不存在的椭圆 */
  std::vector<int> delList;
  for (auto& pr : m_EllipseSet) {
    int id = pr.first;
    auto& ellipse = pr.second;
    if (m_NodeMap.count(ellipse.nodeIdA) <= 0 ||
        m_NodeMap.count(ellipse.nodeIdB) <= 0) { /* 椭圆不存在 */
      delList.push_back(id);
    }
  }

  /* 删除不存在的椭圆 */
  for (auto id : delList) {
    m_EllipseSet.erase(id);
  }

  /* 绘制存在的椭圆 */
  for (auto& pr : m_EllipseSet) {
    int id = pr.first;
    auto& ellipse = pr.second;
    CPoint pA = m_NodeMap[ellipse.nodeIdA].pos,
           pB = m_NodeMap[ellipse.nodeIdB].pos;

    RECT rect;
    rect.left = min(pA.x, pB.x);
    rect.right = max(pA.x, pB.x);
    rect.top = min(pA.y, pB.y);
    rect.bottom = max(pA.y, pB.y);
    MyStaticFunc_DrawEllipse(pDC, &rect);
  }
}

void CCG10612View::MyFunc_AddEllipseByNodeId(int id1, int id2) {
  assert(id1 != id2);

  int eid = ++m_MaxObjId;
  CEllipse tmp;
  tmp.ellipseId = eid;
  tmp.nodeIdA = id1;
  tmp.nodeIdB = id2;

  m_EllipseSet[eid] = tmp;
}

void CCG10612View::MyFunc_AddCircleByNodeId(int idC, int idR) {
  assert(idC != idR);
  int circleId = ++m_MaxObjId;

  CCircle tmp;
  tmp.circleId = circleId;
  tmp.nodeIdC = idC;
  tmp.nodeIdR = idR;

  m_CircleSet[circleId] = tmp;
}

void CCG10612View::MyFunc_ShowAllCircle(CDC* pDC) {
  /* 找到不存在的圆 */
  std::vector<int> delList;
  for (auto& pr : m_CircleSet) {
    int id = pr.first;
    auto& circle = pr.second;
    if (m_NodeMap.count(circle.nodeIdC) <= 0 ||
        m_NodeMap.count(circle.nodeIdR) <= 0) { /* 圆不存在 */
      delList.push_back(id);
    }
  }

  /* 删除不存在的圆 */
  for (auto id : delList) {
    m_CircleSet.erase(id);
  }

  /* 绘制存在的椭圆 */
  for (auto& pr : m_CircleSet) {
    int id = pr.first;
    auto& circle = pr.second;
    CPoint pA = m_NodeMap[circle.nodeIdC].pos,
           pB = m_NodeMap[circle.nodeIdR].pos;

    double dR = MyStaticFunc_GetPointDistance(pA, pB);

    RECT rect;
    rect.left = int(pA.x - dR + 0.5);
    rect.right = int(pA.x + dR + 0.5);
    rect.top = int(pA.y - dR + 0.5);
    rect.bottom = int(pA.y + dR + 0.5);
    MyStaticFunc_DrawEllipse(pDC, &rect);
  }
}

void CCG10612View::MyFunc_PutDownMovingNodeAndMerge(CPoint point) {
  int nid = UNDEFINED;
  for (auto& pr : m_NodeMap) {
    auto id = pr.first;
    const auto& node = pr.second;
    if (id == m_PickUpNodeId) continue;
    if (MyStaticFunc_GetPointDistance(node.pos, point) <= NODE_RADIUS) {
      nid = id;
      break;
    }
  }
  if (nid != UNDEFINED) {
    MyFunc_NodeMergeInto(m_PickUpNodeId, nid);
    m_PickUpNodeId = UNDEFINED;
  }
}

void CCG10612View::MyFunc_NodeMergeInto(int idFrom, int idTo) {
  /* 对圆形合并 */
  for (auto& pr : m_CircleSet) {
    if (pr.second.nodeIdR == idFrom) {
      pr.second.nodeIdR = idTo;
    }
  }

  /* 对椭圆合并 */
  for (auto& pr : m_EllipseSet) {
    if (pr.second.nodeIdB == idFrom) {
      pr.second.nodeIdB = idTo;
    }
  }

  // TODO: 对所有图形实现合并

  MyFunc_DeleteNodeById(idFrom);
}

int CCG10612View::MyFunc_GetOrCreateNode(CPoint point) {
  int nid1 = UNDEFINED;
  if (!MyFunc_CheckCursorOnNode(point)) {
    nid1 = MyFunc_AddNewNodeAt(point); /* 新建一个结点 */
  } else {
    nid1 = MyFunc_GetCursorOnNodeId(point);
  }
  return nid1;
}

void CCG10612View::MyFunc_DeleteNodeById(int idFrom) {
  m_NodeMap.erase(idFrom);
  MyFunc_ShowAllItem();
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
    case STATE_SETELLIPSE: {
      int nid1 = MyFunc_GetOrCreateNode(point);
      int nid2 = MyFunc_AddNewNodeAt(point); /* 新建另一个节点 */
      MyFunc_AddEllipseByNodeId(nid1, nid2);
      m_PickUpNodeId = nid2;
      m_State = STATE_MOVENODE; /* 切换到移动状态 */
      m_Merge = true;
    } break;
    case STATE_SETCIRCLE:
      int nid1 = MyFunc_GetOrCreateNode(point);
      int nid2 = MyFunc_AddNewNodeAt(point); /* 新建另一个节点 */
      MyFunc_AddCircleByNodeId(nid1, nid2);
      m_PickUpNodeId = nid2;
      m_State = STATE_MOVENODE; /* 切换到移动状态 */
      m_Merge = true;
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
      if (!m_Merge) {
        MyFunc_PutDownMovingNode();
      } else {
        MyFunc_PutDownMovingNodeAndMerge(point);
      }
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
    case STATE_SETELLIPSE:
      MyWarning("[CCG10612View::OnLButtonUp] m_State = STATE_SETELLIPSE.");
      break;
    case STATE_SETCIRCLE:
      MyWarning("[CCG10612View::OnLButtonUp] m_State = STATE_SETCIRCLE.");
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
    case STATE_SETELLIPSE:
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

void CCG10612View::OnSetellipse() { MyFunc_ChangeStateTo(STATE_SETELLIPSE); }

void CCG10612View::OnSetcircle() { MyFunc_ChangeStateTo(STATE_SETCIRCLE); }
