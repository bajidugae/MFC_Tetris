
// NetrisClientView.cpp : CNetrisClientView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "NetrisClient.h"
#endif

#include "NetrisClientDoc.h"
#include "NetrisClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNetrisClientView

IMPLEMENT_DYNCREATE(CNetrisClientView, CView)

BEGIN_MESSAGE_MAP(CNetrisClientView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(25001, &CNetrisClientView::On25001)
	ON_MESSAGE(25002, &CNetrisClientView::On25002)
	ON_WM_DESTROY()
	ON_COMMAND(ID_32772, &CNetrisClientView::On32772)
	ON_COMMAND(ID_32771, &CNetrisClientView::On32771)
	ON_COMMAND(ID_32773, &CNetrisClientView::On32773)
	ON_COMMAND(ID_32774, &CNetrisClientView::On32774)
END_MESSAGE_MAP()

// CNetrisClientView 생성/소멸

CNetrisClientView::CNetrisClientView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	srand((unsigned)time(NULL));
	CString strPath;

	for( int i = 0 ; i < MAXWIDTH ; i ++ )
	{
		for( int j = 0 ; j < MAXHEIGHT ; j ++ )
		{
			m_nBoard[i][j] = 0;
			m_nEnemyBoard[i][j] = 0;
		}
	}

	for( int i = 0 ; i < 9 ; i ++ )
	{
		m_pBckImage[i] = new CImage();
		m_pBckEnemyImage[i] = new CImage();
		
		strPath.Format( _T( "%d.bmp" ), i); // 블럭이미지 경로(0~9)
		m_pBckImage[i]->Load( strPath );

		strPath.Format( _T( "e%d.bmp" ), i); // 적 블럭이미지 경로(0~9)
		m_pBckEnemyImage[i]->Load( strPath );		
	}

	for( int i = 0 ; i < 7 ; i ++ )
	{
		m_pNextBlock[i] = new CImage();
		m_pGhostImage[i] = new CImage();

		strPath.Format( _T( "g%d.bmp" ), i); // 고스트 블럭이미지 경로(0~9)
		m_pGhostImage[i]->Load( strPath );
	}
	m_pNextBlock[0]->Load( _T( "O.bmp" ) );
	m_pNextBlock[1]->Load( _T( "I.bmp" ) );
	m_pNextBlock[2]->Load( _T( "J.bmp" ) );
	m_pNextBlock[3]->Load( _T( "L.bmp" ) );
	m_pNextBlock[4]->Load( _T( "T.bmp" ) );
	m_pNextBlock[5]->Load( _T( "S.bmp" ) );
	m_pNextBlock[6]->Load( _T( "Z.bmp" ) );

	for( int i = 0 ; i < 4 ; i ++ )
	{
		m_nWaitBlock[i] = (rand()%7) + 1;
	}

	m_pBackground = new CImage();
	m_pBackground->Load( _T( "bg1.bmp" ) );

	for( int i = 0 ; i < 10 ; i ++ )
	{
		m_pNumber[i] = new CImage();
		
		strPath.Format( _T( "number%d.bmp" ), i); // 숫자이미지 경로(number0~9)
		m_pNumber[i]->Load( strPath );
	}

	m_pGameover = new CImage();
	m_pGameover->Load( _T( "gameover.bmp" ) );

	m_pWin = new CImage();
	m_pWin->Load( _T( "win.bmp" ) );

	m_pLose = new CImage();
	m_pLose->Load( _T( "lose.bmp" ) );

	for( int i = 0 ; i < 7 ; i ++ )
	{
		m_nScoreNum[i] = 0;
	}

	m_bGameOver = false;

	m_nScore = 0;
	m_nCombo = 0;
	m_bBeforeErase = 0;
	m_nStage = 0;

	m_nPlayer = 0;				// 내 플레이어 번호

	m_bIsEnemyLogin = false;	// 상대방이 접속했는지
	m_bIsSingle = false;		// 싱글인지
	m_bIsMulti = false;			// 멀티인지

	m_pClientSock = NULL;

	m_nGhostX = 0;
	m_nGhostY = 0;

	m_bGhostCell = false;
	m_bIsGhost = true;			// 고스트 보기
	m_bIsSound = true;			// 소리 듣기
	m_bIsWin = false;
}

CNetrisClientView::~CNetrisClientView()
{
	for( int i = 0 ; i < 9 ; i ++ )
	{
		delete m_pBckImage[i];
		delete m_pBckEnemyImage[i];
	}

	for( int i = 0 ; i < 7 ; i ++ )
	{
		delete m_pNextBlock[i];
		delete m_pGhostImage[i];
	}

	for( int i = 0 ; i < 10 ; i ++ )
		delete m_pNumber[i];

	delete m_pBackground;

	if( m_pClientSock != NULL)
		delete m_pClientSock;

	delete m_pGameover;
	delete m_pWin;
	delete m_pLose;
}

BOOL CNetrisClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CNetrisClientView 그리기

