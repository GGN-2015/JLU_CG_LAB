
// CG30612View.h: CCG30612View 类的接口
//

#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

struct CVector2d { /* 平面二维坐标 */
  double x[2];
  double& GetX();
  double& GetY();
};
struct CVector3d { /* 空间三维坐标 */
  double x[3];
  double& GetX();
  double& GetY();
  double& GetZ();
};
struct CPlane { /* 平面 */
  CVector3d pos, up, left;
};

struct CCube; /* 立方体移动算法 */
typedef void (*CubeChanger)(CCube* cube, double tnow);

struct CCube { /* 正方体 */
  CVector3d pos, up, left;
  double d; /* 棱长 */
  double t; /* 上次绘制时间 */

  CubeChanger p_CubeChanger = nullptr; /* 位置移动函数 */
  void Unify();
};

struct CSurface2d { /* 记录一个四变型 */
  CVector2d x[4];
  double z[4];              /* 用于计算 z 缓冲 */
  double GetZ(CVector2d v); /* 为多边形中的一个点计算 z */
  double GetZ(CVector2d v, int base, int t1, int t2);
  int sid; /* 用于决定面的颜色 */

  static COLORREF GetColorById(int sid);
  bool Contain(CVector2d npos); /* 检测一个二维点是否在一个多边形里 */
};

#define BACKGROUND_COLOR RGB(255, 255, 255)
#define IDT_TIMER (WM_USER + 200)
#define PROJECTMETHOD_PERSPECTIVE (0)  /* 透视投影 */
#define PROJECTMETHOD_ORTHOGRAPHIC (1) /* 平行投影 */

typedef std::map<int, CCube> CCubeMap;
typedef std::vector<CSurface2d> CSurface2dList; /* 多边形序列 */

namespace math {
const double c_DoubleMax = 1e100;
const double c_SpanRateDelta = 0.1;
const double c_SpanRateMin = 0.3;
const double c_SpanRateMax = 3.0;
}  // namespace math

struct CBufferElement { /* z 缓冲中的元素 */
  double z;
  COLORREF color;

  CBufferElement() {
    z = math::c_DoubleMax;
    color = {};
  } /* 足够远 */
};

typedef std::vector<CBufferElement> CBuffer1d;
typedef std::vector<CBuffer1d> CBuffer2d;

class CZBuffer {
 public:
  CZBuffer(int wt, int ht, CVector2d topleft); /* 申请缓冲区 */
  void AddSurface(CSurface2d& surface);
  void OutputToDC(CDC* pDC); /* 输出到屏幕 */

 private:
  int m_Width, m_Height;
  CBuffer2d m_Buffer2d;
  CVector2d m_TopLeft;
};

