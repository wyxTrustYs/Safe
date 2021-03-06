// ClearVir.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "ClearVir.h"
#include "afxdialogex.h"
#include "Md5.h"
#include <TlHelp32.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")
// CClearVir 对话框

IMPLEMENT_DYNAMIC(CClearVir, CDialogEx)

CClearVir::CClearVir(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ClearVirDlg, pParent)
	, Cpath(_T("文件路径："))
	, strSelModel(_T("黑名单"))
	, strSelVirLib(_T("本地"))
{

}

CClearVir::~CClearVir()
{
}

void CClearVir::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VirList, CVirList);
	DDX_Control(pDX, IDC_SelPath, CSelPath);
	DDX_Text(pDX, IDC_PATH, Cpath);

	DDX_CBString(pDX, IDC_SelModel, strSelModel);
	DDX_CBString(pDX, IDC_SelVirLib, strSelVirLib);
}


BEGIN_MESSAGE_MAP(CClearVir, CDialogEx)
	ON_CBN_SELCHANGE(IDC_SelPath, &CClearVir::OnCbnSelchangeSelpath)
	ON_BN_CLICKED(ID_btnVirScan, &CClearVir::OnBnClickedbtnvirscan)
	ON_BN_CLICKED(ID_btnClearVir, &CClearVir::OnBnClickedbtnclearvir)
	ON_MESSAGE(WM_MYUPDATEDATA, &CClearVir::OnUpDateData)
END_MESSAGE_MAP()


BOOL CClearVir::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CVirList.InsertColumn(0, L"文件名称", 0, 100);
	CVirList.InsertColumn(1, L"病毒位置", 0, 300);
	CVirList.InsertColumn(2, L"病毒大小", 0, 50);
	CSelPath.SetCurSel(0);
	
	WSAData WsaData = { 0 };

	// 初始化模块并进行判断
	if (WSAStartup(MAKEWORD(2, 2), &WsaData))
	{
		MessageBox(L"初始化模块失败", L"错误信息", MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}

	// 判断版本号是否正确

	// 使用 Create 创建一个套接字
	if (!MySocket.Create())
	{
		int index = GetLastError();
		MessageBox( L"创建套接字失败", L"错误信息", MB_OK | MB_ICONERROR);
		ExitProcess(-1);
	}
	MySocket.Connect(L"127.0.0.1", 8000);
	
	return TRUE;
}

DWORD WINAPI ScanThread(LPVOID lpVoid)
{
	CClearVir* pScanCtrl = (CClearVir*)lpVoid;
	//查杀类型判断
	if (pScanCtrl->IsMD5)
	{
		//查杀路径判断
		if (pScanCtrl->IsSingleScan)
		{
			//单文件查杀
			pScanCtrl->Scan_Single(pScanCtrl->m_SinglePath);
		}
		else
		{
			//全路径查杀
			//调用路径扫描，传入路径参数
			pScanCtrl->Scan_All(pScanCtrl->m_AllPath);
		}
	}
	else
	{
		//黑名单扫描进程
		pScanCtrl->Scan_Process();
	}



	pScanCtrl->Cpath.Format(
		_T("扫 描 完 毕 ！ 本 次 查 杀 发 现 病 毒 数 ：%d"),
		pScanCtrl->m_vecVirInfo.size());
	pScanCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);

	return 0;
}

