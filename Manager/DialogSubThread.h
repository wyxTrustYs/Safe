#pragma once
#include "afxcmn.h"


// DialogSubThread �Ի���

class DialogSubThread : public CDialogEx
{
	DECLARE_DYNAMIC(DialogSubThread)

public:
	DialogSubThread(int PID,CWnd* pParent = NULL);   // ��׼���캯��
	
	virtual ~DialogSubThread();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SubThread };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl SubThreadList;
	int mPID;
	virtual BOOL OnInitDialog();
};
