#pragma once

#include "resource.h"
#include "afxwin.h"
// CIPDlg ��ȭ �����Դϴ�.

class CIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIPDlg)

public:
	CIPDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CIPDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_pIP1;
	CEdit m_pIP2;
	CEdit m_pIP3;
	CEdit m_pIP4;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
