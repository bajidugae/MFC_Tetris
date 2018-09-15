
// NetrisServerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "NetrisServer.h"
#include "NetrisServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetrisServerDlg ��ȭ ����




CNetrisServerDlg::CNetrisServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetrisServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNetrisServerDlg::~CNetrisServerDlg()
{
	delete m_pServerSock;
	delete m_pPlayer1Sock;
	delete m_pPlayer2Sock;
}

void CNetrisServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_pList);
	DDX_Control(pDX, IDC_PLAYER1, m_sttPlayer1);
	DDX_Control(pDX, IDC_PLAYER2, m_sttPlayer2);
	DDX_Control(pDX, IDC_SERVERIP, m_sttServerIP);
	
}

BEGIN_MESSAGE_MAP(CNetrisServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CNetrisServerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CNetrisServerDlg �޽��� ó����

BOOL CNetrisServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pServerSock = new CServerSock();
	m_pServerSock->Create(700);
	m_pServerSock->Listen();

	m_pPlayer1Sock = NULL;
	m_pPlayer2Sock = NULL;
	m_bPlayer1Enter = false;
	m_bPlayer2Enter = false;

	//CString str;
	//UINT port;
	//m_pServerSock->GetPeerName( str, port );
	m_sttServerIP.SetWindowTextA( GetIpAddress() );

	m_nPlayer = 1; // 1�÷��̾����

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CNetrisServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CNetrisServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CNetrisServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNetrisServerDlg::OnAccept()
{
	if( m_nPlayer == 1 )
	{
		m_pPlayer1Sock = new CClientSock;
		m_pServerSock->Accept(*m_pPlayer1Sock);
		m_pList.AddString( _T("�÷��̾�1 �����") );
		m_sttPlayer1.SetWindowTextA( _T("���� ��") );
		m_pPlayer1Sock->Send( "10", 2 );
		m_bPlayer1Enter = true;

		if( m_pPlayer2Sock == NULL )
		{
			m_nPlayer = 2;
		}
		else
		{
			m_nPlayer = 3;
		}
	}
	else if( m_nPlayer == 2 )
	{
		m_pPlayer2Sock = new CClientSock;
		m_pServerSock->Accept(*m_pPlayer2Sock);
		m_pList.AddString( _T("�÷��̾�2 �����") );
		m_sttPlayer2.SetWindowTextA( _T("���� ��") );
		m_pPlayer2Sock->Send( "20", 2 );
		m_bPlayer2Enter = true;
		m_nPlayer = 3;

		if( m_pPlayer1Sock == NULL )
		{
			m_nPlayer = 1;
		}
		else
		{
			m_nPlayer = 3;
		}
	}
	else if( m_nPlayer == 3 )
	{
		m_pList.AddString( _T("Ǯ��") );
	}
}

void CNetrisServerDlg::OnReceive(CClientSock* pSock)
{
	char buf[201];
	CString str;
	//ZeroMemory( buf, sizeof(buf) );
	int nErr = pSock->Receive(buf,201);
	int nType = buf[0];
	
	if(nErr == SOCKET_ERROR)
	{
		int err = pSock->GetLastError();
	}

	if( m_bPlayer1Enter == true && m_bPlayer2Enter == true )
	{
		if( buf[0] == PLAYER1_BOARD ) // 1P����->2P ����
		{
			m_pPlayer2Sock->Send( buf, 201 );
		}
		else if( buf[0] == PLAYER2_BOARD ) // 2P����->1P ����
		{
			m_pPlayer1Sock->Send( buf, 201 );
		}
		else if( buf[0] == PLAYER1_ATTACK ) // 1P->2P ����
		{
			Attack( 2 );
		}
		else if( buf[0] == PLAYER2_ATTACK ) // 2P->1P ����
		{
			Attack( 1 );
		}
		else if( buf[0] == PLAYER1_LOSE ) // 1P ����
		{
			SendWin( 2 );
		}
		else if( buf[0] == PLAYER2_LOSE ) // 2P ����
		{
			SendWin( 1 );
		}
	}
	str.Format( "Receive : %d", nType );
	m_pList.AddString( str );
}

void CNetrisServerDlg::OnSend() 
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	char buf[200];
	ZeroMemory( buf, sizeof(buf) );

	Send( buf, strlen( buf ) );
}

void CNetrisServerDlg::Send(char *buf, int len)
{
	m_pPlayer1Sock->Send( buf, len );
	m_pPlayer2Sock->Send( buf, len );
}

void CNetrisServerDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( m_bPlayer1Enter == true && m_bPlayer2Enter == true )
	{
		m_pPlayer1Sock->Send( "S", 1 );
		m_pPlayer2Sock->Send( "S", 1 );
	}
}

void CNetrisServerDlg::Attack( int nPlayer )
{
	char buf[201];
	ZeroMemory( buf, sizeof( buf ) );
	
	if( nPlayer == 1 )
	{
		buf[0] = PLAYER2_ATTACK;
		m_pPlayer1Sock->Send( buf, 201 );
	}
	else if( nPlayer == 2 )
	{
		buf[0] = PLAYER1_ATTACK;
		m_pPlayer2Sock->Send( buf, 201 );
	}
}

void CNetrisServerDlg::SendWin( int nPlayer )
{
	char buf[201];
	ZeroMemory( buf, sizeof( buf ) );
	buf[0] = PLAYER_WIN;

	if( nPlayer == 1 )
	{
		m_pPlayer1Sock->Send( buf, 201 );
	}
	else if( nPlayer == 2 )
	{
		m_pPlayer2Sock->Send( buf, 201 );
	}
}

CString CNetrisServerDlg::GetIpAddress()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	CString strIpAddress = _T("");
	wVersionRequested = MAKEWORD(2, 0);

	if(WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if(gethostname(name, sizeof(name)) == 0)
		{
			if((hostinfo = gethostbyname(name)) != NULL)
				strIpAddress = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
		} 
		WSACleanup();
	}
	return strIpAddress;
}

CString CNetrisServerDlg::GetInIpAddress()
{
	WSADATA wsadata;
	CString strIP; // �� ������ IP�ּҰ� ����ȴ�.
	strIP = "";

	if( !WSAStartup( DESIRED_WINSOCK_VERSION, &wsadata ) )
	{
		if( wsadata.wVersion >= MINIMUM_WINSOCK_VERSION )
		{
			HOSTENT *p_host_info;
			IN_ADDR in;
			char host_name[128]={0, };

			gethostname(host_name, 128);
			p_host_info = gethostbyname( host_name );

			if( p_host_info != NULL )
			{
				for( int i = 0; p_host_info->h_addr_list[i]; i++ )
				{
					memcpy( &in, p_host_info->h_addr_list[i], 4 );
					strIP = inet_ntoa( in );
				}
			}
		}
		WSACleanup();
	}
	return strIP;
}

void CNetrisServerDlg::OnClose( CClientSock* pSock )
{
	if( pSock == m_pPlayer1Sock )
	{
		AfxMessageBox( "Player1 disconnect" );
		m_sttPlayer1.SetWindowTextA( _T("���� �ȵ�") );
		m_pPlayer1Sock = NULL;
		m_nPlayer = 1;
	}
	else if( pSock == m_pPlayer2Sock )
	{
		AfxMessageBox( "Player2 disconnect" );
		m_sttPlayer2.SetWindowTextA( _T("���� �ȵ�") );
		m_pPlayer2Sock = NULL;
		m_nPlayer = 2;
	}
}