void CClearVir::StartScan()
{
	//0.判断是否为黑名单查杀
	if (!IsMD5) {
		//判断是否为本地黑名单查杀
		if (IsLocal) {
			//本地黑名单	加载本地黑名单病毒库
			//先获取病毒库中黑名单的个数
			CString Temp;
			GetPrivateProfileString(_T("Num"),
				_T("Black"), _T("DefaultName"),
				Temp.GetBuffer(MAX_PATH), MAX_PATH,
				_T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//在根据个数遍历信息，加载到Vector
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(_T("%d"), i);
				GetPrivateProfileString(_T("Black"),
					tempCount, _T("DefaultName"),
					Temp.GetBuffer(MAX_PATH), MAX_PATH,
					_T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalBlack.push_back(Temp);
			}
			if (m_LocalBlack.size() == 0)
			{
				Cpath = _T("读 取 白 名 单 错误 ！");
				UpdateData(FALSE);
				return;
			}
		}
	}
	//1.再判断扫描路径
	else if (IsSingleScan)
	{
		m_SinglePath.Empty();
		//如果为单个文件查杀，弹出文件选择框，路径保存在m_szSingleFilePath
		CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
			NULL,
			NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
			NULL);
		if (dlg.DoModal() == IDOK)
		{
			//打开成功，保存文件路径
			m_SinglePath = dlg.GetPathName();
		}
		else
		{
			Cpath = _T("打 开 文 件 失 败 ！");
			UpdateData(FALSE);
			return;
		}
		if (m_SinglePath.IsEmpty())
		{
			return;
		}
	}
	else
	{
		//如果为全路径查杀，弹出文件夹选择框，路径保存在m_AllPath
		m_AllPath.Empty();
		m_AllPath = BrowseFolder(this->m_hWnd, _T("请选择查杀路径"));
		if (m_AllPath.IsEmpty())
		{
			return;
		}
	}

	CVirList.DeleteAllItems();
	//2.判断是云端还是本地
	//如果是本地，再判断是MD5还是黑名单，加载本地病毒库信息
	if (IsLocal)
	{
		if (IsMD5)
		{
			//本地MD5	加载本地MD5病毒库
			//先获取病毒库中MD5的个数
			CString Temp;
			GetPrivateProfileString(_T("Num"),
				_T("MD5"), _T("DefaultName"),
				Temp.GetBuffer(MAX_PATH), MAX_PATH,
				_T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//再根据个数遍历MD5信息，加载到Vector
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(_T("%d"), i);
				GetPrivateProfileString(_T("MD5"),
					tempCount, _T("DefaultName"),
					Temp.GetBuffer(MAX_PATH), MAX_PATH,
					_T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalMD5.push_back(Temp);
			}
			if (m_LocalMD5.size() == 0)
			{
				Cpath = _T("读 取 病 毒 库 错误 ！");
				UpdateData(FALSE);
				return;
			}
		}
	}
	//如果是云端,下载云端病毒库信息
	else
	{
		char flag[] = "client";
		char strFinish[] = "finish";
		Msg s = { 0 };
		MySocket.Send(flag, sizeof(flag));
		MySocket.Receive(&s, sizeof(s));
		int num = s.flat;
		for (int i = 0; i < num; i++) {
			MySocket.Receive(&s, sizeof(s));
			if (s.flat == 0) {
				m_ServerMD5.push_back(CString(s.str.strMD5));
			}
			else
			{
				m_ServerBlack.push_back(CString(s.str.strProcName));
			}
		}
	}
	//3.创建启动扫描线程
	//根据以上四项，调用主线程中的函数进行查杀，将结果保存在主线程的Vector中
	CloseHandle(CreateThread(NULL, 0, ScanThread, this, 0, NULL));
}

BOOL CClearVir::MD5Scan(LPCTSTR szPath)
{
	//LPCTSTR 转CHAR*
	int num = WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, NULL, 0, NULL, FALSE);
	char *pchar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, pchar, num, NULL, FALSE);
	//获取MD5值
	char* md5 = md5FileValue(pchar);
	//CHAR* 转LPCTSTR
	num = MultiByteToWideChar(0, 0, md5, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, md5, -1, wide, num);
	m_szMD5 = wide;
	delete[]pchar;
	delete[]wide;

	//获取文件MD5信息完毕
	//判断是本地查杀还是云端查杀
	if (IsLocal)
	{
		//本地MD5查杀		与m_LocalMD5作对比
		for (DWORD i = 0; i < m_LocalMD5.size(); i++)
		{
			if (m_LocalMD5[i] == m_szMD5)
			{
				//是病毒 返回真
				return TRUE;
			}
		}
	}
	else
	{
		//云端MD5查杀与m_ServerMD5作对比
		for (DWORD i = 0; i < m_ServerMD5.size(); i++)
		{
			if (m_ServerMD5[i] == m_szMD5)
			{
				//是病毒 返回真
				return TRUE;
			}
		}
	}
	return FALSE;
}

