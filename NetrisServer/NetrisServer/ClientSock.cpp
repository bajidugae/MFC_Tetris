// ClientSock.cpp : implementation file
//

#include "stdafx.h"
#include "NetrisServer.h"
#include "ClientSock.h"
#include "NetrisServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSock

CClientSock::CClientSock()
{
}

CClientSock::~CClientSock()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CClientSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSock member functions

void CClientSock::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CNetrisServerDlg* dlg = (CNetrisServerDlg*)AfxGetApp()->m_pMainWnd;
	dlg->OnReceive(this);
	CAsyncSocket::OnReceive(nErrorCode);
}

void CClientSock::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CNetrisServerDlg* dlg = (CNetrisServerDlg*)AfxGetApp()->m_pMainWnd;
	dlg->OnClose(this);
	CAsyncSocket::OnClose(nErrorCode);
}