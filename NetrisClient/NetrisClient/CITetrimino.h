#include "CTetrimino.h"

#pragma once

class CITetrimino : public CTetrimino
{
private:
	int m_nBlock[2][4][4];

public:
	CITetrimino();
	~CITetrimino();

	int GetBlock( int nWay, int nX, int nY );

	virtual void GetBlock();
	virtual bool MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void Reset();
	virtual void Turn();
};