
// CG20612View.cpp: CCG20612View 类的实现
//

#include "framework.h"
#include "pch.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG20612.h"
#endif

#include "CG20612Doc.h"
#include "CG20612View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCG20612View

IMPLEMENT_DYNCREATE(CCG20612View, CView)

BEGIN_MESSAGE_MAP(CCG20612View, CView)
// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_SETNODE, &CCG20612View::OnSetnode)
ON_COMMAND(ID_DELETENODE, &CCG20612View::OnDeletenode)
ON_COMMAND(ID_SETELLIPSE, &CCG20612View::OnSetellipse)
ON_COMMAND(ID_SETCIRCLE, &CCG20612View::OnSetcircle)
ON_COMMAND(ID_SETPOLYGON, &CCG20612View::OnSetpolygon)
END_MESSAGE_MAP()

// CCG20612View 构造/析构

CCG20612View::CCG20612View() noexcept {
  // TODO: 在此处添加构造代码
}

CCG20612View::~CCG20612View() {}

BOOL CCG20612View::PreCreateWindow(CREATESTRUCT& cs) {
  //  CREATESTRUCT cs 来修改窗口类或样式

  return CView::PreCreateWindow(cs);
}

// CCG20612View 绘图

void CCG20612View::OnDraw(CDC* /*pDC*/) {
  CCG20612Doc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc) return;

  MyFunc_ShowAllItem();
}

// CCG20612View 打印

BOOL CCG20612View::OnPreparePrinting(CPrintInfo* pInfo) {
  // 默认准备
  return DoPreparePrinting(pInfo);
}

void CCG20612View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // 添加额外的打印前进行的初始化过程
}

void CCG20612View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // 添加打印后进行的清理过程
}

// CCG20612View 诊断

#ifdef _DEBUG
void CCG20612View::AssertValid() const { CView::AssertValid(); }

void CCG20612View::Dump(CDumpContext& dc) const { CView::Dump(dc); }

CCG20612Doc* CCG20612View::GetDocument() const  // 非调试版本是内联的
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG20612Doc)));
  return (CCG20612Doc*)m_pDocument;
}
#endif  //_DEBUG

// CCG20612View 消息处理程序

double CCG20612View::MyStaticFunc_GetPointDistance(CPoint p1, CPoint p2) {
#define sqr(x) ((double)(x) * (x))
  double ans = sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
  return ans;
#undef sqr
}

void CCG20612View::MyStaticFunc_Warning(const char* n_File, int n_Line,
                                        const char* n_Msg) {
  fprintf(stderr, "Warning: %s\n - line %d: %s\n", n_File, n_Line, n_Msg);
  fflush(stderr);
}

void CCG20612View::MyStaticFunc_DrawEllipse(CDC* pDC, RECT* pRect) {
#define sqr(x) ((double)(x) * (x))
  CBrush* pBrush =
      CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)); /* 空刷子 */
  CBrush* oldBrush = pDC->SelectObject(pBrush);

  // pDC->Ellipse(pRect);
  /* 使用 SetPixel 实现 DrawEllipse. */

  MyMathFunc_DrawEllipse(pDC, pRect, ELLIPSE_COLOR);

  pDC->SelectObject(oldBrush);
#undef sqr
}