void CNetrisClientView::OnDraw(CDC* /*pDC*/)
{
	CNetrisClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CNetrisClientView 인쇄

BOOL CNetrisClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CNetrisClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CNetrisClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CNetrisClientView 진단

#ifdef _DEBUG
void CNetrisClientView::AssertValid() const
{
	CView::AssertValid();
}

void CNetrisClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNetrisClientDoc* CNetrisClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetrisClientDoc)));
	return (CNetrisClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CNetrisClientView 메시지 처리기


void CNetrisClientView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.

	// 텍스트 출력용 CString
	CString str;

	// 더블버퍼링
	CMDC pDC(&dc);
	
	// 배경
	m_pBackground->BitBlt( pDC.m_hDC, 0, 0 );

	// 보드 그리기
	for( int i = 0 ; i < MAXWIDTH ; i ++ )
	{
		for( int j = 0 ; j < MAXHEIGHT ; j ++ )
		{
			m_pBckImage[ m_nBoard[i][j] ]->BitBlt( pDC.m_hDC, i * 32 + 32, j * 32 + 32 );
			m_pBckEnemyImage[ m_nEnemyBoard[i][j] ]->BitBlt( pDC.m_hDC, i * 16 + 576, j * 16 + 352 );
		}
	}

	// 고스트 블록
	if( m_bIsGhost == true )
	{
		GhostCheckAndDraw( &pDC );
	}

	// 블록 그리기
	if( pOBlock.GetNow() == true )
	{
		for( int i = 0 ; i < 2 ; i ++ )
		{
			for( int j = 0 ; j < 2 ; j ++ )
			{
				m_pBckImage[ 1 ]->BitBlt( pDC.m_hDC, ( pOBlock.GetPosX() + i ) * 32 + 32, ( pOBlock.GetPosY() + j ) * 32 + 32 );				
			}
		}
	} 
	else if( pIBlock.GetNow() == true )
	{
		for( int i = 0 ; i < 4 ; i ++ )
		{
			for( int j = 0 ; j < 4 ; j ++ )
			{
				if( pIBlock.GetBlock( pIBlock.GetWay(), i, j ) == 2 )
				{
					m_pBckImage[ 2 ]->BitBlt( pDC.m_hDC, ( pIBlock.GetPosX() + i - 1 ) * 32 + 32, ( pIBlock.GetPosY() + j ) * 32 + 32 );					
				}
			}
		}
	}
	else if( pJBlock.GetNow() == true )
	{
		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pJBlock.GetBlock( pJBlock.GetWay(), i, j ) == 3 )
				{
					m_pBckImage[ 3 ]->BitBlt( pDC.m_hDC, ( pJBlock.GetPosX() + i ) * 32 + 32, ( pJBlock.GetPosY() + j ) * 32 + 32 );					
				}
			}
		}
	}
	else if( pLBlock.GetNow() == true )
	{
		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pLBlock.GetBlock( pLBlock.GetWay(), i, j ) == 4 )
				{
					m_pBckImage[ 4 ]->BitBlt( pDC.m_hDC, ( pLBlock.GetPosX() + i ) * 32 + 32, ( pLBlock.GetPosY() + j ) * 32 + 32 );					
				}
			}
		}
	}
	else if( pTBlock.GetNow() == true )
	{
		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pTBlock.GetBlock( pTBlock.GetWay(), i, j ) == 5 )
				{
					m_pBckImage[ 5 ]->BitBlt( pDC.m_hDC, ( pTBlock.GetPosX() + i ) * 32 + 32, ( pTBlock.GetPosY() + j ) * 32 + 32 );					
				}
			}
		}
	}
	else if( pSBlock.GetNow() == true )
	{
		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pSBlock.GetBlock( pSBlock.GetWay(), i, j ) == 6 )
				{
					m_pBckImage[ 6 ]->BitBlt( pDC.m_hDC, ( pSBlock.GetPosX() + i ) * 32 + 32, ( pSBlock.GetPosY() + j ) * 32 + 32 );					
				}
			}
		}
	}
	else if( pZBlock.GetNow() == true )
	{
		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pZBlock.GetBlock( pZBlock.GetWay(), i, j ) == 7 )
				{
					m_pBckImage[ 7 ]->BitBlt( pDC.m_hDC, ( pZBlock.GetPosX() + i ) * 32 + 32, ( pZBlock.GetPosY() + j ) * 32 + 32 );					
				}
			}
		}
	}


	// 다음 블록 이미지 출력
	for( int i = 0 ; i < 4 ; i ++ )
	{
		for( int j = 0 ; j < 7 ; j ++ )
		{
			if( m_nWaitBlock[i] == j + 1 )
			{
				m_pNextBlock[j]->BitBlt( pDC.m_hDC, 384, i * 160 + 32 + 32 );
			}
		}
	}

	// 숫자(점수) 이미지 출력
	for( int i = 6 ; i >= 0 ; i -- )
	{
		m_pNumber[ m_nScoreNum[i] ]->BitBlt( pDC.m_hDC, 510 - ( i * 21 ) + 8, 0 );
	}

	// 게임 오버 시
	if( m_bGameOver == true )
	{
		if( m_bIsSingle == true ) // 싱글일때
		{
			m_pGameover->BitBlt( pDC.m_hDC, 234, 246 );
		}		
		if( m_bIsMulti == true ) // 멀티일때
		{
			m_pLose->BitBlt( pDC.m_hDC, 234, 246 );
			SendLose();
		}
	}

	// 이겼을때
	if( m_bIsWin == true )
	{
		m_pWin->BitBlt( pDC.m_hDC, 234, 246 );
	}
}


