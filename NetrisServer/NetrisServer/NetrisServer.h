
// NetrisServer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CNetrisServerApp:
// �� Ŭ������ ������ ���ؼ��� NetrisServer.cpp�� �����Ͻʽÿ�.
//

class CNetrisServerApp : public CWinApp
{
public:
	CNetrisServerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CNetrisServerApp theApp;