void CCG20612View::MyMathFunc_DrawEllipse(CDC* pDC, RECT* pRect,
                                          COLORREF color) {
#define sqr(x) ((double)(x) * (x))
  /* 计算圆心坐标 */
  int cx = int((pRect->left + pRect->right) / 2.0 + 0.5);
  int cy = int((pRect->top + pRect->bottom) / 2.0 + 0.5);

  /* 计算椭圆的轴线长度 */
  int a = int((pRect->right - pRect->left) / 2.0 + 0.5);
  int b = int((pRect->bottom - pRect->top) / 2.0 + 0.5);

  /* 从 y 轴正方向极值点开始绘制，所有坐标都是相对圆心坐标 */
  int dx = 0;
  int dy = b;

  /* 向右前进 */
  do {
    for (int i = 0; i < 4; i += 1) { /* 绘制四个对称点 */
      int rdx = (i & 1) ? dx : -dx;
      int rdy = (i & 2) ? dy : -dy;
      pDC->SetPixel(cx + rdx, cy + rdy, color);
    }
    /* 得到后继中点 */
    double mx = dx + 1;
    double my = dy - 0.5;
    /* 对中点计算判别式 */
    double delta = sqr(b) * sqr(mx) + sqr(a) * sqr(my) - sqr(a) * sqr(b);
    /* 得到后继 */
    if (delta >= 0) { /* 中点在圆外，选择 dx + 1, dy - 1 */
      dx++;
      dy--;
    } else { /* 中点在园内，选择 dx + 1, dy */
      dx++;
    }
    /* 计算中点处的法向量 */
    double vx = 2 * sqr(b) * mx;
    double vy = 2 * sqr(a) * my;
    if (vx >= vy) break; /* 进入横向绘图区域 */
  } while (true);

  do {
    for (int i = 0; i < 4; i += 1) { /* 绘制四个对称点 */
      int rdx = (i & 1) ? dx : -dx;
      int rdy = (i & 2) ? dy : -dy;
      pDC->SetPixel(cx + rdx, cy + rdy, color);
    }
    /* 得到后继中点 */
    double mx = dx + 0.5;
    double my = dy - 1;
    /* 对中点计算判别式 */
    double delta = sqr(b) * sqr(mx) + sqr(a) * sqr(my) - sqr(a) * sqr(b);
    /* 得到后继 */
    if (delta >= 0) { /* 中点在圆外，选择 dx, dy - 1 */
      dy--;
    } else { /* 中点在圆内，选择 dx + 1, dy - 1*/
      dx++;
      dy--;
    }
  } while (dy >= 0);
#undef sqr
}

void CCG20612View::MyFunc_FillPolygon(CDC* pDC, const CPolygon& n_Polygon) {
  RECT rect;
  rect = MyFunc_GetPolygonRect(n_Polygon);

  /* 区域宽度, 区域高度 */
  int wt = rect.right - rect.left + 1;
  int ht = rect.bottom - rect.top + 1;

  /* 记录左上角 */
  CPoint topleft;
  topleft.x = rect.left;
  topleft.y = rect.top;

  /* 申请临时缓冲区 */
  std::vector<std::vector<int>> buffer;
  for (int i = 0; i < wt; i += 1) {
    buffer.push_back(std::vector<int>(ht, 0)); /* wt * ht 个 0 */
  }

  /* 实现一个边填充算法 */
  const auto& nodes = n_Polygon.nodeIds;
  for (int i = 1; i < nodes.size(); i += 1) {
    CPoint from = m_NodeMap[nodes[i - 1]].pos;
    CPoint to = m_NodeMap[nodes[i]].pos;
    MyMathFunc_XorBuffer(buffer, from - topleft, to - topleft);
  }
  {
    CPoint from = m_NodeMap[nodes[nodes.size() - 1]].pos;
    CPoint to = m_NodeMap[nodes[0]].pos;
    MyMathFunc_XorBuffer(buffer, from - topleft, to - topleft);
  }

  CPoint lastPos, nowPos, nextPos;
  for (int i = 0; i < nodes.size(); i += 1) {
    lastPos = i == 0 ? m_NodeMap[nodes[nodes.size() - 1]].pos
                     : m_NodeMap[nodes[i - 1]].pos;
    nextPos = i == nodes.size() - 1 ? m_NodeMap[nodes[0]].pos
                                    : m_NodeMap[nodes[i + 1]].pos;
    nowPos = m_NodeMap[nodes[i]].pos;
    /* 极值点 */
    if (lastPos.x < nowPos.x && nextPos.x < nowPos.x ||
        lastPos.x > nowPos.x && nextPos.x > nowPos.x) {
      buffer[nowPos.x - topleft.x][nowPos.y - topleft.y] ^= 1;
    }
  }

  /* output */
  for (int x = 0; x < buffer.size(); x += 1) {
    int powNow = 0;
    for (int y = 0; y < buffer[x].size(); y += 1) {
      CPoint pos(x, y);
      pos += topleft;
      powNow ^= buffer[x][y];
      if (powNow) {
        pDC->SetPixel(pos.x, pos.y, RGB(0, 255, 0));
      }
    }
  }
}

