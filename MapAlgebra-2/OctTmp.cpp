#include "stdafx.h"
#include "OctTmp.h"


COctTmp::COctTmp()
{
	this->a_OffX = sht_OffX;
	this->a_OffY = sht_OffY;
	this->a_TmpDis = sht_DisTmp;
}


COctTmp::~COctTmp()
{

}

int COctTmp::TmpSize()
{
	return 13;
}

float COctTmp::GetTmpDis(int i)
{
	return a_TmpDis[i];
}

int COctTmp::GetOffX(int i)
{
	return a_OffX[i];
}

int COctTmp::GetOffY(int i)
{
	return a_OffY[i];
}

