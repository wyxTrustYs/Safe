// RelocDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "RelocDlg.h"
#include "afxdialogex.h"



std::vector<PRELOCAREAINFO> vecpRelocAreaInfo;
// CRelocDlg 对话框

IMPLEMENT_DYNAMIC(CRelocDlg, CDialogEx)

CRelocDlg::CRelocDlg(char* g_lpbase,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Reloc_Dlg, pParent)
{
	this->lpbase = g_lpbase;
}

CRelocDlg::~CRelocDlg()
{
}

void CRelocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RelocSecLIST, CRelocSecList);
	DDX_Control(pDX, IDC_RelocBlockLIST, RelocBlockList);
}


BEGIN_MESSAGE_MAP(CRelocDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_RelocSecLIST, &CRelocDlg::OnClickRelocseclist)
END_MESSAGE_MAP()


// CRelocDlg 消息处理程序


BOOL CRelocDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//区段
	CRelocSecList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRelocSecList.InsertColumn(0, L"索引",0,130);
	CRelocSecList.InsertColumn(1, L"区段",0,130);
	CRelocSecList.InsertColumn(2, L"RVA",0,130);
	CRelocSecList.InsertColumn(3, L"项目",0,130);
	//块项目
	RelocBlockList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	RelocBlockList.InsertColumn(0, L"索引",0,130);
	RelocBlockList.InsertColumn(1, L"RVA",0,130);
	RelocBlockList.InsertColumn(2, L"偏移",0,130);                                                                                                                                                 
	RelocBlockList.InsertColumn(3, L"属性",0,150);
	RelocBlockList.InsertColumn(4, L"Far 地址",0,200);
	//RelocBlockList.InsertColumn(5, L"Data Interpretation",0,130);	 

	ShowReloc();
	return TRUE;  
}

DWORD CRelocDlg::RVAtoFOA(DWORD dwRVA) {
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




void CRelocDlg::ShowReloc() {
	int index = 1;
	//找到重定位表
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt =(PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpbase);
	PIMAGE_OPTIONAL_HEADER pOption = &pNt->OptionalHeader;
	DWORD ImageBase = pOption->ImageBase;
	PIMAGE_DATA_DIRECTORY pDir = &pNt->OptionalHeader.DataDirectory[5];
	
	DWORD dwRelocFOA = RVAtoFOA(pDir->VirtualAddress);
	//遍历

	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)(dwRelocFOA + lpbase);
	
	while (pReloc->SizeOfBlock)
	{
		PRELOCAREAINFO pRelocAreaInfo = new RELOCAREAINFO();
		char tmp[20] = { 0 };
		
		//重定位块所在区段位置
		pRelocAreaInfo->dwAreaRVA = pReloc->VirtualAddress;
		
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
		DWORD SectionCount = pNt->FileHeader.NumberOfSections;
		for (DWORD i = 0; i < SectionCount; i++) {
			if (pReloc->VirtualAddress >= pSection->VirtualAddress && pReloc->VirtualAddress <= pSection->VirtualAddress + pSection->SizeOfRawData) {
				pRelocAreaInfo->szSectionName = (DWORD)pSection->Name;
				break;
			}
			else
			{
				pSection++;
			}
		}

		//重定位起始位置
		PTYPEOFFSET pOffset = PTYPEOFFSET(pReloc + 1);
		//需要重定位的个数
		DWORD dwCount = (pReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
		pRelocAreaInfo->NumberOfReloc = dwCount;

		for (int i = 0; i < dwCount; i++) {
			RELOCINFO relocinfo;
			relocinfo.bType = pOffset->Type;
			
			if (pOffset->Type == 3) {

				//需要重定位的相对虚拟地址（RVA）
				DWORD RelocRva = DWORD(pOffset->Offsset + pReloc->VirtualAddress);
				relocinfo.dwRelocRVA = RelocRva;

				//文件偏移地址
				DWORD dwDataFOA = RVAtoFOA(RelocRva);
				relocinfo.dwOffset = dwDataFOA;
				
				//需要重定位的数据
				DWORD pData = (DWORD)(dwDataFOA + lpbase);
				//sprintf(relocinfo.bData, "%08X", pData);
				
				pRelocAreaInfo->vecRelocInfo.push_back(relocinfo);

				relocinfo.dwRelocValue = *(pOffset->Offsset + lpbase);
			}
			
			pOffset++;
		}
		vecpRelocAreaInfo.push_back(pRelocAreaInfo);
		pReloc = (PIMAGE_BASE_RELOCATION)(pReloc->SizeOfBlock + (DWORD)pReloc);
		index++;
	}
	ShowSectionInList();
}

void CRelocDlg::ShowSectionInList()
{
	int index = 1;
	RelocBlockList.DeleteAllItems();
	std::vector<PRELOCAREAINFO>::iterator iter;

	for (iter = vecpRelocAreaInfo.begin(); iter != vecpRelocAreaInfo.end(); iter++)
	{
		char tmp[20] = { 0 };
		PRELOCAREAINFO tmpRelocAreaInfo = *iter;

		sprintf(tmp, "%d", index);
		CRelocSecList.InsertItem(index - 1, CString(tmp));

		//显示区段名字
		sprintf(tmp, "%s", tmpRelocAreaInfo->szSectionName);
		CRelocSecList.SetItemText(index - 1, 1, CString(tmp));

		//显示RVA
		sprintf(tmp, "%08X", tmpRelocAreaInfo->dwAreaRVA);
		CRelocSecList.SetItemText(index - 1, 2, CString(tmp));

		//显示项目
		sprintf(tmp, "%Xh / %dd", tmpRelocAreaInfo->NumberOfReloc, tmpRelocAreaInfo->NumberOfReloc);
		CRelocSecList.SetItemText(index - 1, 3, CString(tmp));
		index++;
	}

}

//将块数据显示出来
void CRelocDlg::ShowBlockInList(int index)
{
	RelocBlockList.DeleteAllItems();
	PRELOCAREAINFO tmpRelocAreaInfo = vecpRelocAreaInfo[index];
	std::vector<RELOCINFO> vecRelocInfo = tmpRelocAreaInfo->vecRelocInfo;
	std::vector<RELOCINFO>::iterator iter;
	int num = 0;
	for (iter = vecRelocInfo.begin(); iter != vecRelocInfo.end(); iter++)
	{
		char tmp[20] = { 0 };
		RELOCINFO tmpRelocInfo = *iter;
		//显示索引
		sprintf(tmp, "%d", num);
		RelocBlockList.InsertItem(num, CString(tmp));
		//显示RVA
		sprintf(tmp, "%08X", tmpRelocInfo.dwRelocRVA);
		RelocBlockList.SetItemText(num, 1, CString(tmp));
		//显示偏移
		sprintf(tmp, "%08X", tmpRelocInfo.dwOffset);
		RelocBlockList.SetItemText(num, 2, CString(tmp));
		//显示属性
		sprintf(tmp, "HIGHLOW(%d)", tmpRelocInfo.bType);
		RelocBlockList.SetItemText(num, 3, CString(tmp));
		//显示Far地址
		sprintf(tmp, "%08X", tmpRelocInfo.dwRelocRVA);
		RelocBlockList.SetItemText(num, 4, CString(tmp));

//		RelocBlockList.SetItemText(num, 5, tmpRelocInfo.bData);
		num++;
	}

}

void CRelocDlg::OnClickRelocseclist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	int index = pNMItemActivate->iItem;
	ShowBlockInList(index);


	*pResult = 0;
}