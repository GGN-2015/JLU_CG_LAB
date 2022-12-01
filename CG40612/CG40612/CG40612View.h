﻿
// CG40612View.h: CCG40612View 类的接口
//

#pragma once

#include <map>
#include <vector>

typedef std::vector<int> CNodeIdList; /* 选中的结点 ID 序列 */

struct CEdge {
  CPoint from, to;
};
typedef std::vector<CEdge> CEdgeList;
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
#define CUT_SQUARE_COLOR RGB(0, 0, 0)      /* 裁剪区域边框颜色 */
#define ELLIPSE_COLOR RGB(0, 0, 0)         /* 椭圆颜色 */

#define STATE_FREE (0)       /* 自由光标 */
#define STATE_SETNODE (1)    /* 设置结点 */
#define STATE_MOVENODE (2)   /* 移动节点 */
#define STATE_DELETENODE (3) /* 移动节点 */
#define STATE_SETELLIPSE (4) /* 绘制椭圆 */
#define STATE_SETCIRCLE (5)  /* 绘制圆形 */
#define STATE_SETPOLYGON (6) /* 绘制多边形 */
#define STATE_CUTPOLYGON (7) /* 剪切多边形 */

#define NEW_POLYGON_COLOR RGB(0, 255, 0)    /* 新的多边形 */
#define OLD_POLYGON_COLOR RGB(255, 0, 0)    /* 旧的多边形 */
#define BACKGROUND_COLOR RGB(255, 255, 255) /* 设置背景颜色 */
#define UNDEFINED (-1)

class CCG40612View : public CView {
 protected:  // 仅从序列化创建
  CCG40612View() noexcept;
  DECLARE_DYNCREATE(CCG40612View)

  // 特性
 public:
  CCG40612Doc* GetDocument() const;

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
  virtual ~CCG40612View();
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
  bool m_FillModOn;                 /* 填充模式 */
  bool m_Merge;                     /* 是否对目标点进行合并 */
  bool m_CompareMod;                /* 剪切对比模式 */
  RECT m_LastCutRect;               /* 上次裁剪的矩形边框 */
  CPoint m_CursorPos;               /* 鼠标位置 */
  CPoint m_CutBegin;                /* 裁剪矩形起点 */
  CPoint m_CutEnd;                  /* 裁剪举行终点 */
  CPointMap m_NodeMap;              /* 结点列表 */
  CNodeIdList m_SelectedNodeIdList; /* 选中的结点 ID 列表 */
  CEllipseMap m_EllipseSet;         /* 椭圆集合 */
  CCircleMap m_CircleSet;           /* 圆集合 */
  CPolygonMap m_PolygonMap;         /* 多边形集合 */
  CPolygonMap m_LastPolygonMap;     /* 多边形集合进行备份 */

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
  static COLORREF MyMathFunc_GetRgbByHsv(double H, double S, double V);
  static void MyStaticFunc_CutXAbove(CEdgeList&, int);
  static void MyStaticFunc_CutYAbove(CEdgeList&, int);
  static void MyStaticFunc_CutXBelow(CEdgeList&, int);
  static void MyStaticFunc_CutYBelow(CEdgeList&, int);
  static void MyStaticFunc_DrawSquare(CDC* pDC, RECT rect, COLORREF color);

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
  void MyFunc_CutPolygons(); /* 对所有多边形进行裁剪 */
  RECT MyFunc_GetCutRect();  /* 获取用于裁剪的矩形框 */
  void MyFunc_CutPolygon(RECT& rect, CPolygon& n_CPolygon);
  void MyFunc_ShowCutSquare(CDC*);     /* 显示切割矩形 */
  void MyFunc_ShowLastCutSpuqre(CDC*); /* 显示上次的切割矩形 */
  void MyFunc_ShowPolygonInMap(CDC* pDC, CPolygonMap& n_PolygonMap,
                               int lineWidth, COLORREF color);
  void MyFunc_DeleteAll();
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
  afx_msg void OnTogglefill();
  afx_msg void OnCutpolygon();
  afx_msg void OnShowcompare();
  afx_msg void OnDeleteall();
};

#ifndef _DEBUG  // CG40612View.cpp 中的调试版本
inline CCG40612Doc* CCG40612View::GetDocument() const {
  return reinterpret_cast<CCG40612Doc*>(m_pDocument);
}
#endif
