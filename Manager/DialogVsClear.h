#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>


// CDialogVsClear �Ի���

class CDialogVsClear : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogVsClear)

public:
	CDialogVsClear(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogVsClear();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VSCLEAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void FindFile(CString Path);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl CListPath;
	CListCtrl CWaitDel;
	CListCtrl CDel;
	std::vector<CString> PathStrings;// ����·��������
	LARGE_INTEGER CountSize;	// �ļ��ܴ�С
	LARGE_INTEGER DestSize;		// ��ɾ���ļ��Ĵ�С
	std::vector<CString> DelStrings;	// Ҫɾ�����ļ�
	CString ExtendString;
	INT FileCount, DestCount;	// ���ļ������Ϳ�ɾ�ļ�����
	INT DeleteCount;			// ��ɾ�����ļ�����
	virtual BOOL OnInitDialog();
//	afx_msg HCURSOR OnQueryDragIcon();
//	afx_msg void OnPaint();
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedCheck();
};
