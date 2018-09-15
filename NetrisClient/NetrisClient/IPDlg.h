#pragma once

#include "resource.h"
#include "afxwin.h"
// CIPDlg 대화 상자입니다.

class CIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIPDlg)

public:
	CIPDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIPDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_pIP1;
	CEdit m_pIP2;
	CEdit m_pIP3;
	CEdit m_pIP4;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