void CCG20612View::MyStaticFunc_DrawNode(CDC* pDC, CPoint pos, COLORREF color) {
  RECT rect;
  rect.left = pos.x - NODE_RADIUS;
  rect.right = pos.x + NODE_RADIUS;
  rect.top = pos.y - NODE_RADIUS;
  rect.bottom = pos.y + NODE_RADIUS;

  // pDC->Ellipse(&rect);
  MyMathFunc_DrawEllipse(pDC, &rect, color);
}

bool CCG20612View::MyStaticFunc_CheckNodeInList(const CNodeIdList& n_NodeIdList,
                                                int nid) {
  for (const auto& id : n_NodeIdList) {
    if (id == nid) return true;
  }
  return false;
}

void CCG20612View::MyMathFunc_XorBuffer(std::vector<std::vector<int>>& buffer,
                                        CPoint from, CPoint to) {
  double dx = to.x - from.x;
  double dy = to.y - from.y;

  if (dx == 0) return;

  int minx = min(from.x, to.x);
  int basey = minx == from.x ? from.y : to.y;
  int maxx = max(from.x, to.x);

  for (int i = minx; i <= maxx; i += 1) {
    if (i == to.x) continue;

    int xnow = int((i - minx) / dx * dy + basey + 0.5);
    buffer[i][xnow] ^= 1;
  }
}

RECT CCG20612View::MyFunc_GetPolygonRect(const CPolygon& n_Polygon) {
  RECT rect = {};
  rect.left = rect.right = m_NodeMap[n_Polygon.nodeIds[0]].pos.x;
  rect.top = rect.bottom = m_NodeMap[n_Polygon.nodeIds[0]].pos.y;

  for (auto& nid : n_Polygon.nodeIds) {
    CPoint pos = m_NodeMap[nid].pos;
    rect.left = min(rect.left, pos.x);
    rect.right = max(rect.right, pos.x);
    rect.top = min(rect.top, pos.y);
    rect.bottom = max(rect.bottom, pos.y);
  }
  return rect;
}

