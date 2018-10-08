// DialogC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "DialogC.h"
#include "afxdialogex.h"

CListCtrl  CDialogC::WIndowsList = {};
static int index = 0;
// DialogC �Ի���

IMPLEMENT_DYNAMIC(CDialogC, CDialogEx)

CDialogC::CDialogC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINDOWSDIALOG, pParent)
{

}

CDialogC::~CDialogC()
{
}

void CDialogC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WindowsLIST, WIndowsList);
}



BEGIN_MESSAGE_MAP(CDialogC, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CDialogC::OnBnClickedCancel)
	ON_BN_CLICKED(IDFlushWnd, &CDialogC::OnBnClickedFlushwnd)
END_MESSAGE_MAP()


// DialogC ��Ϣ�������

BOOL CDialogC::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buff[200];
	::GetWindowText(hwnd, buff, 200);

	if (::IsWindowVisible(hwnd) == TRUE && wcslen(buff) != 0) {
		WIndowsList.InsertItem(index, buff);
		index++;
	}
	return 1;
}


BOOL CDialogC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	WIndowsList.InsertColumn(0, L"����", 0, 600);
	
	WIndowsList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	EnumWindows(&EnumWindowsProc, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDialogC::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}



void CDialogC::OnBnClickedFlushwnd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WIndowsList.DeleteAllItems();
	EnumWindows(&EnumWindowsProc, NULL);
}
