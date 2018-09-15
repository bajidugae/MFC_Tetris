#pragma once

class CTetrimino
{
protected:
	int m_nX;
	int m_nY;
	int m_nWay;
	bool m_bNow;

public:
	CTetrimino();
	~CTetrimino();

	int GetPosX() { return m_nX; };
	void SetPosX( int nX ) { m_nX = nX; };
	int GetPosY() { return m_nY; };
	void SetPosY( int nY ) { m_nY = nY; };

	bool GetNow() { return m_bNow; };
	void SetNow( bool bNow ) { m_bNow = bNow; };

	int GetWay() { return m_nWay; };
	void SetWay( int nWay ) { m_nWay = nWay; };

	virtual void GetBlock() = 0;
	virtual bool MoveDown() = 0;
	virtual void MoveLeft() = 0;
	virtual void MoveRight() = 0;
	virtual void Reset() = 0;
	virtual void Turn() = 0;
};