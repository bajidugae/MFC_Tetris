// IPDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "NetrisClient.h"
#include "NetrisClientDoc.h"
#include "NetrisClientView.h"
#include "IPDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CIPDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CIPDlg, CDialogEx)

CIPDlg::CIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIPDlg::IDD, pParent)
{

}

CIPDlg::~CIPDlg()
{
	DestroyWindow();
}

void CIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_pIP1);
	DDX_Control(pDX, IDC_EDIT2, m_pIP2);
	DDX_Control(pDX, IDC_EDIT3, m_pIP3);
	DDX_Control(pDX, IDC_EDIT4, m_pIP4);
}


BEGIN_MESSAGE_MAP(CIPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CIPDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CIPDlg �޽��� ó�����Դϴ�.


void CIPDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame *pFrame = ( CMainFrame * )AfxGetMainWnd();
	CNetrisClientView *pView = ( CNetrisClientView * )pFrame->GetActiveView();

	CString str;
	CString ip1, ip2, ip3, ip4;
	m_pIP1.GetWindowTextW( ip1 );
	m_pIP2.GetWindowTextW( ip2 );
	m_pIP3.GetWindowTextW( ip3 );
	m_pIP4.GetWindowTextW( ip4 );

	if( _ttoi( ip1 ) < 256 &&
		_ttoi( ip2 ) < 256 &&
		_ttoi( ip3 ) < 256 &&
		_ttoi( ip4 ) < 256 )
	{
		str.Format( _T( "%d.%d.%d.%d" ), _ttoi( ip1 ), _ttoi( ip2 ), _ttoi( ip3 ), _ttoi( ip4 ) );
		AfxMessageBox( str );

		pView->ResetGame();
		pView->m_strIP = str;
		pView->m_bIsMulti = true;
		pView->Connect();

		EndDialog(0);
	}
	else
		AfxMessageBox( _T( "IP �ּҰ� �߸� �ƽ��ϴ�." ) );
}


BOOL CIPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pIP1.SetWindowTextW( _T( "127" ) );
	m_pIP2.SetWindowTextW( _T( "0" ) );
	m_pIP3.SetWindowTextW( _T( "0" ) );
	m_pIP4.SetWindowTextW( _T( "1" ) );

	return TRUE;
}
