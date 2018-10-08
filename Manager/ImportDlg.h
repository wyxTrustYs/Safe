#pragma once
#include "afxcmn.h"


// CImportDlg 对话框

class CImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDlg)

public:
	CImportDlg(DWORD vAddr,char* g_lpBase,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImportDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowImportModleTab();
	void ShowImportFunTab(DWORD number);
	DWORD RVAtoFOA(DWORD dwRVA);
	DWORD virtualaddr;
	CListCtrl CImportFlieList;
	CListCtrl CImportFunList;
	char* lpbase;
	afx_msg void OnClickImportfilelist(NMHDR *pNMHDR, LRESULT *pResult);
};
