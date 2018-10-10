#pragma once
#include "afxcmn.h"
#include <vector>

// CRelocDlg 对话框

class CRelocDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRelocDlg)

public:
	CRelocDlg(char* g_lpbase,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRelocDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Reloc_Dlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowReloc();
	DWORD RVAtoFOA(DWORD dwRVA);
	void ShowSectionInList();
	void ShowBlockInList(int index);
	char* lpbase;

	CListCtrl CRelocSecList;
	CListCtrl RelocBlockList;
	
	afx_msg void OnClickRelocseclist(NMHDR *pNMHDR, LRESULT *pResult);
};


typedef struct _RELOCINFO
{
	DWORD dwRelocRVA;	//需要重定位的相对虚拟地址（RVA）
	DWORD dwOffset;		//根据RVA算出的文件偏移地址
	BYTE bType;			//重定位方式（属性也就是type高四位）
	DWORD dwRelocValue;	//从算出的文件偏移取出的数据，这个数据就是VA
	CString bData;		//用VA减去基址得到的RVA，再算一下文件偏移将其中的数据取出，放到这个数组
}RELOCINFO, *PRELOCINFO;//这个结构体描述的一个重定位信息

typedef struct _RELOCAREAINFO
{
	DWORD szSectionName;	//区域所在的节名
	DWORD dwAreaRVA;		//区域的基址（他是一个相对虚拟地址）
	DWORD NumberOfReloc;	//这个区域的重定位个数
	std::vector<RELOCINFO> vecRelocInfo;	//这个区域重定位的具体信息
}RELOCAREAINFO, *PRELOCAREAINFO;	//这个结构体描述一个区域的重定位的信息
typedef struct Offset
{
	WORD Offsset : 12;		//偏移值
	WORD Type : 4;			//重定位属性（方式）
}TYPEOFFSET, *PTYPEOFFSET;