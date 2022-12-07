
// CG30612View.h: CCG30612View 类的接口
//

#pragma once

#include <iostream>
#include <map>

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
struct CVector4d { /* 空间三维齐次坐标 */
  double x[4];
  double& GetX();
  double& GetY();
  double& GetZ();
  double& GetW();
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

#define BACKGROUND_COLOR RGB(255, 255, 255)

typedef std::map<int, CCube> CCubeMap;

namespace math {
const double c_MoveEps = 1e-1;
const double c_ScreenDistance = 5.0;        /* 投影面到原点的距离 */
const double c_CenterDistance = 6.0;        /* 投影点到原点的距离 */
const double c_LogicRate = 1000.0;          /* 1 LogicUnit = ? Pixel */
const CVector3d c_Vector3d_z = {0, 0, 1.0}; /* z轴 */

CVector3d vadd(CVector3d, CVector3d);   /* 向量加法 */
CVector3d vmul(CVector3d, double p);    /* 数乘向量 */
CVector3d vuni(CVector3d);              /* 计算单位向量 */
CVector3d vneg(CVector3d);              /* 反向 */
double vlen(CVector3d);                 /* 数乘向量模长 */
double vdot(CVector3d, CVector3d);      /* 向量点积 */
CVector3d vcross(CVector3d, CVector3d); /* 向量叉积 */
double vdprj(CVector3d v, CVector3d e); /* 得到 v 在 e 上的投影长度 */
CVector3d vprj(CVector3d v, CVector3d e); /* 得到 v 在 e 上的投影 */

double lowbit(int x); /* 计算最低二进制位 */

CPlane GetProjectionPlaneByPoint(
    CVector3d pos); /* 根据观察点找到对应的投影面 */
CVector3d GetPrjV3d(CVector3d pos, const CPlane& plane,
                    const CVector3d& center); /* 得到投影的三维坐标 */
CVector2d GetPrjV2d(CVector3d pos, const CPlane& plane,
                    const CVector3d& center); /* 得到投影的二维坐标 */

CVector2d GetDevicePos(CVector2d p, int width, int height); /* 计算设备坐标 */
double GetClockTime();
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

  CVector2d MyFunc_ProjectionLogicToDevice(CVector2d pos);
  CVector2d MyFunc_ProjectionWorldToLogic(CVector3d pos);
  CVector2d MyFunc_ProjectionWorldToDevice(CVector3d pos);
  void MyFunc_ShowAllItem();
  void MyFunc_ShowAllCubes(CDC* pDC);
  void MyFunc_ShowCube(CDC* pDC, CCube* cube);
  void MyFunc_GetCornersForCube(const CCube* cube, CVector3d* corners);
  void MyFunc_AddCube(CCube n_Cube);
  void MyFunc_ImmediateShow();

 protected:
  // 生成的消息映射函数
 protected:
  DECLARE_MESSAGE_MAP()

 private:
  int m_Width, m_Height, m_EntityId = 0;
  CVector3d m_Center = {math::c_CenterDistance, 0, 0}; /* 投影点 */
  CCubeMap m_CubeMap;

 public:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // CG30612View.cpp 中的调试版本
inline CCG30612Doc* CCG30612View::GetDocument() const {
  return reinterpret_cast<CCG30612Doc*>(m_pDocument);
}
#endif
