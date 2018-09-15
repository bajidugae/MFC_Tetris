
// NetrisServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "NetrisServer.h"
#include "NetrisServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CNetrisServerDlg 대화 상자




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


// CNetrisServerDlg 메시지 처리기

BOOL CNetrisServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
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

	m_nPlayer = 1; // 1플레이어부터

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNetrisServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
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
		m_pList.AddString( _T("플레이어1 연결됨") );
		m_sttPlayer1.SetWindowTextA( _T("접속 중") );
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
		m_pList.AddString( _T("플레이어2 연결됨") );
		m_sttPlayer2.SetWindowTextA( _T("접속 중") );
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
		m_pList.AddString( _T("풀방") );
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
		if( buf[0] == PLAYER1_BOARD ) // 1P보드->2P 전송
		{
			m_pPlayer2Sock->Send( buf, 201 );
		}
		else if( buf[0] == PLAYER2_BOARD ) // 2P보드->1P 전송
		{
			m_pPlayer1Sock->Send( buf, 201 );
		}
		else if( buf[0] == PLAYER1_ATTACK ) // 1P->2P 공격
		{
			Attack( 2 );
		}
		else if( buf[0] == PLAYER2_ATTACK ) // 2P->1P 공격
		{
			Attack( 1 );
		}
		else if( buf[0] == PLAYER1_LOSE ) // 1P 죽음
		{
			SendWin( 2 );
		}
		else if( buf[0] == PLAYER2_LOSE ) // 2P 죽음
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	CString strIP; // 이 변수에 IP주소가 저장된다.
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
		m_sttPlayer1.SetWindowTextA( _T("접속 안됨") );
		m_pPlayer1Sock = NULL;
		m_nPlayer = 1;
	}
	else if( pSock == m_pPlayer2Sock )
	{
		AfxMessageBox( "Player2 disconnect" );
		m_sttPlayer2.SetWindowTextA( _T("접속 안됨") );
		m_pPlayer2Sock = NULL;
		m_nPlayer = 2;
	}
}