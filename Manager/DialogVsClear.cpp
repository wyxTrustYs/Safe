// DialogVsClear.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "DialogVsClear.h"
#include "afxdialogex.h"


// CDialogVsClear �Ի���

IMPLEMENT_DYNAMIC(CDialogVsClear, CDialogEx)

CDialogVsClear::CDialogVsClear(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VSCLEAR, pParent), CountSize(), DestSize(), FileCount(0), DestCount(0), DeleteCount(0)
	, ExtendString(_T(".obj\\.tlog\\.lastbuildstate\\.idb\\.pdb\\.pch\\.res\\.ilk\\.exe\\.sdf\\.ipch\\.log\\.db\\.suo\\"))
{

}

CDialogVsClear::~CDialogVsClear()
{
}

void CDialogVsClear::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATH, CListPath);

	DDX_Control(pDX, IDC_WAITDEL, CWaitDel);
	DDX_Control(pDX, IDC_DEL, CDel);

	DDX_Text(pDX, IDC_TYPE, ExtendString);
}




BEGIN_MESSAGE_MAP(CDialogVsClear, CDialogEx)
	//	ON_WM_QUERYDRAGICON()
	//	ON_WM_PAINT()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_DELETE, &CDialogVsClear::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_RESET, &CDialogVsClear::OnBnClickedReset)
	ON_BN_CLICKED(IDC_CHECK, &CDialogVsClear::OnBnClickedCheck)
END_MESSAGE_MAP()


// CDialogVsClear ��Ϣ�������


BOOL CDialogVsClear::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CListPath.InsertColumn(0, L"�ļ�·��", 0, 400);
	CWaitDel.InsertColumn(0, L"��ɾ�����ļ��б�", 0, 400);
	CDel.InsertColumn(0, L"��ɾ�����ļ��б�", 0, 400);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}





void CDialogVsClear::OnDropFiles(HDROP hDropInfo)
{
	CDialogEx::OnDropFiles(hDropInfo);
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	// 2. ѭ����ȡÿһ���ļ�
	for (int i = 0; i < DropCount; i++)
	{
		WCHAR wcStr[MAX_PATH];
		// 3. �����ҷ�ĵ�i���ļ����ļ���  
		// DragQueryFile(hDropInfo, -1, NULL, 0);
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);

		// 4. �������һ���ļ��У�����뵽�б���
		if (GetFileAttributes(wcStr) & FILE_ATTRIBUTE_DIRECTORY)
		{
			// 5. �����·�����������б���
			if (find(PathStrings.begin(), PathStrings.end(), wcStr) == PathStrings.end())
			{
				// 6. ��ӵ�����·�����������б���
				PathStrings.push_back(wcStr);
				CListPath.InsertItem(0, wcStr);
			}
		}
	}

	// 7.�ϷŽ�����,�ͷ��ڴ�  
	DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}




void CDialogVsClear::OnBnClickedDelete()
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
			CDel.InsertItem(0, File);
		}
	}

	// 3. ��ʾɾ����Ϣ
	CString Msg;
	Msg.Format(L"��ɾ���ļ� %d ��\n��ɾ���ļ� %d ��",
		DestCount, DeleteCount);
	MessageBox(Msg);
}


void CDialogVsClear::OnBnClickedReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	MessageBox(L"�ļ������³ɹ�!");
}


void CDialogVsClear::OnBnClickedCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// 0. ���������Ϣ
	CWaitDel.DeleteAllItems();
	DelStrings.clear();
	FileCount = DestCount = 0;
	CountSize.QuadPart = DestSize.QuadPart = 0;

	// 1. ɾ��·���е������ļ�
	for (auto Path : PathStrings)
		FindFile(Path);

	// for
	// for each
	// for (auto xxx =xx.end)
	// for (auto a: xxx)

	// 2. ��ȡ�������Ĵ�С��Ϣ
	CString Msg;
	Msg.Format(L"�����ļ�����: %-3d ��С: %.2lfMB\n"
		"��ɾ�ļ�����: %-3d ��С: %.2lfMB",
		FileCount, CountSize.QuadPart / 1024.0 / 1024.0,
		DestCount, DestSize.QuadPart / 1024.0 / 1024.0);

	// 3. �����Ϣ
	MessageBox(Msg);
}

void CDialogVsClear::FindFile(CString Path)
{
	// 1. ���Բ��ҵ�һ���ļ�
	WIN32_FIND_DATA FileInfo = { 0 };
	HANDLE File = FindFirstFile(Path + "\\*", &FileInfo);

	// 2. �����ļ��е������ļ�
	if (File != INVALID_HANDLE_VALUE)
	{
		do {
			// 3.1 ������ļ��о͵ݹ�
			if (FileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 3.1.1 �ų������ļ���
				if (CString(".") != FileInfo.cFileName && CString("..") != FileInfo.cFileName)
					FindFile(Path + CString("\\") + FileInfo.cFileName);
			}
			// 3.2 ������ļ�
			else
			{
				// 3.2.0 �ۼ��ļ���С�͸���
				++FileCount;
				CountSize.QuadPart += FileInfo.nFileSizeLow;

				// 3.2.1 �жϺ�׺��
				if (-1 != ExtendString.Find(PathFindExtension(FileInfo.cFileName)))
				{
					++DestCount;
					DestSize.QuadPart += FileInfo.nFileSizeLow;
					CWaitDel.InsertItem(0, Path + CString("\\") + FileInfo.cFileName);
					DelStrings.push_back(Path + CString("\\") + FileInfo.cFileName);
				}
			}
		} while (FindNextFile(File, &FileInfo));
	}
}