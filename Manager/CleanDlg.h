#pragma once
#include "afxcmn.h"
#include <vector>

// CCleanDlg �Ի���

class CCleanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCleanDlg)

public:
	CCleanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCleanDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CleanDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	LARGE_INTEGER CountSize;	// �ļ��ܴ�С
	LARGE_INTEGER DestSize;		// ��ɾ���ļ��Ĵ�С
	std::vector<CString> DelStrings;	// Ҫɾ�����ļ�
	CString ExtendString;
	INT FileCount, DestCount;	// ���ļ������Ϳ�ɾ�ļ�����
	INT DeleteCount;			// ��ɾ�����ļ�����
	CListCtrl CDiskRubbish;
	CListCtrl CBroserRubbish;
	CString CDiskDir;
	CString CSizeofRubbish;
	virtual BOOL OnInitDialog();
	void FindFile(CString Path, int flag);
	afx_msg void OnBnClickedbtnclean();
	afx_msg void OnBnClickedbtnscan();
};
