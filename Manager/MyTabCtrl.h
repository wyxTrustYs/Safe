#pragma once


// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();
	void InsertTab(DWORD dwCount,...);		//����ѡ�
	void AddDig(DWORD dwCount, ...);			//���ӶԻ���
	void SetSelAndShow(int Sel);	//���õ�ǰѡ�еĶԻ�����ʾ
	CDialogEx* m_pDlg[20];	//ѡ��д洢�ĶԻ�������
	DWORD m_dwCount;		//��ǰѡ��Ի�����Ϣ
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