CString CClearVir::GetShowSize(DOUBLE dwSize)
{
	CString szSize;
	if (dwSize < 1024)
	{
		//B
		szSize.Format(_T("%d B"), (DWORD)dwSize);
		return szSize;
	}
	else if (dwSize < 1024 * 1024)
	{
		//KB
		szSize.Format(_T("%.2f KB"), ((DOUBLE)dwSize) / 1024);
		return szSize;
	}
	else if (dwSize < 1024 * 1024 * 1024)
	{
		//MB
		szSize.Format(_T("%.2f MB"), (DOUBLE)dwSize / 1024 / 1024);
		return szSize;
	}if (dwSize > 1024 * 1024 * 1024)
	{
		//GB
		szSize.Format(_T("%.2f GB"), (DOUBLE)dwSize / 1024 / 1024 / 1024);
		return szSize;
	}
	return _T("");

}

CString CClearVir::BrowseFolder(HWND hWnd, LPCTSTR lpTitle)
{
	TCHAR szPath[MAX_PATH] = { 0 };
	BROWSEINFO m_bi;

	m_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	m_bi.hwndOwner = hWnd;
	m_bi.pidlRoot = NULL;
	m_bi.lpszTitle = lpTitle;
	m_bi.lpfn = NULL;
	m_bi.lParam = NULL;
	m_bi.pszDisplayName = szPath;

	LPITEMIDLIST pidl = ::SHBrowseForFolder(&m_bi);
	if (pidl)
	{
		if (!::SHGetPathFromIDList(pidl, szPath)) szPath[0] = 0;

		IMalloc * pMalloc = NULL;
		if (SUCCEEDED(::SHGetMalloc(&pMalloc))) // 取得IMalloc分配器接口
		{
			pMalloc->Free(pidl); // 释放内存
			pMalloc->Release(); // 释放接口
		}
	}
	return szPath;
}

BOOL CClearVir::Scan_Process()
{
	//黑名单查杀
	//如果是病毒 将文件信息保存到病毒Vector
	CVirList.DeleteColumn(1);
	CVirList.InsertColumn(1, L"PID", 0, 300);
	TCHAR temp[20] = { 0 };
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	//1 创建一个快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	int PID = 0;
	if (Process32First(hSnap, &pe) == TRUE) {
		do
		{
			if (BlackScan(pe.szExeFile))
			{
				//插入List中显示
				PID = pe.th32ProcessID;
				_stprintf_s(temp, 20, L"%d", pe.th32ProcessID);
				CVirList.InsertItem(0, pe.szExeFile);	//进程名
				CVirList.SetItemText(0, 1, temp);	//PID
				CVirList.SetItemText(0, 2,				//文件大小
					GetShowSize(pe.dwSize));
				//保存病毒信息到Vector容器
				VIRINFO VirInfo;
				_tcscpy_s(VirInfo.szVirName, pe.szExeFile);
				_tcscpy_s(VirInfo.szVirPath, temp);
				_tcscpy_s(VirInfo.szVirSize, GetShowSize(pe.dwSize));
			
				HANDLE hProcess = OpenProcess(
					PROCESS_ALL_ACCESS,  //要申请的一个权限
					FALSE,               //句柄是否可继承
					PID                 //进程ID
				);
				VirInfo.hProcess = hProcess;
				m_vecVirInfo.push_back(VirInfo);
			}
		} while (Process32Next(hSnap, &pe));
	}
	return 0;
}

//单个查杀
void CClearVir::Scan_Single(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	CString sFindFilter;
	DWORD dwAttributes = 0;
	sFindFilter = szPath;
	FindFirstFile(sFindFilter, &wfd);
	//判断是否是病毒
	if (IsMD5)
	{
		//MD5查杀
		if (MD5Scan(szPath)) {
			//插入List
			CVirList.InsertItem(0, wfd.cFileName);
			//文件名
			CVirList.SetItemText(0, 1, szPath);
			//文件路径
			CVirList.SetItemText(0, 2, GetShowSize(wfd.nFileSizeLow));
			//保存到Vector
			VIRINFO VirInfo;
			_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
			_tcscpy_s(VirInfo.szVirPath, szPath);
			_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
			m_vecVirInfo.push_back(VirInfo);

		}
	}

}

