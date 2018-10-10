// TLSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "TLSDlg.h"
#include "afxdialogex.h"


// CTLSDlg 对话框

IMPLEMENT_DYNAMIC(CTLSDlg, CDialogEx)

CTLSDlg::CTLSDlg(char* g_lpbase, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TLSDlg, pParent)
{
	this->lpBase = g_lpbase;
}

CTLSDlg::~CTLSDlg()
{
}

void CTLSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTLSDlg, CDialogEx)
END_MESSAGE_MAP()


// CTLSDlg 消息处理程序

DWORD CTLSDlg::RVAtoFOA(DWORD dwRVA)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpBase);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	for (DWORD i = 0; i < dwCount; i++) {
		if (dwRVA >= pSection->VirtualAddress && dwRVA <= pSection->VirtualAddress + pSection->SizeOfRawData) {
			return dwRVA - pSection->VirtualAddress + pSection->PointerToRawData;
		}
		else
		{
			pSection++;
		}
	}
	return 0;
}


BOOL CTLSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpBase);
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[9];
	DWORD TLSRva = RVAtoFOA(pDir->VirtualAddress);
	PIMAGE_TLS_DIRECTORY TLS = (PIMAGE_TLS_DIRECTORY)(TLSRva + lpBase);
	char tmp[20] = {0};
	sprintf(tmp, "%08X", TLS->StartAddressOfRawData);
	GetDlgItem(IDC_DataStar)->SetWindowText(CString(tmp));
	sprintf(tmp, "%08X", TLS->EndAddressOfRawData);
	GetDlgItem(IDC_DataFinish)->SetWindowText(CString(tmp));
	sprintf(tmp, "%08X", TLS->AddressOfIndex);
	GetDlgItem(IDC_IndexValue)->SetWindowText(CString(tmp));
	sprintf(tmp, "%08X", TLS->AddressOfCallBacks);
	GetDlgItem(IDC_CallBackTab)->SetWindowText(CString(tmp));
	sprintf(tmp, "%08X", TLS->SizeOfZeroFill);
	GetDlgItem(IDC_ZeroSize)->SetWindowText(CString(tmp));
	sprintf(tmp, "%08X", TLS->Characteristics);
	GetDlgItem(IDC_Charactor)->SetWindowText(CString(tmp));
	UpdateData(FALSE);
	return TRUE;
}

