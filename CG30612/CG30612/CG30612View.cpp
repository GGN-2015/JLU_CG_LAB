
// CG30612View.cpp: CCG30612View 类的实现
//

#include "framework.h"
#include "pch.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CG30612.h"
#endif

#include "CG30612Doc.h"
#include "CG30612View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <cassert>

// CCG30612View

IMPLEMENT_DYNCREATE(CCG30612View, CView)

BEGIN_MESSAGE_MAP(CCG30612View, CView)
// 标准打印命令
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_SIZE()
ON_WM_KEYDOWN()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_TIMER()
ON_COMMAND(ID_TOGGLECOLOR, &CCG30612View::OnTogglecolor)
ON_COMMAND(ID_Orthographic, &CCG30612View::OnOrthographic)
ON_COMMAND(ID_PERSPECTIVE, &CCG30612View::OnPerspective)
END_MESSAGE_MAP()

// CCG30612View 构造/析构

static int m_ProjectMethod =
    PROJECTMETHOD_PERSPECTIVE; /* 改到后来懒得设计了，直接设成全局变量了 */

static void EnablePrintfAtMFC() {
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    FILE* pCout;
    freopen_s(&pCout, "CONOUT$", "w", stdout);
    std::cout.clear();
    std::wcout.clear();
  }
}

CCG30612View::CCG30612View() noexcept {
  // TODO: 在此处添加构造代码

  EnablePrintfAtMFC();
  m_Center = {3.464, 3.464, 3.464};

  /* 插入大正方形 */
  CCube tmpCube = {};
  tmpCube.pos = {0, 0, 0};
  tmpCube.up = {0, 0, 1};
  tmpCube.left = {0, 1, 0};
  tmpCube.d = 1;
  MyFunc_AddCube(tmpCube);

  /* 插入小正方形 */
  tmpCube.pos = {0, 0, 0.5 + 0.5 * sqrt(2) / 2};
  tmpCube.up = {0, sqrt(2) / 2, sqrt(2) / 2};
  tmpCube.left = {0, -sqrt(2) / 2, sqrt(2) / 2};
  tmpCube.d = 0.5;
  tmpCube.p_CubeChanger = math::ZSpinner;
  MyFunc_AddCube(tmpCube);
}

CCG30612View::~CCG30612View() {}

BOOL CCG30612View::PreCreateWindow(CREATESTRUCT& cs) {
  // TODO: 在此处通过修改
  //  CREATESTRUCT cs 来修改窗口类或样式

  return CView::PreCreateWindow(cs);
}

// CCG30612View 绘图

void CCG30612View::OnDraw(CDC* /*pDC*/) {
  CCG30612Doc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);
  if (!pDoc) return;

  MyFunc_ShowAllItem();
}

// CCG30612View 打印

BOOL CCG30612View::OnPreparePrinting(CPrintInfo* pInfo) {
  // 默认准备
  return DoPreparePrinting(pInfo);
}

void CCG30612View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加额外的打印前进行的初始化过程
}

void CCG30612View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/) {
  // TODO: 添加打印后进行的清理过程
}

// CCG30612View 诊断

#ifdef _DEBUG
void CCG30612View::AssertValid() const { CView::AssertValid(); }

void CCG30612View::Dump(CDumpContext& dc) const { CView::Dump(dc); }

RECT CCG30612View::MyStaticFunc_GetRectFromCSurface2dList(
    CSurface2dList& slist) {
  RECT ans = {};
  if (slist.size() != 0) {
    ans.left = ans.right = (int)(slist[0].x[0].GetX() + 0.5);
    ans.top = ans.bottom = (int)(slist[0].x[0].GetY() + 0.5);
  }
  for (auto& surf : slist) {
    for (int i = 0; i <= 3; i += 1) {
      ans.left = min(ans.left, int(surf.x[i].GetX() + 0.5));
      ans.right = max(ans.right, int(surf.x[i].GetX() + 0.5));
      ans.top = min(ans.top, int(surf.x[i].GetY() + 0.5));
      ans.bottom = max(ans.bottom, int(surf.x[i].GetY() + 0.5));
    }
  }
  return ans; /* 用于确定缓冲区大小 */
}

