// ModuleDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "ModuleDialog.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// ModuleDialog �Ի���

IMPLEMENT_DYNAMIC(ModuleDialog, CDialogEx)

ModuleDialog::ModuleDialog(int Pid,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Module, pParent)
{
	this->nPID = Pid;
}

ModuleDialog::~ModuleDialog()
{
}

void ModuleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTModle, ListModule);
}


BEGIN_MESSAGE_MAP(ModuleDialog, CDialogEx)
END_MESSAGE_MAP()


// ModuleDialog ��Ϣ�������


BOOL ModuleDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ListModule.InsertColumn(0, L"��������", 0, 200);
	ListModule.InsertColumn(1, L"����ģ��", 0, 300);

	ListModule.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// 1. ����һ��ģ����صĿ��վ��
	hModuleSnap = CreateToolhelp32Snapshot(
		TH32CS_SNAPMODULE,  // ָ�����յ�����
		nPID);            // ָ������
	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return false;
	// 2. ͨ��ģ����վ����ȡ��һ��ģ����Ϣ
	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return false;
	}
	// 3. ѭ����ȡģ����Ϣ
	int index = 0;

	//3 ѭ����������ģ�����Ϣ
	do
	{
		TCHAR temp[20] = { 0 };

		_stprintf_s(temp, 20, L"%d", me32.th32ProcessID);
		ListModule.InsertItem(index, temp);

		//_stprintf_s(temp, 20, L"%d", me32.szExePath);
		ListModule.SetItemText(index, 1, me32.szExePath);
		
		++index;
	} while (Module32Next(hModuleSnap, &me32));
	// 4. �رվ�����˳�����
	CloseHandle(hModuleSnap);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
