// ClearVir.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "ClearVir.h"
#include "afxdialogex.h"
#include "Md5.h"

// CClearVir �Ի���

IMPLEMENT_DYNAMIC(CClearVir, CDialogEx)

CClearVir::CClearVir(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ClearVirDlg, pParent)
	, Cpath(_T("�ļ�·����"))
	, strSelModel(_T("������"))
	, strSelVirLib(_T("����"))
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CVirList.InsertColumn(0, L"�ļ�����", 0, 100);
	CVirList.InsertColumn(1, L"����λ��", 0, 300);
	CVirList.InsertColumn(2, L"������С", 0, 50);
	CSelPath.SetCurSel(0);

	return TRUE;  
}

DWORD WINAPI ScanThread(LPVOID lpVoid)
{
	CClearVir* pScanCtrl = (CClearVir*)lpVoid;
	//��ɱ·���ж�
	if (pScanCtrl->IsSingleScan)
	{
		//���ļ���ɱ
		pScanCtrl->Scan_Single(pScanCtrl->m_SinglePath);
	}
	else
	{
		//ȫ·����ɱ
		//����·��ɨ�裬����·������
		pScanCtrl->Scan_All(pScanCtrl->m_AllPath);
	}


	pScanCtrl->Cpath.Format(
		_T("�� ɱ �� �� �� �� �� �� ɱ �� �� �� �� �� ��%d"),
		pScanCtrl->m_vecVirInfo.size());
	pScanCtrl->SendMessage(WM_MYUPDATEDATA, FALSE);

	return 0;
}

void CClearVir::StartScan()
{
	//1.���ж�ɨ��·��
	if (IsSingleScan)
	{
		m_SinglePath.Empty();
		//���Ϊ�����ļ���ɱ�������ļ�ѡ���·��������m_szSingleFilePath
		CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
			NULL,
			NULL,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			(LPCTSTR)_TEXT("All Files (*.*)|*.*||"),
			NULL);
		if (dlg.DoModal() == IDOK)
		{
			//�򿪳ɹ��������ļ�·��
			m_SinglePath = dlg.GetPathName();
		}
		else
		{
			Cpath = _T("�� �� �� �� ʧ �� ��");
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
		//���Ϊȫ·����ɱ�������ļ���ѡ���·��������m_AllPath
		m_AllPath.Empty();
		m_AllPath = BrowseFolder(this->m_hWnd, _T("��ѡ���ɱ·��"));
		if (m_AllPath.IsEmpty())
		{
			return;
		}
	}

	CVirList.DeleteAllItems();
	//2.�ж����ƶ˻��Ǳ���
	//����Ǳ��أ����ж���MD5���ǰ����������ر��ز�������Ϣ
	if (IsLocal)
	{
		if (IsMD5)
		{
			//����MD5	���ر���MD5������
			//�Ȼ�ȡ��������MD5�ĸ���
			CString Temp;
			GetPrivateProfileString(_T("Num"),
				_T("MD5"), _T("DefaultName"),
				Temp.GetBuffer(MAX_PATH), MAX_PATH,
				_T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//�ٸ��ݸ�������MD5��Ϣ�����ص�Vector
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
				Cpath = _T("�� ȡ �� �� �� ���� ��");
				UpdateData(FALSE);
				return;
			}
		}
		else
		{
			//���ذ�����	���ر��ذ�����������
			//�Ȼ�ȡ�������а������ĸ���
			CString Temp;
			GetPrivateProfileString(_T("Num"),
				_T("White"), _T("DefaultName"),
				Temp.GetBuffer(MAX_PATH), MAX_PATH,
				_T(".\\LocalVirBase\\Vir.ini"));
			Temp.ReleaseBuffer();
			int n = _ttoi(Temp);
			//�ڸ��ݸ�������MD5��Ϣ�����ص�Vector
			for (int i = 0; i < n; i++)
			{
				CString tempCount;
				tempCount.Format(_T("%d"), i);
				GetPrivateProfileString(_T("White"),
					tempCount, _T("DefaultName"),
					Temp.GetBuffer(MAX_PATH), MAX_PATH,
					_T(".\\LocalVirBase\\Vir.ini"));
				Temp.ReleaseBuffer();
				m_LocalWhite.push_back(Temp);
			}
			if (m_LocalWhite.size() == 0)
			{
				Cpath = _T("�� ȡ �� �� �� ���� ��");
				UpdateData(FALSE);
				return;
			}
		}
	}
	//������ƶˣ����ж���MD5���ǰ������������ƶ˲�������Ϣ
	else
	{
		// 		if (IsMD5)
		// 		{
		// 			//�ƶ�MD5	�����ƶ�MD5������
		// 			//��ȡ����������  TRUE �����ȡ MD5���ݿ�
		// 			if (!m_ServerObj.SendMyMessage(TRUE))
		// 			{
		// 				m_szStatus = _T("�� �� �� �� �� ʧ �� ...");
		// 				UpdateData(FALSE);
		// 				return;
		// 			}
		// 			//�������ӷ�����
		// 			m_szStatus = _T("�� �� �� �� �� �� �� ...");
		// 			UpdateData(FALSE);
		// 			m_ServerMD5 = m_ServerObj.RetVector();
		// 		}
		// 		else
		// 		{
		// 			//�ƶ˰�����	�����ƶ˰�����������
		// 			//��ȡ����������  FALSE �����ȡ ���������ݿ�
		// 			if (!m_ServerObj.SendMyMessage(FALSE))
		// 			{
		// 				m_szStatus = _T("�� �� �� �� �� ʧ �� ...");
		// 				UpdateData(FALSE);
		// 				return;
		// 			}
		// 			//�������ӷ�����
		// 			m_szStatus = _T("�� �� �� �� �� �� �� ...");
		// 			UpdateData(FALSE);
		// 			m_ServerWhite = m_ServerObj.RetVector();
		// 		}
	}
	//3.��������ɨ���߳�
	//������������������߳��еĺ������в�ɱ����������������̵߳�Vector��
	CloseHandle(CreateThread(NULL, 0, ScanThread, this, 0, NULL));
}