namespace math {
const double c_PI = acos(-1.0);
const double c_MathEps = 1e-4;
const double c_PixelMoveEps = 10;
const double c_z_max = 5.0;
const double c_z_min = -5.0;
const double c_MoveEps = 1e-1;
const double c_ScreenDistance = 7.0; /* 投影面到原点的距离 */
const double c_CenterDistance = 8.0; /* 投影点到原点的距离 */
// const double c_LogicRate = 1000.0;   /* 1 LogicUnit = ? Pixel */
double GetLogicRate(int m_ProjectionMethod); /* 获取单位转换 */

const double c_SpinSpeed = 2 * acos(-1.0) / 3.0; /* 三秒一圈 */
const CVector3d c_Vector3d_z = {0, 0, 1.0};      /* z轴 */

CVector3d vadd(CVector3d, CVector3d);   /* 向量加法 */
CVector2d vadd(CVector2d, CVector2d);   /* 向量加法 */
CVector3d vmul(CVector3d, double p);    /* 数乘向量 */
CVector2d vmul(CVector2d, double p);    /* 数乘向量 */
CVector3d vuni(CVector3d);              /* 计算单位向量 */
CVector2d vuni(CVector2d);              /* 计算单位向量 */
CVector3d vneg(CVector3d);              /* 反向 */
CVector2d vneg(CVector2d);              /* 反向 */
double vlen(CVector3d);                 /* 数乘向量模长 */
double vlen(CVector2d);                 /* 数乘向量模长 */
double vdot(CVector3d, CVector3d);      /* 向量点积 */
double vdot(CVector2d, CVector2d);      /* 向量点积 */
CVector3d vcross(CVector3d, CVector3d); /* 向量叉积 */
double vdprj(CVector3d v, CVector3d e); /* 得到 v 在 e 上的投影长度 */
CVector3d vprj(CVector3d v, CVector3d e); /* 得到 v 在 e 上的投影 */
CVector3d vrot(CVector3d v, CVector3d base, double rad); /* 旋转 */
double GetDisFromPointToPlane(CVector3d p, CPlane plane); /* 点到平面距离 */

int lowbit(int x);    /* 计算最低二进制位 */
double sgn(double x); /* 计算符号 */

CPlane GetProjectionPlaneByPoint(
    CVector3d pos); /* 根据观察点找到对应的投影面 */
CVector3d GetPrjV3d(CVector3d pos, const CPlane& plane,
                    const CVector3d& center); /* 得到投影的三维坐标 */
CVector2d GetPrjV2d(CVector3d pos, const CPlane& plane,
                    const CVector3d& center); /* 得到投影的二维坐标 */

CVector2d GetDevicePos(CVector2d p, int width, int height); /* 计算设备坐标 */
double GetClockTime();

/* 绕 z 轴匀速旋转 */
void ZSpinner(CCube* cube, double tnow);

/* 向量分解 */
void vdec(CVector2d r, CVector2d d1, CVector2d d2, double& x, double& y);
double dcross(double a1, double b1, double a2, double b2);
double dcross(CVector2d u, CVector2d v);
double vrad(CVector2d rf, CVector2d rt); /* 计算有向夹角 */

}  // namespace math

class CCG30612View : public CView {
 protected:  // 仅从序列化创建
  CCG30612View() noexcept;
  DECLARE_DYNCREATE(CCG30612View)

  // 特性
 public:
  CCG30612Doc* GetDocument() const;

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
  virtual ~CCG30612View();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  /* 用于确定矩形范围 */
  static RECT MyStaticFunc_GetRectFromCSurface2dList(CSurface2dList& slist);

  CVector2d MyFunc_ProjectionLogicToDevice(CVector2d pos);
  CVector2d MyFunc_ProjectionWorldToLogic(CVector3d pos);
  CVector2d MyFunc_ProjectionWorldToDevice(CVector3d pos);
  void MyFunc_ShowAllItem();
  void MyFunc_ShowAllItem(CDC* pDC);
  void MyFunc_ShowAllCubes(CDC* pDC);
  void MyFunc_ShowCube(CDC* pDC, CCube* cube);
  void MyFunc_GetCornersForCube(const CCube* cube, CVector3d* corners);
  void MyFunc_AddCube(CCube n_Cube);
  void MyFunc_ImmediateShow();
  void MyFunc_ShowHelpText(CDC* pDC);
  void MyFunc_SetCenter(CPoint dpos); /* 根据鼠标位置变化切换视角 */
  void MyFunc_MoveRight();
  void MyFunc_MoveLeft();
  void MyFunc_MoveUp();
  void MyFunc_MoveDown();
  void MyFunc_StartTimer(UINT id, UINT duration);
  void MyFunc_ShowLinesForCube(CDC* pDC, CCube* cube);
  void MyFunc_ShowSurfaces(CDC* pDC, CSurface2dList& slist);

  /* 获得正方体的每一个面 */
  void MyFunc_GetSurfaceForCube(CSurface2dList& slist, CCube* cube);

 protected:
  // 生成的消息映射函数
 protected:
  DECLARE_MESSAGE_MAP()

 private:
  int m_Width, m_Height, m_EntityId = 0;
  CVector3d m_Center = {math::c_CenterDistance, 0, 0}; /* 投影点 */
  CCubeMap m_CubeMap;
  bool m_LButtonDown = false;
  CPoint m_LButtonDownPos; /* 鼠标左键按下时鼠标的位置 */
  bool m_LineMode = true;

 public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  virtual void OnInitialUpdate();
  afx_msg void OnTogglecolor();
  afx_msg void OnOrthographic();
  afx_msg void OnPerspective();
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // CG30612View.cpp 中的调试版本
inline CCG30612Doc* CCG30612View::GetDocument() const {
  return reinterpret_cast<CCG30612Doc*>(m_pDocument);
}
#endif
