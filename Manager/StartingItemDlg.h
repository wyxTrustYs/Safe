#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

typedef struct _BOOTINFO
{
	TCHAR szBootName[50];			//������
	TCHAR szBootOrder[255];			//����
	TCHAR szHKEY[50];				//������Ϣ
	TCHAR szBootPos[255];			//ɾ��ʱҪ�õ���
	TCHAR szBootPos_All[255];		//List��Ҫ��ʾ��
}BOOTINFO, *PBOOTINFO;


// CStartingItemDlg �Ի���

class CStartingItemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartingItemDlg)

public:
	CStartingItemDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartingItemDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_StartingItemDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl StartingItemList;
	virtual BOOL OnInitDialog();
	//������������Ϣ
	vector<BOOTINFO> m_vecBootInfo;
	//��ȡ��������Ϣ
	void GetBootInfo();
	//��ѡ������
	int m_Select;
	CString strTemp;
	afx_msg void OnRclickStartingitemlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdate();
	afx_msg void Onaddmenu();
	afx_msg void OnDeleItemmenu();
};
