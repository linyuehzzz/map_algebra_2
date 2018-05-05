#include "stdafx.h"
#include "Eu5Tmp.h"


CEu5Tmp::CEu5Tmp()
{
	for (unsigned i = 0; i < 25; i++)
	{
		float x = GetOffX(i);
		float y = GetOffY(i);
		a_Mtx[i] = sqrt(x * x + y * y);
	}
}


CEu5Tmp::~CEu5Tmp()
{

}

int CEu5Tmp::TmpSize()
{
	return 25;
}

float CEu5Tmp::GetTmpDis(int i)
{
	return a_Mtx[i];
}

int CEu5Tmp::GetOffX(int i)
{
	return i % 5 - 2;
}

int CEu5Tmp::GetOffY(int i)
{
	return i / 5 - 2;
}