CVector2d CCG30612View::MyFunc_ProjectionLogicToDevice(CVector2d pos) {
  return math::GetDevicePos(pos, m_Width, m_Height);
}

CVector2d CCG30612View::MyFunc_ProjectionWorldToLogic(CVector3d pos) {
  if (m_ProjectMethod == PROJECTMETHOD_ORTHOGRAPHIC) { /* 平行投影投影 */
    CPlane plane = math::GetProjectionPlaneByPoint(m_Center);
    return math::GetPrjV2d(pos, plane, math::vadd(m_Center, pos));

  } else { /* 透视投影*/
    CPlane plane = math::GetProjectionPlaneByPoint(m_Center);
    return math::GetPrjV2d(pos, plane, m_Center);
  }
}

CVector2d CCG30612View::MyFunc_ProjectionWorldToDevice(CVector3d pos) {
  return MyFunc_ProjectionLogicToDevice(MyFunc_ProjectionWorldToLogic(pos));
}

void CCG30612View::MyFunc_ShowAllItem() {
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

  MyFunc_ShowAllItem(&dc);

  /* 最后要记得显示到屏幕上 */
  pDC->BitBlt(rect.left, rect.top, zcRect.Width(), zcRect.Height(), &dc, 0, 0,
              SRCCOPY);
  ReleaseDC(pDC); /* 记得 release */
}

void CCG30612View::MyFunc_ShowAllItem(CDC* pDC) {
  /* 统一距离 */
  m_Center = math::vmul(math::vuni(m_Center), math::c_CenterDistance);

  MyFunc_ShowHelpText(pDC);
  MyFunc_ShowAllCubes(pDC); /* 绘制所有正方体 */
}

void CCG30612View::MyFunc_ShowAllCubes(CDC* pDC) {
  if (m_LineMode) { /* 线框模式 */
    for (auto& pr : m_CubeMap) {
      MyFunc_ShowCube(pDC, &pr.second);
    }
  } else {
    /* 获得所有平面 */
    CSurface2dList slist;
    for (auto& pr : m_CubeMap) {
      MyFunc_GetSurfaceForCube(slist, &pr.second);
    }

    /* 确定缓冲区大小 */
    RECT rect = MyStaticFunc_GetRectFromCSurface2dList(slist);

    int wt = rect.right - rect.left + 1;
    int ht = rect.bottom - rect.top + 1;
    CVector2d topleft = {};
    topleft.GetX() = rect.left;
    topleft.GetY() = rect.top;

    /* 申请 z buffer 缓冲 */
    CZBuffer zbuf(wt, ht, topleft);
    for (auto& surface : slist) {
      /* 向缓冲中增加平面 */
      if (surface.sid >= 0) {
        zbuf.AddSurface(surface);
      }
    }

    /* 绘制到 DC */
    zbuf.OutputToDC(pDC);

    /*for (auto& pr : m_CubeMap) {
      MyFunc_ShowCube(pDC, &pr.second);
    }*/
  }
}

void CCG30612View::MyFunc_ShowCube(CDC* pDC, CCube* cube) {
  if (cube != nullptr) {
    // assert(false);

    cube->Unify();
    if (cube->p_CubeChanger != nullptr && m_LineMode) { /* 更改正方体的姿态 */
      double tnow = math::GetClockTime();
      (cube->p_CubeChanger)(cube, tnow);
      cube->t = tnow;
      cube->Unify();
    }

    MyFunc_ShowLinesForCube(pDC, cube); /* 显示正方体的所有棱 */

  } else {
    assert(false);
  }
}

