
// CG20612.h: CG20612 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CCG20612App:
// 有关此类的实现，请参阅 CG20612.cpp
//

class CCG20612App : public CWinApp
{
public:
	CCG20612App() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCG20612App theApp;
