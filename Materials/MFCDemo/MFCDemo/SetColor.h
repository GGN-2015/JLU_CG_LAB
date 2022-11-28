#pragma once


// SetColor 对话框

class SetColor : public CDialog
{
	DECLARE_DYNAMIC(SetColor)

public:
	SetColor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetColor();

// 对话框数据
	enum { IDD = IDD_SETCOLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_g;
	int m_b;
	int m_r;
};
