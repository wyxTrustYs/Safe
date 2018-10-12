#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

#define Win32PATH _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define Win64PATH _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")

//�����Ϣ�ṹ��
typedef struct _SOFTINFO
{
	TCHAR szSoftName[50];				//�������
	TCHAR szSoftVer[50];				//����汾��
	TCHAR strSoftVenRel[50];			//�����������
	TCHAR szSoftData[20];				//�����װ����
	TCHAR strSoftInsPath[MAX_PATH];		//�����װ·��
	TCHAR strSoftUniPath[MAX_PATH];		//���ж��·��
}SOFTINFO, *PSOFTINFO;
// CSoftwareDlg �Ի���

class CSoftwareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftwareDlg)

public:
	CSoftwareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSoftwareDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SoftwareDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//���������Ϣ
	void GetSoftwareInfo();
	//������������Ϣ
	vector<SOFTINFO> m_vecSoftInfo;
	//�ж�ϵͳ�Ƿ���64λ
	BOOL Is64();
	//List����
	CListCtrl CSoftWareList;
	int m_Select;
	CString strTemp;
	afx_msg void OnUninstall();
	afx_msg void OnUpdate();
	afx_msg void OnOpenDir();
	afx_msg void OnRclickSoftwarelist(NMHDR *pNMHDR, LRESULT *pResult);
};
