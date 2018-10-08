// SectionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "SectionDlg.h"
#include "afxdialogex.h"


// CSectionDlg 对话框

IMPLEMENT_DYNAMIC(CSectionDlg, CDialogEx)

CSectionDlg::CSectionDlg(char* g_lpbase,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SectionDlg, pParent)
{
	this->lpbase = g_lpbase;
}

CSectionDlg::~CSectionDlg()
{
}

void CSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SectionLIST, CSecList);
}


BEGIN_MESSAGE_MAP(CSectionDlg, CDialogEx)
END_MESSAGE_MAP()


// CSectionDlg 消息处理程序


BOOL CSectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CSecList.InsertColumn(0, L"名称", 0, 70);
	CSecList.InsertColumn(1, L"VOffset", 0, 70);
	CSecList.InsertColumn(2, L"VSize", 0, 70);
	CSecList.InsertColumn(3, L"ROffset", 0, 70);
	CSecList.InsertColumn(4, L"RSize", 0, 70);
	CSecList.InsertColumn(5, L"标志", 0, 70);
	ShowSectionInfo();
	return TRUE;
}

BOOL CSectionDlg::ShowSectionInfo()
{
	PIMAGE_DOS_HEADER pdos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNT = (PIMAGE_NT_HEADERS32)(pdos->e_lfanew + lpbase);
	PIMAGE_SECTION_HEADER pSection = (PIMAGE_SECTION_HEADER)IMAGE_FIRST_SECTION(pNT);
	
	DWORD dCount = pNT->FileHeader.NumberOfSections;
	for (int i = 0; i < dCount; i++) {
		TCHAR temp[20] = { 0 };
		CSecList.InsertItem(i, CString(pSection[i].Name));
		_stprintf_s(temp, 20, L"%08X", pSection[i].VirtualAddress);
		CSecList.SetItemText(i, 1, temp);
		_stprintf_s(temp, 20, L"%08X", pSection[i].Misc.VirtualSize);
		CSecList.SetItemText(i, 2, temp);
		_stprintf_s(temp, 20, L"%08X", pSection[i].PointerToRawData);
		CSecList.SetItemText(i, 3, temp);
		_stprintf_s(temp, 20, L"%08X", pSection[i].SizeOfRawData);
		CSecList.SetItemText(i, 4, temp);
		_stprintf_s(temp, 20, L"%08X", pSection[i].Characteristics);
		CSecList.SetItemText(i, 5, temp);
	}
	return 0;
}