void CNetrisClientView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( m_bIsEnemyLogin == true || m_bIsSingle == true )
	{
		if( m_bIsSound == true )
			PlaySound( _T("effect.wav"), NULL, SND_ASYNC);

		if( nChar == VK_LEFT )
		{
			switch( m_nNowBlock )
			{
			case 1:
				pOBlock.MoveLeft();
				break;
			case 2:
				pIBlock.MoveLeft();
				break;
			case 3:
				pJBlock.MoveLeft();
				break;
			case 4:
				pLBlock.MoveLeft();
				break;
			case 5:
				pTBlock.MoveLeft();
				break;
			case 6:
				pSBlock.MoveLeft();
				break;
			case 7:
				pZBlock.MoveLeft();
				break;
			}			
		}
		else if( nChar == VK_RIGHT )
		{
			switch( m_nNowBlock )
			{
			case 1:
				pOBlock.MoveRight();
				break;
			case 2:
				pIBlock.MoveRight();
				break;
			case 3:
				pJBlock.MoveRight();
				break;
			case 4:
				pLBlock.MoveRight();
				break;
			case 5:
				pTBlock.MoveRight();
				break;
			case 6:
				pSBlock.MoveRight();
				break;
			case 7:
				pZBlock.MoveRight();
				break;
			}		
		}
		else if( nChar == VK_DOWN )
		{
			switch( m_nNowBlock )
			{
			case 1:
				pOBlock.MoveDown();
				break;
			case 2:
				pIBlock.MoveDown();
				break;
			case 3:
				pJBlock.MoveDown();
				break;
			case 4:
				pLBlock.MoveDown();
				break;
			case 5:
				pTBlock.MoveDown();
				break;
			case 6:
				pSBlock.MoveDown();
				break;
			case 7:
				pZBlock.MoveDown();
				break;
			}	
		}
		else if( nChar == VK_UP )
		{
			switch( m_nNowBlock )
			{
			case 1:
				break;
			case 2:
				pIBlock.Turn();
				break;
			case 3:
				pJBlock.Turn();
				break;
			case 4:
				pLBlock.Turn();
				break;
			case 5:
				pTBlock.Turn();
				break;
			case 6:
				pSBlock.Turn();
				break;
			case 7:
				pZBlock.Turn();
				break;
			}	
		}
		else if( nChar == VK_SPACE )
		{
			switch( m_nNowBlock )
			{
			case 1:
				while( 1 )
				{
					if( pOBlock.MoveDown() == true )
						break;
				}
				break;
			case 2:
				while( 1 )
				{
					if( pIBlock.MoveDown() == true )
						break;
				}
				break;
			case 3:
				while( 1 )
				{
					if( pJBlock.MoveDown() == true )
						break;
				}
				break;
			case 4:
				while( 1 )
				{
					if( pLBlock.MoveDown() == true )
						break;
				}
				break;
			case 5:
				while( 1 )
				{
					if( pTBlock.MoveDown() == true )
						break;
				}
				break;
			case 6:
				while( 1 )
				{
					if( pSBlock.MoveDown() == true )
						break;
				}
				break;
			case 7:
				while( 1 )
				{
					if( pZBlock.MoveDown() == true )
						break;
				}
				break;
			}
		}
		
		// 고스트 X좌표
		pG_OBlock.SetPosX( pOBlock.GetPosX() );
		pG_IBlock.SetPosX( pIBlock.GetPosX() );
		pG_JBlock.SetPosX( pJBlock.GetPosX() );
		pG_LBlock.SetPosX( pLBlock.GetPosX() );
		pG_TBlock.SetPosX( pTBlock.GetPosX() );
		pG_SBlock.SetPosX( pSBlock.GetPosX() );
		pG_ZBlock.SetPosX( pZBlock.GetPosX() );
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNetrisClientView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( nIDEvent == 1 )
	{
		if( m_bIsMulti == true )
			OnSend();
		if( m_bIsEnemyLogin == true || m_bIsSingle == true )
		{
			if( m_bGameOver == false )
			{
				if( pOBlock.GetNow() == true )
				{
					if( pOBlock.MoveDown() == true )
					{
						for( int i = 0 ; i < 2 ; i ++ )
						{
							for( int j = 0 ; j < 2 ; j ++ )
							{
								m_nBoard[ pOBlock.GetPosX() + i ][ pOBlock.GetPosY() + j ] = 1;
							}
						}
						pOBlock.Reset();
						ResetGhostBlock();
					}
				}
				else if( pIBlock.GetNow() == true )
				{
					if( pIBlock.MoveDown() == true )
					{
						for( int i = 0 ; i < 4 ; i ++ )
						{
							for( int j = 0 ; j < 4 ; j ++ )
							{
								if( pIBlock.GetBlock( pIBlock.GetWay(), i, j ) == 2 )
									m_nBoard[ pIBlock.GetPosX() + i - 1 ][ pIBlock.GetPosY() + j ] = 2;
							}
						}
						pIBlock.Reset();
						ResetGhostBlock();
					}
				}
				else if( pJBlock.GetNow() == true )
				{
					if( pJBlock.MoveDown() == true )
					{
						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pJBlock.GetBlock( pJBlock.GetWay(), i, j ) == 3 )
									m_nBoard[ pJBlock.GetPosX() + i ][ pJBlock.GetPosY() + j ] = 3;
							}
						}
						pJBlock.Reset();
						ResetGhostBlock();
					}
				}
				else if( pLBlock.GetNow() == true )
				{
					if( pLBlock.MoveDown() == true )
					{
						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pLBlock.GetBlock( pLBlock.GetWay(), i, j ) == 4 )
									m_nBoard[ pLBlock.GetPosX() + i ][ pLBlock.GetPosY() + j ] = 4;
							}
						}
						pLBlock.Reset();
						ResetGhostBlock();
					}
				}
				else if( pTBlock.GetNow() == true )
				{
					if( pTBlock.MoveDown() == true )
					{
						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pTBlock.GetBlock( pTBlock.GetWay(), i, j ) == 5 )
									m_nBoard[ pTBlock.GetPosX() + i ][ pTBlock.GetPosY() + j ] = 5;
							}
						}
						pTBlock.Reset();
						ResetGhostBlock();
					}
				}
				else if( pSBlock.GetNow() == true )
				{
					if( pSBlock.MoveDown() == true )
					{
						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pSBlock.GetBlock( pSBlock.GetWay(), i, j ) == 6 )
									m_nBoard[ pSBlock.GetPosX() + i ][ pSBlock.GetPosY() + j ] = 6;
							}
						}
						pSBlock.Reset();
						ResetGhostBlock();
					}
				}
				else if( pZBlock.GetNow() == true )
				{
					if( pZBlock.MoveDown() == true )
					{
						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pZBlock.GetBlock( pZBlock.GetWay(), i, j ) == 7 )
									m_nBoard[ pZBlock.GetPosX() + i ][ pZBlock.GetPosY() + j ] = 7;
							}
						}
						pZBlock.Reset();
						ResetGhostBlock();
					}
				}
				else
				{
					m_nNowBlock = m_nWaitBlock[0]; // 대기열 첫번째 테트리미노를 현재 테트리미노로
					for( int i = 0 ; i < 3 ; i ++ )
					{
						m_nWaitBlock[i] = m_nWaitBlock[i+1]; // 앞 쪽으로 땡기고
					}
					m_nWaitBlock[3] = (rand()%7) + 1; // 마지막 랜덤

					switch( m_nNowBlock )
					{
					case 1:
						pOBlock.Reset();
						pOBlock.SetNow( true );

						for( int i = 0 ; i < 2 ; i ++ )
						{
							for( int j = 0 ; j < 2 ; j ++ )
							{
								if( m_nBoard[ pOBlock.GetPosX()+i ][ pOBlock.GetPosY()+j ] != 0 )
								{
									m_bGameOver = true;
								}
							}
						}

						break;
					case 2:
						pIBlock.Reset();
						pIBlock.SetNow( true );

						for( int i = 0 ; i < 4 ; i ++ )
						{
							for( int j = 0 ; j < 4 ; j ++ )
							{
								if( pIBlock.GetBlock( pIBlock.GetWay(), i, j ) == 2 &&
									m_nBoard[ pIBlock.GetPosX()+i ][ pIBlock.GetPosY()+j ] != 0 )
								{
									m_bGameOver = true;
								}
							}
						}

						break;
					case 3:
						pJBlock.Reset();
						pJBlock.SetNow( true );

						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pJBlock.GetBlock( pJBlock.GetWay(), i, j ) == 3 &&
									m_nBoard[ pJBlock.GetPosX()+i ][ pJBlock.GetPosY()+j ] != 0 )
								{
									m_bGameOver = true;
								}
							}
						}

						break;
					case 4:
						pLBlock.Reset();
						pLBlock.SetNow( true );

						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pLBlock.GetBlock( pLBlock.GetWay(), i, j ) == 4 &&
									m_nBoard[ pLBlock.GetPosX()+i ][ pLBlock.GetPosY()+j ] != 0 )
								{
									m_bGameOver = true;
								}
							}
						}

						break;
					case 5:
						pTBlock.Reset();
						pTBlock.SetNow( true );

						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pTBlock.GetBlock( pTBlock.GetWay(), i, j ) == 5 &&
									m_nBoard[ pTBlock.GetPosX()+i ][ pTBlock.GetPosY()+j ] != 0 )
								{
									m_bGameOver = true;
								}
							}
						}

						break;
					case 6:
						pSBlock.Reset();
						pSBlock.SetNow( true );

						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pSBlock.GetBlock( pSBlock.GetWay(), i, j ) == 6 &&
									m_nBoard[ pSBlock.GetPosX()+i ][ pSBlock.GetPosY()+j ] != 0 )
								{
									m_bGameOver = true;
								}
							}
						}

						break;
					case 7:
						pZBlock.Reset();
						pZBlock.SetNow( true );

						for( int i = 0 ; i < 3 ; i ++ )
						{
							for( int j = 0 ; j < 3 ; j ++ )
							{
								if( pZBlock.GetBlock( pZBlock.GetWay(), i, j ) == 6 &&
									m_nBoard[ pZBlock.GetPosX()+i ][ pZBlock.GetPosY()+j ] != 0 )
								{
									m_bGameOver = true;
								}
							}
						}

						break;
					}
				}

				int nLineClear = 0;

				for( int j = 0 ; j < MAXHEIGHT ; j ++ )
				{
					for( int i = 0 ; i < MAXWIDTH ; i ++ )
					{
						if( m_nBoard[i][j] != 0 )
						{
							nLineClear++;
						}
					}

					if( nLineClear == 10 )
					{
						for( int k = 0 ; k < MAXWIDTH ; k ++ )
						{
							m_nBoard[k][j] = 0;
						}

						int nClearHeight = j;

						for( int m = j ; m > 0 ; m -- )
						{
							for( int n = 0 ; n < MAXWIDTH ; n ++ )
							{
								m_nBoard[n][m] = m_nBoard[n][m - 1];
							}

						}

						m_bBeforeErase = true; // 콤보용 bool값

						m_nScore+=10; // 점수+
						
						if( m_bIsMulti == true )
							SendAttack();

						int nTemp = m_nScore;

						for( int i = 0 ; i < 7 ; i ++ )
						{               
							m_nScoreNum[i] = nTemp % 10; // 10으로 나눠서 나머지를 sum에 입력
							nTemp = nTemp /10; //맨 뒤에 자리수를 없앤다
						}

						m_nStage++;
						SetTimer( 1, 500 - ( m_nStage * 10 ), NULL );

						Invalidate(); // 화면 갱신
					}
					else
						nLineClear = 0;
				}
				// 줄 지우기
			}
		}
	}

	if( nIDEvent == 2 )
	{
		Invalidate();
		// 1초에 50프레임 갱신
	}

	CView::OnTimer(nIDEvent);
}


