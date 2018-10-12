#pragma once
#include "afxcmn.h"


// CDelayDlg 对话框

class CDelayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDelayDlg)

public:
	CDelayDlg(char* g_lpbase,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDelayDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DelayDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
