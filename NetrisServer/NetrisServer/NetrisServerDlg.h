
// NetrisServerDlg.h : ��� ����
//
//
#include "ServerSock.h"
#include "ClientSock.h"

#pragma once

#define PLAYER1_BOARD 1
#define PLAYER2_BOARD 2
#define PLAYER1_ATTACK 3
#define PLAYER2_ATTACK 4
#define PLAYER1_LOSE 5
#define PLAYER2_LOSE 6
#define PLAYER_WIN 7

// CNetrisServerDlg ��ȭ ����
class CNetrisServerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CNetrisServerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CNetrisServerDlg();

	void Send(char* buf, int len);
	void OnReceive(CClientSock* pSock);
	void OnAccept();
	void OnClose(CClientSock* pSock);
	void Attack( int nPlayer );
	void SendWin( int nPlayer );
	CServerSock* m_pServerSock;
	CClientSock* m_pPlayer1Sock;
	CClientSock* m_pPlayer2Sock;
	int m_nPlayer;

	CListBox m_pList;
	CString	m_strSend;
	bool m_bPlayer1Enter;
	bool m_bPlayer2Enter;

	CStatic m_sttPlayer1;
	CStatic m_sttPlayer2;
	CStatic m_sttServerIP;

	CString GetIpAddress();
	CString GetInIpAddress();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NETRISSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSend();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
