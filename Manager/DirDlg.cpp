// DirDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "DirDlg.h"
#include "afxdialogex.h"
#include "ImportDlg.h"
#include "ExportDlg.h"
#include "SourceDlg.h"
#include "RelocDlg.h"
#include "TLSDlg.h"
// CDirDlg 对话框

IMPLEMENT_DYNAMIC(CDirDlg, CDialogEx)

CDirDlg::CDirDlg(char* g_lpbase,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DirDlg, pParent)
{
	this->lpbase = g_lpbase;
}

CDirDlg::~CDirDlg()
{
}

void CDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, CShowImport);
}


BEGIN_MESSAGE_MAP(CDirDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDirDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_ExportBtn, &CDirDlg::OnBnClickedExportbtn)
	ON_BN_CLICKED(IDC_BtnReSource, &CDirDlg::OnBnClickedBtnresource)
	ON_BN_CLICKED(IDC_BUTTON2, &CDirDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BtnTLS, &CDirDlg::OnBnClickedBtntls)
END_MESSAGE_MAP()


// CDirDlg 消息处理程序


BOOL CDirDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//GetDlgItem(EditID[0])->SetWindowText(NULL);
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpbase;
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + lpbase);
	PIMAGE_OPTIONAL_HEADER pOption = &(pNt->OptionalHeader);
	PIMAGE_DATA_DIRECTORY pDataDir = pOption->DataDirectory;
	DWORD i = 0;
	while (i!=0x10)
	{
		TCHAR tmp[20];
		_stprintf_s(tmp, 20, L"%08X", pDataDir[i].VirtualAddress);
		GetDlgItem(EditID[i])->SetWindowText(tmp);
		_stprintf_s(tmp, 20, L"%08X", pDataDir[i].Size);
		GetDlgItem(EditID[i + 16])->SetWindowTextW(tmp);
		i++;
	}


	return TRUE; 
}


void CDirDlg::OnBnClickedButton1()
{
	unsigned char sva[50] = { 0 };
	GetDlgItem(EditID[1])->GetWindowTextW((LPTSTR)sva, 20);
	DWORD iva = wcstol((LPTSTR)sva, NULL, 16);
	//DWORD iva = atoi((char*)sva);
	CImportDlg* importdlg = new CImportDlg(iva,lpbase);
	importdlg->Create(IDD_ImportDlg);
	importdlg->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void CDirDlg::OnBnClickedExportbtn()
{
	unsigned char sva[50] = { 0 };
	GetDlgItem(EditID[0])->GetWindowTextW((LPTSTR)sva, 20);
	DWORD iva = wcstol((LPTSTR)sva, NULL, 16);
	//DWORD iva = atoi((char*)sva);
	CExportDlg* exportdlg = new CExportDlg(iva, lpbase);
	exportdlg->Create(IDD_ExportDlg);
	exportdlg->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void CDirDlg::OnBnClickedBtnresource()
{
	CSourceDlg* resource = new CSourceDlg(lpbase);
	resource->Create(IDD_SourceDlg);
	resource->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void CDirDlg::OnBnClickedButton2()
{
	CRelocDlg* relocDlg = new CRelocDlg(lpbase);
	relocDlg->Create(IDD_Reloc_Dlg);
	relocDlg->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}


void CDirDlg::OnBnClickedBtntls()
{
	CTLSDlg* TLS = new CTLSDlg(lpbase);
	TLS->Create(IDD_TLSDlg);
	TLS->ShowWindow(SW_SHOW);
	// TODO: 在此添加控件通知处理程序代码
}