void CCG30612View::MyFunc_GetCornersForCube(const CCube* cube,
                                            CVector3d* corners) {
  CVector3d front = math::vcross(cube->left, cube->up);
  front = math::vuni(front);

  /* 确定八个顶点 */
  const CVector3d* arr[3] = {&front, &cube->left, &cube->up};
  for (int i = 0; i <= 7; i += 1) {
    int bits[3] = {i & 1, (i >> 1) & 1,
                   (i >> 2) & 1}; /* 0: negative, 1: positive */
    corners[i] = cube->pos;
    CVector3d tmp;
    for (int d = 0; d <= 2; d += 1) {
      if (bits[d]) {
        corners[i] = tmp =
            math::vadd(corners[i], math::vmul(*arr[d], cube->d / 2.0));
      } else {
        corners[i] = tmp =
            math::vadd(corners[i], math::vmul(*arr[d], -cube->d / 2.0));
      }
    }
  }
}

void CCG30612View::MyFunc_AddCube(CCube n_Cube) {
  int cid = ++m_EntityId;
  m_CubeMap[cid] = n_Cube;
}

void CCG30612View::MyFunc_ImmediateShow() {
  MyFunc_ShowAllItem(); /* 更新显示区域 */
}

void CCG30612View::MyFunc_ShowHelpText(CDC* pDC) {
  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = 400;
  rect.bottom = 30;
  pDC->DrawText(TEXT("使用 “↑”、“↓”、“←”、“→” 控制视角转动"), &rect,
                DT_SINGLELINE | DT_CENTER | DT_VCENTER);

  rect.left = 0;
  rect.top = 30;
  rect.right = 400;
  rect.bottom = 60;
  pDC->DrawText(TEXT("也可使用鼠标拖拽控制视角转动"), &rect,
                DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void CCG30612View::MyFunc_SetCenter(CPoint dpos) {
  if (dpos.x != 0) {
    double xtmp = dpos.x;
    double xstp = math::sgn(dpos.x) * math::c_PixelMoveEps;
    while (math::sgn(xtmp) == math::sgn(xstp)) { /* 直到变号为止 */
      if (xstp > 0) {
        MyFunc_MoveLeft();
      } else {
        MyFunc_MoveRight();
      }
      xtmp -= xstp;
    }
  }
  if (dpos.y != 0) {
    double ytmp = dpos.y;
    double ystp = math::sgn(dpos.y) * math::c_PixelMoveEps;
    while (math::sgn(ytmp) == math::sgn(ystp)) { /* 直到变号为止 */
      if (ystp > 0) {
        MyFunc_MoveUp();
      } else {
        MyFunc_MoveDown();
      }
      ytmp -= ystp;
    }
  }

  MyFunc_ImmediateShow();
}

void CCG30612View::MyFunc_MoveRight() {
  CVector3d left = math::vuni(math::vcross(m_Center, math::c_Vector3d_z));
  m_Center = math::vadd(m_Center, math::vmul(left, math::c_MoveEps));
}

void CCG30612View::MyFunc_MoveLeft() {
  CVector3d left = math::vuni(math::vcross(m_Center, math::c_Vector3d_z));
  m_Center = math::vadd(m_Center, math::vmul(left, -math::c_MoveEps));
}

void CCG30612View::MyFunc_MoveUp() {
  if (m_Center.GetZ() < math::c_z_max) {
    m_Center.GetZ() += math::c_MoveEps;
  }
}

void CCG30612View::MyFunc_MoveDown() {
  if (m_Center.GetZ() > math::c_z_min) {
    m_Center.GetZ() -= math::c_MoveEps;
  }
}

void CCG30612View::MyFunc_StartTimer(UINT id, UINT duration) {
  UINT_PTR TimerVal;
  TimerVal = SetTimer(id, duration, NULL);  // Starting the Timer
  assert(TimerVal != 0);
}

void CCG30612View::MyFunc_ShowLinesForCube(CDC* pDC, CCube* cube) {
  CVector3d corners[8] = {}; /* 确定八个顶点 */
  MyFunc_GetCornersForCube(cube, corners);

  /* 绘制 12 条棱 */
  int lcnt = 0;
  for (int i = 0; i <= 7; i += 1) {
    for (int j = i + 1; j <= 7; j += 1) {
      if ((i ^ j) == math::lowbit(i ^ j)) { /* 只有一位不同 */
        lcnt += 1;
        CVector2d from = MyFunc_ProjectionWorldToDevice(corners[i]);
        CVector2d to = MyFunc_ProjectionWorldToDevice(corners[j]);
        pDC->MoveTo((int)(from.GetX() + 0.5), (int)(from.GetY() + 0.5));
        pDC->LineTo((int)(to.GetX() + 0.5), (int)(to.GetY() + 0.5));
      }
    }
  }
}

void CCG30612View::MyFunc_ShowSurfaces(CDC* pDC, CSurface2dList& slist) {
  assert(false);
  for (auto& surface : slist) {
    // TODO: 显示一个面
  }
}

void CCG30612View::MyFunc_GetSurfaceForCube(CSurface2dList& slist,
                                            CCube* cube) {
  static const int idBuf[4] = {0, 2, 3, 1}; /* 枚举顺序 */

  CVector3d corners[12];
  MyFunc_GetCornersForCube(cube, corners);
  int nid = 0; /* 面的编号从 0 到 5 */

  /* 获得观察平面 */
  CPlane plane = math::GetProjectionPlaneByPoint(m_Center);

  for (int id = 0; id <= 2; id++) { /* id = 0, 前后, id = 1 左右, id = 2, 上下*/
    for (int bit = 0; bit <= 1; bit++) { /* bit = 1, 前左上, bit = 0, 后右下 */
      int idList[4] = {};
      for (int i = 0; i <= 3; i++) {
        int val = 0;
        int tmp = idBuf[i];
        for (int j = 0; j <= 2; j += 1) {
          if (j == id) {
            val = (val << 1) | bit;
          } else {
            val = (val << 1) | (tmp & 1);
            tmp >>= 1;
          }
        }
        idList[i] = val; /* 得到结点编号 */
      }

      /* 将多边形放到 Surface 中 */
      CSurface2d tmpSurf = {};
      tmpSurf.sid = nid++;
      for (int i = 0; i <= 3; i += 1) {
        tmpSurf.x[i] = MyFunc_ProjectionWorldToDevice(corners[idList[i]]);
        tmpSurf.z[i] = math::GetDisFromPointToPlane(corners[idList[i]], plane);
      }

      /* 尾部追加 */
      slist.push_back(tmpSurf);
    }
  }
}

double math::sgn(double x) {
  if (fabs(x) < 1e-6) {
    return 0;
  }
  if (x > 0) {
    return 1;
  } else {
    return -1;
  }
}

CCG30612Doc* CCG30612View::GetDocument() const  // 非调试版本是内联的
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCG30612Doc)));
  return (CCG30612Doc*)m_pDocument;
}
#endif  //_DEBUG

