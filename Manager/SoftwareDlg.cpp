// SoftwareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "SoftwareDlg.h"
#include "afxdialogex.h"


// CSoftwareDlg 对话框

IMPLEMENT_DYNAMIC(CSoftwareDlg, CDialogEx)

CSoftwareDlg::CSoftwareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SoftwareDlg, pParent)
{

}

CSoftwareDlg::~CSoftwareDlg()
{
}

void CSoftwareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SoftWareList, CSoftWareList);
}


BEGIN_MESSAGE_MAP(CSoftwareDlg, CDialogEx)
	ON_COMMAND(ID_32800, &CSoftwareDlg::OnUninstall)
	ON_COMMAND(ID_32801, &CSoftwareDlg::OnUpdate)
	ON_COMMAND(ID_32802, &CSoftwareDlg::OnOpenDir)
	ON_NOTIFY(NM_RCLICK, IDC_SoftWareList, &CSoftwareDlg::OnRclickSoftwarelist)
END_MESSAGE_MAP()


// CSoftwareDlg 消息处理程序


BOOL CSoftwareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CSoftWareList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CSoftWareList.InsertColumn(0, L"名称", 0, 200);
	CSoftWareList.InsertColumn(1, L"发布者", 0, 200);
	CSoftWareList.InsertColumn(2, L"安装时间", 0, 200);
	CSoftWareList.InsertColumn(3, L"版本", 0, 200);
	GetSoftwareInfo();
	return TRUE;  
}

void CSoftwareDlg::GetSoftwareInfo()
{
	CSoftWareList.DeleteAllItems();
	m_vecSoftInfo.clear();
	SOFTINFO stcSoftInfo = { 0 };
	HKEY RootKey;            // 主键
	LPCTSTR lpSubKey;        // 子键名称
	HKEY hkResult;           // 将要打开键的句柄 
	HKEY hkRKey;
	LONG lReturn;            // 记录读取注册表是否成功
	CString strBuffer;
	CString strMidReg;

	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };        // 注册表项名称
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;

	RootKey = HKEY_LOCAL_MACHINE;
	lpSubKey = Is64() ? Win64PATH : Win32PATH;

	lReturn = RegOpenKeyEx(RootKey, lpSubKey, 0,
		KEY_ALL_ACCESS, &hkResult);
	if (lReturn == ERROR_SUCCESS)
	{
		DWORD index = 0;
		DWORD ListIndex = 0;
		while (ERROR_NO_MORE_ITEMS !=
			RegEnumKeyEx(hkResult, index, szKeyName, &dwKeyLen,
				0, NULL, NULL, NULL))
		{
			strBuffer.Format(_T("%s"), szKeyName);
			if (!strBuffer.IsEmpty())
			{
				strMidReg = (CString)lpSubKey + _T("\\") + strBuffer;

				if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, strMidReg,
					0, KEY_ALL_ACCESS, &hkRKey) == ERROR_SUCCESS)
				{
					RegQueryValueEx(hkRKey, _T("DisplayName"),
						0, &dwType, (LPBYTE)stcSoftInfo.szSoftName, &dwNameLen);
					dwNameLen = 255;

					RegQueryValueEx(hkRKey, _T("DisplayVersion"),
						0, &dwType, (LPBYTE)stcSoftInfo.szSoftVer, &dwNameLen);
					dwNameLen = 255;

					RegQueryValueEx(hkRKey, _T("InstallLocation"),
						0, &dwType, (LPBYTE)stcSoftInfo.strSoftInsPath, &dwNameLen);
					dwNameLen = 255;

					RegQueryValueEx(hkRKey, _T("Publisher"),
						0, &dwType, (LPBYTE)stcSoftInfo.strSoftVenRel, &dwNameLen);
					dwNameLen = 255;

					RegQueryValueEx(hkRKey, _T("UninstallString"),
						0, &dwType, (LPBYTE)stcSoftInfo.strSoftUniPath, &dwNameLen);
					dwNameLen = 255;

					RegQueryValueEx(hkRKey, _T("InstallDate"),
						0, &dwType, (LPBYTE)stcSoftInfo.szSoftData, &dwNameLen);
					if (stcSoftInfo.szSoftData[0])
					{
						stcSoftInfo.szSoftData[9] = stcSoftInfo.szSoftData[7];
						stcSoftInfo.szSoftData[8] = stcSoftInfo.szSoftData[6];
						stcSoftInfo.szSoftData[7] = '-';
						stcSoftInfo.szSoftData[6] = stcSoftInfo.szSoftData[5];
						stcSoftInfo.szSoftData[5] = stcSoftInfo.szSoftData[4];
						stcSoftInfo.szSoftData[4] = '-';
					}
					dwNameLen = 255;

					//保存数据
					if (stcSoftInfo.szSoftName[0] != '\0')
					{
						m_vecSoftInfo.push_back(stcSoftInfo);
						CSoftWareList.InsertItem(ListIndex, CString(stcSoftInfo.szSoftName));
						CSoftWareList.SetItemText(ListIndex, 1, stcSoftInfo.strSoftVenRel);
						CSoftWareList.SetItemText(ListIndex, 2, stcSoftInfo.szSoftData);
						CSoftWareList.SetItemText(ListIndex, 3, stcSoftInfo.szSoftVer);
						ListIndex++;
					}

					index++;
				}
				dwKeyLen = 255;
				memset(szKeyName, 0, 255);
				memset(&stcSoftInfo, 0, sizeof(SOFTINFO));
			}
		}
		RegCloseKey(hkResult);
	}
}

BOOL CSoftwareDlg::Is64()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_IA64)
	{
		//64 位操作系统
		return TRUE;
	}
	else
	{
		// 32 位操作系统
		return FALSE;
	}
	return FALSE;
}

//软件卸载
void CSoftwareDlg::OnUninstall()
{
	if (m_Select == -1)
	{
		return;
	}
	ShellExecute(NULL, _T("open"),
		m_vecSoftInfo[m_Select].strSoftUniPath, NULL, NULL, SW_SHOWNORMAL);
	// TODO: 在此添加命令处理程序代码
}

//刷新目录
void CSoftwareDlg::OnUpdate()
{

	GetSoftwareInfo();
}


void CSoftwareDlg::OnOpenDir()
{
	if (!m_vecSoftInfo[m_Select].strSoftInsPath[0])
	{
		return;
	}
	LPSHELLFOLDER pDesktopFolder;
	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		LPITEMIDLIST     pidl;
		ULONG             chEaten;
		ULONG             dwAttributes;
		HRESULT             hr;

		hr = pDesktopFolder->ParseDisplayName(
			NULL, NULL, m_vecSoftInfo[m_Select].strSoftInsPath, &chEaten, &pidl, &dwAttributes);
		if (FAILED(hr))
		{
			pDesktopFolder->Release();
			return ;
		}
		LPCITEMIDLIST pidlFolder = pidl;

		CoInitialize(NULL);
		hr = SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);

		pDesktopFolder->Release();

		if (hr == S_OK)
		{
			return;
		}
	}
}


void CSoftwareDlg::OnRclickSoftwarelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//防止在空白区点击弹出菜单  
	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选软件的卸载命令
		strTemp = CSoftWareList.GetItemText(n, 0);
		for (DWORD i = 0; i < m_vecSoftInfo.size(); i++)
		{
			if (strTemp == m_vecSoftInfo[i].szSoftName)
			{
				m_Select = i;
				break;
			}
		}
		strTemp.Empty();
		//下面的这段代码, 不单单适应于ListCtrl  
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU5);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}
