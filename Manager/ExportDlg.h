#pragma once
#include "afxcmn.h"


// CExportDlg 对话框

class CExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDlg)

public:
	CExportDlg(DWORD vAddr, char* g_lpBase, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ExportDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl CExportList;
	DWORD virtualaddr;
	char* lpbase;
	virtual BOOL OnInitDialog();
	void ShowExportTab();
	DWORD RVAtoFOA(DWORD dwRVA);
	int Edit[10] = { IDC_offsetEDIT,IDC_CharEDIT,IDC_BaseEDIT,IDC_NameEDIT,IDC_StrNameEDIT,IDC_NumFunEDIT,IDC_NameFunNumEDIT,IDC_BaseFunEDIT,IDC_BaseFunNameEDIT,IDC_BaseFunNumEDIT };
};
