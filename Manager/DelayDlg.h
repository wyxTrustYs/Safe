#pragma once
#include "afxcmn.h"


// CDelayDlg �Ի���

class CDelayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDelayDlg)

public:
	CDelayDlg(char* g_lpbase,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDelayDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DelayDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	char* lpbase;
	CListCtrl CDelayDllList;
	CListCtrl CDelayAPIList;
	DWORD RVAtoFOA(DWORD dwRVA);
	virtual BOOL OnInitDialog();
	void ShowDelay();
	void ShowAPITable(DWORD number);
	afx_msg void OnClickDelaydlllist(NMHDR *pNMHDR, LRESULT *pResult);
};