//全路径查杀
void CClearVir::Scan_All(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += TEXT("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if (_tcscmp(wfd.cFileName, TEXT(".")) == 0 ||
			_tcscmp(wfd.cFileName, TEXT("..")) == 0)
		{
			continue;
		}

		//获取完整路径名
		sFullPath = szPath;
		sFullPath += TEXT("\\");
		sFullPath += wfd.cFileName;

		//如果当前路径是文件夹，则需要递归文件夹中的文件
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			Scan_All(sFullPath);
		}
		//否则，这是一个文件
		else
		{
			//输出正在搜索的文件路径界面，让用户直观的看到正在扫描的文件路径
			Cpath = _T("正在扫描 : ") + sFullPath;
			SendMessage(WM_MYUPDATEDATA, FALSE);
			//UpdateData(FALSE);
			//判断是否是病毒
			if (IsMD5)
			{
				//MD5查杀
				//如果是病毒 将文件信息保存到病毒Vector
				if (MD5Scan(sFullPath))
				{
					//插入List中显示
					CVirList.InsertItem(0, wfd.cFileName);	//文件名
					CVirList.SetItemText(0, 1, sFullPath);	//文件路径
					CVirList.SetItemText(0, 2,				//文件大小
						GetShowSize(wfd.nFileSizeLow));
					//保存病毒信息到Vector容器
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);
				}
			}

		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
}



BOOL CClearVir::BlackScan(LPCTSTR szPath)
{

	//判断是本地查杀还是云端查杀
	if (IsLocal)
	{
		//本地黑名单查杀		与m_LocalBlack作对比
		for (DWORD i = 0; i < m_LocalBlack.size(); i++)
		{
			if (m_LocalBlack[i] == szPath)
			{
				//是病毒 返回真
				return TRUE;
			}
		}

	}
	else
	{
		//云端黑名单查杀与m_ServerBlack作对比
		for (DWORD i = 0; i < m_ServerBlack.size(); i++)
		{
			if (m_ServerBlack[i] == szPath)
			{
				//是病毒 返回真
				return TRUE;
			}
		}
	}
	return FALSE;
}



void CClearVir::OnCbnSelchangeSelpath()
{
	int nIndex = CSelPath.GetCurSel();
	CString strText;

	CSelPath.GetLBText(nIndex, strText);
	if (!strText.Compare(L"单个文件扫描")) {
		//单文件查杀
		IsSingleScan = TRUE;
	}
	else
	{
		//单文件查杀
		IsSingleScan = FALSE;
	}
}


void CClearVir::OnBnClickedbtnvirscan()
{
	UpdateData(TRUE);
	
	if (!strSelModel.Compare(L"MD5")) {

		//MD5
		IsMD5 = TRUE;
	}
	else
	{
		//黑名单
		IsMD5 = FALSE;
	}
	if (!strSelVirLib.Compare(L"本地"))
	{
		//本地
		IsLocal = TRUE;
	}
	else
	{
		//云端
		IsLocal = FALSE;
	}
	m_vecVirInfo.clear();
	StartScan();
}


void CClearVir::OnBnClickedbtnclearvir()
{
	//先判断病毒Vector是否为空，若为空，则提示没有可以清除的文件

	if (m_vecVirInfo.size() == 0)
	{
		return;
	}
	if (IsMD5) {
		//删除病毒
		for (DWORD i = 0; i < m_vecVirInfo.size(); i++)
		{
			DeleteFile(m_vecVirInfo[i].szVirPath);
		}
		m_vecVirInfo.clear();
		CVirList.DeleteAllItems();
		Cpath = _T("清 除 病 毒 完 毕 !");
	}
	else
	{
		for (DWORD i = 0; i < m_vecVirInfo.size(); i++)
		{

			TerminateProcess(m_vecVirInfo[i].hProcess, 0);
		}
		m_vecVirInfo.clear();
		CVirList.DeleteAllItems();
		Cpath = _T("清 除 病 毒 完 毕 !");
	}
	UpdateData(FALSE);
}


afx_msg LRESULT CClearVir::OnUpDateData(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(wParam);
}
