#include "stdafx.h"
#include "MainFrm.h"
#include "NetrisClientDoc.h"
#include "NetrisClientView.h"
#include "CITetrimino.h"

CITetrimino::CITetrimino()
{
	m_nBlock[0][0][0] = 0; m_nBlock[0][1][0] = 2; m_nBlock[0][2][0] = 0; m_nBlock[0][3][0] = 0;
	m_nBlock[0][0][1] = 0; m_nBlock[0][1][1] = 2; m_nBlock[0][2][1] = 0; m_nBlock[0][3][1] = 0;
	m_nBlock[0][0][2] = 0; m_nBlock[0][1][2] = 2; m_nBlock[0][2][2] = 0; m_nBlock[0][3][2] = 0;
	m_nBlock[0][0][3] = 0; m_nBlock[0][1][3] = 2; m_nBlock[0][2][3] = 0; m_nBlock[0][3][3] = 0;

	m_nBlock[1][0][0] = 0; m_nBlock[1][1][0] = 0; m_nBlock[1][2][0] = 0; m_nBlock[1][3][0] = 0;
	m_nBlock[1][0][1] = 0; m_nBlock[1][1][1] = 0; m_nBlock[1][2][1] = 0; m_nBlock[1][3][1] = 0;
	m_nBlock[1][0][2] = 2; m_nBlock[1][1][2] = 2; m_nBlock[1][2][2] = 2; m_nBlock[1][3][2] = 2;
	m_nBlock[1][0][3] = 0; m_nBlock[1][1][3] = 0; m_nBlock[1][2][3] = 0; m_nBlock[1][3][3] = 0;

	m_nX = 4;
	m_nY = 0;
	m_nWay = 0;
}

CITetrimino::~CITetrimino()
{
}

void CITetrimino::GetBlock()
{
}

bool CITetrimino::MoveDown()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 && m_nY < 16 )
	{
		if( pView->m_nBoard[m_nX][m_nY+4] == 0 )
		{
			m_nY++;
		}
		else
			return true;
	}
	else if( m_nWay == 1 && m_nY < 17 )
	{
		if( pView->m_nBoard[m_nX-1][m_nY+3] == 0 &&
			pView->m_nBoard[m_nX][m_nY+3] == 0 &&
			pView->m_nBoard[m_nX+1][m_nY+3] == 0 &&
			pView->m_nBoard[m_nX+2][m_nY+3] == 0 )
		{
			m_nY++;
		}
		else
			return true;
	}
	else
		return true;

	return false;
}

void CITetrimino::MoveLeft()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 )
	{
		if( m_nX > 0 )
		{
			if( pView->m_nBoard[m_nX-1][m_nY] == 0 &&
				pView->m_nBoard[m_nX-1][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX-1][m_nY+2] == 0 &&
				pView->m_nBoard[m_nX-1][m_nY+3] == 0)
			{
				m_nX--;
			}
		}
	}
	else if( m_nWay == 1 )
	{
		if( m_nX > 1 )
		{
			if( pView->m_nBoard[m_nX-1][m_nY] == 0 )
			{
				m_nX--;
			}
		}
	}
}

void CITetrimino::MoveRight()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 )
	{
		if( m_nX < 9 )
		{
			if( pView->m_nBoard[m_nX+1][m_nY] == 0 &&
				pView->m_nBoard[m_nX+1][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX+1][m_nY+2] == 0 &&
				pView->m_nBoard[m_nX+1][m_nY+3] == 0 )
			{
				m_nX++;
			}
		}
	}
	else if( m_nWay == 1 )
	{
		if( m_nX < 7 )
		{
			if( pView->m_nBoard[m_nX+1][m_nY] == 0 )
			{
				m_nX++;
			}
		}
	}
}

void CITetrimino::Reset()
{
	m_nX = 4;
	m_nY = 0;
	m_nWay = 0;
	m_bNow = false;
}

int CITetrimino::GetBlock( int nWay, int nX, int nY )
{
	return m_nBlock[ nWay ][ nX ][ nY ];
}

void CITetrimino::Turn()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 )
	{
		//벽차기
		if( m_nX == 0 )
		{
			m_nX++;		
		}
		else if( m_nX == 9 )
		{
			m_nX-=2;
		}

		if( pView->m_nBoard[m_nX-1][m_nY+3] == 0 &&
			pView->m_nBoard[m_nX][m_nY+3] == 0 &&
			pView->m_nBoard[m_nX+1][m_nY+3] == 0 &&
			pView->m_nBoard[m_nX+2][m_nY+3] == 0 )
		{
			m_nWay = 1;
		}
	}
	else if( m_nWay == 1 )
	{
		//벽차기
		if( m_nX == 1 )
		{
			m_nX++;		
		}

		if( pView->m_nBoard[m_nX][m_nY-2] == 0 &&
			pView->m_nBoard[m_nX][m_nY-1] == 0 &&
			pView->m_nBoard[m_nX][m_nY] == 0 &&
			pView->m_nBoard[m_nX][m_nY+1] == 0 )
		{
			m_nWay = 0;
		}
	}
}