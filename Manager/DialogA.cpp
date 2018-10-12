// DialogA.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "DialogA.h"
#include "afxdialogex.h"
#include "DialogSubThread.h"
#include "DialogHeap.h"
#include "ModuleDialog.h"
#include <TlHelp32.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")

// CDialogA �Ի���

IMPLEMENT_DYNAMIC(CDialogA, CDialogEx)

CDialogA::CDialogA(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGA, pParent)
{

}

CDialogA::~CDialogA()
{
}

void CDialogA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrlA);

	DDX_Control(pDX, FlushProcess, Flush);
}


BEGIN_MESSAGE_MAP(CDialogA, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CDialogA::OnRclickList2)
	ON_COMMAND(ID_32781, &CDialogA::On32781)
	//	ON_BN_CLICKED(IDC_BUTTON2, &CDialogA::OnBnClickedButton2)
	ON_COMMAND(ID_32782, &CDialogA::On32782)
	ON_BN_CLICKED(FlushProcess, &CDialogA::OnBnClickedFlushprocess)
	ON_COMMAND(ID_32786, &CDialogA::On32786)
	ON_COMMAND(ID_32787, &CDialogA::On32787)

	ON_BN_CLICKED(IDC_btnCleanMem, &CDialogA::OnBnClickedbtncleanmem)
	ON_COMMAND(ID_ProtectProc, &CDialogA::OnProtectproc)
	ON_COMMAND(ID_EndProtect, &CDialogA::OnEndprotect)
END_MESSAGE_MAP()


// CDialogA ��Ϣ�������


BOOL CDialogA::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_ListCtrlA.InsertColumn(0, L"������", 0, 200);
	m_ListCtrlA.InsertColumn(1, L"pID", 0, 70);
	m_ListCtrlA.InsertColumn(2, L"���ȼ�", 0, 70);
	m_ListCtrlA.InsertColumn(3, L"�߳���", 0, 70);

	m_ListCtrlA.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//1 ����һ������
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//2 �ҵ���һ�����̵���Ϣ
	if (Process32First(hSnap, &pe) == TRUE)
	{
		int index = 0;

		//3 ѭ�������������̵���Ϣ
		do
		{
			TCHAR temp[20] = { 0 };

			m_ListCtrlA.InsertItem(index, pe.szExeFile);
			_stprintf_s(temp, 20, L"%d", pe.th32ProcessID);
			m_ListCtrlA.SetItemText(index, 1, temp);

			_stprintf_s(temp, 20, L"%d", pe.pcPriClassBase);
			m_ListCtrlA.SetItemText(index, 2, temp);
			_stprintf_s(temp, 20, L"%d", pe.cntThreads);
			m_ListCtrlA.SetItemText(index, 3, temp);
			++index;
		} while (Process32Next(hSnap, &pe));
	}
	//	EnumWindows(&EnumWindowProc, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogA::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu, *pmenu;
	int i = m_ListCtrlA.GetSelectionMark();
	menu.LoadMenu(IDR_MENU1);
	pmenu = menu.GetSubMenu(0);
	//����һ������ȷ�����λ�õ�λ�� 
	CPoint point1;
	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����
	GetCursorPos(&point1);
	//��ָ��λ����ʾ�����˵�
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	*pResult = 0;
	//ListId = m_ListCtrlA.Get
}


void CDialogA::On32781()
{
	// TODO: �ڴ���������������
	int i = m_ListCtrlA.GetSelectionMark();
	int PID = _tstoi(m_ListCtrlA.GetItemText(i, 1));
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,  //Ҫ�����һ��Ȩ��
		FALSE,               //����Ƿ�ɼ̳�
		PID                 //����ID
	);
	//2 ��������
	TerminateProcess(hProcess, 0);

}



