#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CFileDirDlg �Ի���

class CFileDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDirDlg)

public:
	CFileDirDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileDirDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FileDirDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateDir(LPCTSTR FullPath);
	void ShowFileList(LPCTSTR FullPath);
	CComboBox m_DiskCom;
	CListCtrl m_FileList;
	TCHAR CurrentDir[MAX_PATH] = { 0 };
	TCHAR CurrentFile[MAX_PATH] = { 0 };
	afx_msg void OnCbnSelchangeDircombo();
	afx_msg void OnDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFilecheck();
	afx_msg void OnRclickFilelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFileattr();
};
