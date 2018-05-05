/**************************************
Author:ÁÖ«h
Date:2018/5/4
Update:2018/5/4
Description:¾àÀë±ä»»¡ª¡ªChamfer
***************************************/
#pragma once
#include "DistanceTemplet.h"
class COctTmp :
	public CDistanceTemplet
{
public:
	COctTmp();
	~COctTmp();
	int TmpSize();
	float GetTmpDis(int i);
	int GetOffX(int i);
	int GetOffY(int i);

private:
	char* a_OffX;
	char* a_OffY;
	char* a_TmpDis;

	/* Âü¹þ¶Ù¾àÀë */
	char sht_OffX[13] = { 0, -1, 0, 1, -2, -1, 0 ,1, 2, -1, 0, 1, 0 };
	char sht_OffY[13] = { -2, -1, -1, -1, -2, 0, 0 , 0, 0, 1, 1, 1, 2 };
	char sht_DisTmp[13] = { 2, 2, 1, 2, 2, 1, 0 , 1, 2, 2, 1, 2, 2 };
};

