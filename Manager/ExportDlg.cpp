// ExportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "ExportDlg.h"
#include "afxdialogex.h"


// CExportDlg 对话框

IMPLEMENT_DYNAMIC(CExportDlg, CDialogEx)

CExportDlg::CExportDlg(DWORD vAddr, char* g_lpBase, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ExportDlg, pParent)
{
	this->virtualaddr = vAddr;
	this->lpbase = g_lpBase;
}

CExportDlg::~CExportDlg()
{
}

void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ExportLIST, CExportList);
}


BEGIN_MESSAGE_MAP(CExportDlg, CDialogEx)
END_MESSAGE_MAP()


// CExportDlg 消息处理程序

//Oridinal
//RVA
//Offset
//Function Name
BOOL CExportDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CExportList.InsertColumn(0, L"Oridinal",0,100);
	CExportList.InsertColumn(1, L"RVA", 0, 100);
	CExportList.InsertColumn(2, L"Offset", 0, 100);
	CExportList.InsertColumn(3, L"Function Name", 0, 100);
	ShowExportTab();
	return TRUE;  
}

DWORD CExportDlg::RVAtoFOA(DWORD dwRVA)
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

void CExportDlg::ShowExportTab()
{
	char tmp[20] = { 0 };
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpbase);
	PIMAGE_DATA_DIRECTORY pDir = &(pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
	//导出表在文件中的位置
	auto pExportAdd = RVAtoFOA(pDir->VirtualAddress);
	PIMAGE_EXPORT_DIRECTORY pExportTable =(PIMAGE_EXPORT_DIRECTORY)(pExportAdd + lpbase);

	//输出表偏移地址
	memset(tmp, 0, 20 * sizeof(char));
	sprintf(tmp, "%08X", pExportAdd);
	GetDlgItem(Edit[0])->SetWindowText(CString(tmp));

	//特征值
	memset(tmp, 0, 20 * sizeof(char));
	DWORD pCharacter = (DWORD)(pExportTable->Characteristics);
	sprintf(tmp, "%08X", pCharacter);
	GetDlgItem(Edit[1])->SetWindowText(CString(tmp));

	//基址
	memset(tmp, 0, 20 * sizeof(char));
	DWORD dwBase = (DWORD)pExportTable->Base;
	sprintf(tmp, "%08X", dwBase);
	GetDlgItem(Edit[2])->SetWindowText(CString(tmp));

	//名称的RVA
	memset(tmp, 0, 20 * sizeof(char));
	DWORD NameRVA = pExportTable->Name;
	sprintf(tmp, "%08X", NameRVA);
	GetDlgItem(Edit[3])->SetWindowText(CString(tmp));

	//导出函数的名字
	memset(tmp, 0, 20 * sizeof(char));
	char* FunName = (char*)(RVAtoFOA(pExportTable->Name)+lpbase);
	GetDlgItem(Edit[4])->SetWindowText(CString(FunName));
	//函数数量
	memset(tmp, 0, 20 * sizeof(char));
	DWORD NumofFun = pExportTable->NumberOfFunctions;
	sprintf(tmp, "%d", NumofFun);
	GetDlgItem(Edit[5])->SetWindowText(CString(tmp));
	//函数名字的数量
	memset(tmp, 0, 20 * sizeof(char));
	DWORD NumofName = pExportTable->NumberOfNames;
	sprintf(tmp, "%d", NumofName);
	GetDlgItem(Edit[6])->SetWindowText(CString(tmp));

	//函数地址
	memset(tmp, 0, 20 * sizeof(char));
	DWORD AddrFunRVA = pExportTable->AddressOfFunctions;
	//DWORD AddrFun = (DWORD)(RVAtoFOA(pExportTable->AddressOfFunctions) + lpbase);
	sprintf(tmp, "%08X", AddrFunRVA);
	GetDlgItem(Edit[7])->SetWindowText(CString(tmp));

	//函数名称地址
	memset(tmp, 0, 20 * sizeof(char));
	DWORD pOrdinalAddrRVA = pExportTable->AddressOfNames;
		//(DWORD)(RVAtoFOA(pExportTable->AddressOfNameOrdinals) + lpbase);
	sprintf(tmp, "%08X", pOrdinalAddrRVA);
	GetDlgItem(Edit[8])->SetWindowText(CString(tmp));
	//函数名序号地址
	DWORD AddrFunNameNumRVA = pExportTable->AddressOfNameOrdinals;
	//DWORD AddrFunNameNum = (DWORD)(RVAtoFOA(pExportTable->AddressOfNameOrdinals) + lpbase);
	sprintf(tmp, "%08X", AddrFunNameNumRVA);
	GetDlgItem(Edit[9])->SetWindowText(CString(tmp));
	//函数地址
	 DWORD* AddrFun = (DWORD*)(RVAtoFOA(pExportTable->AddressOfFunctions) + lpbase);
	 //名称表
	 DWORD* pNameAddr =
		 (DWORD*)(RVAtoFOA(pExportTable->AddressOfNames) + lpbase);
	 //序号表
	 WORD* AddrFunNameNum = (WORD*)(RVAtoFOA(pExportTable->AddressOfNameOrdinals) + lpbase);
	
	for (DWORD orignal = 0; orignal < NumofFun; orignal++) {
		//如果为0说明是无效地址，直接跳过
		if (AddrFun == 0) {
			continue;
		}
		BOOL bFlag = FALSE;
		//遍历序号表中是否有此序号，如果有说明此函数有名字
		sprintf(tmp, "%08d", orignal);
		CExportList.InsertItem(orignal, CString(tmp));
		sprintf(tmp, "%08x", AddrFun[orignal]);
		CExportList.SetItemText(orignal, 1, CString(tmp));
		sprintf(tmp, "%08X", RVAtoFOA(AddrFun[orignal]));
		CExportList.SetItemText(orignal, 2, CString(tmp));
		for (DWORD j = 0; j < NumofName; j++) {
 			if (orignal == AddrFunNameNum[j]) {
				bFlag = TRUE;
				DWORD dwNameRVA = pNameAddr[j];
				sprintf(tmp, "%s", RVAtoFOA(dwNameRVA) + lpbase);
				CExportList.SetItemText(orignal, 3, CString(tmp));
 			}
		}
		if (!bFlag)
		{
			sprintf(tmp, "%s", "NULL");
			CExportList.SetItemText(orignal, 3, CString(tmp));
			//printf("函数名【NULL】，函数序号：%04X\n", i + pExportTable->Base);
		}
	}
	UpdateData(FALSE);
}

