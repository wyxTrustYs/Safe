// FileAttr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "FileAttr.h"
#include "afxdialogex.h"
#include <Wincrypt.h>

// CFileAttr �Ի���

IMPLEMENT_DYNAMIC(CFileAttr, CDialogEx)

CFileAttr::CFileAttr(LPCTSTR file, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FileAttributeDlg, pParent), m_CurrentFile(file)
	, m_Name(_T(""))
	, m_Type(_T(""))
	, m_Path(_T(""))
	, m_CreatTime(_T(""))
	, m_LVisitTime(_T(""))
	, m_LChangeTime(_T(""))
	, m_Read(FALSE)
	, m_Hide(FALSE)
	, m_md5value(_T(""))
{

}

CFileAttr::~CFileAttr()
{
}

void CFileAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FANameEdit, m_Name);
	DDX_Text(pDX, IDC_FATypeEdit, m_Type);
	DDX_Text(pDX, IDC_FALocEdit, m_Path);
	DDX_Text(pDX, IDC_FAFoundEdit, m_CreatTime);
	DDX_Text(pDX, IDC_FAVisEdit, m_LVisitTime);
	DDX_Text(pDX, IDC_FACheEdit, m_LChangeTime);
	DDX_Check(pDX, IDC_CHECK1, m_Read);
	DDX_Check(pDX, IDC_CHECK2, m_Hide);
	DDX_Text(pDX, IDC_FAMD5Edit, m_md5value);
}


BEGIN_MESSAGE_MAP(CFileAttr, CDialogEx)
END_MESSAGE_MAP()


// CFileAttr ��Ϣ�������


BOOL CFileAttr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SHFILEINFO ShInfo = { 0 };
	WCHAR FullPath[MAX_PATH] = {}, *Name;
	SHGetFileInfo(m_CurrentFile, 0, &ShInfo, sizeof(ShInfo), SHGFI_DISPLAYNAME);

	GetFullPathName(m_CurrentFile, MAX_PATH, FullPath, &Name);
	m_Path = FullPath;
	SHGetFileInfo(FullPath, 0, &ShInfo, sizeof(ShInfo), SHGFI_DISPLAYNAME);
	m_Name = ShInfo.szDisplayName;
	SHGetFileInfo(FullPath, 0, &ShInfo, sizeof(ShInfo), SHGFI_TYPENAME);
	WIN32_FIND_DATA FileInfo = { 0 };
	FindFirstFile(FullPath, &FileInfo);
	FILETIME Time = { 0 };

	//��ȡ����ʱ��
	FileTimeToLocalFileTime(&FileInfo.ftCreationTime, &Time);
	SYSTEMTIME sysTime = { 0 };
	FileTimeToSystemTime(&Time, &sysTime);
	m_CreatTime.Format(L"%d-%d-%d %d:%d:%d", sysTime.wYear,
		sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
		sysTime.wSecond);
	//��ȡ������ʱ��
	FileTimeToLocalFileTime(&FileInfo.ftLastAccessTime, &Time);
	FileTimeToSystemTime(&Time, &sysTime);
	m_LVisitTime.Format(L"%d-%d-%d %d:%d:%d", sysTime.wYear,
		sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
		sysTime.wSecond);
	//��ȡ����޸�ʱ��
	FileTimeToLocalFileTime(&FileInfo.ftLastWriteTime, &Time);
	FileTimeToSystemTime(&Time, &sysTime);
	m_LChangeTime.Format(L"%d-%d-%d %d:%d:%d", sysTime.wYear,
		sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute,
		sysTime.wSecond);
	m_Type = ShInfo.szTypeName;

	//����ֻ����ѡ��
	if (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_READONLY)
		m_Read = TRUE;
	//�������ظ�ѡ��
	if (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)
		m_Hide = TRUE;
	md5fileValue();
	UpdateData(FALSE);
	return TRUE;  
}

BOOL CFileAttr::md5fileValue()
{
	m_md5value = "";
	HANDLE hFile = CreateFile(m_Path, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	HCRYPTPROV hProv = NULL;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT) == FALSE)
	{
		return 0;
	}
	HCRYPTPROV hHash = NULL;
	if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)
	{
		return 0;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);
	if (dwFileSize == 0xFFFFFFFF)return 0;
	byte* lpReadFileBuffer = new byte[dwFileSize];
	DWORD lpReadNumberOfBytes;
	//��ȡ�ļ�
	if (ReadFile(hFile, lpReadFileBuffer, dwFileSize,
		&lpReadNumberOfBytes, NULL) == 0)return 0;
	//hash�ļ�
	if (CryptHashData(hHash, lpReadFileBuffer,
		lpReadNumberOfBytes, 0) == FALSE)return 0;
	delete[] lpReadFileBuffer;
	//�ر��ļ����
	CloseHandle(hFile);
	BYTE *pbHash;
	DWORD dwHashLen = sizeof(DWORD);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, NULL,
		&dwHashLen, 0))return 0;
	if (!(pbHash = (byte*)malloc(dwHashLen)))return 0;
	if (CryptGetHashParam(hHash, HP_HASHVAL,
		pbHash, &dwHashLen, 0))
	{
		for (DWORD i = 0; i < dwHashLen; ++i)
		{
			CString content;
			content.Format(L"%02x", pbHash[i]);
			m_md5value += content;
		}
	}
	return 0;
}
