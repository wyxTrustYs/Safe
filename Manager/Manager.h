
// Manager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CManagerApp: 
// �йش����ʵ�֣������ Manager.cpp
//

class CManagerApp : public CWinApp
{
public:
	CManagerApp();

// ��д
public:
	void UpLevel();
	void onNum(UINT Num);
	
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
//	afx_msg void OnShutDown();
};

extern CManagerApp theApp;