// CCG30612View 消息处理程序

double& CVector2d::GetX() { return x[0]; }

double& CVector2d::GetY() { return x[1]; }

double& CVector3d::GetX() { return x[0]; }

double& CVector3d::GetY() { return x[1]; }

double& CVector3d::GetZ() { return x[2]; }

CVector3d math::vmul(CVector3d v, double p) {
  for (int i = 0; i <= 2; i += 1) {
    v.x[i] *= p;
  }
  return v;
}

CVector2d math::vmul(CVector2d v, double p) {
  for (int i = 0; i <= 1; i += 1) {
    v.x[i] *= p;
  }
  return v;
}

double math::vlen(CVector3d v) {
  double ans = 0;
  for (int i = 0; i <= 2; i += 1) {
    ans += v.x[i] * v.x[i];
  }
  return sqrt(ans);
}

double math::vlen(CVector2d v) {
  double ans = 0;
  for (int i = 0; i <= 1; i += 1) {
    ans += v.x[i] * v.x[i];
  }
  return sqrt(ans);
}

double math::vdot(CVector3d v1, CVector3d v2) {
  double ans = 0;
  for (int i = 0; i <= 2; i += 1) {
    ans += v1.x[i] * v2.x[i];
  }
  return ans;
}

double math::vdot(CVector2d v1, CVector2d v2) {
  double ans = 0;
  for (int i = 0; i <= 1; i += 1) {
    ans += v1.x[i] * v2.x[i];
  }
  return ans;
}

