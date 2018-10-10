#pragma once
#include "afxcmn.h"


// CServiceDlg 对话框

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServiceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ServiceDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL EnumService();
	int Selrow;
	CListCtrl CServiceList;

	afx_msg void OnRclickServicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStartservice();
	afx_msg void OnCloseservice();
};
