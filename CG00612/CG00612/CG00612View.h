
// CG00612View.h: CCG00612View 类的接口
//

#pragma once

#include <map>
typedef std::map<int, CPoint> CPointMap; /* 点坐标序列 */

/* ---------- 我的常量 ---------- */
#define NODE_RADIUS (5)                     /* 结点半径*/
#define STATE_FREE (0)                      /* 自由光标 */
#define STATE_SETNODE (1)                   /* 设置结点 */
#define STATE_MOVENODE (2)                  /* 移动节点 */
#define STATE_DELETENODE (3)                /* 移动节点 */
#define BACKGROUND_COLOR RGB(255, 255, 255) /* 设置背景颜色 */

class CCG00612View : public CView {
 protected:  // 仅从序列化创建
  CCG00612View() noexcept;
  DECLARE_DYNCREATE(CCG00612View)

  // 特性
 public:
  CCG00612Doc* GetDocument() const;

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
  virtual ~CCG00612View();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

 protected:
  /* ---------- 我的变量 ---------- */

  int m_MaxNodeId = 0; /* 最大结点编号 */
  int m_State;         /* 自动机状态 */
  int m_PickUpNodeId;  /* 当前正在被移动的 NodeId */
  bool m_LButtonDown;  /* 鼠标左键是否按下 */
  CPoint m_CursorPos;  /* 鼠标位置 */
  CPointMap m_NodeMap; /* 结点列表 */

  /* ---------- 我的函数 ---------- */

  static double MyStaticFunc_GetPointDistance(CPoint, CPoint);
  static void MyStaticFunc_Warning(const char* n_File, int n_Line,
                                   const char* n_Msg);

/* 用于报错的宏 */
#define MyWarning(n_Msg) MyStaticFunc_Warning(__FILE__, __LINE__, n_Msg)

  int MyFunc_GetCursorOnNodeId(CPoint n_CursorPosition);
  bool MyFunc_CheckCursorOnNode(CPoint n_CursorPosition);
  void MyFunc_TestAndDeleteNode(int nodeId); /* 检测结点是否在左上角之外 */
  void MyFunc_AddNewNodeAt(CPoint point); /* 防止新节点 */
  void MyFunc_PutDownMovingNode();        /* 放下正在移动的节点 */
  void MyFunc_ShowAllItem();              /* 显示所有对象 */
  void MyFunc_ShowAllNode(CDC* pDC);      /* 显示所有结点 */

  // 生成的消息映射函数
 protected:
  DECLARE_MESSAGE_MAP()
 public:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnSetnode();
  afx_msg void OnDeletenode();
};

#ifndef _DEBUG  // CG00612View.cpp 中的调试版本
inline CCG00612Doc* CCG00612View::GetDocument() const {
  return reinterpret_cast<CCG00612Doc*>(m_pDocument);
}
#endif
