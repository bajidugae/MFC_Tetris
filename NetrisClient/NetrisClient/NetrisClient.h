
// NetrisClient.h : NetrisClient ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CNetrisClientApp:
// �� Ŭ������ ������ ���ؼ��� NetrisClient.cpp�� �����Ͻʽÿ�.
//

class CNetrisClientApp : public CWinApp
{
public:
	CNetrisClientApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNetrisClientApp theApp;
