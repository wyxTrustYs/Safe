#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
using namespace std;
#define WM_MYUPDATEDATA				WM_USER+100 
//������Ϣ�ṹ��
typedef struct _VIRINFO
{
	TCHAR szVirName[50];
	TCHAR szVirPath[MAX_PATH];
	TCHAR szVirSize[50];
}VIRINFO, *PVIRINFO;

// CClearVir �Ի���

class CClearVir : public CDialogEx
{
	DECLARE_DYNAMIC(CClearVir)

public:
	CClearVir(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClearVir();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ClearVirDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	//�����ļ���ɱ
	void Scan_Single(LPCTSTR szPath);
	//ȫ·����ɱ
	void Scan_All(LPCTSTR szPath);
	//��ʼ��ɱ
	void StartScan();

	//MD5��ɱ
	BOOL MD5Scan(LPCTSTR szPath);
	CString m_szMD5;
	//��������ɱ
	BOOL WhiteScan(LPCTSTR szPath);
	//��ȡ������С
	CString GetShowSize(DOUBLE dwSize);
	CString BrowseFolder(HWND hWnd, LPCTSTR lpTitle);
	//��Ӧ���̸߳���״̬��Ϣ
// 	LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam)
// 	{
// 		return UpdateData(wParam);
// 	}

	//����λ��
	CListCtrl CVirList;
	//ѡ��ȫ·����ɱ����ָ��������ɱ
	CComboBox CSelPath;

	//��ɱλ��
	CString Cpath;
	//ѡ���ɱ��ʽ
	CString strSelModel;
	//ѡ�񲡶���
	CString strSelVirLib;
	BOOL IsSingleScan;		//�Ƿ�Ϊ�����ļ���ɱ������Ϊȫ·��
	BOOL IsMD5;				//�Ƿ���MD5��ɱ������Ϊ������
	BOOL IsLocal;			//�Ƿ�Ϊ���ز�ɱ������Ϊ�ƶ�
	CString szTemp;

	//��ɱ·������
	CString m_SinglePath;
	
	CString m_AllPath;

	//���没����ϢVector
	vector<VIRINFO> m_vecVirInfo;
	//�����������Ϣ
	vector<CString> m_LocalMD5;		//����MD5��
	vector<CString> m_LocalWhite;	//���ذ�������
	vector<CString> m_ServerWhite;	//�ƶ˰�������
	vector<CString> m_ServerMD5;	//�ƶ�MD5��
	afx_msg void OnCbnSelchangeSelpath();
	afx_msg void OnBnClickedbtnvirscan();
	afx_msg void OnBnClickedbtnclearvir();
protected:
	afx_msg LRESULT OnUpDateData(WPARAM wParam, LPARAM lParam);
};