int CCG20612View::MyFunc_GetCursorOnNodeId(CPoint n_CursorPosition) {
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

bool CCG20612View::MyFunc_CheckCursorOnNode(CPoint n_CursorPosition) {
  return MyFunc_GetCursorOnNodeId(n_CursorPosition) != UNDEFINED;
}

void CCG20612View::MyFunc_TestAndDeleteNode(int nodeId) {
  if (m_NodeMap.count(nodeId) > 0) {
    auto nodePoint = m_NodeMap[nodeId];
    if (nodePoint.pos.x <= 0 &&
        nodePoint.pos.y <= 0) { /* 将结点移动到左上角之外就会被删除 */

      m_NodeMap.erase(nodeId);
    }
  }
}

int CCG20612View::MyFunc_AddNewNodeAt(CPoint point) {
  fprintf(stderr, "[CCG20612View::MyFunc_AddNewNodeAt] point = {%d,%d}\n",
          point.x, point.y);
  int nodeId = ++m_MaxObjId;
  CMyNode tmp;
  tmp.nodeId = nodeId;
  tmp.pos = point;
  tmp.tag = UNDEFINED; /* undefined. */
  m_NodeMap[nodeId] = tmp;

  return nodeId;
}

void CCG20612View::MyFunc_PutDownMovingNode() {
  if (m_NodeMap.count(m_PickUpNodeId) > 0) {
    MyFunc_TestAndDeleteNode(m_PickUpNodeId);
    m_PickUpNodeId = UNDEFINED;
  }
}

void CCG20612View::MyFunc_ShowAllItem() {
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

  MyFunc_ShowAllPolygon(&dc); /* 绘制所有多边形 */
  MyFunc_ShowAllCircle(&dc);  /* 绘制所有圆 */
  MyFunc_ShowAllEllipse(&dc); /* 绘制所有椭圆 */
  MyFunc_ShowAllNode(&dc);    /* 绘制所有结点 */

  /* 最后要记得显示到屏幕上 */
  pDC->BitBlt(rect.left, rect.top, zcRect.Width(), zcRect.Height(), &dc, 0, 0,
              SRCCOPY);
  ReleaseDC(pDC); /* 记得 release */
}

void CCG20612View::MyFunc_ShowAllNode(CDC* pDC) {
  MyFunc_SetTagsOnNode();

  /* 绘制所有的结点 */
  for (auto ptr = m_NodeMap.rbegin(); ptr != m_NodeMap.rend(); ptr++) {
    const auto& nodePoint = ptr->second;
    /* 选择合适的颜色 */
    COLORREF color =
        ptr->second.tag == UNDEFINED ? NODE_COLOR : NODE_SELECTED_COLOR;

    MyStaticFunc_DrawNode(pDC, nodePoint.pos, color); /* 绘制椭圆结点 */
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
}

void CCG20612View::MyFunc_ChangeStateTo(int STATE_TO) {
  m_State = STATE_TO;
  m_SelectedNodeIdList.clear();
  m_Merge = false;
  for (auto& pr : m_NodeMap) {
    auto& node = pr.second;
    node.tag = UNDEFINED; /* < 0, undefined. */
  }
}

void CCG20612View::MyFunc_SetTagsOnNode() {
  std::map<int, int> idToTag;

  for (int i = 0; i < m_SelectedNodeIdList.size(); i += 1) {
    int id = m_SelectedNodeIdList[i];
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

void CCG20612View::MyFunc_ShowAllEllipse(CDC* pDC) {
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

void CCG20612View::MyFunc_AddEllipseByNodeId(int id1, int id2) {
  assert(id1 != id2);

  int eid = ++m_MaxObjId;
  CEllipse tmp;
  tmp.ellipseId = eid;
  tmp.nodeIdA = id1;
  tmp.nodeIdB = id2;

  m_EllipseSet[eid] = tmp;
}

void CCG20612View::MyFunc_AddCircleByNodeId(int idC, int idR) {
  assert(idC != idR);
  int circleId = ++m_MaxObjId;

  CCircle tmp;
  tmp.circleId = circleId;
  tmp.nodeIdC = idC;
  tmp.nodeIdR = idR;

  m_CircleSet[circleId] = tmp;
}

void CCG20612View::MyFunc_ShowAllCircle(CDC* pDC) {
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

void CCG20612View::MyFunc_ShowAllPolygon(CDC* pDC) {
  /* 删除额外的多边形 */
  CNodeIdList del;
  for (auto& pr : m_PolygonMap) {
    /* 显示一个多边形 */
    const auto& nodes = pr.second.nodeIds;
    for (auto& id : nodes) {
      if (m_NodeMap.count(id) <= 0) {
        del.push_back(pr.first);
        break;
      }
    }
  }
  for (auto& pid : del) {
    m_PolygonMap.erase(pid);
  }

  /* 显示多边形 */
  for (auto& pr : m_PolygonMap) {
    MyFunc_FillPolygon(pDC, pr.second); /* 涂色 */

    const auto& nodes = pr.second.nodeIds;
    pDC->MoveTo(m_NodeMap[nodes[0]].pos);
    for (int i = 1; i < nodes.size(); i += 1) {
      pDC->LineTo(m_NodeMap[nodes[i]].pos);
    }
    pDC->LineTo(m_NodeMap[nodes[0]].pos);
  }
}

void CCG20612View::MyFunc_PutDownMovingNodeAndMerge(CPoint point) {
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

void CCG20612View::MyFunc_NodeMergeInto(int idFrom, int idTo) {
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

int CCG20612View::MyFunc_GetOrCreateNode(CPoint point) {
  int nid1 = UNDEFINED;
  if (!MyFunc_CheckCursorOnNode(point)) {
    nid1 = MyFunc_AddNewNodeAt(point); /* 新建一个结点 */
  } else {
    nid1 = MyFunc_GetCursorOnNodeId(point);
  }
  return nid1;
}

void CCG20612View::MyFunc_DeleteNodeById(int idFrom) {
  m_NodeMap.erase(idFrom);
  MyFunc_ShowAllItem();
}

bool CCG20612View::MyFunc_CheckSelectedByNodeId(int nid) {
  return MyStaticFunc_CheckNodeInList(m_SelectedNodeIdList, nid);
}

void CCG20612View::MyFunc_AddNewPolygon() {
  int npId = ++m_MaxObjId;

  CPolygon tmp;
  tmp.polygonId = npId;
  tmp.nodeIds = m_SelectedNodeIdList;

  m_PolygonMap[npId] = tmp;
}

void CCG20612View::OnLButtonDown(UINT nFlags, CPoint point) {
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
    case STATE_SETCIRCLE: {
      int nid1 = MyFunc_GetOrCreateNode(point);
      int nid2 = MyFunc_AddNewNodeAt(point); /* 新建另一个节点 */
      MyFunc_AddCircleByNodeId(nid1, nid2);
      m_PickUpNodeId = nid2;
      m_State = STATE_MOVENODE; /* 切换到移动状态 */
      m_Merge = true;
    } break;
    case STATE_SETPOLYGON:
      break;
      // TODO: 填充其他的自动机切换
  }

  MyFunc_ShowAllItem();
  CView::OnLButtonDown(nFlags, point);
}

void CCG20612View::OnLButtonUp(UINT nFlags, CPoint point) {
  ReleaseCapture();
  m_LButtonDown = false;

  switch (m_State) {
    case STATE_FREE:
      break;
    case STATE_SETNODE: /* 每次放下一个点就重新恢复自由状态 */
      MyFunc_AddNewNodeAt(point);
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
      break;
    case STATE_SETELLIPSE:
      MyWarning("[CCG20612View::OnLButtonUp] m_State = STATE_SETELLIPSE.");
      break;
    case STATE_SETCIRCLE:
      MyWarning("[CCG20612View::OnLButtonUp] m_State = STATE_SETCIRCLE.");
      break;
    case STATE_SETPOLYGON: {
      int nid = MyFunc_GetOrCreateNode(point);
      if (m_SelectedNodeIdList.size() >= 3 &&
          (m_SelectedNodeIdList[0] == nid ||
           m_SelectedNodeIdList[m_SelectedNodeIdList.size() - 1] ==
               nid)) {          /* 结束当前多边形的绘制 */
        MyFunc_AddNewPolygon(); /* 新建多边形 */
        MyFunc_ChangeStateTo(STATE_FREE);
      } else if (!MyFunc_CheckSelectedByNodeId(nid)) {
        m_SelectedNodeIdList.push_back(nid); /* 增加新的结点 */
      }
    } break;
      // TODO: 鼠标抬起事件
  }

  MyFunc_ShowAllItem();
  CView::OnLButtonUp(nFlags, point);
}

void CCG20612View::OnMouseMove(UINT nFlags, CPoint point) {
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
      break;
    case STATE_DELETENODE:
      break;
    case STATE_SETELLIPSE:
      break;
      // TODO: MouseMove 事件处理
  }

  MyFunc_ShowAllItem();
  CView::OnMouseMove(nFlags, point);
}

void CCG20612View::OnSetnode() {
  /* 点击了工具栏上的 SetNode 按钮 */

  MyFunc_ChangeStateTo(STATE_SETNODE);
}

void CCG20612View::OnDeletenode() { MyFunc_ChangeStateTo(STATE_DELETENODE); }

void CMyNode::GetRect(RECT* rect) const {
  rect->left = pos.x - NODE_RADIUS;
  rect->right = pos.x + NODE_RADIUS;
  rect->top = pos.y - NODE_RADIUS;
  rect->bottom = pos.y + NODE_RADIUS;
}

void CCG20612View::OnSetellipse() { MyFunc_ChangeStateTo(STATE_SETELLIPSE); }

void CCG20612View::OnSetcircle() { MyFunc_ChangeStateTo(STATE_SETCIRCLE); }

void CCG20612View::OnSetpolygon() { MyFunc_ChangeStateTo(STATE_SETPOLYGON); }
