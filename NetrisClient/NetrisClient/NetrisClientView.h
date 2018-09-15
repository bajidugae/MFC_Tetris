
// NetrisClientView.h : CNetrisClientView 클래스의 인터페이스
//
#include "COTetrimino.h"
#include "CITetrimino.h"
#include "CJTetrimino.h"
#include "CLTetrimino.h"
#include "CTTetrimino.h"
#include "CSTetrimino.h"
#include "CZTetrimino.h"
#include "MemDC.h"
#include "ClientSock.h"
#include "IPDlg.h"

#pragma once

#define MAXWIDTH 10
#define MAXHEIGHT 20
#define PLAYER1_BOARD 1
#define PLAYER2_BOARD 2
#define PLAYER1_ATTACK 3
#define PLAYER2_ATTACK 4
#define PLAYER1_LOSE 5
#define PLAYER2_LOSE 6
#define PLAYER_WIN 7

class CNetrisClientView : public CView
{
protected: // serialization에서만 만들어집니다.
	CNetrisClientView();
	DECLARE_DYNCREATE(CNetrisClientView)

// 특성입니다.
public:
	CNetrisClientDoc* GetDocument() const;

	CString m_name;
	void OnReceive();
	CString	m_strSend;
	CString m_strIP;
	
protected:
	CClientSock* m_pClientSock;

// 작업입니다.
public:
	CImage * m_pBckImage[9];
	CImage * m_pGhostImage[7];
	CImage * m_pBckEnemyImage[9];
	CImage * m_pNextBlock[7];
	CImage * m_pBackground;
	CImage * m_pNumber[10];
	CImage * m_pGameover;
	CImage * m_pWin;
	CImage * m_pLose;
	int m_nBoard[MAXWIDTH][MAXHEIGHT];
	int m_nEnemyBoard[MAXWIDTH][MAXHEIGHT];
	int m_nScore;
	int m_nScoreNum[7];

	int m_nStage;

	int m_nGhostX;
	int m_nGhostY;
	bool m_bGhostCell;

	int m_nNowBlock;
	COTetrimino pOBlock;
	CITetrimino pIBlock;
	CJTetrimino pJBlock;
	CLTetrimino pLBlock;
	CTTetrimino pTBlock;
	CSTetrimino pSBlock;
	CZTetrimino pZBlock;

	COTetrimino pG_OBlock;
	CITetrimino pG_IBlock;
	CJTetrimino pG_JBlock;
	CLTetrimino pG_LBlock;
	CTTetrimino pG_TBlock;
	CSTetrimino pG_SBlock;
	CZTetrimino pG_ZBlock;

	int m_nWaitBlock[4];

	int m_nCombo;
	int m_bBeforeErase;

	bool m_bGameOver;
	int m_nPlayer;

	bool m_bIsWin;
	bool m_bIsEnemyLogin;
	bool m_bIsSingle;
	bool m_bIsMulti;

	bool m_bIsGhost;
	bool m_bIsSound;

	MCI_OPEN_PARMS m_mciOpenParms;
	MCI_PLAY_PARMS m_mciPlayParms;
	DWORD m_dwDeviceID;
	MCI_OPEN_PARMS mciOpen;
	MCI_PLAY_PARMS mciPlay;
	int dwID;

	void AddLine();
	void SendAttack();
	void SendLose();
	void Connect();
	void ResetGame();
	void GhostCheckAndDraw( CMDC * pDC );
	void ResetGhostBlock();
/*private:
	SOCKET mh_socket;
	char m_connect_flag;
	void sendData();
	void SendFrameData( SOCKET ah_socket, char a_message_id, unsigned short int a_body_size, char* ap_send_data );*/
	//CString BoardToString();
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CNetrisClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSend();
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	afx_msg LRESULT On25001(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT On25002(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	afx_msg void On32772();
	afx_msg void On32771();
	afx_msg void On32773();
	afx_msg void On32774();
};

#ifndef _DEBUG  // NetrisClientView.cpp의 디버그 버전
inline CNetrisClientDoc* CNetrisClientView::GetDocument() const
   { return reinterpret_cast<CNetrisClientDoc*>(m_pDocument); }
#endif

