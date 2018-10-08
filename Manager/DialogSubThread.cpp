// DialogSubThread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Manager.h"
#include "DialogSubThread.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// DialogSubThread �Ի���

IMPLEMENT_DYNAMIC(DialogSubThread, CDialogEx)

DialogSubThread::DialogSubThread(int PID,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SubThread, pParent)
{
	this->mPID = PID;
}

DialogSubThread::~DialogSubThread()
{
}

void DialogSubThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTSubThread, SubThreadList);
}

BEGIN_MESSAGE_MAP(DialogSubThread, CDialogEx)
END_MESSAGE_MAP()


// DialogSubThread ��Ϣ�������


BOOL DialogSubThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SubThreadList.InsertColumn(0, L"��������", 0, 200);
	SubThreadList.InsertColumn(1, L"threadID", 0, 70);
	SubThreadList.InsertColumn(2, L"���ȼ�", 0, 70);


	SubThreadList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);


	HANDLE hThread = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;
	// �������� ���������ǵ�ǰ����ϵͳ�������̵߳Ŀ���
	hThread =
		CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	// ��������������ṹ�Ĵ�С
	te32.dwSize = sizeof(THREADENTRY32);
	// ��ʼ��ȡ��Ϣ
	if (Thread32First(hThread, &te32) == TRUE)
	{
		int index = 0;

		//3 ѭ�����������̵߳���Ϣ
		do
		{
			if (te32.th32OwnerProcessID != mPID)
				continue;
			TCHAR temp[20] = { 0 };
			
			_stprintf_s(temp, 20, L"%d", te32.th32OwnerProcessID);
			SubThreadList.InsertItem(index, temp);

			_stprintf_s(temp, 20, L"%d", te32.th32ThreadID);
			SubThreadList.SetItemText(index, 1, temp);
			_stprintf_s(temp, 20, L"%d", te32.tpBasePri);
			SubThreadList.SetItemText(index, 2, temp);
			++index;
		} while (Thread32Next(hThread, &te32));
	}



	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
