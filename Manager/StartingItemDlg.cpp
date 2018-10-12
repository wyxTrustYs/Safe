// StartingItemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Manager.h"
#include "StartingItemDlg.h"
#include "afxdialogex.h"


// CStartingItemDlg 对话框

IMPLEMENT_DYNAMIC(CStartingItemDlg, CDialogEx)

CStartingItemDlg::CStartingItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_StartingItemDlg, pParent)
{

}

CStartingItemDlg::~CStartingItemDlg()
{
}

void CStartingItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_StartingItemList, StartingItemList);
}


BEGIN_MESSAGE_MAP(CStartingItemDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_StartingItemList, &CStartingItemDlg::OnRclickStartingitemlist)
	ON_COMMAND(ID_32804, &CStartingItemDlg::OnUpdate)
	ON_COMMAND(ID_addmenu, &CStartingItemDlg::Onaddmenu)
	ON_COMMAND(ID_Delemenu, &CStartingItemDlg::OnDeleItemmenu)
END_MESSAGE_MAP()


// CStartingItemDlg 消息处理程序


BOOL CStartingItemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	StartingItemList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// TODO:  在此添加额外的初始化
	StartingItemList.InsertColumn(0, L"启动项目",0,200);
	StartingItemList.InsertColumn(1, L"命令", 0, 200);
	StartingItemList.InsertColumn(2, L"位置", 0, 200);
	GetBootInfo();
	UpdateData(FALSE);
	return TRUE;  
}

void CStartingItemDlg::GetBootInfo()
{
	StartingItemList.DeleteAllItems();
	m_vecBootInfo.clear();

	BOOTINFO stcBootInfo = { 0 };

	HKEY RootKey = NULL;						// 主键
	HKEY hkResult = NULL;						// 将要打开键的句柄 
	LONG lReturn;						// 记录读取注册表是否成功
	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };       // 注册表项名称
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;
	CString buffer;
	LPCTSTR lpSubKey[2];				// 子键名称
	DWORD ListIndex = 0;

	lpSubKey[0] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	lpSubKey[1] = _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");

	for (DWORD i = 0; i < 2; i++)
	{
		if (i == 0)
			RootKey = HKEY_CURRENT_USER;
		if (i == 1)
			RootKey = HKEY_LOCAL_MACHINE;

		//打开lpSubkey
		lReturn = RegOpenKeyEx(RootKey, lpSubKey[i], 0,
			KEY_ALL_ACCESS, &hkResult);
		if (lReturn == ERROR_SUCCESS)
		{
			//如果打开成功，开始枚举其值
			while (ERROR_NO_MORE_ITEMS !=
				RegEnumValue(hkResult, index, stcBootInfo.szBootName, &dwKeyLen,
					0, NULL, NULL, NULL))
			{
				//获取每个值得数据 szKeyName为值得名称 buffer为值的数据
				RegQueryValueEx(hkResult, stcBootInfo.szBootName,
					0, &dwType, (LPBYTE)stcBootInfo.szBootOrder, &dwNameLen);
				//保存信息
				//启动项	stcBootInfo.szBootName
				//命令	stcBootInfo.szBootOrder

				//保存位置信息
				//主键信息
				stcBootInfo.szHKEY;
				if (i == 0)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_CURRENT_USER"));
				if (i == 1)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_LOCAL_MACHINE"));
				//删除时要用到的
				_tcscpy_s(stcBootInfo.szBootPos, lpSubKey[i]);
				//List中要显示的
				buffer.Empty();
				buffer = buffer + stcBootInfo.szHKEY + _T("\\") + stcBootInfo.szBootPos;
				_tcscpy_s(stcBootInfo.szBootPos_All, buffer);


				//添加到vector
				m_vecBootInfo.push_back(stcBootInfo);
				//插入List
				StartingItemList.InsertItem(ListIndex, stcBootInfo.szBootName);
				StartingItemList.SetItemText(ListIndex, 1, stcBootInfo.szBootOrder);
				StartingItemList.SetItemText(ListIndex, 2, stcBootInfo.szBootPos_All);
				//
				ListIndex++;
				index++;
				dwNameLen = 255;
				dwKeyLen = 255;
				memset(&stcBootInfo, 0, sizeof(BOOTINFO));
			}
			index = 0;
			RegCloseKey(hkResult);
		}
	}
}


void CStartingItemDlg::OnRclickStartingitemlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//防止在空白区点击弹出菜单  
	int n = pNMItemActivate->iItem;//选择了第几行
	if (n != -1)
	{
		//保存所选软件的启动项

		m_Select = n;

		//下面的这段代码, 不单单适应于ListCtrl  
		CMenu menu, *pPopup;
		menu.LoadMenu(IDR_MENU6);
		pPopup = menu.GetSubMenu(0);
		CPoint myPoint;
		ClientToScreen(&myPoint);
		GetCursorPos(&myPoint);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, myPoint.x, myPoint.y, this);
	}
	*pResult = 0;
}




void CStartingItemDlg::OnUpdate()
{
	GetBootInfo();
}

//添加启动项
void CStartingItemDlg::Onaddmenu()
{
	//打开文件代码
	TCHAR FilePathName[MAX_PATH] = { 0 };
	CString FileName;
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		//文件名保存在了FilePathName里
		_tcscpy_s(FilePathName, dlg.GetPathName());
	}
	else
	{
		return;
	}

	//打开启动项Key
	HKEY hKey;
	LPCTSTR lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//获取路径长度
		DWORD dwRet = _tcslen(FilePathName);

		//添加启动项信息
		FileName = dlg.GetFileTitle();
		int n = sizeof(TCHAR);
		lRet = RegSetValueEx(hKey, FileName, 0, REG_SZ,
			(BYTE *)FilePathName, dwRet * sizeof(TCHAR));

		//关闭注册表   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("添加启动项失败!"));
		}
		else
		{
			AfxMessageBox(_T("添加启动项成功!"));
			GetBootInfo();
		}
	}
}
//删除启动项
void CStartingItemDlg::OnDeleItemmenu()
{
	if (m_Select == -1)
	{
		return;
	}
	HKEY RootKey = NULL;		//根键
	HKEY hKey;
	//确定要删除的根键
	strTemp = m_vecBootInfo[m_Select].szHKEY;
	if (strTemp == _T("HKEY_CURRENT_USER"))
	{
		RootKey = HKEY_CURRENT_USER;
	}
	if (strTemp == _T("HKEY_LOCAL_MACHINE"))
	{
		RootKey = HKEY_LOCAL_MACHINE;
	}
	if (RootKey == NULL)
	{
		return;
	}
	//确定要删除的Run子键路径
	LPCTSTR lpRun = m_vecBootInfo[m_Select].szBootPos;
	//打开启动项Key   
	long lRet = RegOpenKeyEx(RootKey, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//获取要删除启动项的名字
		strTemp = m_vecBootInfo[m_Select].szBootName;
		//删除此启动项 
		lRet = RegDeleteValue(hKey, strTemp);
		//关闭注册表   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("删除启动项成功！"));
		}
		else {
			AfxMessageBox(_T("删除启动项成功！"));
			GetBootInfo();
		}
	}
}
