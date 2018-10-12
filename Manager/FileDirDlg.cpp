// FileDirDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "FileDirDlg.h"
#include "afxdialogex.h"
#include "FileAttr.h"

// CFileDirDlg �Ի���

IMPLEMENT_DYNAMIC(CFileDirDlg, CDialogEx)

CFileDirDlg::CFileDirDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FileDirDlg, pParent)
{

}

CFileDirDlg::~CFileDirDlg()
{
}

void CFileDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DirCombo, m_DiskCom);
	DDX_Control(pDX, IDC_FileList, m_FileList);
}


BEGIN_MESSAGE_MAP(CFileDirDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_DirCombo, &CFileDirDlg::OnCbnSelchangeDircombo)
	ON_NOTIFY(NM_DBLCLK, IDC_FileList, &CFileDirDlg::OnDblclkFilelist)
	ON_BN_CLICKED(IDC_FileCheck, &CFileDirDlg::OnBnClickedFilecheck)
	ON_NOTIFY(NM_RCLICK, IDC_FileList, &CFileDirDlg::OnRclickFilelist)
	ON_COMMAND(ID_FileAttr, &CFileDirDlg::OnFileattr)
END_MESSAGE_MAP()


// CFileDirDlg ��Ϣ�������


BOOL CFileDirDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	TCHAR temp[MAX_PATH];
	GetLogicalDriveStrings(MAX_PATH, temp);
	for (int i = 0; temp[i]; ++i)
	{
		m_DiskCom.AddString(&temp[i]);
		i += _tcslen(&temp[i]);
	}
	m_DiskCom.SetCurSel(0);
	m_FileList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_FileList.InsertColumn(0, L"����", 0, 100);
	m_FileList.InsertColumn(1, L"����ʱ��", 0, 200);
	m_FileList.InsertColumn(2, L"����", 0, 100);
	m_FileList.InsertColumn(3, L"��С", 0, 100);
	ShowFileList(L"C:\\");
	return TRUE;  
}



void CFileDirDlg::ShowFileList(LPCTSTR FullPath)
{
	_tcscpy_s(CurrentDir, MAX_PATH, FullPath);
	m_FileList.DeleteAllItems();
	TCHAR FindPath[MAX_PATH] = { 0 };
	_tcscat_s(FindPath, MAX_PATH, FullPath);
	_tcscat_s(FindPath, MAX_PATH, _T("\\*"));
	WIN32_FIND_DATA FindData = { 0 };
	HANDLE Handle = FindFirstFile(FindPath, &FindData);
	if (Handle != INVALID_HANDLE_VALUE)
	{
		int index = 0;
		LVITEM LvItem = { 0 };
		do {
			if (_tcscmp(FindData.cFileName, TEXT(".")) != 0)
			{
				LvItem.iItem = index;
				m_FileList.InsertItem(&LvItem);
				m_FileList.SetItemText(index, 0, FindData.cFileName);
				SYSTEMTIME SystemTime = { 0 };
				FileTimeToSystemTime(&FindData.ftCreationTime, &SystemTime);
				TCHAR CreateTime[MAX_PATH] = { 0 };
				_stprintf_s(CreateTime, MAX_PATH, TEXT("%d/%d/%d %d:%d:%d"),
					SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
					SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
				m_FileList.SetItemText(index, 1, CreateTime);

				CreateTime[0] = 0;
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("Ŀ¼ "));
				else
					_tcscat_s(CreateTime, MAX_PATH, TEXT("�ļ� "));
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("���� "));
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("ֻ��"));
				m_FileList.SetItemText(index, 2, CreateTime);

				CreateTime[0] = 0;
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					_tcscat_s(CreateTime, MAX_PATH, TEXT("-"));
				else
				{
					if (FindData.nFileSizeLow > 1073741824)
						_stprintf_s(CreateTime, MAX_PATH, TEXT("%.2lfGB"), FindData.nFileSizeLow / 1024.0 / 1024.0 / 1024.0);
					else if (FindData.nFileSizeLow > 1048576)
						_stprintf_s(CreateTime, MAX_PATH, TEXT("%.2lfMB"), FindData.nFileSizeLow / 1024.0 / 1024.0);
					else
						_stprintf_s(CreateTime, MAX_PATH, TEXT("%.2lfKB"), FindData.nFileSizeLow / 1024.0 / 1024.0);

				}
				m_FileList.SetItemText(index, 3, CreateTime);

				++index;
			}

		} while (FindNextFile(Handle, &FindData));
	}
}

void CFileDirDlg::UpdateDir(LPCTSTR FullPath)
{
	TCHAR Path[MAX_PATH] = { 0 };
	_stprintf_s(Path, MAX_PATH, TEXT("%s\\%s"), CurrentDir, FullPath);

	if (GetFileAttributes(Path) & FILE_ATTRIBUTE_DIRECTORY)
	{
		ShowFileList(Path);
	}
}

void CFileDirDlg::OnCbnSelchangeDircombo()
{
	TCHAR temp[MAX_PATH] = { 0 };
	m_DiskCom.GetWindowText(temp, MAX_PATH);
	ShowFileList(temp);
}


void CFileDirDlg::OnDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i = m_FileList.GetSelectionMark();
	CString FileName = m_FileList.GetItemText(i, 0);
	UpdateDir(FileName);
	*pResult = 0;
}


void CFileDirDlg::OnBnClickedFilecheck()
{
	// TODO: �ڴ���������������
	
}


void CFileDirDlg::OnRclickFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu, *pmenu;
	menu.LoadMenu(IDR_MENU7);
	pmenu = menu.GetSubMenu(0);
	//����һ������ȷ�����λ�õ�λ�� 
	CPoint point1;
	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����
	GetCursorPos(&point1);
	//��ָ��λ����ʾ�����˵�
	pmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);
	*pResult = 0;
}


void CFileDirDlg::OnFileattr()
{
	int i = m_FileList.GetSelectionMark();
	CString FileName = m_FileList.GetItemText(i, 0);
	_stprintf_s(CurrentFile, MAX_PATH, L"%s\\%s", CurrentDir, FileName);

	CFileAttr *pFaD = new CFileAttr(CurrentFile);
	pFaD->DoModal();
	delete pFaD;
}