CVector3d math::vcross(CVector3d v1, CVector3d v2) {
  CVector3d ans = {
      v1.GetY() * v2.GetZ() - v2.GetY() * v1.GetZ(), /* y1z2 - y2z1 */
      v1.GetZ() * v2.GetX() - v2.GetZ() * v1.GetX(), /* z1x2 - z2x1 */
      v1.GetX() * v2.GetY() - v2.GetX() * v1.GetY()  /* x1y2 - x2y1 */
  };
  return ans;
}

double math::vdprj(CVector3d v, CVector3d e) {
  double r = vdot(v, e);
  r /= vlen(e); /* 有向投影长度 */
  return r;
}

CVector3d math::vprj(CVector3d v, CVector3d e) {
  return vmul(vuni(e), vdprj(v, e));
}

CVector3d math::vrot(CVector3d v, CVector3d base, double rad) {
  if (vlen(v) < math::c_MathEps) return v;
  if (fabs(vdot(v, base) / (vlen(v) * vlen(base)) - 1) < math::c_MathEps) {
    return v;
  }
  CVector3d dir = vuni(v);
  CVector3d rdir = vadd(dir, vneg(vprj(dir, base))); /* 径向向量 */
  CVector3d front = vuni(rdir);                      /* 径向单位向量 */
  CVector3d tdir = vadd(dir, vneg(rdir));            /* 沿轴分量 */
  CVector3d left = vuni(vcross(base, rdir));

  CVector3d nrdir =
      vmul(vadd(vmul(front, cos(rad)), vmul(left, sin(rad))), vlen(rdir));
  CVector3d ndir = vadd(nrdir, tdir);

  CVector3d ans = vmul(ndir, vlen(v));
  return ans;
}

double math::GetDisFromPointToPlane(CVector3d p, CPlane plane) {
  CVector3d front = vcross(plane.up, plane.left);
  p = vadd(p, vneg(plane.pos));
  return fabs(vdprj(p, front));
}

int math::lowbit(int x) { return x & -x; }

CPlane math::GetProjectionPlaneByPoint(CVector3d pos) {
  CPlane tmp = {};
  tmp.pos = vmul(vuni(pos), c_ScreenDistance);
  tmp.up = vuni(vadd(c_Vector3d_z, vneg(vprj(c_Vector3d_z, pos))));
  tmp.left = vuni(vcross(tmp.pos, tmp.up));
  return tmp; /* 根据投影点计算投影平面 */
}

CVector3d math::vuni(CVector3d v) {
  double len = vlen(v);
  return vmul(v, 1.0 / len); /* 得到给定方向上的单位向量 */
}

CVector2d math::vuni(CVector2d v) {
  double len = vlen(v);
  return vmul(v, 1.0 / len); /* 得到给定方向上的单位向量 */
}

CVector3d math::vneg(CVector3d v) {
  CVector3d ans = {};
  for (int i = 0; i <= 2; i += 1) {
    ans.x[i] = -v.x[i];
  }
  return ans;
}

CVector2d math::vneg(CVector2d v) {
  CVector2d ans = {};
  for (int i = 0; i <= 1; i += 1) {
    ans.x[i] = -v.x[i];
  }
  return ans;
}

double math::GetLogicRate(int m_ProjectionMethod) {
  if (m_ProjectionMethod == PROJECTMETHOD_ORTHOGRAPHIC)
    return 131.25;
  else if (m_ProjectionMethod == PROJECTMETHOD_PERSPECTIVE)
    return 1000.0;
  assert(false);
}

CVector3d math::vadd(CVector3d v1, CVector3d v2) {
  for (int i = 0; i <= 2; i += 1) {
    v1.x[i] += v2.x[i];
  }
  return v1;
}

CVector2d math::vadd(CVector2d v1, CVector2d v2) {
  for (int i = 0; i <= 1; i += 1) {
    v1.x[i] += v2.x[i];
  }
  return v1;
}

