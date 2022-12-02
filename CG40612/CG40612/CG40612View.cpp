
// CG40612View.cpp: CCG40612View 类的实现
//

#include "framework.h"
#include "pch.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG40612.h"
#endif

#include "CG40612Doc.h"
#include "CG40612View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCG40612View

IMPLEMENT_DYNCREATE(CCG40612View, CView)

BEGIN_MESSAGE_MAP(CCG40612View, CView)
// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_SETNODE, &CCG40612View::OnSetnode)
ON_COMMAND(ID_DELETENODE, &CCG40612View::OnDeletenode)
ON_COMMAND(ID_SETELLIPSE, &CCG40612View::OnSetellipse)
ON_COMMAND(ID_SETCIRCLE, &CCG40612View::OnSetcircle)
ON_COMMAND(ID_SETPOLYGON, &CCG40612View::OnSetpolygon)
ON_COMMAND(ID_TOGGLEFILL, &CCG40612View::OnTogglefill)
ON_COMMAND(ID_CUTPOLYGON, &CCG40612View::OnCutpolygon)
ON_COMMAND(ID_SHOWCOMPARE, &CCG40612View::OnShowcompare)
ON_COMMAND(ID_DELETEALL, &CCG40612View::OnDeleteall)
END_MESSAGE_MAP()

// CCG40612View 构造/析构

#define NAME_CARD_HEIGHT (16)
#define NAME_CARD_WIDTH (32)
static const int NAME_CARD[NAME_CARD_HEIGHT][NAME_CARD_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
     0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
     0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

CCG40612View::CCG40612View() noexcept { /* 清空数据 */
  MyFunc_DeleteAll();
}

CCG40612View::~CCG40612View() {}

BOOL CCG40612View::PreCreateWindow(CREATESTRUCT& cs) {
  //  CREATESTRUCT cs 来修改窗口类或样式

  return CView::PreCreateWindow(cs);
}

// CCG40612View 绘图

void CCG40612View::OnDraw(CDC* /*pDC*/) {
  CCG40612Doc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc) return;

  MyFunc_ShowAllItem();
}

// CCG40612View 打印

BOOL CCG40612View::OnPreparePrinting(CPrintInfo* pInfo) {
  // 默认准备
  return DoPreparePrinting(pInfo);
}

void CCG40612View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // 添加额外的打印前进行的初始化过程
}

void CCG40612View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // 添加打印后进行的清理过程
}

// CCG40612View 诊断

#ifdef _DEBUG
void CCG40612View::AssertValid() const { CView::AssertValid(); }

void CCG40612View::Dump(CDumpContext& dc) const { CView::Dump(dc); }

CCG40612Doc* CCG40612View::GetDocument() const  // 非调试版本是内联的
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG40612Doc)));
  return (CCG40612Doc*)m_pDocument;
}
#endif  //_DEBUG

// CCG40612View 消息处理程序

double CCG40612View::MyStaticFunc_GetPointDistance(CPoint p1, CPoint p2) {
#define sqr(x) ((double)(x) * (x))
  double ans = sqrt(sqr(p1.x - p2.x) + sqr(p1.y - p2.y));
  return ans;
#undef sqr
}

void CCG40612View::MyStaticFunc_Warning(const char* n_File, int n_Line,
                                        const char* n_Msg) {
  fprintf(stderr, "Warning: %s\n - line %d: %s\n", n_File, n_Line, n_Msg);
  fflush(stderr);
}

