
// ManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "ManagerDlg.h"
#include "afxdialogex.h"
#include "DialogA.h"
#include "DialogB.h"
#include "DialogC.h"
#include "DialogPE.h"
#include "DialogVsClear.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// DWORD WINAPI CPUThreadProc(LPVOID lpThreadParameter) {
// 	WaitForSingleObject(lpThreadParameter, 3000);
// 	return 0;
// }
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnPaint();
	//	afx_msg HCURSOR OnQueryDragIcon();
	//	afx_msg void OnDropFiles(HDROP hDropInfo);
//	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//	ON_WM_PAINT()
	//	ON_WM_QUERYDRAGICON()
	//	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CManagerDlg �Ի���



CManagerDlg::CManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);

}

BEGIN_MESSAGE_MAP(CManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_TIMER()
END_MESSAGE_MAP()

double FILETIME2Double(const _FILETIME& filetime) {
	return double(filetime.dwHighDateTime*4.294967296e9) + double(filetime.dwLowDateTime);
}

int GetCpuUsage() {
	_FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 100);
	//CreateThread(NULL, NULL, CPUThreadProc, hEvent, NULL, NULL);
	_FILETIME newIdleTime, newkernelTime, newuserTime;
	GetSystemTimes(&newIdleTime, &newkernelTime, &newuserTime);
	double dOldIdleTime = FILETIME2Double(idleTime);
	double dNewIdleTime = FILETIME2Double(newIdleTime);
	double dOldKernelTime = FILETIME2Double(kernelTime);
	double dNewKernelTime = FILETIME2Double(newkernelTime);
	double dOldUsertime = FILETIME2Double(userTime);
	double dNewUserTime = FILETIME2Double(newuserTime);
	return int(100.0 - (dNewIdleTime - dOldIdleTime) / (dNewKernelTime - dOldKernelTime + dNewUserTime - dOldUsertime)*100.0);
}

// CManagerDlg ��Ϣ��������

BOOL CManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������
	SetTimer(1, 5000, NULL);

	CDialogEx* m_Dlg[5];
	m_Dlg[0] = new CDialogA();
	m_Dlg[1] = new CDialogB();
	m_Dlg[2] = new CDialogC();
	m_Dlg[3] = new CDialogVsClear();
	m_Dlg[4] = new DialogPE();
	m_TabCtrl.InsertTab(5, L"������Ϣ", L"�߳���Ϣ", L"������Ϣ", L"VS��������", L"PE�ļ�����");
	m_TabCtrl.AddDig(5, IDD_DIALOGA, m_Dlg[0], IDD_DIALOGB, m_Dlg[1], IDD_WINDOWSDIALOG, m_Dlg[2], IDD_VSCLEAR, m_Dlg[3], IDD_PEDIALOG, m_Dlg[4]);
	m_TabCtrl.SetSelAndShow(0);

	//CPU״̬
	UINT buf[4] = { 1, 2, 3 };
	m_status.Create(this);
	m_status.SetIndicators(buf, 3);
	m_status.SetPaneInfo(0, 1, 0, 100);
	m_status.SetPaneInfo(1, 2, 0, 100);
	m_status.SetPaneInfo(2, 3, 0, 100);
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	DWORD occupancy_rate;
	occupancy_rate = memStatus.dwMemoryLoad;
	TCHAR MeBuff[100] = { 0 };
	TCHAR CpuTime[100] = { 0 };

	_stprintf_s(MeBuff, 100, L"�ڴ�ʹ���ʣ�%d", occupancy_rate);
	m_status.SetPaneText(0, MeBuff);
	//	m_status.SetPaneText(1, L"���");

	_stprintf_s(CpuTime, 100, L"CPUʹ���ʣ�%d", GetCpuUsage());
	m_status.SetPaneText(2, CpuTime);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	MEMORYSTATUS memStatus;
	GlobalMemoryStatus(&memStatus);
	DWORD occupancy_rate;
	occupancy_rate = memStatus.dwMemoryLoad;
	TCHAR MeBuff[100] = { 0 };
	TCHAR CpuTime[100] = { 0 };

	_stprintf_s(MeBuff, 100, L"�ڴ�ʹ���ʣ�%d", occupancy_rate);
	m_status.SetPaneText(0, MeBuff);
	//	m_status.SetPaneText(1, L"���");

	_stprintf_s(CpuTime, 100, L"CPUʹ���ʣ�%d", GetCpuUsage());
	m_status.SetPaneText(2, CpuTime);
	CDialogEx::OnTimer(nIDEvent);
}
