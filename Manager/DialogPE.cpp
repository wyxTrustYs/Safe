// DialogPE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "DialogPE.h"
#include "SectionDlg.h"
#include "afxdialogex.h"
#include "DirDlg.h"
#include <time.h>

// DialogPE �Ի���

IMPLEMENT_DYNAMIC(DialogPE, CDialogEx)

DialogPE::DialogPE(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEDIALOG, pParent)
	, CEntryEdit(_T(""))
	, CSubSysEdit(_T(""))
	, CMirrorBaseEdit(_T(""))
	, CSectionNumEdit(_T(""))
	, CMirrorSizeEdit(_T(""))
	, CTimeEdit(_T(""))
	, CCodeBaseEdit(_T(""))
	, CHeadEdit(_T(""))
	, CDataBaseEdit(_T(""))
	, CSecAlignmentEdit(_T(""))
	, CCheckSumEdit(_T(""))
	, CFileAlignmentEdit(_T(""))
	, CSleHeadSizeEdit(_T(""))
	, CMagicEdit(_T(""))
	, CRVAEdit(_T(""))
	, CCharacteristicsEdit(_T(""))
{

}

DialogPE::~DialogPE()
{
}

void DialogPE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EntryEdit, CEntryEdit);
	DDX_Text(pDX, IDC_SubSysEdit, CSubSysEdit);
	DDX_Text(pDX, IDC_MirrorBaseEdit, CMirrorBaseEdit);
	DDX_Text(pDX, IDC_SectionNumEDIT, CSectionNumEdit);
	DDX_Text(pDX, IDC_MirrorSizeEdit, CMirrorSizeEdit);
	DDX_Text(pDX, IDC_TimeEdit, CTimeEdit);
	DDX_Text(pDX, IDC_CodeBaseEdit, CCodeBaseEdit);
	DDX_Text(pDX, IDC_HeadEdit, CHeadEdit);
	DDX_Text(pDX, IDC_DataBaseEdit, CDataBaseEdit);
	DDX_Text(pDX, IDC_SecAlignmentEdit, CSecAlignmentEdit);
	DDX_Text(pDX, IDC_CheckSumEdit, CCheckSumEdit);
	DDX_Text(pDX, IDC_FileAlignmentEdit, CFileAlignmentEdit);
	DDX_Text(pDX, IDC_SleHeadSizeEdit, CSleHeadSizeEdit);
	DDX_Text(pDX, IDC_MagicEdit, CMagicEdit);
	DDX_Text(pDX, IDC_RVAEdit, CRVAEdit);
	DDX_Text(pDX, IDC_CharacteristicsEdit, CCharacteristicsEdit);
	DDX_Control(pDX, IDSECTION, btnSection);
	DDX_Control(pDX, IDDIR, btnShowDir);
}


BEGIN_MESSAGE_MAP(DialogPE, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDSECTION, &DialogPE::OnBnClickedSection)
	
	ON_BN_CLICKED(IDDIR, &DialogPE::OnBnClickedDir)
END_MESSAGE_MAP()


// DialogPE ��Ϣ�������


void DialogPE::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int DropCount = DragQueryFile(hDropInfo, 0, FilePath, 100);
	ReadFileToMemory();
	IsPE(g_lpBase);
	ShowHeaderInfo(g_lpBase);
	ShowOptionInfo();
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}



char* DialogPE::ReadFileToMemory()
{
	//���ļ���ȡ���
	HANDLE hFile = CreateFile(FilePath,
		GENERIC_READ,
		FALSE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"�ļ���ʧ��", L"����", MB_OK);
		return 0;
	}

	//��ȡ�ļ���С
	DWORD dwSize = GetFileSize(hFile, NULL);

	/*char* pBuf*/g_lpBase = new char[dwSize] {};
	//���ļ�
	DWORD dwCount = 1;
	BOOL bRet =
		ReadFile(hFile, g_lpBase/*pBuf*/, dwSize, &dwCount, NULL);

	if (bRet)
	{
		return g_lpBase/*pBuf*/;
	}
	//�ͷ���Դ
	CloseHandle(hFile);
	delete g_lpBase/*pBuf*/;
	return 0;
}


