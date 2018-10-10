#pragma once
#include "afxcmn.h"
#include <vector>

// CCleanDlg 对话框

class CCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCleanDlg)

public:
	CCleanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCleanDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CleanDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	LARGE_INTEGER CountSize;	// 文件总大小
	LARGE_INTEGER DestSize;		// 可删除文件的大小
	std::vector<CString> DelStrings;	// 要删除的文件
	CString ExtendString;
	INT FileCount, DestCount;	// 总文件数量和可删文件数量
	INT DeleteCount;			// 已删除的文件个数
	CListCtrl CDiskRubbish;
	CListCtrl CBroserRubbish;
	CString CDiskDir;
	CString CSizeofRubbish;
	virtual BOOL OnInitDialog();
	void FindFile(CString Path, int flag);
	afx_msg void OnBnClickedbtnclean();
	afx_msg void OnBnClickedbtnscan();
};
