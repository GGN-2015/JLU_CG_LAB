
// MFCDemo.h : MFCDemo Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMFCDemoApp:
// �йش����ʵ�֣������ MFCDemo.cpp
//

class CMFCDemoApp : public CWinAppEx
{
public:
	CMFCDemoApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCDemoApp theApp;