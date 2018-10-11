// ImportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "ImportDlg.h"
#include "afxdialogex.h"


// CImportDlg 对话框

IMPLEMENT_DYNAMIC(CImportDlg, CDialogEx)

CImportDlg::CImportDlg(DWORD vAddr,char* g_lpBase,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ImportDlg, pParent)
{
	this->virtualaddr = vAddr;
	this->lpbase = g_lpBase;
}

CImportDlg::~CImportDlg()
{
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImportFileList, CImportFlieList);
	DDX_Control(pDX, IDC_ImportFunList, CImportFunList);
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_ImportFileList, &CImportDlg::OnClickImportfilelist)
END_MESSAGE_MAP()


// CImportDlg 消息处理程序


BOOL CImportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//文件导入控件
	CImportFlieList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CImportFlieList.InsertColumn(0, L"DLLName", 0, 100);
	CImportFlieList.InsertColumn(1, L"OriginalFirstThunk", 0, 100);
	CImportFlieList.InsertColumn(2, L"TimeDateStamp", 0, 100);
	CImportFlieList.InsertColumn(3, L"ForWordChain", 0, 100);
	CImportFlieList.InsertColumn(4, L"名称RVA", 0, 100);
	CImportFlieList.InsertColumn(5, L"FirstThunk", 0, 100);
	//函数导入控件
	CImportFunList.InsertColumn(0, L"ThunkRVA", 0, 100);
	CImportFunList.InsertColumn(1, L"ThunkOffset", 0, 100);
	CImportFunList.InsertColumn(2, L"ThunkValue", 0, 100);
	CImportFunList.InsertColumn(3, L"Hint", 0, 100);
	CImportFunList.InsertColumn(4, L"APIName", 0, 100);
	ShowImportModleTab();
	
	return TRUE;  
}


DWORD CImportDlg::RVAtoFOA(DWORD dwRVA)
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

void CImportDlg::ShowImportModleTab()
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew+lpbase);
	PIMAGE_DATA_DIRECTORY pDir = &(pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]);
	DWORD dwpImport = RVAtoFOA(pDir->VirtualAddress);
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)(dwpImport + lpbase);
	int index = 0;
	while (pImport->Name)
	{
		char tmp[20] = { 0 };
 		DWORD dwNameFOA = RVAtoFOA(pImport->Name);
// 		sprintf_s(tmp, 20, "%s", dwNameFOA+lpbase);
		CImportFlieList.InsertItem(index, CString(dwNameFOA + lpbase));

		DWORD OrinalFirThunk = (DWORD)(pImport->OriginalFirstThunk );
		sprintf_s(tmp, 20, "%08X", OrinalFirThunk);
		CImportFlieList.SetItemText(index, 1,CString(tmp));

		DWORD dwtime = RVAtoFOA(pImport->TimeDateStamp);
		sprintf_s(tmp, 20, "%08X", dwtime);
		CImportFlieList.SetItemText(index, 2, CString(tmp));

		DWORD dwforwordchain = pImport->ForwarderChain;
		sprintf_s(tmp, 20, "%08X", dwforwordchain);
		CImportFlieList.SetItemText(index, 3, CString(tmp));

		sprintf_s(tmp, 20, "%08X", pImport->Name);
		CImportFlieList.SetItemText(index, 4, CString(tmp));

		sprintf_s(tmp, 20, "%08X", pImport->FirstThunk);
		CImportFlieList.SetItemText(index, 5, CString(tmp));
		pImport++;
		index++;
	}

}
//ThunkRVA", 
//ThunkOffset
//ThunkValue"
//Hint", 0, 1
//APIName", 0
void CImportDlg::ShowImportFunTab(DWORD number)
{
	CImportFunList.DeleteAllItems();
	auto pINTAddr =
		(PIMAGE_THUNK_DATA)(RVAtoFOA(number) + lpbase);
	int index = 0;
	while (pINTAddr->u1.Function)
	{
		char tmp[20] = { 0 };

		sprintf_s(tmp, 20, "%08X", number);
		CImportFunList.InsertItem(index, CString(tmp));

		sprintf_s(tmp, 20, "%08X", RVAtoFOA(number));
		CImportFunList.SetItemText(index, 1, CString(tmp));
		//判断是否是名称导入，如果最高位为0说明是名称导入
		//pThunk->u1.Ordinal & 0x80000000

		//ThunkValue
		sprintf_s(tmp, 20, "%08X", pINTAddr->u1.Function);
		CImportFunList.SetItemText(index, 2, CString(tmp));



		if (!IMAGE_SNAP_BY_ORDINAL32(pINTAddr->u1.Ordinal))
		{
			//
			DWORD pNameFOA = (DWORD)RVAtoFOA(pINTAddr->u1.AddressOfData);
			PIMAGE_IMPORT_BY_NAME pName =
				(PIMAGE_IMPORT_BY_NAME)(pNameFOA + lpbase);
			sprintf_s(tmp, 20, "%08X", pName->Hint);
			CImportFunList.SetItemText(index, 3, CString(tmp));
			CImportFunList.SetItemText(index, 4, CString(pName->Name));

		}
		else
		{
			//序号导入
			sprintf_s(tmp, 20, "%08X", (pINTAddr->u1.Ordinal & 0xFFFF));
			CImportFunList.SetItemText(index, 3, CString(tmp));
			//printf("函数序号：%04X 函数名：【NULL】\n",
			//	pThunk->u1.Ordinal & 0xFFFF);
		}

		//下一个函数
		pINTAddr++;
		index++;
	}
}



void CImportDlg::OnClickImportfilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//int i = CImportFlieList.GetSelectionMark();
	CString modlethunk = CImportFlieList.GetItemText(pNMItemActivate->iItem, 1);
	DWORD thunkvalue = wcstol(modlethunk, NULL, 16);
	ShowImportFunTab(thunkvalue);
	*pResult = 0;
}
