// ServiceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "ServiceDlg.h"
#include "afxdialogex.h"
#include <Winsvc.h>

// CServiceDlg �Ի���

WCHAR *servicestatus[] = { L"", L"��ֹͣ" , L"��������" ,L"����ֹͣ" ,L"��������",L"��Ҫ����",
L"������ͣ",L"����ͣ" };
WCHAR *StartType[] = { L"ϵͳ����",L"��ʼ������",L"�Զ�",L"�ֶ�",L"����" };
SC_HANDLE hSCM;
IMPLEMENT_DYNAMIC(CServiceDlg, CDialogEx)

CServiceDlg::CServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ServiceDlg, pParent)
{

}

CServiceDlg::~CServiceDlg()
{
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ServiceList, CServiceList);
}


BEGIN_MESSAGE_MAP(CServiceDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_ServiceList, &CServiceDlg::OnRclickServicelist)
	ON_COMMAND(ID_Startservice, &CServiceDlg::OnStartservice)
	ON_COMMAND(ID_Closeservice, &CServiceDlg::OnCloseservice)
END_MESSAGE_MAP()


// CServiceDlg ��Ϣ�������


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CServiceList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CServiceList.InsertColumn(0, L"����", 0, 120);
	CServiceList.InsertColumn(1, L"PID", 0, 120);
	CServiceList.InsertColumn(2, L"����", 0, 120);
	CServiceList.InsertColumn(3, L"״̬", 0, 120);
	CServiceList.InsertColumn(4, L"��������", 0, 120);
	if (!EnumService()) MessageBox(L"�򿪷���ʧ��", L"ʧ��", MB_OK);
	return TRUE;  
}

BOOL CServiceDlg::EnumService()
{
	//�򿪷�����ƹ�����
	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(
		hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32, 
		SERVICE_STATE_ALL,		//���з���״̬
		NULL,					//������
		0,						//��������С
		&dwSize,				//��Ҫ�Ĵ�С
		&dwServiceNum,			//�������еķ������
		NULL, NULL
	);
	//������Ҫ���ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//�ڶ���ö��
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(
		hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,		//���з���״̬
		(PBYTE)pEnumService,					//������
		dwSize,						//��������С
		&dwSize,				//��Ҫ�Ĵ�С
		&dwServiceNum,			//�������еķ������
		NULL, NULL
	);

	//������Ϣ
	for (DWORD i = 0; i < dwServiceNum; i++) {
		int index ;
		//��ȡ������Ϣ
		//������
		CServiceList.InsertItem(i, pEnumService[i].lpServiceName);
		//����״̬���� ��ֹͣ �������� ������ͣ
		//���ݵõ���ֵ�ֶ�����ַ���
		index = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		char pid[10] = { 0 };
		sprintf(pid, "%d", pEnumService[i].ServiceStatusProcess.dwProcessId);
		CServiceList.SetItemText(i,3,CString(servicestatus[index]));
		CServiceList.SetItemText(i, 1, CString(pid));
		//��������
		//�� �ļ�ϵͳ���������������񣬶������̷���
		//index = pEnumService[i].ServiceStatusProcess.dwServiceType;
		//��������ʾ����
		CServiceList.SetItemText(i, 2, CString(pEnumService[i].lpDisplayName));
		//�������ϸ��Ϣ
		//�򿪷���
		SC_HANDLE hService = OpenService(
			hSCM,								//������ƹ��������
			pEnumService[i].lpServiceName,		//������
			SERVICE_QUERY_CONFIG				//��Ȩ��
		);
		//��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//�ڶ��ε��ã���ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//ͨ�������ȡ���Ľṹ����Ϣ����õ���Ҫ��ֵ
		//��ȡ��������
		index = pServiceConfig->dwStartType;
		CServiceList.SetItemText(i, 4, CString(StartType[index]));
		//�����У����������������ֶ������������ѽ��á�
		//��ȡ·����Ϣ
		//pServiceConfig->lpBinaryPathName;
	}
	return TRUE;
}




void CServiceDlg::OnRclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu, *pmenu;
	menu.LoadMenu(IDR_MENU3);
	pmenu = menu.GetSubMenu(0);
	Selrow = CServiceList.GetSelectionMark();
	CPoint point1;
	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����
	GetCursorPos(&point1);
	//��ָ��λ����ʾ�����˵�
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	*pResult = 0;
}


void CServiceDlg::OnStartservice()
{
	// TODO: �ڴ���������������

	SC_HANDLE hService = OpenService(hSCM, CServiceList.GetItemText(Selrow, 0),
		SERVICE_START | SERVICE_STOP | SERVICE_INTERROGATE);
	if (!hService)
	{
		MessageBox(L"Ȩ�޲���", 0, MB_OK | MB_ICONERROR);
		CloseServiceHandle(hService);
		return;
	}
	SERVICE_STATUS sStatus = {};
	QueryServiceStatus(hService, &sStatus);
	if (sStatus.dwCurrentState == SERVICE_RUNNING)
	{
		MessageBox(L"�����Ѿ���", 0, MB_OK | MB_ICONERROR);
		CloseServiceHandle(hService);
		return;
	}
	else
	{
		if (StartService(hService, NULL, NULL))
		{
			CloseServiceHandle(hService);
			Sleep(100);
			EnumService();
		}

		else
		{
			MessageBox(L"�����ʧ��", 0, MB_OK | MB_ICONERROR);
			CloseServiceHandle(hService);
			return;
		}
	}

}


void CServiceDlg::OnCloseservice()
{
	SC_HANDLE hService = OpenService(hSCM, CServiceList.GetItemText(Selrow, 0),
		SERVICE_START | SERVICE_STOP | SERVICE_INTERROGATE);
	if (!hService)
	{
		MessageBox(L"Ȩ�޲���", 0, MB_OK | MB_ICONERROR);
		CloseServiceHandle(hService);
		return;
	}
	SERVICE_STATUS sStatus = {};
	QueryServiceStatus(hService, &sStatus);
	if (sStatus.dwCurrentState == SERVICE_STOPPED)
	{
		MessageBox(L"�����Ѿ�ֹͣ", 0, MB_OK | MB_ICONERROR);
		CloseServiceHandle(hService);
		return;
	}
	else
	{
		if (ControlService(hService, SERVICE_CONTROL_STOP, &sStatus))
		{
			CloseServiceHandle(hService);
			Sleep(100);
			EnumService();
		}

		else
		{
			MessageBox(L"�ر�ʧ��", 0, MB_OK | MB_ICONERROR);
			CloseServiceHandle(hService);
			return;
		}
	}

}
