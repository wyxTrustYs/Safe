#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDialogA �Ի���

class CDialogA : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogA)

public:
	CDialogA(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogA();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	CListCtrl m_ListCtrl;
	CListCtrl m_ListCtrlA;
	int ListId;

	virtual BOOL OnInitDialog();
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32781();
	CButton Flush;
//	afx_msg void OnBnClickedButton2();
	afx_msg void On32782();
	afx_msg void OnBnClickedFlushprocess();
	afx_msg void On32786();
	afx_msg void On32787();
};
