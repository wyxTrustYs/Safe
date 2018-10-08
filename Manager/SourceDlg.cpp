// SourceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "SourceDlg.h"
#include "afxdialogex.h"
#include <map>
using namespace std;
DWORD CSourceDlg::StaticpResHeader = 0;
map<HTREEITEM, PIMAGE_RESOURCE_DIRECTORY_ENTRY> pResEntry;
// CSourceDlg 对话框

IMPLEMENT_DYNAMIC(CSourceDlg, CDialogEx)

CSourceDlg::CSourceDlg(char* lpBase, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SourceDlg, pParent)
{
	this->lpbase = lpBase;
}

CSourceDlg::~CSourceDlg()
{
}

void CSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SourceTREE1, CSourceTree);
}


BEGIN_MESSAGE_MAP(CSourceDlg, CDialogEx)

	//	ON_NOTIFY(NM_CLICK, IDC_SourceTREE1, &CSourceDlg::OnClickSourcetree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SourceTREE1, &CSourceDlg::OnSelchangedSourcetree1)
END_MESSAGE_MAP()


// CSourceDlg 消息处理程序



DWORD CSourceDlg::RVAtoFOA(DWORD dwRVA)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpbase);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	for (DWORD i = 0; i < dwCount; i++) {
		if (dwRVA >= pSection->VirtualAddress && dwRVA < pSection->VirtualAddress + pSection->SizeOfRawData) {
			return dwRVA - pSection->VirtualAddress + pSection->PointerToRawData;
		}
		pSection++;
	}
	return 0;
}




BOOL CSourceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowSourceTab();
	return TRUE;
}


void CSourceDlg::ShowSourceTab()
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpbase);
	PIMAGE_DATA_DIRECTORY pDir = (PIMAGE_DATA_DIRECTORY)(&pNt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE]);
	//找到资源表的FOA
	DWORD dwResourceFOA = RVAtoFOA(pDir->VirtualAddress);
	//遍历
	PIMAGE_RESOURCE_DIRECTORY pResHeader = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceFOA + lpbase);
	StaticpResHeader = dwResourceFOA;
//	StaticpResHeader = (DWORD)pResHeader;
	//第一层：种类
	char* arryResType[] = { "", "鼠标指针（Cursor）", "位图（Bitmap）", "图标（Icon）", "菜单（Menu）"
		, "对话框（Dialog）", "字符串列表（String Table）", "字体目录（Font Directory）", "字体（Font）", "快捷键（Accelerators）"
		, "非格式化资源（Unformatted）", "消息列表（Message Table）", "鼠标指针组（Croup Cursor）", "", "图标组（Group Icon）", ""
		, "版本信息（Version Information）" };

	DWORD dwCount_1 = pResHeader->NumberOfIdEntries + pResHeader->NumberOfNamedEntries;
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceHeaderEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResHeader + 1);
	char tmpNumname[20] = {};
	sprintf(tmpNumname, "%d", pResHeader->NumberOfNamedEntries);
	GetDlgItem(IDC_RootNamNumEDIT)->SetWindowText(CString(tmpNumname));
	char tmpNumID[20] = {};
	sprintf(tmpNumID, "%d", pResHeader->NumberOfIdEntries);
	GetDlgItem(IDC_RootIDNumEDIT)->SetWindowText(CString(tmpNumID));
	
	for (DWORD i = 0; i < dwCount_1; i++) {
		
		//判断是否是以字符串方式命名
		if (pResourceHeaderEntry->NameIsString)
		{
			PIMAGE_RESOURCE_DIR_STRING_U ResName = (PIMAGE_RESOURCE_DIR_STRING_U)(pResourceHeaderEntry->NameOffset + (DWORD)pResHeader);
			WCHAR* wcName = new WCHAR[ResName->Length + 1]{};
			memcpy(wcName, ResName->NameString, ResName->Length * 2);
			CString csName(wcName);
			hitem[i] = CSourceTree.InsertItem(csName, NULL, NULL);
		
			delete wcName;
		}
		else
		{
			if (pResourceHeaderEntry->Id <= 16 && pResourceHeaderEntry->Id > 0) {
				//ID命名
				char* cName = arryResType[pResourceHeaderEntry->Id];
				//memcpy(wcName, ResName->NameString, ResName->Length * 2);
				hitem[i] = CSourceTree.InsertItem(CString(cName), NULL, NULL);
			}
			else
			{
				char tmp[10] = { 0 };
				sprintf(tmp, "%04X", pResourceHeaderEntry->Id);
				hitem[i] = CSourceTree.InsertItem(CString(tmp), NULL, NULL);
			}
		}
		pResEntry.insert({ hitem[i], pResourceHeaderEntry });

		//判断是否有第二层
		if (pResourceHeaderEntry->DataIsDirectory) {
			//进入第二层
			PIMAGE_RESOURCE_DIRECTORY pResHeader_2 = (PIMAGE_RESOURCE_DIRECTORY)(pResourceHeaderEntry->OffsetToDirectory + (DWORD)pResHeader);
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry_2 =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResHeader_2 + 1);
			
			DWORD dwCount_2 = pResHeader_2->NumberOfIdEntries + pResHeader_2->NumberOfNamedEntries;
			
			//遍历
			for (DWORD j = 0; j < dwCount_2; j++) {
				char tmp_2[10] = {};
				if (pResEntry_2->NameIsString) {
					PIMAGE_RESOURCE_DIR_STRING_U ResName_2 = (PIMAGE_RESOURCE_DIR_STRING_U)(pResEntry_2->NameOffset + (DWORD)pResHeader_2);
					WCHAR* wcName_2 = new WCHAR[ResName_2->Length + 1]{};
					memcpy(wcName_2, ResName_2->NameString, ResName_2->Length * 2);
					hSubItem[j] = CSourceTree.InsertItem(CString(wcName_2), NULL, NULL, hitem[i]);
					delete wcName_2;
				}
				else
				{
					sprintf(tmp_2, "%04d", pResEntry_2->Id);
					hSubItem[j] = CSourceTree.InsertItem(CString(tmp_2), NULL, NULL, hitem[i]);

				}
				pResEntry.insert({ hSubItem[j], pResEntry_2 });

				pResEntry_2++;

			}//第二层
		}
		pResourceHeaderEntry++;
	}//第一层
}

