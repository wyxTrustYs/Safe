#pragma once
#include "afxcmn.h"


// CSectionDlg �Ի���

class CSectionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSectionDlg)

public:
	CSectionDlg(char* g_lpbase,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSectionDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SectionDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl CSecList;
	char* lpbase;
	virtual BOOL OnInitDialog();
	BOOL ShowSectionInfo();

	
};
