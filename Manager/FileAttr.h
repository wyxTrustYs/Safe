#pragma once


// CFileAttr 对话框

class CFileAttr : public CDialogEx
{
	DECLARE_DYNAMIC(CFileAttr)

public:
	CFileAttr(LPCTSTR file,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileAttr();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FileAttributeDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL md5fileValue();
	CString m_Name;
	CString m_Type;
	CString m_Path;
	CString m_CreatTime;
	CString m_LVisitTime;
	CString m_LChangeTime;
	CString m_CurrentFile;
	
	BOOL m_Read;
	BOOL m_Hide;
	CString m_md5value;
};
