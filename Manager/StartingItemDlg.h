#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

typedef struct _BOOTINFO
{
	TCHAR szBootName[50];			//启动项
	TCHAR szBootOrder[255];			//命令
	TCHAR szHKEY[50];				//主键信息
	TCHAR szBootPos[255];			//删除时要用到的
	TCHAR szBootPos_All[255];		//List中要显示的
}BOOTINFO, *PBOOTINFO;


// CStartingItemDlg 对话框

class CStartingItemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartingItemDlg)

public:
	CStartingItemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartingItemDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_StartingItemDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl StartingItemList;
	virtual BOOL OnInitDialog();
	//保存软件相关信息
	vector<BOOTINFO> m_vecBootInfo;
	//获取启动项信息
	void GetBootInfo();
	//所选则的软件
	int m_Select;
	CString strTemp;
	afx_msg void OnRclickStartingitemlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdate();
	afx_msg void Onaddmenu();
	afx_msg void OnDeleItemmenu();
};