CVector3d math::GetPrjV3d(CVector3d pos, const CPlane& plane,
                          const CVector3d& center) {
  CVector3d n = vcross(plane.up, plane.left); /* 面法向量 */
  CVector3d v = vadd(center, vneg(pos));      /* 投影线方向向量 */
  const CVector3d& p = pos;
  const CVector3d& c = plane.pos;
  double t = vdot(vadd(c, vneg(p)), n) / vdot(v, n);
  return vadd(p, vmul(v, t)); /* 得到投影点一定在面上 */
}

CVector2d math::GetPrjV2d(CVector3d pos, const CPlane& plane,
                          const CVector3d& center) {
  CVector3d p = GetPrjV3d(pos, plane, center);
  p = vadd(p, vneg(plane.pos)); /* 相对坐标 */
  CVector2d ans = {};
  ans.GetY() = vdprj(p, plane.up);
  ans.GetX() = vdprj(p, plane.left);
  return ans;
}

CVector2d math::GetDevicePos(CVector2d p, int width, int height) {
  double w = width / GetLogicRate(m_ProjectMethod);
  double h =
      height / GetLogicRate(m_ProjectMethod); /* 获得设备尺寸（逻辑单位）*/

  CVector2d ans = {p.GetX() + w / 2, h / 2 - p.GetY()};
  for (int i = 0; i <= 1; i += 1) {
    ans.x[i] *= GetLogicRate(m_ProjectMethod);
  }
  return ans; /* 得到用于输出的坐标 */
}

double math::GetClockTime() { /* 获取程序运行时间 */
  return 1.0 * clock() / CLOCKS_PER_SEC;
}

void math::ZSpinner(CCube* cube, double tnow) {
  if (cube != nullptr) {
    double dt = tnow - cube->t;
    cube->pos = vrot(cube->pos, c_Vector3d_z, dt * c_SpinSpeed);
    cube->left = vrot(cube->left, c_Vector3d_z, dt * c_SpinSpeed);
    cube->up = vrot(cube->up, c_Vector3d_z, dt * c_SpinSpeed);
  }
}

void math::vdec(CVector2d r, CVector2d d1, CVector2d d2, double& x, double& y) {
  /* 不允许向量恰好同向或者反向 */
  assert(fabs(fabs(vdot(d1, d2)) - vlen(d1) * vlen(d2)) >= c_MathEps);

  /* 不允许出现零向量 */
  assert(vlen(d1) >= c_MathEps && vlen(d2) >= c_MathEps);

  double A1 = vdot(d1, d1), B1 = vdot(d1, d2), C1 = vdot(r, d1);
  double A2 = vdot(d1, d2), B2 = vdot(d2, d2), C2 = vdot(r, d2);

  /* 克莱蒙法则求解 */
  x = dcross(C1, B1, C2, B2) / dcross(A1, B1, A2, B2);
  y = dcross(A1, C1, A2, C2) / dcross(A1, B1, A2, B2);
}

double math::dcross(double a1, double b1, double a2, double b2) {
  return a1 * b2 - a2 * b1;
}

double math::dcross(CVector2d u, CVector2d v) { /* 二维叉积 */
  return dcross(u.GetX(), v.GetX(), u.GetY(), v.GetY());
}

double math::vrad(CVector2d rf, CVector2d rt) {
  static const double M_PI = acos(-1.0);

  /* 单位化 */
  rf = vuni(rf);
  rt = vuni(rt);
  double cross = rf.GetX() * rt.GetY() - rf.GetY() * rt.GetX(); /* sin */
  double dot = vdot(rf, rt);                                    /* cos */

  if (fabs(cross) < 1e-6) {  // sin = 0
    if (dot > 0)
      return 0;
    else
      return M_PI;
  } else if (fabs(dot) < 1e-6) {  // cos = 0
    if (cross > 0)
      return M_PI / 2;
    else
      return -M_PI / 2;
  } else {
    if (cross > 0 && dot > 0) { /* 第一象限 */
      return asin(cross);
    } else if (cross > 0 && dot < 0) { /* 第二象限 */
      return acos(dot);
    } else if (cross < 0 && dot < 0) { /* 第三象限 */
      return asin(-cross) - M_PI;
    } else { /* 第四象限 */
      return acos(-dot) - M_PI;
    }
  }
}

