#include "stdafx.h"
#include "MainFrm.h"
#include "NetrisClientDoc.h"
#include "NetrisClientView.h"
#include "CTTetrimino.h"

CTTetrimino::CTTetrimino()
{
	m_nBlock[0][0][0] = 0; m_nBlock[0][1][0] = 0; m_nBlock[0][2][0] = 0;
	m_nBlock[0][0][1] = 5; m_nBlock[0][1][1] = 5; m_nBlock[0][2][1] = 5;
	m_nBlock[0][0][2] = 0; m_nBlock[0][1][2] = 5; m_nBlock[0][2][2] = 0;

	m_nBlock[1][0][0] = 0; m_nBlock[1][1][0] = 5; m_nBlock[1][2][0] = 0;
	m_nBlock[1][0][1] = 5; m_nBlock[1][1][1] = 5; m_nBlock[1][2][1] = 0;
	m_nBlock[1][0][2] = 0; m_nBlock[1][1][2] = 5; m_nBlock[1][2][2] = 0;

	m_nBlock[2][0][0] = 0; m_nBlock[2][1][0] = 5; m_nBlock[2][2][0] = 0;
	m_nBlock[2][0][1] = 5; m_nBlock[2][1][1] = 5; m_nBlock[2][2][1] = 5;
	m_nBlock[2][0][2] = 0; m_nBlock[2][1][2] = 0; m_nBlock[2][2][2] = 0;

	m_nBlock[3][0][0] = 0; m_nBlock[3][1][0] = 5; m_nBlock[3][2][0] = 0;
	m_nBlock[3][0][1] = 0; m_nBlock[3][1][1] = 5; m_nBlock[3][2][1] = 5;
	m_nBlock[3][0][2] = 0; m_nBlock[3][1][2] = 5; m_nBlock[3][2][2] = 0;

	m_nX = 4;
	m_nY = 0;
	m_nWay = 0;
}

CTTetrimino::~CTTetrimino()
{
}

void CTTetrimino::GetBlock()
{
}

bool CTTetrimino::MoveDown()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 && m_nY < 17 )
	{
		if( pView->m_nBoard[m_nX][m_nY+2] == 0 &&
			pView->m_nBoard[m_nX+1][m_nY+3] == 0 && 
			pView->m_nBoard[m_nX+2][m_nY+2] == 0 )
		{
			m_nY++;
		}
		else
			return true;
	}
	else if( m_nWay == 1 && m_nY < 17 )
	{
		if( pView->m_nBoard[m_nX][m_nY+2] == 0 &&
			pView->m_nBoard[m_nX+1][m_nY+3] == 0 )
		{
			m_nY++;
		}
		else
			return true;
	}
	else if( m_nWay == 2 && m_nY < 18 )
	{
		if( pView->m_nBoard[m_nX][m_nY+2] == 0 &&
			pView->m_nBoard[m_nX+1][m_nY+2] == 0 &&
			pView->m_nBoard[m_nX+2][m_nY+2] == 0 )
		{
			m_nY++;
		}
		else
			return true;
	}
	else if( m_nWay == 3 && m_nY < 17 )
	{
		if( pView->m_nBoard[m_nX+1][m_nY+3] == 0 &&
			pView->m_nBoard[m_nX+2][m_nY+2] == 0 )
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

void CTTetrimino::MoveLeft()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 )
	{
		if( m_nX > 0 )
		{
			if( pView->m_nBoard[m_nX-1][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX][m_nY+2] == 0 )
			{
				m_nX--;
			}
		}
	}
	else if( m_nWay == 1 )
	{
		if( m_nX > 0 )
		{
			if( pView->m_nBoard[m_nX][m_nY] == 0 &&
				pView->m_nBoard[m_nX-1][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX][m_nY+2] == 0 )
			{
				m_nX--;
			}
		}
	}
	else if( m_nWay == 2 )
	{
		if( m_nX > 0 )
		{
			if( pView->m_nBoard[m_nX-1][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX][m_nY] == 0 )
			{
				m_nX--;
			}
		}
	}
	else if( m_nWay == 3 )
	{
		if( m_nX >= 0 )
		{
			if( pView->m_nBoard[m_nX][m_nY] == 0 &&
				pView->m_nBoard[m_nX][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX][m_nY+2] == 0 )
			{
				m_nX--;
			}
		}
	}
}

void CTTetrimino::MoveRight()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 )
	{
		if( m_nX < 7 )
		{
			if( pView->m_nBoard[m_nX+3][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX+2][m_nY+2] == 0 )
			{
				m_nX++;
			}
		}
	}
	else if( m_nWay == 1 )
	{
		if( m_nX < 8 )
		{
			if( pView->m_nBoard[m_nX+2][m_nY] == 0 &&
				pView->m_nBoard[m_nX+2][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX+2][m_nY+2] == 0 )
			{
				m_nX++;
			}
		}
	}
	else if( m_nWay == 2 )
	{
		if( m_nX < 7 )
		{
			if( pView->m_nBoard[m_nX+2][m_nY] == 0 &&
				pView->m_nBoard[m_nX+3][m_nY+1] == 0 )
			{
				m_nX++;
			}
		}
	}
	else if( m_nWay == 3 )
	{
		if( m_nX < 7 )
		{
			if( pView->m_nBoard[m_nX+2][m_nY] == 0 &&
				pView->m_nBoard[m_nX+3][m_nY+1] == 0 &&
				pView->m_nBoard[m_nX+2][m_nY+2] == 0 )
			{
				m_nX++;
			}
		}
	}
}

void CTTetrimino::Reset()
{
	m_nX = 4;
	m_nY = 0;
	m_nWay = 0;
	m_bNow = false;
}

void CTTetrimino::Turn()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nWay == 0 )
	{
		if( pView->m_nBoard[m_nX+1][m_nY] == 0 )
			m_nWay = 1;
	}
	else if( m_nWay == 1 )
	{
		if( m_nX == 8 )
			m_nX--;

		if( pView->m_nBoard[m_nX+2][m_nY+1] == 0 )
			m_nWay = 2;
	}
	else if( m_nWay == 2 )
	{
		if( pView->m_nBoard[m_nX+1][m_nY+2] == 0 )
			m_nWay = 3;
	}
	else if( m_nWay == 3 )
	{
		if( m_nX == -1 )
			m_nX++;

		if( pView->m_nBoard[m_nX][m_nY+1] == 0 )
			m_nWay = 0;
	}
}

int CTTetrimino::GetBlock( int nWay, int nX, int nY )
{
	return m_nBlock[ nWay ][ nX ][ nY ];
}