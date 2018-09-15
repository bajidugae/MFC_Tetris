#include "CTetrimino.h"

#pragma once

class CSTetrimino : public CTetrimino
{
private:
	int m_nBlock[2][3][3];

public:
	CSTetrimino();
	~CSTetrimino();

	int GetBlock( int nWay, int nX, int nY );

	virtual void GetBlock();
	virtual bool MoveDown();
	virtual void MoveLeft();
	virtual void MoveRight();
	virtual void Reset();
	virtual void Turn();
};