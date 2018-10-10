// CleanDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "CleanDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <strsafe.h>
//�û���ʱ�ļ���C:\Users\Administrator\AppData\Local\Temp
//ϵͳ��־ C:\Users\Administrator\AppData\Local\Microsoft\Windows\WER\Report
/*
Google�������־
	C:\Users\Administrator\AppData\Local\Google\Chrome\User Data\Default\Session Storage
	C:\Users\Administrator\AppData\Local\Google\Chrome\User Data\Default\Cache
	C:\Users\Administrator\AppData\Local\Google\Chrome\User Data\Default\GPUCache
*/
char* FilePath[] = { "C:\\Users\\Administrator\\AppData\\Local\\Temp","C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows\\WER\\Report" };
char* BroserFilePath[] = { "C:\\Users\\Administrator\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Session Storage","C:\\Users\\Administrator\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Cache","C:\\Users\\Administrator\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\GPUCache" };
int index = 0;
// CCleanDlg �Ի���

IMPLEMENT_DYNAMIC(CCleanDlg, CDialogEx)

CCleanDlg::CCleanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CleanDlg, pParent)
	, CDiskDir(_T(""))
	, CSizeofRubbish(_T(""))
{

}

CCleanDlg::~CCleanDlg()
{
}

void CCleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DiskRubbish, CDiskRubbish);
	DDX_Control(pDX, IDC_BroserRubbish, CBroserRubbish);
	DDX_Text(pDX, IDC_DiskDir, CDiskDir);
	DDX_Text(pDX, IDC_SizeofRubbish, CSizeofRubbish);
}


BEGIN_MESSAGE_MAP(CCleanDlg, CDialogEx)
	ON_BN_CLICKED(IDbtnClean, &CCleanDlg::OnBnClickedbtnclean)
	ON_BN_CLICKED(IDbtnScan, &CCleanDlg::OnBnClickedbtnscan)
END_MESSAGE_MAP()


// CCleanDlg ��Ϣ�������


BOOL CCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDiskRubbish.InsertColumn(0, L"�ļ�·��",0,800);
	CBroserRubbish.InsertColumn(0, L"�ļ�·��",0,800);

	return TRUE;  
}

void CCleanDlg::FindFile(CString Path, int flag)
{
	
	// 1. ���Բ��ҵ�һ���ļ�
	WIN32_FIND_DATA FileInfo = { 0 };
	HANDLE File = FindFirstFile(Path + "\\*", &FileInfo);
	CountSize = { 0 };
	// 2. �����ļ��е������ļ�
	if (File != INVALID_HANDLE_VALUE)
	{
		
		do {
			// 3.1 ������ļ��о͵ݹ�
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 3.1.1 �ų������ļ���
				if (CString(".") != FileInfo.cFileName && CString("..") != FileInfo.cFileName)
					FindFile(Path + CString("\\") + FileInfo.cFileName,flag);
			}
			// 3.2 ������ļ�
			else
			{
				// 3.2.0 �ۼ��ļ���С�͸���
				++FileCount;
			
				DestSize.QuadPart += FileInfo.nFileSizeLow;
				CDiskDir.Format(L"%s", Path + CString("\\") + FileInfo.cFileName);
				CSizeofRubbish.Format(L"%.2lfMB", DestSize.QuadPart / 1024.0 / 1024.0);
				// 3.2.1 �жϺ�׺��
				if (-1 != ExtendString.Find(PathFindExtension(FileInfo.cFileName)))
				{
					
					++DestCount;
					DestSize.QuadPart += FileInfo.nFileSizeLow;
					if (flag == 0) {
						CDiskRubbish.InsertItem(index, Path + CString("\\") + FileInfo.cFileName);
					}
					else if (flag == 1) {
						CBroserRubbish.InsertItem(index, Path + CString("\\") + FileInfo.cFileName);
					}
					DelStrings.push_back(Path + CString("\\") + FileInfo.cFileName);
				}
			}
			index++;
			UpdateData(FALSE);
		} while (FindNextFile(File, &FileInfo));
	}
}



void CCleanDlg::OnBnClickedbtnclean()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DeleteCount = 0;

	// 1. �������е�Ҫɾ�����ļ�·��
	for (auto File : DelStrings)
	{
		// 2. �ж��Ƿ�ɾ���ɹ�
		
		if (DeleteFile(File))
		{
			++DeleteCount;
			
		}
	}
	CDiskRubbish.DeleteAllItems();
	CBroserRubbish.DeleteAllItems();
	// 3. ��ʾɾ����Ϣ
	CString Msg;
	Msg.Format(L"��ɾ���ļ� %d ��\n��ɾ���ļ� %d ��",
		DestCount, DeleteCount);
	MessageBox(Msg);
}


void CCleanDlg::OnBnClickedbtnscan()
{
	DestSize = { 0 };
	DestCount = 0;
	int i = 0;
	while (i < 2)
	{
		FindFile(CString(FilePath[i]), 0);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		FindFile(CString(BroserFilePath[i]), 1);
		i++;
	}
	
}
