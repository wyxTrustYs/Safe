#pragma once
#include "afxwin.h"


// CDirDlg 对话框

class CDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDirDlg)

public:
	CDirDlg(char* g_lpbase,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDirDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DirDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	char* lpbase;
	int EditID[32] = { IDC_EDIT1,IDC_EDIT2,IDC_EDIT3,IDC_EDIT4,IDC_EDIT5,IDC_EDIT6,IDC_EDIT7,IDC_EDIT8,IDC_EDIT9,IDC_EDIT10,IDC_EDIT11,IDC_EDIT12,IDC_EDIT13,IDC_EDIT14,IDC_EDIT15,IDC_EDIT16,IDC_EDIT17,IDC_EDIT18,IDC_EDIT19,IDC_EDIT20,IDC_EDIT21,IDC_EDIT22,IDC_EDIT23,IDC_EDIT24,IDC_EDIT25,IDC_EDIT26,IDC_EDIT27,IDC_EDIT28,IDC_EDIT29,IDC_EDIT30,IDC_EDIT31,IDC_EDIT32 };
	CButton CShowImport;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedExportbtn();
	afx_msg void OnBnClickedBtnresource();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtntls();
	afx_msg void OnBnClickedButton3();
};
