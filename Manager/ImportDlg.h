#pragma once
#include "afxcmn.h"


// CImportDlg �Ի���

class CImportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImportDlg)

public:
	CImportDlg(DWORD vAddr,char* g_lpBase,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImportDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImportDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