DWORD DialogPE::RVAtoFOA(DWORD dwRVA)
{
	//�����ĸ�������
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)g_lpBase;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)
		(pDos->e_lfanew + g_lpBase);

	//�ҵ����α�ͷ��ַ
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	//��ȡ���θ���
	DWORD dwCount = pNt->FileHeader.NumberOfSections;
	for (DWORD i = 0; i < dwCount; i++)
	{
		if (dwRVA >= pSection->VirtualAddress &&
			dwRVA < pSection->VirtualAddress + pSection->SizeOfRawData)
		{
			return dwRVA -
				pSection->VirtualAddress +
				pSection->PointerToRawData;
		}
		pSection++;
	}
	return 0;
}

BOOL DialogPE::IsPE(char* lpBase)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE/*0x4D5A*/)
	{
		MessageBox(L"���ļ�����PE�ļ�", L"ERROR", MB_OK);
		return FALSE;
	}
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpBase);
	if (pNt->Signature != IMAGE_NT_SIGNATURE/*0x4550*/)
	{
		MessageBox(L"Ѱ��NTͷʧ��", L"ERROR", MB_OK);
		return FALSE;
	}
	return TRUE;
}

void DialogPE::ShowHeaderInfo(char* lpBase)
{
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpBase;
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpBase);

	//OEP
	CEntryEdit.Format(L" %08X", pNt->OptionalHeader.AddressOfEntryPoint);
	//printf("OEP: %08X\n", pNt->OptionalHeader.AddressOfEntryPoint);
	//ImageBase
	CSectionNumEdit.Format(L"%08X", pNt->FileHeader.NumberOfSections);
	UpdateData(FALSE);
	//printf("ImageBase: %08X\n", pNt->OptionalHeader.ImageBase);
	//���θ���
	//printf("���θ���: %08X\n", pNt->FileHeader.NumberOfSections);

}

void DialogPE::ShowOptionInfo() {
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)g_lpBase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS32)(pDos->e_lfanew + g_lpBase);
	PIMAGE_OPTIONAL_HEADER32 pOptionHearder = &pNt->OptionalHeader;
	PIMAGE_FILE_HEADER pFile = &(pNt->FileHeader);
	tm* FileTime = gmtime((time_t*)&pFile->TimeDateStamp);

	CMirrorBaseEdit.Format(L"%08X", pOptionHearder->ImageBase);
	CMirrorSizeEdit.Format(L"%08X", pOptionHearder->SizeOfImage);
	CCodeBaseEdit.Format(L"%08X", pOptionHearder->BaseOfCode);
	CDataBaseEdit.Format(L"%08X", pOptionHearder->BaseOfData);
	CSecAlignmentEdit.Format(L"%08X", pOptionHearder->SectionAlignment);
	CFileAlignmentEdit.Format(L"%08X", pOptionHearder->FileAlignment);
	CMagicEdit.Format(L"%08X", pOptionHearder->Magic);
	CSubSysEdit.Format(L"%08X", pOptionHearder->Subsystem);
	CTimeEdit.Format(L"%d:%d:%d", FileTime->tm_hour+8,FileTime->tm_min,FileTime->tm_sec);
	CHeadEdit.Format(L"%08X", pOptionHearder->SizeOfHeaders);
	CCharacteristicsEdit.Format(L"%08X", pFile->Characteristics);
	CCheckSumEdit.Format(L"%08X", pOptionHearder->CheckSum);
	CSleHeadSizeEdit.Format(L"%08X", pFile->SizeOfOptionalHeader);
	CRVAEdit.Format(L"%08X", pOptionHearder->NumberOfRvaAndSizes);
	UpdateData(FALSE);

}



BOOL DialogPE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void DialogPE::OnBnClickedSection()
{
	CSectionDlg* secDlg = new CSectionDlg(g_lpBase);
	secDlg->Create(IDD_SectionDlg);
	secDlg->ShowWindow(SW_SHOW);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
}




void DialogPE::OnBnClickedDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDirDlg* dirdlg = new CDirDlg(g_lpBase);
	dirdlg->Create(IDD_DirDlg);
	dirdlg->ShowWindow(SW_SHOW);
}