void CNetrisClientView::OnInitialUpdate()
{
	CView::OnInitialUpdate();


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SetTimer( 1, 500, NULL ); // 블럭 떨어지는 타이머 등
	SetTimer( 2, 20, NULL ); // 화면 갱신용 타이머

	mciOpen.lpstrElementName = _T( "tetris.mp3" ); // 파일 경로 입력
	mciOpen.lpstrDeviceType = _T( "mpegvideo" );

	mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,
		(DWORD)(LPVOID)&mciOpen);

	dwID = mciOpen.wDeviceID;

	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
		(DWORD)(LPVOID)&m_mciPlayParms);

	//
	//mciSendString( _T("bgm.wav"),NULL,0,NULL);
	 
}


BOOL CNetrisClientView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return true;
	//return CView::OnEraseBkgnd(pDC);
}


afx_msg LRESULT CNetrisClientView::On25001(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

afx_msg LRESULT CNetrisClientView::On25002(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CNetrisClientView::OnReceive()
{
	char p[2];
	char st[1];
	char buf[201];
	//ZeroMemory( buf, sizeof(buf) );
	int nNum = 1;

	if( m_nPlayer == 0 )
	{
		m_pClientSock->Receive(p,2);
		m_nPlayer = atoi(p) / 10;
		CString str;
		str.Format( _T("당신은 %d플레이어입니다."), m_nPlayer );
		AfxMessageBox( str );
	}
	else if( m_nPlayer > 0 )
	{
		if( m_bIsEnemyLogin == false )
		{
			m_pClientSock->Receive( st,1 );

			if( st[0] == 'S' )
				m_bIsEnemyLogin = true;
		}
		if( m_bIsEnemyLogin == true )
		{
			m_pClientSock->Receive(buf,201);

			if( ( buf[0] == PLAYER1_BOARD && m_nPlayer == 2 ) ||
				( buf[0] == PLAYER2_BOARD && m_nPlayer == 1 ) )
			{
				for( int i = 0 ; i < MAXHEIGHT ; i ++ )
				{
					for( int j = 0 ; j < MAXWIDTH ; j ++ )
					{
						if( buf[nNum] != -52 )
							m_nEnemyBoard[j][i] = buf[nNum];
						nNum++;
					}
				}
			}
			else if( ( buf[0] == PLAYER1_ATTACK && m_nPlayer == 2 ) ||
					( buf[0] == PLAYER2_ATTACK && m_nPlayer == 1 ) )
			{
				AddLine();
			}
			else if( buf[0] == PLAYER_WIN )
			{
				// 이김
				m_bIsWin = true;
				m_bIsEnemyLogin = false;				
			}
		}
	}
}

void CNetrisClientView::OnSend()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	if( m_bIsEnemyLogin == true )
	{
		char buf[ 201 ];
		ZeroMemory( buf, sizeof( buf ) );
		int nNum = 1;
		buf[ 0 ] = m_nPlayer;

		for( int i = 0 ; i < MAXHEIGHT ; i ++ )
		{
			for( int j = 0 ; j < MAXWIDTH ; j ++ )
			{
				buf[ nNum ] = m_nBoard[ j ][ i ];
				nNum++;
			}
		}

		int nErr = m_pClientSock->Send( buf, 201 );
		if( nErr == SOCKET_ERROR )
		{
			int err = m_pClientSock->GetLastError();
		}
	}
}
void CNetrisClientView::OnDestroy()
{
	CView::OnDestroy();
}

void CNetrisClientView::AddLine()
{
	int nArray[ 10 ];
	for( int i = 0 ; i < 10 ; i ++ )
	{
		nArray[ i ] = 8;
	}
	nArray[ rand()%10 ] = 0;

	for( int i = 0 ; i < MAXWIDTH ; i ++ )
	{
		for( int j = 1 ; j < MAXHEIGHT ; j ++ )
		{
			m_nBoard[ i ][ j - 1 ] = m_nBoard[ i ][ j ];
		}
	}

	for( int i = 0 ; i < MAXWIDTH ; i ++ )
	{
		m_nBoard[ i ][ 19 ] = nArray[ i ];
	}
}

void CNetrisClientView::SendAttack()
{
	char buf[201];
	ZeroMemory( buf, sizeof( buf ) );

	if( m_nPlayer == 1 )
	{
		buf[0] = 3;
	}
	else if( m_nPlayer == 2 )
	{
		buf[0] = 4;
	}

	m_pClientSock->Send( buf, 201 );
}

void CNetrisClientView::On32772()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 메뉴->멀티플레이
	CIPDlg m_pIPDlg;
	m_pIPDlg.DoModal();
}

