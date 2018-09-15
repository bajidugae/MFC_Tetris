// IPDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MainFrm.h"
#include "NetrisClient.h"
#include "NetrisClientDoc.h"
#include "NetrisClientView.h"
#include "IPDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CIPDlg 대화 상자입니다.

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


// CIPDlg 메시지 처리기입니다.


void CIPDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
		AfxMessageBox( _T( "IP 주소가 잘못 됐습니다." ) );
}


BOOL CIPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pIP1.SetWindowTextW( _T( "127" ) );
	m_pIP2.SetWindowTextW( _T( "0" ) );
	m_pIP3.SetWindowTextW( _T( "0" ) );
	m_pIP4.SetWindowTextW( _T( "1" ) );

	return TRUE;
}
