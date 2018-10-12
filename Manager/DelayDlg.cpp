// DelayDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "DelayDlg.h"
#include "afxdialogex.h"


// CDelayDlg 对话框

IMPLEMENT_DYNAMIC(CDelayDlg, CDialogEx)

CDelayDlg::CDelayDlg(char* g_lpbase,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DelayDlg, pParent)
{
	lpbase = g_lpbase;
}

CDelayDlg::~CDelayDlg()
{
}

void CDelayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DelayDLLList, CDelayDllList);
	DDX_Control(pDX, IDC_DelayAPIList, CDelayAPIList);
}


BEGIN_MESSAGE_MAP(CDelayDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_DelayDLLList, &CDelayDlg::OnClickDelaydlllist)
END_MESSAGE_MAP()


// CDelayDlg 消息处理程序


DWORD CDelayDlg::RVAtoFOA(DWORD dwRVA)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpbase);
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

BOOL CDelayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CDelayDllList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CDelayDllList.InsertColumn(0, L"DLLName", 0, 100);
	CDelayDllList.InsertColumn(1, L"INT", 0, 100);
	CDelayDllList.InsertColumn(2, L"时间标志",0,100);
	CDelayDllList.InsertColumn(3, L"IAT",0,100);
	CDelayDllList.InsertColumn(4, L"Offset",0,100);
	CDelayDllList.InsertColumn(5, L"Bound IAT RVA",0,100);

	CDelayAPIList.InsertColumn(0, L"ThunkRVA", 0, 100);
	CDelayAPIList.InsertColumn(1, L"ThunkOffset", 0, 100);
	CDelayAPIList.InsertColumn(2, L"ThunkValue", 0, 100);
	CDelayAPIList.InsertColumn(3, L"Hint", 0, 100);
	CDelayAPIList.InsertColumn(4, L"APIName", 0, 100);
	ShowDelay();
	UpdateData(FALSE);
	return TRUE;
}

void CDelayDlg::ShowDelay()
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpbase);
	PIMAGE_DATA_DIRECTORY pDir = &(pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT]);
	DWORD dwDelay = RVAtoFOA(pDir->VirtualAddress);
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = (PIMAGE_DELAYLOAD_DESCRIPTOR)(dwDelay + lpbase);
	int index = 0;
	while (pDelay->DllNameRVA)
	{
		char tmp[20] = { 0 };
		DWORD dwNameFOA = RVAtoFOA(pDelay->DllNameRVA);
		// 		sprintf_s(tmp, 20, "%s", dwNameFOA+lpbase);
		CDelayDllList.InsertItem(index, CString(dwNameFOA + lpbase));

		DWORD ImportNameTableRVA = (DWORD)(pDelay->ImportNameTableRVA);
		sprintf_s(tmp, 20, "%08X", ImportNameTableRVA);
		CDelayDllList.SetItemText(index, 1, CString(tmp));

		DWORD dwtime = RVAtoFOA(pDelay->TimeDateStamp);
		sprintf_s(tmp, 20, "%08X", dwtime);
		CDelayDllList.SetItemText(index, 2, CString(tmp));

		DWORD IAT = pDelay->ImportAddressTableRVA;
		sprintf_s(tmp, 20, "%08X", IAT);
		CDelayDllList.SetItemText(index, 3, CString(tmp));

		sprintf_s(tmp, 20, "%08X", pDelay->DllNameRVA);
		CDelayDllList.SetItemText(index, 4, CString(tmp));

		sprintf_s(tmp, 20, "%08X", pDelay->BoundImportAddressTableRVA);
		CDelayDllList.SetItemText(index, 5, CString(tmp));
		pDelay++;
		index++;
	}

}

void CDelayDlg::ShowAPITable(DWORD number)
{
	CDelayAPIList.DeleteAllItems();
	auto pINTAddr =
		(PIMAGE_THUNK_DATA)(RVAtoFOA(number) + lpbase);
	int index = 0;
	while (pINTAddr->u1.Function)
	{
		char tmp[20] = { 0 };

		sprintf_s(tmp, 20, "%08X", number);
		CDelayAPIList.InsertItem(index, CString(tmp));

		sprintf_s(tmp, 20, "%08X", RVAtoFOA(number));
		CDelayAPIList.SetItemText(index, 1, CString(tmp));
		//判断是否是名称导入，如果最高位为0说明是名称导入
		//pThunk->u1.Ordinal & 0x80000000

		//ThunkValue
		sprintf_s(tmp, 20, "%08X", pINTAddr->u1.Function);
		CDelayAPIList.SetItemText(index, 2, CString(tmp));



		if (!IMAGE_SNAP_BY_ORDINAL32(pINTAddr->u1.Ordinal))
		{
			//
			DWORD pNameFOA = (DWORD)RVAtoFOA(pINTAddr->u1.AddressOfData);
			PIMAGE_IMPORT_BY_NAME pName =
				(PIMAGE_IMPORT_BY_NAME)(pNameFOA + lpbase);
			sprintf_s(tmp, 20, "%08X", pName->Hint);
			CDelayAPIList.SetItemText(index, 3, CString(tmp));
			CDelayAPIList.SetItemText(index, 4, CString(pName->Name));

		}
		else
		{
			//序号导入
			sprintf_s(tmp, 20, "%08X", (pINTAddr->u1.Ordinal & 0xFFFF));
			CDelayAPIList.SetItemText(index, 3, CString(tmp));
			//printf("函数序号：%04X 函数名：【NULL】\n",
			//	pThunk->u1.Ordinal & 0xFFFF);
		}

		//下一个函数
		pINTAddr++;
		index++;
	}
}


void CDelayDlg::OnClickDelaydlllist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString modlethunk = CDelayDllList.GetItemText(pNMItemActivate->iItem, 1);
	DWORD thunkvalue = wcstol(modlethunk, NULL, 16);
	ShowAPITable(thunkvalue);
	UpdateData(FALSE);
	*pResult = 0;
}
