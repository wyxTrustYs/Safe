#pragma once


// CTLSDlg �Ի���

class CTLSDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTLSDlg)

public:
	CTLSDlg(char* g_lpbase,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTLSDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLSDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	char* lpBase;
	virtual BOOL OnInitDialog();
	DWORD RVAtoFOA(DWORD dwRVA);
};