void CDialogA::OnBnClickedFlushprocess()
{
	m_ListCtrlA.DeleteAllItems();
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//1 ����һ������
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//2 �ҵ���һ�����̵���Ϣ
	if (Process32First(hSnap, &pe) == TRUE)
	{
		int index = 0;

		//3 ѭ�������������̵���Ϣ
		do
		{
			TCHAR temp[20] = { 0 };

			m_ListCtrlA.InsertItem(index, pe.szExeFile);
			_stprintf_s(temp, 20, L"%d", pe.th32ProcessID);
			m_ListCtrlA.SetItemText(index, 1, temp);

			_stprintf_s(temp, 20, L"%d", pe.pcPriClassBase);
			m_ListCtrlA.SetItemText(index, 2, temp);
			_stprintf_s(temp, 20, L"%d", pe.cntThreads);
			m_ListCtrlA.SetItemText(index, 3, temp);
			++index;
		} while (Process32Next(hSnap, &pe));
	}
}


void CDialogA::On32782()
{
	// TODO: �ڴ���������������
	int i = m_ListCtrlA.GetSelectionMark();
	int PID = _tstoi(m_ListCtrlA.GetItemText(i, 1));
	DialogSubThread* newDialog = new DialogSubThread(PID);
	newDialog->DoModal();
	delete newDialog;
}




void CDialogA::On32786()
{
	// TODO: �ڴ���������������

	int i = m_ListCtrlA.GetSelectionMark();
	int PID = _tstoi(m_ListCtrlA.GetItemText(i, 1));
	ModuleDialog* md = new ModuleDialog(PID);
	md->DoModal();
	delete md;
}

//��ȡ����Ϣ
void CDialogA::On32787()
{
	// TODO: �ڴ���������������
	int i = m_ListCtrlA.GetSelectionMark();
	int PID = _tstoi(m_ListCtrlA.GetItemText(i, 1));
	CDialogHeap* heap = new CDialogHeap(PID);

	heap->DoModal();
	delete heap;
}



void CDialogA::OnBnClickedbtncleanmem()
{
	//��ȡ��ǰ�ڴ�״̬
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;

	//�����ڴ�
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize; i++) {
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	//��ȡ�������ڴ�״̬
	GlobalMemoryStatusEx(&stcMemStatusEx);
	DWORDLONG afterCleanUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
}


void CDialogA::OnProtectproc()
{
	int i = m_ListCtrlA.GetSelectionMark();
	DWORD pPid = _tstoi(m_ListCtrlA.GetItemText(i, 1));
	DWORD Pid = 0, RealWrite = 0;
	//DWORD pPid = GetCurrentProcessId();
	CreateSemaphoreA(NULL, pPid, pPid + 1, "MyPid");
	HWND hwnd = ::FindWindow(NULL, L"���������");
	GetWindowThreadProcessId(hwnd, &Pid);

	TaskhProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);

	LPVOID ProcessAddr = VirtualAllocEx(TaskhProcess, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	WriteProcessMemory(TaskhProcess, ProcessAddr, "E:\\InlineHook.dll", strlen("E:\\InlineHook.dll") + 1, &RealWrite);

	HANDLE Thread = CreateRemoteThread(TaskhProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, ProcessAddr, NULL, NULL);

	WaitForSingleObject(Thread, INFINITE);
	//TestModule = LoadLibraryA("E:\\InlineHook.dll");
}


void CDialogA::OnEndprotect()
{


// 	DWORD Pid = 0, RealWrite = 0;
// 	LPVOID ProcessAddr = VirtualAllocEx(TaskhProcess, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
// 
// 	WriteProcessMemory(TaskhProcess, ProcessAddr, "E:\\InlineHook.dll", strlen("E:\\InlineHook.dll") + 1, &RealWrite);
	DWORD dwPid = GetProcessId(TaskhProcess);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);

	MODULEENTRY32 me32;
	me32.dwSize = sizeof(me32);

	//����ƥ��Ľ�������
	BOOL bRet = Module32First(hSnap, &me32);
	WCHAR dllPath[] = L"E:\\InlineHook.dll";
	while (bRet)
	{
		if (lstrcmp(me32.szExePath, dllPath) == 0)
		{
			break;
		}
		bRet = Module32Next(hSnap, &me32);
	}

	CloseHandle(hSnap);
	TestModule = me32.hModule;
	HANDLE Thread = CreateRemoteThread(TaskhProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, TestModule, NULL, NULL);
	//FreeLibrary()
	WaitForSingleObject(Thread, INFINITE);
	//FreeLibrary(TestModule);
}