void CSourceDlg::OnSelchangedSourcetree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	char tmpdata[20] = {};
	HTREEITEM htree = pNMTreeView->itemNew.hItem;//CSourceTree.GetSelectedItem();
	PIMAGE_RESOURCE_DIRECTORY pResHeader = (PIMAGE_RESOURCE_DIRECTORY)(StaticpResHeader + lpbase);
	PIMAGE_RESOURCE_DIRECTORY_ENTRY TmpResEntry = pResEntry[htree];
	PIMAGE_RESOURCE_DIRECTORY pResHeader_2 = (PIMAGE_RESOURCE_DIRECTORY)(TmpResEntry->OffsetToDirectory + (DWORD)pResHeader);
	
	DWORD IdNum = pResHeader_2->NumberOfIdEntries;
	DWORD NameNum = pResHeader_2->NumberOfNamedEntries;
	sprintf(tmpdata, "%d", NameNum);
	GetDlgItem(IDC_SelNamNumEDIT)->SetWindowText(CString(tmpdata));

	sprintf(tmpdata, "%d", IdNum);
	GetDlgItem(IDC_SelIDNumEDIT)->SetWindowText(CString(tmpdata));
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry_2 =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResHeader_2 + 1);
	if (!(pResEntry_2->DataIsDirectory)) {
		PIMAGE_RESOURCE_DATA_ENTRY pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(pResEntry_2->OffsetToData + (DWORD)pResHeader);

		DWORD dwDataRVA = pDataEntry->OffsetToData;
		sprintf(tmpdata, "%08x", dwDataRVA);
		GetDlgItem(IDC_RVAEdit)->SetWindowText(CString(tmpdata));

		DWORD dwDataFOA = RVAtoFOA(dwDataRVA);
		sprintf(tmpdata, "%08x", dwDataFOA);
		GetDlgItem(IDC_offsetEDIT)->SetWindowText(CString(tmpdata));

		DWORD dwSize = pDataEntry->Size;
		sprintf(tmpdata, "%08X", dwSize);
		GetDlgItem(IDC_SizeEDIT)->SetWindowText(CString(tmpdata));
		
	}
	UpdateData(FALSE);

	*pResult = 0;
}
