// SetColor.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDemo.h"
#include "SetColor.h"
#include "afxdialogex.h"


// SetColor 对话框

IMPLEMENT_DYNAMIC(SetColor, CDialog)

SetColor::SetColor(CWnd* pParent /*=NULL*/)
	: CDialog(SetColor::IDD, pParent)
	, m_r(0)
	, m_g(0)
	, m_b(0)
{

}

SetColor::~SetColor()
{
}

void SetColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_G, m_g);
	DDV_MinMaxInt(pDX, m_g, 0, 255);
	DDX_Text(pDX, IDC_B, m_b);
	DDV_MinMaxInt(pDX, m_b, 0, 255);
	DDX_Text(pDX, IDC_R, m_r);
	DDV_MinMaxInt(pDX, m_r, 0, 255);
}


BEGIN_MESSAGE_MAP(SetColor, CDialog)
END_MESSAGE_MAP()


// SetColor 消息处理程序
