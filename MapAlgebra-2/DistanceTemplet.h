#pragma once
#include <math.h>
class CDistanceTemplet
{
public:
	CDistanceTemplet();
	~CDistanceTemplet();

	virtual int TmpSize() = 0;
	virtual float GetTmpDis(int i) = 0;
	virtual int GetOffX(int i) = 0;
	virtual int GetOffY(int i) = 0;
};