void CCG30612View::OnSize(UINT nType, int cx, int cy) {
  CView::OnSize(nType, cx, cy);

  m_Width = cx;
  m_Height = cy;
}

void CCube::Unify() {
  up = math::vuni(up);
  left = math::vuni(left);
}

void CCG30612View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
  if (GetAsyncKeyState(VK_UP)) {
    MyFunc_MoveUp();
  }
  if (GetAsyncKeyState(VK_DOWN)) {
    MyFunc_MoveDown();
  }
  if (GetAsyncKeyState(VK_LEFT)) {
    MyFunc_MoveLeft();
  }
  if (GetAsyncKeyState(VK_RIGHT)) {
    MyFunc_MoveRight();
  }

  MyFunc_ImmediateShow();
  CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCG30612View::OnLButtonDown(UINT nFlags, CPoint point) {
  this->SetCapture();
  m_LButtonDown = true;
  m_LButtonDownPos = point;

  CView::OnLButtonDown(nFlags, point);
}

void CCG30612View::OnLButtonUp(UINT nFlags, CPoint point) {
  ReleaseCapture();
  m_LButtonDown = false;
  m_LButtonDownPos = {};

  CView::OnLButtonUp(nFlags, point);
}

void CCG30612View::OnMouseMove(UINT nFlags, CPoint point) {
  if (m_LButtonDown) {
    /* 左键按下时拖动鼠标可以切换视角 */
    if (point != m_LButtonDownPos) {
      MyFunc_SetCenter(point - m_LButtonDownPos);
      m_LButtonDownPos = point;
    }
  }

  CView::OnMouseMove(nFlags, point);
}

void CCG30612View::OnTimer(UINT_PTR nIDEvent) {
  // TODO: 在此添加消息处理程序代码和/或调用默认值
  if (nIDEvent == IDT_TIMER) {
    if (m_LineMode) {
      MyFunc_ImmediateShow(); /* 只在画线模式，即时显示 */
    }
  }

  CView::OnTimer(nIDEvent);
}

void CCG30612View::OnInitialUpdate() {
  CView::OnInitialUpdate();

  MyFunc_StartTimer(IDT_TIMER, 30);
}

struct CmpNode {
  int id;
  double dis;
};
bool NodeCmp(const CmpNode& A, const CmpNode& B) {
  return A.dis < B.dis; /* 按距离从小到大排序 */
}

double CSurface2d::GetZ(CVector2d v) {
  /* 采用了三角剖分的做法 */
  int base = 0;

  CVector2d d12 = math::vadd(x[2], math::vneg(x[1]));
  CVector2d d13 = math::vadd(x[3], math::vneg(x[1]));
  CVector2d d1v = math::vadd(v, math::vneg(x[1]));

  if (math::sgn(math::dcross(d12, d1v)) == math::sgn(math::dcross(d12, d13))) {
    base = 3;
  }

  int t1 = base ^ 1;
  int t2 = base ^ 2;
  return GetZ(v, base, t1, t2);
}

double CSurface2d::GetZ(CVector2d v, int base, int t1, int t2) {
  CVector2d d1 = math::vadd(x[t1], math::vneg(x[base])); /* 两个方向向量 */
  CVector2d d2 = math::vadd(x[t2], math::vneg(x[base]));

  CVector2d r = math::vadd(v, math::vneg(x[base])); /* 坐标方向 */

  if (math::vlen(d1) < math::c_MathEps || math::vlen(d2) < math::c_MathEps) {
    /* 存在长度为零的边 */
    double lenSum = math::vlen(d1) + math::vlen(d2);
    double dz = fabs(z[t2] - z[base]) > fabs(z[t1] - z[base]) ? z[t2] - z[base]
                                                              : z[t1] - z[base];
    return math::vlen(r) / lenSum * dz + z[base];
  } else if (fabs(fabs(math::vdot(d1, d2)) - math::vlen(d1) * math::vlen(d2)) <
             math::c_MathEps) {
    /* 两个向量同向或者反向 */
    double len, dz;
    if (math::vdot(r, d1) > 0) {
      /* r 和 d1 同向 */
      len = math::vlen(d1);
      dz = z[t1] - z[base];
    } else {
      /* r 和 d2 同向 */
      len = math::vlen(d2);
      dz = z[t2] - z[base];
    }
    return math::vlen(r) / len * dz + z[base];
  } else {
    /* d1, d2 既不同向也不反向 */
    double x, y;
    math::vdec(r, d1, d2, x, y); /* 对 r 在 d1, d2, 方向上进行分解 */

    double ans = z[base];
    ans += y * (z[t2] - z[base]);
    ans += x * (z[t1] - z[base]);

    return ans;
  }
}

