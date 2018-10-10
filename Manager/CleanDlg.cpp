// CleanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "CleanDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <strsafe.h>
//用户临时文件夹C:\Users\Administrator\AppData\Local\Temp
//系统日志 C:\Users\Administrator\AppData\Local\Microsoft\Windows\WER\Report
/*
Google浏览器日志
	C:\Users\Administrator\AppData\Local\Google\Chrome\User Data\Default\Session Storage
	C:\Users\Administrator\AppData\Local\Google\Chrome\User Data\Default\Cache
	C:\Users\Administrator\AppData\Local\Google\Chrome\User Data\Default\GPUCache
*/
char* FilePath[] = { "C:\\Users\\Administrator\\AppData\\Local\\Temp","C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows\\WER\\Report" };
char* BroserFilePath[] = { "C:\\Users\\Administrator\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Session Storage","C:\\Users\\Administrator\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Cache","C:\\Users\\Administrator\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\GPUCache" };
int index = 0;
// CCleanDlg 对话框

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


// CCleanDlg 消息处理程序


BOOL CCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDiskRubbish.InsertColumn(0, L"文件路径",0,800);
	CBroserRubbish.InsertColumn(0, L"文件路径",0,800);

	return TRUE;  
}

void CCleanDlg::FindFile(CString Path, int flag)
{
	
	// 1. 尝试查找第一个文件
	WIN32_FIND_DATA FileInfo = { 0 };
	HANDLE File = FindFirstFile(Path + "\\*", &FileInfo);
	CountSize = { 0 };
	// 2. 遍历文件夹的所有文件
	if (File != INVALID_HANDLE_VALUE)
	{
		
		do {
			// 3.1 如果是文件夹就递归
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 3.1.1 排除两个文件夹
				if (CString(".") != FileInfo.cFileName && CString("..") != FileInfo.cFileName)
					FindFile(Path + CString("\\") + FileInfo.cFileName,flag);
			}
			// 3.2 如果是文件
			else
			{
				// 3.2.0 累加文件大小和个数
				++FileCount;
			
				DestSize.QuadPart += FileInfo.nFileSizeLow;
				CDiskDir.Format(L"%s", Path + CString("\\") + FileInfo.cFileName);
				CSizeofRubbish.Format(L"%.2lfMB", DestSize.QuadPart / 1024.0 / 1024.0);
				// 3.2.1 判断后缀名
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
	// TODO: 在此添加控件通知处理程序代码
	DeleteCount = 0;

	// 1. 遍历所有的要删除的文件路径
	for (auto File : DelStrings)
	{
		// 2. 判断是否删除成功
		
		if (DeleteFile(File))
		{
			++DeleteCount;
			
		}
	}
	CDiskRubbish.DeleteAllItems();
	CBroserRubbish.DeleteAllItems();
	// 3. 提示删除信息
	CString Msg;
	Msg.Format(L"需删除文件 %d 个\n已删除文件 %d 个",
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
