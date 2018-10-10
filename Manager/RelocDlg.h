#pragma once
#include "afxcmn.h"
#include <vector>

// CRelocDlg �Ի���

class CRelocDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocDlg)

public:
	CRelocDlg(char* g_lpbase,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRelocDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Reloc_Dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowReloc();
	DWORD RVAtoFOA(DWORD dwRVA);
	void ShowSectionInList();
	void ShowBlockInList(int index);
	char* lpbase;

	CListCtrl CRelocSecList;
	CListCtrl RelocBlockList;
	
	afx_msg void OnClickRelocseclist(NMHDR *pNMHDR, LRESULT *pResult);
};


typedef struct _RELOCINFO
{
	DWORD dwRelocRVA;	//��Ҫ�ض�λ����������ַ��RVA��
	DWORD dwOffset;		//����RVA������ļ�ƫ�Ƶ�ַ
	BYTE bType;			//�ض�λ��ʽ������Ҳ����type����λ��
	DWORD dwRelocValue;	//��������ļ�ƫ��ȡ�������ݣ�������ݾ���VA
	CString bData;		//��VA��ȥ��ַ�õ���RVA������һ���ļ�ƫ�ƽ����е�����ȡ�����ŵ��������
}RELOCINFO, *PRELOCINFO;//����ṹ��������һ���ض�λ��Ϣ

typedef struct _RELOCAREAINFO
{
	DWORD szSectionName;	//�������ڵĽ���
	DWORD dwAreaRVA;		//����Ļ�ַ������һ����������ַ��
	DWORD NumberOfReloc;	//���������ض�λ����
	std::vector<RELOCINFO> vecRelocInfo;	//��������ض�λ�ľ�����Ϣ
}RELOCAREAINFO, *PRELOCAREAINFO;	//����ṹ������һ��������ض�λ����Ϣ
typedef struct Offset
{
	WORD Offsset : 12;		//ƫ��ֵ
	WORD Type : 4;			//�ض�λ���ԣ���ʽ��
}TYPEOFFSET, *PTYPEOFFSET;