
// CG10612View.h: CCG10612View 类的接口
//

#pragma once

#include <map>
#include <vector>

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
typedef std::vector<int> CNodeIdList;        /* 选中的结点 ID 序列 */
typedef std::map<int, CEllipse> CEllipseMap; /* 椭圆集合 */

/* ---------- 我的常量 ---------- */
#define NODE_RADIUS (5)                    /* 结点半径*/
#define NODE_WIDTH (1)                     /* 结点线宽 */
#define NODE_COLOR RGB(0, 0, 0)            /* 结点默认颜色 */
#define NODE_SELECTED_COLOR RGB(255, 0, 0) /* 结点选中时颜色 */

#define STATE_FREE (0)       /* 自由光标 */
#define STATE_SETNODE (1)    /* 设置结点 */
#define STATE_MOVENODE (2)   /* 移动节点 */
#define STATE_DELETENODE (3) /* 移动节点 */
#define STATE_SETELLIPSE (4) /* 绘制椭圆 */

#define BACKGROUND_COLOR RGB(255, 255, 255) /* 设置背景颜色 */
#define UNDEFINED (-1)

class CCG10612View : public CView {
 protected:  // 仅从序列化创建
  CCG10612View() noexcept;
  DECLARE_DYNCREATE(CCG10612View)

  // 特性
 public:
  CCG10612Doc* GetDocument() const;

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
  virtual ~CCG10612View();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

 protected:
  /* ---------- 我的变量 ---------- */

  int m_MaxObjId = 0;       /* 最大结点编号 */
  int m_State;              /* 自动机状态 */
  int m_PickUpNodeId;       /* 当前正在被移动的 NodeId */
  bool m_LButtonDown;       /* 鼠标左键是否按下 */
  CPoint m_CursorPos;       /* 鼠标位置 */
  CPointMap m_NodeMap;      /* 结点列表 */
  CNodeIdList m_NodeIdList; /* 选中的结点 ID 列表 */
  CEllipseMap m_EllipseSet; /* 椭圆集合 */

  /* ---------- 我的函数 ---------- */

  static double MyStaticFunc_GetPointDistance(CPoint, CPoint);
  static void MyStaticFunc_Warning(const char* n_File, int n_Line,
                                   const char* n_Msg);

/* 用于报错的宏 */
#define MyWarning(n_Msg) MyStaticFunc_Warning(__FILE__, __LINE__, n_Msg)

  int MyFunc_GetCursorOnNodeId(CPoint n_CursorPosition);
  bool MyFunc_CheckCursorOnNode(CPoint n_CursorPosition);
  void MyFunc_TestAndDeleteNode(int nodeId); /* 检测结点是否在左上角之外 */
  void MyFunc_AddNewNodeAt(CPoint point);  /* 防止新节点 */
  void MyFunc_PutDownMovingNode();         /* 放下正在移动的节点 */
  void MyFunc_ShowAllItem();               /* 显示所有对象 */
  void MyFunc_ShowAllNode(CDC* pDC);       /* 显示所有结点 */
  void MyFunc_ChangeStateTo(int STATE_TO); /* 设置新状态 */
  void MyFunc_SetTagsOnNode();             /* 根据 m_NodeIdList 设置 tag */
  void MyFunc_ShowAllEllipse(CDC* pDC);    /* 显示所有椭圆 */
  void MyFunc_AddEllipseByNodeId(int id1, int id2); /* 增加新的椭圆*/

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
};

#ifndef _DEBUG  // CG10612View.cpp 中的调试版本
inline CCG10612Doc* CCG10612View::GetDocument() const {
  return reinterpret_cast<CCG10612Doc*>(m_pDocument);
}
#endif
