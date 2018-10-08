#pragma once
#include "afxwin.h"


// DialogPE �Ի���

class DialogPE : public CDialogEx
{
	DECLARE_DYNAMIC(DialogPE)

public:
	DialogPE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DialogPE();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString CEntryEdit;
	CString CSubSysEdit;
	CString CMirrorBaseEdit;
	CString CSectionNumEdit;
	CString CMirrorSizeEdit;
	CString CTimeEdit;
	CString CCodeBaseEdit;
	CString CHeadEdit;
	CString CDataBaseEdit;
	CString CSecAlignmentEdit;
	CString CCheckSumEdit;
	CString CFileAlignmentEdit;
	CString CSleHeadSizeEdit;
	CString CMagicEdit;
	CString CRVAEdit;
	CString CCharacteristicsEdit;
	WCHAR FilePath[100];
	char* g_lpBase;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	char* ReadFileToMemory();
	DWORD RVAtoFOA(DWORD dwRVA);
	BOOL IsPE(char* lpBase);
	void ShowOptionInfo();

	virtual BOOL OnInitDialog();
	void ShowHeaderInfo(char* lpBase);

	CButton btnSection;
	afx_msg void OnBnClickedSection();

	CButton btnShowDir;
	afx_msg void OnBnClickedDir();
};
