#pragma once
#include "afxcmn.h"


// ModuleDialog �Ի���

class ModuleDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ModuleDialog)

public:
	ModuleDialog(int Pid,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ModuleDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Module };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl ListModule;
	int nPID;
	virtual BOOL OnInitDialog();
};