void CNetrisClientView::Connect()
{
	m_pClientSock = new CClientSock();
	m_pClientSock->Create();
	if( m_pClientSock->Connect( m_strIP, 700 ) == false)
		int err = m_pClientSock->GetLastError();
}

void CNetrisClientView::SendLose()
{
	char buf[201];
	ZeroMemory( buf, sizeof( buf ) );

	if( m_nPlayer == 1 )
	{
		buf[0] = 5;
	}
	else if( m_nPlayer == 2 )
	{
		buf[0] = 6;
	}

	m_pClientSock->Send( buf, 201 );
}

void CNetrisClientView::On32771()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	ResetGame();
	m_bIsSingle = true;
	m_bIsMulti = false;
	m_bIsWin = false;
}

void CNetrisClientView::ResetGame()
{
	for( int i = 0 ; i < MAXWIDTH ; i ++ )
	{
		for( int j = 0 ; j < MAXHEIGHT ; j ++ )
		{
			m_nBoard[i][j] = 0;
			m_nEnemyBoard[i][j] = 0;
		}
	}

	m_bIsMulti = false;
	m_bIsSingle = false;
	m_bIsEnemyLogin = false;
	m_bGameOver = false;

	m_nScore = 0;
	SetTimer( 1, 500, NULL );

	pIBlock.SetNow( false );
	pIBlock.Reset();
	pJBlock.SetNow( false );
	pJBlock.Reset();
	pLBlock.SetNow( false );
	pLBlock.Reset();
	pOBlock.SetNow( false );
	pOBlock.Reset();
	pSBlock.SetNow( false );
	pSBlock.Reset();
	pTBlock.SetNow( false );
	pTBlock.Reset();
	pZBlock.SetNow( false );
	pZBlock.Reset();

	ResetGhostBlock();
}