BOOL CClearVir::MD5Scan(LPCTSTR szPath)
{
	//LPCTSTR תCHAR*
	int num = WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, NULL, 0, NULL, FALSE);
	char *pchar = new char[num];
	WideCharToMultiByte(CP_OEMCP, NULL, szPath, -1, pchar, num, NULL, FALSE);
	//��ȡMD5ֵ
	char* md5 = md5FileValue(pchar);
	//CHAR* תLPCTSTR
	num = MultiByteToWideChar(0, 0, md5, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, md5, -1, wide, num);
	m_szMD5 = wide;
	delete[]pchar;
	delete[]wide;

	//��ȡ�ļ�MD5��Ϣ���
	//�ж��Ǳ��ز�ɱ�����ƶ˲�ɱ
	if (IsLocal)
	{
		//����MD5��ɱ		��m_LocalMD5���Ա�
		for (DWORD i = 0; i < m_LocalMD5.size(); i++)
		{
			if (m_LocalMD5[i] == m_szMD5)
			{
				//�ǲ��� ������
				return TRUE;
			}
		}
	}
	else
	{
		//�ƶ�MD5��ɱ		��m_ServerMD5���Ա�
		//����MD5��ɱ		��m_LocalMD5���Ա�
		for (DWORD i = 0; i < m_ServerMD5.size(); i++)
		{
			if (m_ServerMD5[i] == m_szMD5)
			{
				//�ǲ��� ������
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
		if (SUCCEEDED(::SHGetMalloc(&pMalloc))) // ȡ��IMalloc�������ӿ�
		{
			pMalloc->Free(pidl); // �ͷ��ڴ�
			pMalloc->Release(); // �ͷŽӿ�
		}
	}
	return szPath;
}

//������ɱ
void CClearVir::Scan_Single(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	CString sFindFilter;
	DWORD dwAttributes = 0;
	sFindFilter = szPath;
	FindFirstFile(sFindFilter, &wfd);
	//�ж��Ƿ��ǲ���
	if (IsMD5)
	{
		//MD5��ɱ
		if (MD5Scan(szPath)) {
			//����List
			CVirList.InsertItem(0, wfd.cFileName);
			//�ļ���
			CVirList.SetItemText(0,1, szPath);
			//�ļ�·��
			CVirList.SetItemText(0, 2, GetShowSize(wfd.nFileSizeLow));
			//���浽Vector
			VIRINFO VirInfo;
			_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
			_tcscpy_s(VirInfo.szVirPath, szPath);
			_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
			m_vecVirInfo.push_back(VirInfo);

		}
	}

}

//ȫ·����ɱ
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

		//��ȡ����·����
		sFullPath = szPath;
		sFullPath += TEXT("\\");
		sFullPath += wfd.cFileName;

		//�����ǰ·�����ļ��У�����Ҫ�ݹ��ļ����е��ļ�
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			Scan_All(sFullPath);
		}
		//��������һ���ļ�
		else
		{
			//��������������ļ�·�����棬���û�ֱ�۵Ŀ�������ɨ����ļ�·��
			Cpath = _T("����ɨ�� : ") + sFullPath;
			SendMessage(WM_MYUPDATEDATA, FALSE);
			//UpdateData(FALSE);
			//�ж��Ƿ��ǲ���
			if (IsMD5)
			{
				//MD5��ɱ
				//����ǲ��� ���ļ���Ϣ���浽����Vector
				if (MD5Scan(sFullPath))
				{
					//����List����ʾ
					CVirList.InsertItem(0, wfd.cFileName);	//�ļ���
					CVirList.SetItemText(0, 1, sFullPath);	//�ļ�·��
					CVirList.SetItemText(0, 2,				//�ļ���С
						GetShowSize(wfd.nFileSizeLow));
					//���没����Ϣ��Vector����
					VIRINFO VirInfo;
					_tcscpy_s(VirInfo.szVirName, wfd.cFileName);
					_tcscpy_s(VirInfo.szVirPath, sFullPath);
					_tcscpy_s(VirInfo.szVirSize, GetShowSize(wfd.nFileSizeLow));
					m_vecVirInfo.push_back(VirInfo);
				}
			}
			else
			{
				//��������ɱ
				//����ǲ��� ���ļ���Ϣ���浽����Vector
				if (WhiteScan(sFullPath))
				{
					//����List����ʾ
					CVirList.InsertItem(0, wfd.cFileName);	//�ļ���
					CVirList.SetItemText(0, 1, sFullPath);	//�ļ�·��
					CVirList.SetItemText(0, 2,				//�ļ���С
						GetShowSize(wfd.nFileSizeLow));
					//���没����Ϣ��Vector����
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



BOOL CClearVir::WhiteScan(LPCTSTR szPath)
{

	//�ж��Ǳ��ز�ɱ�����ƶ˲�ɱ
	if (IsLocal)
	{
		//���ذ�������ɱ		��m_LocalWhite���Ա�
		for (DWORD i = 0; i < m_LocalWhite.size(); i++)
		{
			if (m_LocalWhite[i] == szPath)
			{
				//�ǲ��� ������
				return FALSE;
			}
		}
	}
	else
	{
		//�ƶ�MD5��ɱ		��m_ServerWhite���Ա�
		for (DWORD i = 0; i < m_ServerWhite.size(); i++)
		{
			if (m_ServerWhite[i] == szPath)
			{
				//�ǲ��� ������
				return FALSE;
			}
		}
	}
	return TRUE;
}



void CClearVir::OnCbnSelchangeSelpath()
{
	int nIndex = CSelPath.GetCurSel();
	CString strText;

	CSelPath.GetLBText(nIndex, strText);
	if (!strText.Compare(L"�����ļ�ɨ��") ) {
		//���ļ���ɱ
		IsSingleScan = TRUE;
	}
	else
	{
		//���ļ���ɱ
		IsSingleScan = FALSE;
	}
}


void CClearVir::OnBnClickedbtnvirscan()
{
	if (!strSelModel.Compare(L"MD5")) {
		
		//MD5
		IsMD5 = TRUE;
	}
	else
	{
		//������
		IsMD5 = FALSE;
	}
	if (!strSelVirLib.Compare(L"����"))
	{
		//����
		IsLocal = TRUE;
	}
	else
	{
		//�ƶ�
		IsLocal = FALSE;
	}
	StartScan();
}


void CClearVir::OnBnClickedbtnclearvir()
{
	//���жϲ���Vector�Ƿ�Ϊ�գ���Ϊ�գ�����ʾû�п���������ļ�
	if (m_vecVirInfo.size() == 0)
	{
		return;
	}
	//ɾ������
	for (DWORD i = 0; i < m_vecVirInfo.size(); i++)
	{
		DeleteFile(m_vecVirInfo[i].szVirPath);
	}
	m_vecVirInfo.clear();
	CVirList.DeleteAllItems();
	Cpath = _T("�� �� �� �� �� �� !");
	UpdateData(FALSE);
}


afx_msg LRESULT CClearVir::OnUpDateData(WPARAM wParam, LPARAM lParam)
{
	return UpdateData(wParam);
}
