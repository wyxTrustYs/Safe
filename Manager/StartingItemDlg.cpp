// StartingItemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "StartingItemDlg.h"
#include "afxdialogex.h"


// CStartingItemDlg �Ի���

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


// CStartingItemDlg ��Ϣ�������


BOOL CStartingItemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	StartingItemList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	StartingItemList.InsertColumn(0, L"������Ŀ",0,200);
	StartingItemList.InsertColumn(1, L"����", 0, 200);
	StartingItemList.InsertColumn(2, L"λ��", 0, 200);
	GetBootInfo();
	UpdateData(FALSE);
	return TRUE;  
}

void CStartingItemDlg::GetBootInfo()
{
	StartingItemList.DeleteAllItems();
	m_vecBootInfo.clear();

	BOOTINFO stcBootInfo = { 0 };

	HKEY RootKey = NULL;						// ����
	HKEY hkResult = NULL;						// ��Ҫ�򿪼��ľ�� 
	LONG lReturn;						// ��¼��ȡע����Ƿ�ɹ�
	DWORD index = 0;
	TCHAR szKeyName[255] = { 0 };       // ע���������
	DWORD dwKeyLen = 255;
	DWORD dwNameLen = 255;
	DWORD dwType = 0;
	CString buffer;
	LPCTSTR lpSubKey[2];				// �Ӽ�����
	DWORD ListIndex = 0;

	lpSubKey[0] = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	lpSubKey[1] = _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run");

	for (DWORD i = 0; i < 2; i++)
	{
		if (i == 0)
			RootKey = HKEY_CURRENT_USER;
		if (i == 1)
			RootKey = HKEY_LOCAL_MACHINE;

		//��lpSubkey
		lReturn = RegOpenKeyEx(RootKey, lpSubKey[i], 0,
			KEY_ALL_ACCESS, &hkResult);
		if (lReturn == ERROR_SUCCESS)
		{
			//����򿪳ɹ�����ʼö����ֵ
			while (ERROR_NO_MORE_ITEMS !=
				RegEnumValue(hkResult, index, stcBootInfo.szBootName, &dwKeyLen,
					0, NULL, NULL, NULL))
			{
				//��ȡÿ��ֵ������ szKeyNameΪֵ������ bufferΪֵ������
				RegQueryValueEx(hkResult, stcBootInfo.szBootName,
					0, &dwType, (LPBYTE)stcBootInfo.szBootOrder, &dwNameLen);
				//������Ϣ
				//������	stcBootInfo.szBootName
				//����	stcBootInfo.szBootOrder

				//����λ����Ϣ
				//������Ϣ
				stcBootInfo.szHKEY;
				if (i == 0)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_CURRENT_USER"));
				if (i == 1)
					_tcscpy_s(stcBootInfo.szHKEY, _T("HKEY_LOCAL_MACHINE"));
				//ɾ��ʱҪ�õ���
				_tcscpy_s(stcBootInfo.szBootPos, lpSubKey[i]);
				//List��Ҫ��ʾ��
				buffer.Empty();
				buffer = buffer + stcBootInfo.szHKEY + _T("\\") + stcBootInfo.szBootPos;
				_tcscpy_s(stcBootInfo.szBootPos_All, buffer);


				//��ӵ�vector
				m_vecBootInfo.push_back(stcBootInfo);
				//����List
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
	//��ֹ�ڿհ�����������˵�  
	int n = pNMItemActivate->iItem;//ѡ���˵ڼ���
	if (n != -1)
	{
		//������ѡ�����������

		m_Select = n;

		//�������δ���, ��������Ӧ��ListCtrl  
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

//���������
void CStartingItemDlg::Onaddmenu()
{
	//���ļ�����
	TCHAR FilePathName[MAX_PATH] = { 0 };
	CString FileName;
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		//�ļ�����������FilePathName��
		_tcscpy_s(FilePathName, dlg.GetPathName());
	}
	else
	{
		return;
	}

	//��������Key
	HKEY hKey;
	LPCTSTR lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//��ȡ·������
		DWORD dwRet = _tcslen(FilePathName);

		//�����������Ϣ
		FileName = dlg.GetFileTitle();
		int n = sizeof(TCHAR);
		lRet = RegSetValueEx(hKey, FileName, 0, REG_SZ,
			(BYTE *)FilePathName, dwRet * sizeof(TCHAR));

		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("���������ʧ��!"));
		}
		else
		{
			AfxMessageBox(_T("���������ɹ�!"));
			GetBootInfo();
		}
	}
}
//ɾ��������
void CStartingItemDlg::OnDeleItemmenu()
{
	if (m_Select == -1)
	{
		return;
	}
	HKEY RootKey = NULL;		//����
	HKEY hKey;
	//ȷ��Ҫɾ���ĸ���
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
	//ȷ��Ҫɾ����Run�Ӽ�·��
	LPCTSTR lpRun = m_vecBootInfo[m_Select].szBootPos;
	//��������Key   
	long lRet = RegOpenKeyEx(RootKey, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		//��ȡҪɾ�������������
		strTemp = m_vecBootInfo[m_Select].szBootName;
		//ɾ���������� 
		lRet = RegDeleteValue(hKey, strTemp);
		//�ر�ע���   
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			AfxMessageBox(_T("ɾ��������ɹ���"));
		}
		else {
			AfxMessageBox(_T("ɾ��������ɹ���"));
			GetBootInfo();
		}
	}
}
