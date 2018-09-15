#include "CTetrimino.h"

#pragma once

class CLTetrimino : public CTetrimino
{
private:
	int m_nBlock[4][3][3];

public:
	CLTetrimino();
	~CLTetrimino();

	int GetBlock( int nWay, int nX, int nY );

	virtual void GetBlock();
	virtual bool MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void Reset();
	virtual void Turn();
};