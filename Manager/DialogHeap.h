#pragma once
#include "afxcmn.h"


// CDialogHeap �Ի���

class CDialogHeap : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogHeap)

public:
	CDialogHeap(int PID,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogHeap();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGHEAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl CListHeap;
	DWORD nPID;
	virtual BOOL OnInitDialog();
};
