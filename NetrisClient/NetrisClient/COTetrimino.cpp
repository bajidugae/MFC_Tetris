#include "stdafx.h"
#include "MainFrm.h"
#include "NetrisClientDoc.h"
#include "NetrisClientView.h"
#include "COTetrimino.h"

COTetrimino::COTetrimino()
{
	for( int i = 0 ; i < 2 ; i ++ )
	{
		for( int j = 0 ; j < 2 ; j ++ )
		{
			m_nBlock[i][j] = 1;
		}
	}

	m_nX = 4;
	m_nY = 0;
	m_bNow = false;
}

COTetrimino::~COTetrimino()
{
}

void COTetrimino::GetBlock()
{
}

bool COTetrimino::MoveDown()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nY < 18 )
	{
		if( pView->m_nBoard[m_nX][m_nY+2] == 0 &&
			pView->m_nBoard[m_nX + 1][m_nY+2] == 0 )
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

void COTetrimino::MoveLeft()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nX > 0 )
	{
		if( pView->m_nBoard[m_nX-1][m_nY] == 0 &&
			pView->m_nBoard[m_nX-1][m_nY+1] == 0 )
		{
			m_nX--;
		}
	}
}

void COTetrimino::MoveRight()
{
	CNetrisClientView* pView = (CNetrisClientView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	if( m_nX < 8 )
	{
		if( pView->m_nBoard[m_nX+1][m_nY] == 0 &&
			pView->m_nBoard[m_nX+1][m_nY+1] == 0 )
		{
			m_nX++;
		}
	}
}

void COTetrimino::Reset()
{
	m_nX = 4;
	m_nY = 0;
	m_nWay = 0;
	m_bNow = false;
}

void COTetrimino::Turn()
{
}