#pragma once
#include "afxcmn.h"


// CServiceDlg �Ի���

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServiceDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ServiceDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
