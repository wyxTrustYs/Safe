#pragma once
#include "afxcmn.h"


// DialogC �Ի���

class CDialogC : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogC)

public:
	CDialogC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogC();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWSDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	static BOOL CALLBACK EnumWindowsProc(
		_In_ HWND hwnd,
		_In_ LPARAM lParam
	);

	DECLARE_MESSAGE_MAP()
public:
	static CListCtrl WIndowsList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedFlushwnd();
};
