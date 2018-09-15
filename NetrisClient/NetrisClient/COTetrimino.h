#include "CTetrimino.h"

#pragma once

class COTetrimino : public CTetrimino
{
private:
	int m_nBlock[2][2];

public:
	COTetrimino();
	~COTetrimino();

	virtual void GetBlock();
	virtual bool MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void Reset();
	virtual void Turn();
};