void CCG40612View::MyStaticFunc_DrawEllipse(CDC* pDC, RECT* pRect) {
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

void CCG40612View::MyMathFunc_DrawEllipse(CDC* pDC, RECT* pRect,
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

void CCG40612View::MyFunc_FillPolygon(CDC* pDC, const CPolygon& n_Polygon) {
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
    if (lastPos.x <= nowPos.x && nextPos.x < nowPos.x ||
        lastPos.x >= nowPos.x && nextPos.x > nowPos.x) {
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
#define sqr(x) ((double)(x) * (x))
      if (powNow && NAME_CARD[y % NAME_CARD_HEIGHT][x % NAME_CARD_WIDTH]) {
        pDC->SetPixel(
            pos.x, pos.y,
            MyMathFunc_GetRgbByHsv(
                (double)(sqrt(sqr(x) + sqr(y))) / NAME_CARD_WIDTH, 0.8, 0.8));
      }
#undef sqr
    }
  }
}

void CCG40612View::MyFunc_CutPolygons() {
  RECT rect = MyFunc_GetCutRect();
  if (rect.left == rect.right || rect.top == rect.bottom) {
    return; /* 没有面积，拒绝裁剪（很有可能是操作失误）*/
  }

  /* 裁剪之前对旧的数据进行备份 */
  m_LastCutRect = rect;
  m_LastPolygonMap = m_PolygonMap;

  for (auto& pr : m_PolygonMap) {
    MyFunc_CutPolygon(rect, pr.second); /* 试图切割每一个多边形 */
  }
}

RECT CCG40612View::MyFunc_GetCutRect() {
  RECT ans = {};
  ans.left = min(m_CutBegin.x, m_CutEnd.x);
  ans.right = max(m_CutBegin.x, m_CutEnd.x);
  ans.top = min(m_CutBegin.y, m_CutEnd.y);
  ans.bottom = max(m_CutBegin.y, m_CutEnd.y);
  return ans;
}

void CCG40612View::MyFunc_CutPolygon(RECT& rect, CPolygon& n_CPolygon) {
  /* 要将修改后的内容写回 Cpolygon */
  /* 构建边集合 */
  CEdgeList edgeList;
  auto& nodes = n_CPolygon.nodeIds;
  for (int i = 0; i < nodes.size(); i += 1) {
    CPoint lastNode =
        m_NodeMap[i == 0 ? nodes[nodes.size() - 1] : nodes[i - 1]].pos;
    edgeList.push_back({lastNode, m_NodeMap[nodes[i]].pos});
  }

  /* 进行四次切割，可能切着切着就空了 */
  MyStaticFunc_CutXAbove(edgeList, rect.left);
  MyStaticFunc_CutXBelow(edgeList, rect.right);
  MyStaticFunc_CutYAbove(edgeList, rect.top);
  MyStaticFunc_CutYBelow(edgeList, rect.bottom);

  nodes.clear();
  for (int i = 0; i < edgeList.size(); i += 1) {
    CPoint pn = edgeList[i].to;
    int nid = MyFunc_CheckCursorOnNode(pn);
    if (m_NodeMap[nid].pos != pn) {
      nid = MyFunc_AddNewNodeAt(pn);
    }
    nodes.push_back(nid);
  }
}

void CCG40612View::MyFunc_ShowCutSquare(CDC* pDC) {
  RECT rect = MyFunc_GetCutRect();
  if (m_CutBegin != m_CutEnd && m_CutBegin != CPoint{0, 0} && m_LButtonDown) {
    MyStaticFunc_DrawSquare(pDC, rect, CUT_SQUARE_COLOR);
  }
}

void CCG40612View::MyFunc_ShowLastCutSpuqre(CDC* pDC) {
  if (m_CompareMod) {
    /* 有面积，则绘制 */
    if (m_LastCutRect.right != m_LastCutRect.left &&
        m_LastCutRect.bottom != m_LastCutRect.top) {
      MyStaticFunc_DrawSquare(pDC, m_LastCutRect, CUT_SQUARE_COLOR);
    }
  }
}

void CCG40612View::MyStaticFunc_DrawNode(CDC* pDC, CPoint pos, COLORREF color) {
  RECT rect;
  rect.left = pos.x - NODE_RADIUS;
  rect.right = pos.x + NODE_RADIUS;
  rect.top = pos.y - NODE_RADIUS;
  rect.bottom = pos.y + NODE_RADIUS;

  // pDC->Ellipse(&rect);
  MyMathFunc_DrawEllipse(pDC, &rect, color);
}

bool CCG40612View::MyStaticFunc_CheckNodeInList(const CNodeIdList& n_NodeIdList,
                                                int nid) {
  for (const auto& id : n_NodeIdList) {
    if (id == nid) return true;
  }
  return false;
}

void CCG40612View::MyMathFunc_XorBuffer(std::vector<std::vector<int>>& buffer,
                                        CPoint from, CPoint to) {
  double dx = to.x - from.x;
  double dy = to.y - from.y;

  if (dx == 0) return;

  int minx = min(from.x, to.x);
  int basey = minx == from.x ? from.y : to.y;
  int maxx = max(from.x, to.x);

  for (int i = minx; i <= maxx; i += 1) {
    if (i == to.x) continue;

    int ynow = int((i - minx) / dx * dy + basey + 0.5);  // DDA
    buffer[i][ynow] ^= 1;
  }
}

static double mod(double a, double b) { return a - floor(a / b) * b; }

COLORREF CCG40612View::MyMathFunc_GetRgbByHsv(double H, double S, double V) {
  const double PI = 3.1415926535897932384626433832795;
  H = mod(H, 2 * PI);
  H = H / PI * 180;

  double C = V * S;
  double X = C * (1 - abs(mod(H / 60, 2) - 1));
  double m = V - C;
  double R_ = 0, G_ = 0, B_ = 0;

  if (0 <= H && H < 60) {
    R_ = C;
    G_ = X;
  } else if (60 <= H && H < 120) {
    R_ = X;
    G_ = C;
  } else if (120 <= H && H < 180) {
    G_ = C;
    B_ = X;
  } else if (180 <= H && H < 240) {
    G_ = X;
    B_ = C;
  } else if (240 <= H && H < 300) {
    B_ = C;
    R_ = X;
  } else {
    B_ = X;
    R_ = C;
  }

  int R = int(floor((R_ + m) * 255) + 0.5);
  int G = int(floor((G_ + m) * 255) + 0.5);
  int B = int(floor((B_ + m) * 255) + 0.5);
  return RGB(R, G, B);
}

void CCG40612View::MyStaticFunc_CutXAbove(CEdgeList& edgeList, int xLeft) {
  CEdgeList tmpFrom, tmpTo;
  tmpFrom = edgeList;
  for (auto& edge : tmpFrom) {
    if (edge.from.x >= xLeft &&
        edge.to.x >= xLeft) { /* 整条边都在 xLeft 右侧 */
      tmpTo.push_back(edge);
    } else if (edge.from.x < xLeft &&
               edge.to.x < xLeft) { /* 整条边都在 xLeft 左侧 */
      // Delete
    } else { /* 一半左来一半右 */
      double dx = edge.to.x - edge.from.x;
      double dy = edge.to.y - edge.from.y;
      int yLeft = int((xLeft - edge.from.x) / dx * dy + edge.from.y + 0.5);
      if (edge.from.x < xLeft) { /* 切掉前一半 */
        tmpTo.push_back({CPoint(xLeft, yLeft), edge.to});
      } else {
        /* 切掉后一半 */
        tmpTo.push_back({edge.from, CPoint(xLeft, yLeft)});
      }
    }
  }
  edgeList.clear();
  if (tmpTo.size() > 0) {
    edgeList.push_back(tmpTo[0]); /* 推入第一条边 */
    for (int i = 1; i < tmpTo.size(); i += 1) {
      CPoint lastNode = edgeList[edgeList.size() - 1].to;
      if (lastNode != tmpTo[i].from) {
        edgeList.push_back({lastNode, tmpTo[i].from});
      }
      edgeList.push_back(tmpTo[i]);
    }
    CPoint firstNode = edgeList[0].from;
    CPoint lastNode = edgeList[edgeList.size() - 1].to;
    if (firstNode != lastNode) {
      edgeList.push_back({lastNode, firstNode});
    }
  }
}

void CCG40612View::MyStaticFunc_CutYAbove(CEdgeList& es, int yTop) {
  for (auto& edge : es) {
    std::swap(edge.from.x, edge.from.y);
    std::swap(edge.to.x, edge.to.y);
  }
  MyStaticFunc_CutXAbove(es, yTop);
  for (auto& edge : es) {
    std::swap(edge.from.x, edge.from.y);
    std::swap(edge.to.x, edge.to.y);
  }
}

void CCG40612View::MyStaticFunc_CutXBelow(CEdgeList& es, int xRight) {
  for (auto& edge : es) {
    edge.from.x = -edge.from.x;
    edge.to.x = -edge.to.x;
  }
  MyStaticFunc_CutXAbove(es, -xRight);
  for (auto& edge : es) {
    edge.from.x = -edge.from.x;
    edge.to.x = -edge.to.x;
  }
}

void CCG40612View::MyStaticFunc_CutYBelow(CEdgeList& es, int yBottom) {
  for (auto& edge : es) {
    edge.from.y = -edge.from.y;
    edge.to.y = -edge.to.y;
  }
  MyStaticFunc_CutYAbove(es, -yBottom);
  for (auto& edge : es) {
    edge.from.y = -edge.from.y;
    edge.to.y = -edge.to.y;
  }
}

void CCG40612View::MyStaticFunc_DrawSquare(CDC* pDC, RECT rect,
                                           COLORREF color) {
  CPen pen(PS_SOLID, 1, color);
  CPen* oldpen = pDC->SelectObject(&pen);

  pDC->MoveTo({rect.left, rect.top});
  pDC->LineTo({rect.right, rect.top});
  pDC->LineTo({rect.right, rect.bottom});
  pDC->LineTo({rect.left, rect.bottom});
  pDC->LineTo({rect.left, rect.top});

  pDC->SelectObject(oldpen);
}

RECT CCG40612View::MyFunc_GetPolygonRect(const CPolygon& n_Polygon) {
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

int CCG40612View::MyFunc_GetCursorOnNodeId(CPoint n_CursorPosition) {
  for (auto& pr : m_NodeMap) {
    const auto& nodePos = pr.second;
    if (MyStaticFunc_GetPointDistance(nodePos.pos, n_CursorPosition) <=
        NODE_RADIUS * 1.5) {
      return pr.first;
    }
  }
  assert(m_NodeMap.count(UNDEFINED) == 0);
  return UNDEFINED;
}

bool CCG40612View::MyFunc_CheckCursorOnNode(CPoint n_CursorPosition) {
  return MyFunc_GetCursorOnNodeId(n_CursorPosition) != UNDEFINED;
}

void CCG40612View::MyFunc_TestAndDeleteNode(int nodeId) {
  if (m_NodeMap.count(nodeId) > 0) {
    auto nodePoint = m_NodeMap[nodeId];
    if (nodePoint.pos.x <= 0 &&
        nodePoint.pos.y <= 0) { /* 将结点移动到左上角之外就会被删除 */

      m_NodeMap.erase(nodeId);
    }
  }
}

int CCG40612View::MyFunc_AddNewNodeAt(CPoint point) {
  fprintf(stderr, "[CCG40612View::MyFunc_AddNewNodeAt] point = {%d,%d}\n",
          point.x, point.y);
  int nodeId = ++m_MaxObjId;
  CMyNode tmp;
  tmp.nodeId = nodeId;
  tmp.pos = point;
  tmp.tag = UNDEFINED; /* undefined. */
  m_NodeMap[nodeId] = tmp;

  return nodeId;
}

void CCG40612View::MyFunc_PutDownMovingNode() {
  if (m_NodeMap.count(m_PickUpNodeId) > 0) {
    MyFunc_TestAndDeleteNode(m_PickUpNodeId);
    m_PickUpNodeId = UNDEFINED;
  }
}

void CCG40612View::MyFunc_ShowAllItem() {
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

  MyFunc_ShowCutSquare(&dc);     /* 显示切割矩形 */
  MyFunc_ShowLastCutSpuqre(&dc); /* 显示上次的切割矩形 */
  MyFunc_ShowAllPolygon(&dc);    /* 绘制所有多边形 */
  MyFunc_ShowAllCircle(&dc);     /* 绘制所有圆 */
  MyFunc_ShowAllEllipse(&dc);    /* 绘制所有椭圆 */
  MyFunc_ShowAllNode(&dc);       /* 绘制所有结点 */

  /* 最后要记得显示到屏幕上 */
  pDC->BitBlt(rect.left, rect.top, zcRect.Width(), zcRect.Height(), &dc, 0, 0,
              SRCCOPY);
  ReleaseDC(pDC); /* 记得 release */
}

void CCG40612View::MyFunc_ShowAllNode(CDC* pDC) {
  MyFunc_SetTagsOnNode();

  /* 绘制所有的结点 */
  for (auto ptr = m_NodeMap.rbegin(); ptr != m_NodeMap.rend(); ptr++) {
    const auto& nodePoint = ptr->second;
    /* 选择合适的颜色 */
    COLORREF color =
        ptr->second.tag == UNDEFINED ? NODE_COLOR : NODE_SELECTED_COLOR;

    /* 对比模式下只显示有标签的结点 */
    if (!m_CompareMod || ptr->second.tag != UNDEFINED) {
      MyStaticFunc_DrawNode(pDC, nodePoint.pos, color); /* 绘制椭圆结点 */
    }
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

void CCG40612View::MyFunc_ChangeStateTo(int STATE_TO) {
  m_State = STATE_TO;
  m_SelectedNodeIdList.clear();
  m_Merge = false;
  m_CutBegin = m_CutEnd = {0, 0};
  for (auto& pr : m_NodeMap) {
    auto& node = pr.second;
    node.tag = UNDEFINED; /* < 0, undefined. */
  }
}

void CCG40612View::MyFunc_SetTagsOnNode() {
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

void CCG40612View::MyFunc_ShowAllEllipse(CDC* pDC) {
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

void CCG40612View::MyFunc_AddEllipseByNodeId(int id1, int id2) {
  assert(id1 != id2);

  int eid = ++m_MaxObjId;
  CEllipse tmp;
  tmp.ellipseId = eid;
  tmp.nodeIdA = id1;
  tmp.nodeIdB = id2;

  m_EllipseSet[eid] = tmp;
}

void CCG40612View::MyFunc_AddCircleByNodeId(int idC, int idR) {
  assert(idC != idR);
  int circleId = ++m_MaxObjId;

  CCircle tmp;
  tmp.circleId = circleId;
  tmp.nodeIdC = idC;
  tmp.nodeIdR = idR;

  m_CircleSet[circleId] = tmp;
}

void CCG40612View::MyFunc_ShowAllCircle(CDC* pDC) {
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

void CCG40612View::MyFunc_ShowAllPolygon(CDC* pDC) {
  if (!m_CompareMod) {
    MyFunc_ShowPolygonInMap(pDC, m_PolygonMap, 1, RGB(0, 0, 0));
  } else {
    MyFunc_ShowPolygonInMap(pDC, m_LastPolygonMap, 1, OLD_POLYGON_COLOR);
    MyFunc_ShowPolygonInMap(pDC, m_PolygonMap, 2, NEW_POLYGON_COLOR);
  }
}

void CCG40612View::MyFunc_ShowPolygonInMap(CDC* pDC, CPolygonMap& n_PolygonMap,
                                           int lineWidth, COLORREF color) {
  /* 删除额外的多边形，不要用 m_PolygonMap */
  CNodeIdList del;
  for (auto& pr : n_PolygonMap) {
    /* 显示一个多边形 */
    const auto& nodes = pr.second.nodeIds;
    if (nodes.size() <= 2) { /* 多边形至少有三个顶点 */
      del.push_back(pr.first);
      continue;
    }
    for (auto& id : nodes) { /* 多边形的每个顶点必须有效 */
      if (m_NodeMap.count(id) <= 0) {
        del.push_back(pr.first);
        break;
      }
    }
  }
  for (auto& pid : del) {
    n_PolygonMap.erase(pid);
  }

  /* 显示多边形 */
  CPen pen(PS_SOLID, lineWidth, color);
  CPen* oldPen = pDC->SelectObject(&pen);

  for (auto& pr : n_PolygonMap) {
    if (!m_LButtonDown && m_FillModOn && !m_CompareMod) { /* 移动时渲染开销大 */
      MyFunc_FillPolygon(pDC, pr.second);                 /* 涂色 */
    }

    const auto& nodes = pr.second.nodeIds;
    pDC->MoveTo(m_NodeMap[nodes[0]].pos);
    for (int i = 1; i < nodes.size(); i += 1) {
      pDC->LineTo(m_NodeMap[nodes[i]].pos);
    }
    pDC->LineTo(m_NodeMap[nodes[0]].pos);
  }

  pDC->SelectObject(oldPen);
}

void CCG40612View::MyFunc_PutDownMovingNodeAndMerge(CPoint point) {
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

void CCG40612View::MyFunc_NodeMergeInto(int idFrom, int idTo) {
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

int CCG40612View::MyFunc_GetOrCreateNode(CPoint point) {
  int nid1 = UNDEFINED;
  if (!MyFunc_CheckCursorOnNode(point)) {
    nid1 = MyFunc_AddNewNodeAt(point); /* 新建一个结点 */
  } else {
    nid1 = MyFunc_GetCursorOnNodeId(point);
  }
  return nid1;
}

void CCG40612View::MyFunc_DeleteNodeById(int idFrom) {
  m_NodeMap.erase(idFrom);
  MyFunc_ShowAllItem();
}

bool CCG40612View::MyFunc_CheckSelectedByNodeId(int nid) {
  return MyStaticFunc_CheckNodeInList(m_SelectedNodeIdList, nid);
}

void CCG40612View::MyFunc_AddNewPolygon() {
  int npId = ++m_MaxObjId;

  CPolygon tmp;
  tmp.polygonId = npId;
  tmp.nodeIds = m_SelectedNodeIdList;

  m_PolygonMap[npId] = tmp;
}

void CCG40612View::OnLButtonDown(UINT nFlags, CPoint point) {
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
    case STATE_CUTPOLYGON:
      m_CutBegin = point;
      break;
      // TODO: 填充其他的自动机切换
  }

  MyFunc_ShowAllItem();
  CView::OnLButtonDown(nFlags, point);
}

void CCG40612View::OnLButtonUp(UINT nFlags, CPoint point) {
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
      MyWarning("[CCG40612View::OnLButtonUp] m_State = STATE_SETELLIPSE.");
      break;
    case STATE_SETCIRCLE:
      MyWarning("[CCG40612View::OnLButtonUp] m_State = STATE_SETCIRCLE.");
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
    case STATE_CUTPOLYGON:
      m_CutEnd = point;
      MyFunc_CutPolygons(); /* 对所有多边形进行裁剪 */
      MyFunc_ChangeStateTo(STATE_FREE);
      break;
      // TODO: 鼠标抬起事件
  }

  MyFunc_ShowAllItem();
  CView::OnLButtonUp(nFlags, point);
}

void CCG40612View::OnMouseMove(UINT nFlags, CPoint point) {
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
    case STATE_CUTPOLYGON:
      if (m_LButtonDown) {
        m_CutEnd = m_CursorPos;
      }
      break;
      // TODO: MouseMove 事件处理
  }

  MyFunc_ShowAllItem();
  CView::OnMouseMove(nFlags, point);
}

void CCG40612View::OnSetnode() {
  /* 点击了工具栏上的 SetNode 按钮 */

  MyFunc_ChangeStateTo(STATE_SETNODE);
}

void CCG40612View::OnDeletenode() { MyFunc_ChangeStateTo(STATE_DELETENODE); }

void CMyNode::GetRect(RECT* rect) const {
  rect->left = pos.x - NODE_RADIUS;
  rect->right = pos.x + NODE_RADIUS;
  rect->top = pos.y - NODE_RADIUS;
  rect->bottom = pos.y + NODE_RADIUS;
}

void CCG40612View::OnSetellipse() { MyFunc_ChangeStateTo(STATE_SETELLIPSE); }

void CCG40612View::OnSetcircle() { MyFunc_ChangeStateTo(STATE_SETCIRCLE); }

void CCG40612View::OnSetpolygon() { MyFunc_ChangeStateTo(STATE_SETPOLYGON); }

void CCG40612View::OnTogglefill() {
  m_FillModOn = !m_FillModOn;
  MyFunc_ShowAllItem();
}

void CCG40612View::OnCutpolygon() { MyFunc_ChangeStateTo(STATE_CUTPOLYGON); }

void CCG40612View::OnShowcompare() { /* 开启/ 关闭 剪切对比模式*/
  m_CompareMod = !m_CompareMod;
  m_FillModOn = true;
  MyFunc_ShowAllItem();
}

void CCG40612View::OnDeleteall() {
  MyFunc_DeleteAll();
  MyFunc_ShowAllItem();
}

void CCG40612View::MyFunc_DeleteAll() {
  m_MaxObjId = 0;               /* 最大结点编号 */
  m_State = STATE_FREE;         /* 自动机状态 */
  m_PickUpNodeId = UNDEFINED;   /* 当前正在被移动的 NodeId */
  m_LButtonDown = false;        /* 鼠标左键是否按下 */
  m_FillModOn = true;           /* 填充模式 */
  m_Merge = false;              /* 是否对目标点进行合并 */
  m_CompareMod = false;         /* 剪切对比模式 */
  m_LastCutRect = RECT{};       /* 上次裁剪的矩形边框 */
  m_CursorPos = CPoint{};       /* 鼠标位置 */
  m_CutBegin = CPoint{};        /* 裁剪矩形起点 */
  m_CutEnd = CPoint{};          /* 裁剪举行终点 */
  m_NodeMap.clear();            /* 结点列表 */
  m_SelectedNodeIdList.clear(); /* 选中的结点 ID 列表 */
  m_EllipseSet.clear();         /* 椭圆集合 */
  m_CircleSet.clear();          /* 圆集合 */
  m_PolygonMap.clear();         /* 多边形集合 */
  m_LastPolygonMap.clear();     /* 多边形集合进行备份 */
}
