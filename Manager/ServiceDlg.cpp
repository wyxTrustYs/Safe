// ServiceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "ServiceDlg.h"
#include "afxdialogex.h"
#include <Winsvc.h>

// CServiceDlg 对话框

WCHAR *servicestatus[] = { L"", L"已停止" , L"正在启动" ,L"正在停止" ,L"正在运行",L"将要继续",
L"正在暂停",L"已暂停" };
WCHAR *StartType[] = { L"系统加载",L"初始化加载",L"自动",L"手动",L"禁用" };
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


// CServiceDlg 消息处理程序


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CServiceList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// TODO:  在此添加额外的初始化
	CServiceList.InsertColumn(0, L"名称", 0, 120);
	CServiceList.InsertColumn(1, L"PID", 0, 120);
	CServiceList.InsertColumn(2, L"描述", 0, 120);
	CServiceList.InsertColumn(3, L"状态", 0, 120);
	CServiceList.InsertColumn(4, L"启动类型", 0, 120);
	if (!EnumService()) MessageBox(L"打开服务失败", L"失败", MB_OK);
	return TRUE;  
}

BOOL CServiceDlg::EnumService()
{
	//打开服务控制管理器
	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//第一次调用，获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(
		hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32, 
		SERVICE_STATE_ALL,		//所有服务状态
		NULL,					//缓冲区
		0,						//缓冲区大小
		&dwSize,				//需要的大小
		&dwServiceNum,			//缓冲区中的服务个数
		NULL, NULL
	);
	//申请需要的内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx(
		hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,		//所有服务状态
		(PBYTE)pEnumService,					//缓冲区
		dwSize,						//缓冲区大小
		&dwSize,				//需要的大小
		&dwServiceNum,			//缓冲区中的服务个数
		NULL, NULL
	);

	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++) {
		int index ;
		//获取基础信息
		//服务名
		CServiceList.InsertItem(i, pEnumService[i].lpServiceName);
		//服务状态：有 已停止 正在运行 正在暂停
		//根据得到的值手动输出字符串
		index = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		char pid[10] = { 0 };
		sprintf(pid, "%d", pEnumService[i].ServiceStatusProcess.dwProcessId);
		CServiceList.SetItemText(i,3,CString(servicestatus[index]));
		CServiceList.SetItemText(i, 1, CString(pid));
		//服务类型
		//有 文件系统驱动服务，驱动服务，独立进程服务
		//index = pEnumService[i].ServiceStatusProcess.dwServiceType;
		//描述段显示厂商
		CServiceList.SetItemText(i, 2, CString(pEnumService[i].lpDisplayName));
		//服务的详细信息
		//打开服务
		SC_HANDLE hService = OpenService(
			hSCM,								//服务控制管理器句柄
			pEnumService[i].lpServiceName,		//服务名
			SERVICE_QUERY_CONFIG				//打开权限
		);
		//第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//第二次调用，获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//通过上面获取到的结构体信息具体得到想要的值
		//获取启动类型
		index = pServiceConfig->dwStartType;
		CServiceList.SetItemText(i, 4, CString(StartType[index]));
		//类型有：“自启动”，“手动启动”，“已禁用”
		//获取路径信息
		//pServiceConfig->lpBinaryPathName;
	}
	return TRUE;
}




void CServiceDlg::OnRclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu, *pmenu;
	menu.LoadMenu(IDR_MENU3);
	pmenu = menu.GetSubMenu(0);
	Selrow = CServiceList.GetSelectionMark();
	CPoint point1;
	//获取当前光标的位置，以便使得菜单可以跟随光标
	GetCursorPos(&point1);
	//在指定位置显示弹出菜单
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	*pResult = 0;
}


void CServiceDlg::OnStartservice()
{
	// TODO: 在此添加命令处理程序代码

	SC_HANDLE hService = OpenService(hSCM, CServiceList.GetItemText(Selrow, 0),
		SERVICE_START | SERVICE_STOP | SERVICE_INTERROGATE);
	if (!hService)
	{
		MessageBox(L"权限不够", 0, MB_OK | MB_ICONERROR);
		CloseServiceHandle(hService);
		return;
	}
	SERVICE_STATUS sStatus = {};
	QueryServiceStatus(hService, &sStatus);
	if (sStatus.dwCurrentState == SERVICE_RUNNING)
	{
		MessageBox(L"服务已经打开", 0, MB_OK | MB_ICONERROR);
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
			MessageBox(L"服务打开失败", 0, MB_OK | MB_ICONERROR);
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
		MessageBox(L"权限不够", 0, MB_OK | MB_ICONERROR);
		CloseServiceHandle(hService);
		return;
	}
	SERVICE_STATUS sStatus = {};
	QueryServiceStatus(hService, &sStatus);
	if (sStatus.dwCurrentState == SERVICE_STOPPED)
	{
		MessageBox(L"服务已经停止", 0, MB_OK | MB_ICONERROR);
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
			MessageBox(L"关闭失败", 0, MB_OK | MB_ICONERROR);
			CloseServiceHandle(hService);
			return;
		}
	}

}
