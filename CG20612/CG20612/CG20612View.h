
// CG20612View.h: CCG20612View 类的接口
//

#pragma once

#include <map>
#include <vector>

typedef std::vector<int> CNodeIdList; /* 选中的结点 ID 序列 */

struct CPolygon {
  int polygonId;
  CNodeIdList nodeIds;
};
struct CCircle {
  int circleId;         /* 圆 ID */
  int nodeIdC, nodeIdR; /* 根据圆心和半径绘制圆 */
};
struct CEllipse {
  int ellipseId;        /* 椭圆 ID */
  int nodeIdA, nodeIdB; /* 根据两个角控制点绘制椭圆 */
};
struct CMyNode {
  CPoint pos;                /* 结点位置 */
  int nodeId;                /* 结点编号 */
  int tag;                   /* 结点选中号 */
  void GetRect(RECT*) const; /* 获得矩形范围 */
};
typedef std::map<int, CMyNode> CPointMap;    /* 点坐标序列 */
typedef std::map<int, CEllipse> CEllipseMap; /* 椭圆集合 */
typedef std::map<int, CCircle> CCircleMap;   /* 椭圆集合 */
typedef std::map<int, CPolygon> CPolygonMap; /* 多边形集合 */

/* ---------- 我的常量 ---------- */
#define NODE_RADIUS (5)                    /* 结点半径*/
#define NODE_WIDTH (1)                     /* 结点线宽 */
#define NODE_COLOR RGB(0, 0, 255)          /* 结点默认颜色 */
#define NODE_SELECTED_COLOR RGB(255, 0, 0) /* 结点选中时颜色 */
#define ELLIPSE_COLOR RGB(0, 0, 0) /* 算法绘制椭圆时采用的颜色 */

#define STATE_FREE (0)       /* 自由光标 */
#define STATE_SETNODE (1)    /* 设置结点 */
#define STATE_MOVENODE (2)   /* 移动节点 */
#define STATE_DELETENODE (3) /* 移动节点 */
#define STATE_SETELLIPSE (4) /* 绘制椭圆 */
#define STATE_SETCIRCLE (5)  /* 绘制圆形 */
#define STATE_SETPOLYGON (6) /* 绘制多边形 */

#define BACKGROUND_COLOR RGB(255, 255, 255) /* 设置背景颜色 */
#define UNDEFINED (-1)

class CCG20612View : public CView {
 protected:  // 仅从序列化创建
  CCG20612View() noexcept;
  DECLARE_DYNCREATE(CCG20612View)

  // 特性
 public:
  CCG20612Doc* GetDocument() const;

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
  virtual ~CCG20612View();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

 protected:
  /* ---------- 我的变量 ---------- */

  int m_MaxObjId = 0;               /* 最大结点编号 */
  int m_State;                      /* 自动机状态 */
  int m_PickUpNodeId;               /* 当前正在被移动的 NodeId */
  bool m_LButtonDown;               /* 鼠标左键是否按下 */
  bool m_Merge;                     /* 是否对目标点进行合并 */
  CPoint m_CursorPos;               /* 鼠标位置 */
  CPointMap m_NodeMap;              /* 结点列表 */
  CNodeIdList m_SelectedNodeIdList; /* 选中的结点 ID 列表 */
  CEllipseMap m_EllipseSet;         /* 椭圆集合 */
  CCircleMap m_CircleSet;           /* 圆集合 */
  CPolygonMap m_PolygonMap;         /* 多边形集合 */

  /* ---------- 我的函数 ---------- */

  static double MyStaticFunc_GetPointDistance(CPoint, CPoint);
  static void MyStaticFunc_Warning(const char* n_File, int n_Line,
                                   const char* n_Msg);
  static void MyStaticFunc_DrawEllipse(CDC* pDC,
                                       RECT* pRect); /* 绘制椭圆算法 */
  static void MyMathFunc_DrawEllipse(CDC* pDC, RECT* pRect,
                                     COLORREF color); /* 绘制椭圆算法 */
  static void MyStaticFunc_DrawNode(CDC*, CPoint, COLORREF);
  static bool MyStaticFunc_CheckNodeInList(const CNodeIdList& n_NodeIdList,
                                           int nid);
  static void MyMathFunc_XorBuffer(std::vector<std::vector<int>>& buffer,
                                   CPoint from, CPoint to);

/* 用于报错的宏 */
#define MyWarning(n_Msg) MyStaticFunc_Warning(__FILE__, __LINE__, n_Msg)

  int MyFunc_GetCursorOnNodeId(CPoint n_CursorPosition);
  bool MyFunc_CheckCursorOnNode(CPoint n_CursorPosition);
  void MyFunc_TestAndDeleteNode(int nodeId); /* 检测结点是否在左上角之外 */
  int MyFunc_AddNewNodeAt(CPoint point);   /* 防止新节点 */
  void MyFunc_PutDownMovingNode();         /* 放下正在移动的节点 */
  void MyFunc_ShowAllItem();               /* 显示所有对象 */
  void MyFunc_ShowAllNode(CDC* pDC);       /* 显示所有结点 */
  void MyFunc_ChangeStateTo(int STATE_TO); /* 设置新状态 */
  void MyFunc_SetTagsOnNode(); /* 根据 m_SelectedNodeIdList 设置 tag */
  void MyFunc_ShowAllEllipse(CDC* pDC);             /* 显示所有椭圆 */
  void MyFunc_AddEllipseByNodeId(int id1, int id2); /* 增加新的椭圆*/
  void MyFunc_AddCircleByNodeId(int id1, int id2);  /* 增加新的圆 */
  void MyFunc_ShowAllCircle(CDC* pDC);              /* 绘制所有圆 */
  void MyFunc_ShowAllPolygon(CDC* pDC);             /* 绘制所有多边形 */
  void MyFunc_PutDownMovingNodeAndMerge(CPoint point);
  void MyFunc_NodeMergeInto(int idFrom, int idTo); /* 结点合并 */
  int MyFunc_GetOrCreateNode(CPoint point); /* 获取一个点或新建一个点 */
  void MyFunc_DeleteNodeById(int idFrom);     /* 删除一个节点 */
  bool MyFunc_CheckSelectedByNodeId(int nid); /* 检测点是否被选中 */
  void MyFunc_AddNewPolygon();                /* 增加一个新的多边形 */
  RECT MyFunc_GetPolygonRect(const CPolygon& n_CPolygon);
  void MyFunc_FillPolygon(CDC* pDC, const CPolygon& n_Polygon);

  // 生成的消息映射函数

 protected:
  DECLARE_MESSAGE_MAP()
 public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnSetnode();
  afx_msg void OnDeletenode();
  afx_msg void OnSetellipse();
  afx_msg void OnSetcircle();
  afx_msg void OnSetpolygon();
};

#ifndef _DEBUG  // CG20612View.cpp 中的调试版本
inline CCG20612Doc* CCG20612View::GetDocument() const {
  return reinterpret_cast<CCG20612Doc*>(m_pDocument);
}
#endif
