#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
using namespace std;
#define WM_MYUPDATEDATA				WM_USER+100 
//病毒信息结构体
typedef struct _VIRINFO
{
	TCHAR szVirName[50];
	TCHAR szVirPath[MAX_PATH];
	TCHAR szVirSize[50];
}VIRINFO, *PVIRINFO;

// CClearVir 对话框

class CClearVir : public CDialogEx
{
	DECLARE_DYNAMIC(CClearVir)

public:
	CClearVir(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClearVir();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ClearVirDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	//单个文件查杀
	void Scan_Single(LPCTSTR szPath);
	//全路径查杀
	void Scan_All(LPCTSTR szPath);
	//开始查杀
	void StartScan();

	//MD5查杀
	BOOL MD5Scan(LPCTSTR szPath);
	CString m_szMD5;
	//白名单查杀
	BOOL WhiteScan(LPCTSTR szPath);
	//获取病毒大小
	CString GetShowSize(DOUBLE dwSize);
	CString BrowseFolder(HWND hWnd, LPCTSTR lpTitle);
	//响应子线程更新状态消息
// 	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam)
// 	{
// 		return UpdateData(wParam);
// 	}

	//病毒位置
	CListCtrl CVirList;
	//选择全路径查杀还是指定名单查杀
	CComboBox CSelPath;

	//查杀位置
	CString Cpath;
	//选择查杀方式
	CString strSelModel;
	//选择病毒库
	CString strSelVirLib;
	BOOL IsSingleScan;		//是否为单个文件查杀，否则为全路径
	BOOL IsMD5;				//是否是MD5查杀，否则为白名单
	BOOL IsLocal;			//是否为本地查杀，否则为云端
	CString szTemp;

	//查杀路径保存
	CString m_SinglePath;
	
	CString m_AllPath;

	//保存病毒信息Vector
	vector<VIRINFO> m_vecVirInfo;
	//病毒库加载信息
	vector<CString> m_LocalMD5;		//本地MD5库
	vector<CString> m_LocalWhite;	//本地白名单库
	vector<CString> m_ServerWhite;	//云端白名单库
	vector<CString> m_ServerMD5;	//云端MD5库
	afx_msg void OnCbnSelchangeSelpath();
	afx_msg void OnBnClickedbtnvirscan();
	afx_msg void OnBnClickedbtnclearvir();
protected:
	afx_msg LRESULT OnUpDateData(WPARAM wParam, LPARAM lParam);
};
