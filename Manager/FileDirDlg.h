#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CFileDirDlg 对话框

class CFileDirDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDirDlg)

public:
	CFileDirDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileDirDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FileDirDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
