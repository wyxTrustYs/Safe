#pragma once
#include "afxcmn.h"


// CSourceDlg 对话框

class CSourceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSourceDlg)

public:
	CSourceDlg(char *g_lpBase,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSourceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_SourceDlg
};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	char* lpbase;
	CTreeCtrl CSourceTree;
	HTREEITEM hitem[50];
	HTREEITEM hSubItem[100];

	static DWORD StaticpResHeader;
	virtual BOOL OnInitDialog();
	void ShowSourceTab();
	DWORD RVAtoFOA(DWORD dwRVA);

//	afx_msg void OnClickSourcetree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangedSourcetree1(NMHDR *pNMHDR, LRESULT *pResult);
};