COLORREF CSurface2d::GetColorById(int sid) {
  switch (sid) {
    case 0:
      return RGB(0, 255, 255);
    case 1:
      return RGB(255, 0, 255);
    case 2:
      return RGB(255, 255, 0);
    case 3:
      return RGB(255, 0, 0);
    case 4:
      return RGB(0, 255, 0);
    case 5:
      return RGB(0, 0, 255);
    default:
      assert(false);
  }
  return RGB(0, 0, 0);
}

bool CSurface2d::Contain(CVector2d npos) {
  double rad = 0;
  for (int now = 0; now <= 3; now++) { /* 环游一周 */
    int nxt = now == 3 ? 0 : now + 1;  /* 下一条边 */
    CVector2d nowv = math::vadd(x[now], math::vneg(npos));
    CVector2d nxtv = math::vadd(x[nxt], math::vneg(npos));
    double tmp = math::vrad(nowv, nxtv);
    rad += tmp; /* 计算有向夹角 */
  }
  double tmp = fabs(rad);
  bool ans = fabs(tmp) > math::c_MathEps;

  // assert(ans == false);
  return ans;
}

/* 构造 z buffer */
CZBuffer::CZBuffer(int wt, int ht, CVector2d topleft) : m_TopLeft(topleft) {
  m_Width = wt;
  m_Height = ht;

  /* 构造每一列 */
  m_Buffer2d = CBuffer2d(wt);
  assert(m_Buffer2d.size() == wt);

  /* 构造每一列的每一行 */
  CBuffer1d tmp(ht);
  assert(tmp.size() == ht);

  /* 深拷贝 */
  for (int i = 0; i < m_Buffer2d.size(); i += 1) {
    m_Buffer2d[i] = tmp;
  }
}

void CZBuffer::AddSurface(CSurface2d& surface) {
  for (int i = 0; i < m_Width; i++) {
    for (int j = 0; j < m_Height; j++) {
      /* 如果这个点被这个面包含 */
      double posx = i + m_TopLeft.GetX();
      double posy = j + m_TopLeft.GetY();
      CVector2d npos = {posx, posy};
      if (surface.Contain(npos)) {
        double ndep = surface.GetZ(npos);
        if (m_Buffer2d[i][j].z >= ndep) {
          m_Buffer2d[i][j].z = ndep;
          m_Buffer2d[i][j].color = surface.GetColorById(surface.sid);
        }
      }
    }
  }
}

void CZBuffer::OutputToDC(CDC* pDC) {
  for (int i = 0; i < m_Width; i++) {
    for (int j = 0; j < m_Height; j++) {
      if (m_Buffer2d[i][j].z < math::c_DoubleMax) {
        pDC->SetPixel((int)(i + m_TopLeft.GetX() + 0.5),
                      (int)(j + m_TopLeft.GetY() + 0.5),
                      m_Buffer2d[i][j].color);
      }
    }
  }
}

void CCG30612View::OnTogglecolor() {
  m_LineMode = !m_LineMode;
  MyFunc_ImmediateShow();
}

void CCG30612View::OnOrthographic() {
  m_ProjectMethod = PROJECTMETHOD_ORTHOGRAPHIC;
  MyFunc_ImmediateShow();
}

void CCG30612View::OnPerspective() {
  m_ProjectMethod = PROJECTMETHOD_PERSPECTIVE;
  MyFunc_ImmediateShow();
}