void CNetrisClientView::GhostCheckAndDraw( CMDC * pDC )
{
	m_bGhostCell = false;		// 바닥 체크용

	if( pOBlock.GetNow() == true )
	{
		for( int k = 0 ; k < MAXHEIGHT ; k ++ )
		{
			if( m_nBoard[ pOBlock.GetPosX() ][ k + 2 ] != 0 ||
				m_nBoard[ pOBlock.GetPosX() + 1 ][ k + 2 ] != 0 )
			{
				m_bGhostCell = false;
				m_nGhostY = k;
				break;
			}
			else
				m_bGhostCell = true;
		}

		if( m_nBoard[ pOBlock.GetPosX() ][ 18 ] == 0 &&
			m_nBoard[ pOBlock.GetPosX() + 1 ][ 18 ] == 0 &&
			m_bGhostCell == true )
		{
			m_nGhostY = 18;
		}

		for( int i = 0 ; i < 2 ; i ++ )
		{
			for( int j = 0 ; j < 2 ; j ++ )
			{
				m_pGhostImage[ 0 ]->BitBlt( pDC->m_hDC, ( pOBlock.GetPosX() + i ) * 32 + 32, ( m_nGhostY + j ) * 32 + 32 );				
			}
		}
	} 
	else if( pIBlock.GetNow() == true )
	{
		for( int k = 0 ; k < MAXHEIGHT ; k ++ )
		{
			if( pIBlock.GetWay() == 0 )
			{
				if( m_nBoard[ pIBlock.GetPosX() ][ k + 4 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pIBlock.GetWay() == 1 )
			{
				if( m_nBoard[ pIBlock.GetPosX()-1 ][ k + 3 ] != 0 ||
					m_nBoard[ pIBlock.GetPosX() ][ k + 3 ] != 0 ||
					m_nBoard[ pIBlock.GetPosX()+1 ][ k + 3 ] != 0 ||
					m_nBoard[ pIBlock.GetPosX()+2 ][ k + 3 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
		}

		if( pIBlock.GetWay() == 0 )
		{
			if( m_nBoard[ pIBlock.GetPosX() ][ 16 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 16;
			}
		}
		else if( pIBlock.GetWay() == 1 )
		{
			if( m_nBoard[ pIBlock.GetPosX() - 1 ][ 17 ] == 0 &&
				m_nBoard[ pIBlock.GetPosX() ][ 17 ] == 0 &&
				m_nBoard[ pIBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_nBoard[ pIBlock.GetPosX() + 2 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}

		for( int i = 0 ; i < 4 ; i ++ )
		{
			for( int j = 0 ; j < 4 ; j ++ )
			{
				if( pIBlock.GetBlock( pIBlock.GetWay(), i, j ) == 2 )
				{
					m_pGhostImage[ 1 ]->BitBlt( pDC->m_hDC, ( pG_IBlock.GetPosX() + i - 1 ) * 32 + 32, ( m_nGhostY + j ) * 32 + 32 );
				}
			}
		}
	}
	else if( pJBlock.GetNow() == true )
	{
		for( int k = 0 ; k < MAXHEIGHT ; k ++ )
		{
			if( pJBlock.GetWay() == 0 )
			{
				if( m_nBoard[ pJBlock.GetPosX() + 2 ][ k + 3 ] != 0 ||
					m_nBoard[ pJBlock.GetPosX() ][ k + 2 ] != 0 ||
					m_nBoard[ pJBlock.GetPosX() + 1 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pJBlock.GetWay() == 1 )
			{
				if( m_nBoard[ pJBlock.GetPosX() ][ k + 3 ] != 0 ||
					m_nBoard[ pJBlock.GetPosX() + 1 ][ k + 3 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pJBlock.GetWay() == 2 )
			{
				if( m_nBoard[ pJBlock.GetPosX() ][ k + 2 ] != 0 ||
					m_nBoard[ pJBlock.GetPosX() + 1 ][ k + 2 ] != 0 ||
					m_nBoard[ pJBlock.GetPosX() + 2 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pJBlock.GetWay() == 3 )
			{
				if( m_nBoard[ pJBlock.GetPosX() + 1 ][ k + 3 ] != 0 ||
					m_nBoard[ pJBlock.GetPosX() + 2 ][ k + 1 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
		}

		if( pJBlock.GetWay() == 0 )
		{
			if( m_nBoard[ pJBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pJBlock.GetWay() == 1 )
		{
			if( m_nBoard[ pJBlock.GetPosX() ][ 17 ] == 0 &&
				m_nBoard[ pJBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pJBlock.GetWay() == 2 )
		{
			if( m_nBoard[ pJBlock.GetPosX() ][ 18 ] == 0 &&
				m_nBoard[ pJBlock.GetPosX() + 1 ][ 18 ] == 0 &&
				m_nBoard[ pJBlock.GetPosX() + 2 ][ 18 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 18;
			}
		}
		else if( pJBlock.GetWay() == 3 )
		{
			if( m_nBoard[ pJBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}

		if( m_nGhostY > 17 && pJBlock.GetWay() == 3 )
			m_nGhostY = 17;

		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pJBlock.GetBlock( pJBlock.GetWay(), i, j ) == 3 )
				{
					m_pGhostImage[ 2 ]->BitBlt( pDC->m_hDC, ( pG_JBlock.GetPosX() + i ) * 32 + 32, ( m_nGhostY + j ) * 32 + 32 );
				}
			}
		}
	}
	else if( pLBlock.GetNow() == true )
	{
		for( int k = 0 ; k < MAXHEIGHT ; k ++ )
		{
			if( pLBlock.GetWay() == 0 )
			{
				if( m_nBoard[ pLBlock.GetPosX() ][ k + 3 ] != 0 ||
					m_nBoard[ pLBlock.GetPosX() + 1 ][ k + 2 ] != 0 ||
					m_nBoard[ pLBlock.GetPosX() + 2 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pLBlock.GetWay() == 1 )
			{
				if( m_nBoard[ pLBlock.GetPosX() ][ k + 1 ] != 0 ||
					m_nBoard[ pLBlock.GetPosX() + 1 ][ k + 3 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pLBlock.GetWay() == 2 )
			{
				if( m_nBoard[ pLBlock.GetPosX() ][ k + 2 ] != 0 ||
					m_nBoard[ pLBlock.GetPosX() + 1 ][ k + 2 ] != 0 ||
					m_nBoard[ pLBlock.GetPosX() + 2 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pLBlock.GetWay() == 3 )
			{
				if( m_nBoard[ pLBlock.GetPosX() + 1 ][ k + 3 ] != 0 ||
					m_nBoard[ pLBlock.GetPosX() + 2 ][ k + 3 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
		}

		if( pLBlock.GetWay() == 0 )
		{
			if( m_nBoard[ pLBlock.GetPosX() ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pLBlock.GetWay() == 1 )
		{
			if( m_nBoard[ pLBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pLBlock.GetWay() == 2 )
		{
			if( m_nBoard[ pLBlock.GetPosX() ][ 18 ] == 0 &&
				m_nBoard[ pLBlock.GetPosX() + 1 ][ 18 ] == 0 &&
				m_nBoard[ pLBlock.GetPosX() + 2 ][ 18 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 18;
			}
		}
		else if( pLBlock.GetWay() == 3 )
		{
			if( m_nBoard[ pLBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_nBoard[ pLBlock.GetPosX() + 2 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}

		if( m_nGhostY > 17 && pLBlock.GetWay() == 1 )
			m_nGhostY = 17;

		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pLBlock.GetBlock( pLBlock.GetWay(), i, j ) == 4 )
				{
					m_pGhostImage[ 3 ]->BitBlt( pDC->m_hDC, ( pG_LBlock.GetPosX() + i ) * 32 + 32, ( m_nGhostY + j ) * 32 + 32 );
				}
			}
		}
	}
	else if( pTBlock.GetNow() == true )
	{
		for( int k = 0 ; k < MAXHEIGHT ; k ++ )
		{
			if( pTBlock.GetWay() == 0 )
			{
				if( m_nBoard[ pTBlock.GetPosX() ][ k + 2 ] != 0 ||
					m_nBoard[ pTBlock.GetPosX() + 1 ][ k + 3 ] != 0 ||
					m_nBoard[ pTBlock.GetPosX() + 2 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pTBlock.GetWay() == 1 )
			{
				if( m_nBoard[ pTBlock.GetPosX() ][ k + 2 ] != 0 ||
					m_nBoard[ pTBlock.GetPosX() + 1 ][ k + 3 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pTBlock.GetWay() == 2 )
			{
				if( m_nBoard[ pTBlock.GetPosX() ][ k + 2 ] != 0 ||
					m_nBoard[ pTBlock.GetPosX() + 1 ][ k + 2 ] != 0 ||
					m_nBoard[ pTBlock.GetPosX() + 2 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pTBlock.GetWay() == 3 )
			{
				if( m_nBoard[ pTBlock.GetPosX() + 1 ][ k + 3 ] != 0 ||
					m_nBoard[ pTBlock.GetPosX() + 2 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
		}

		if( pTBlock.GetWay() == 0 )
		{
			if( m_nBoard[ pTBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pTBlock.GetWay() == 1 )
		{
			if( m_nBoard[ pTBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pTBlock.GetWay() == 2 )
		{
			if( m_nBoard[ pTBlock.GetPosX() ][ 18 ] == 0 &&
				m_nBoard[ pTBlock.GetPosX() + 1 ][ 18 ] == 0 &&
				m_nBoard[ pTBlock.GetPosX() + 2 ][ 18 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 18;
			}
		}
		else if( pTBlock.GetWay() == 3 )
		{
			if( m_nBoard[ pTBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}

		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pTBlock.GetBlock( pTBlock.GetWay(), i, j ) == 5 )
				{
					m_pGhostImage[ 4 ]->BitBlt( pDC->m_hDC, ( pG_TBlock.GetPosX() + i ) * 32 + 32, ( m_nGhostY + j ) * 32 + 32 );
				}
			}
		}
	}
	else if( pSBlock.GetNow() == true )
	{
		for( int k = 0 ; k < MAXHEIGHT ; k ++ )
		{
			if( pSBlock.GetWay() == 0 )
			{
				if( m_nBoard[ pSBlock.GetPosX() ][ k + 3 ] != 0 ||
					m_nBoard[ pSBlock.GetPosX() + 1 ][ k + 3 ] != 0 ||
					m_nBoard[ pSBlock.GetPosX() + 2 ][ k + 2 ] != 0  )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pSBlock.GetWay() == 1 )
			{
				if( m_nBoard[ pSBlock.GetPosX() + 1][ k + 2 ] != 0 ||
					m_nBoard[ pSBlock.GetPosX() + 2 ][ k + 3 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
		}

		if( pSBlock.GetWay() == 0 )
		{
			if( m_nBoard[ pSBlock.GetPosX() ][ 17 ] == 0 &&
				m_nBoard[ pSBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pSBlock.GetWay() == 1 )
		{
			if( m_nBoard[ pSBlock.GetPosX() + 2 ][ 17 ] &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}

		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pSBlock.GetBlock( pSBlock.GetWay(), i, j ) == 6 )
				{
					m_pGhostImage[ 5 ]->BitBlt( pDC->m_hDC, ( pG_SBlock.GetPosX() + i ) * 32 + 32, ( m_nGhostY + j ) * 32 + 32 );
				}
			}
		}
	}
	else if( pZBlock.GetNow() == true )
	{
		for( int k = 0 ; k < MAXHEIGHT ; k ++ )
		{
			if( pZBlock.GetWay() == 0 )
			{
				if( m_nBoard[ pZBlock.GetPosX() ][ k + 2 ] != 0 ||
					m_nBoard[ pZBlock.GetPosX() + 1 ][ k + 3 ] != 0 ||
					m_nBoard[ pZBlock.GetPosX() + 2 ][ k + 3 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
			else if( pZBlock.GetWay() == 1 )
			{
				if( m_nBoard[ pZBlock.GetPosX() + 1 ][ k + 3 ] != 0 ||
					m_nBoard[ pZBlock.GetPosX() + 2 ][ k + 2 ] != 0 )
				{
					m_bGhostCell = false;
					m_nGhostY = k;
					break;
				}
				else
					m_bGhostCell = true;
			}
		}

		if( pSBlock.GetWay() == 0 )
		{
			if( m_nBoard[ pSBlock.GetPosX() + 1 ][ 17 ] == 0 &&
				m_nBoard[ pSBlock.GetPosX() + 2 ][ 17 ] == 0 &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}
		else if( pSBlock.GetWay() == 1 )
		{
			if( m_nBoard[ pSBlock.GetPosX() + 1 ][ 17 ] &&
				m_bGhostCell == true )
			{
				m_nGhostY = 17;
			}
		}

		for( int i = 0 ; i < 3 ; i ++ )
		{
			for( int j = 0 ; j < 3 ; j ++ )
			{
				if( pZBlock.GetBlock( pZBlock.GetWay(), i, j ) == 7 )
				{
					m_pGhostImage[ 6 ]->BitBlt( pDC->m_hDC, ( pG_ZBlock.GetPosX() + i ) * 32 + 32, ( m_nGhostY + j ) * 32 + 32 );
				}
			}
		}
	}
}

void CNetrisClientView::ResetGhostBlock()
{
	pG_OBlock.Reset();
	pG_IBlock.Reset();
	pG_JBlock.Reset();
	pG_LBlock.Reset();
	pG_TBlock.Reset();
	pG_SBlock.Reset();
	pG_ZBlock.Reset();
}

void CNetrisClientView::On32773()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 고스트 메뉴
	if( m_bIsGhost == true )
	{
		m_bIsGhost = false;
	}
	else if( m_bIsGhost == false )
	{
		m_bIsGhost = true;
	}
}


void CNetrisClientView::On32774()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 소리 메뉴
	if( m_bIsSound == true )
	{
		m_bIsSound = false;
		mciSendCommand( dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD)(LPVOID)&m_mciPlayParms);
	}
	else if( m_bIsSound == false )
	{
		m_bIsSound = true;
		mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&m_mciPlayParms);
	}
}
