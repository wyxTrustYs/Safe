
// ManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "MyTabCtrl.h"
#include "afxwin.h"


// CManagerDlg �Ի���
class CManagerDlg : public CDialogEx
{
// ����
public:
	CManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CStatusBar m_status;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	CTabCtrl m_TabCtrl;
	CMyTabCtrl m_TabCtrl;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
