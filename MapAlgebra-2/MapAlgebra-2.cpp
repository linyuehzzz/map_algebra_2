// MapAlgebra-2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BMPBasics.h"
#include "DistanceTransformer.h"

int main()
{
	DistanceTransformer* dtf1 = new DistanceTransformer("point.bmp", "OutLoc_oct.bmp", "OutDis_oct.bmp");
	dtf1->OctDistance();

	DistanceTransformer* dtf2 = new DistanceTransformer("point.bmp", "OutLoc_eu5.bmp", "OutDis_eu5.bmp");
	dtf2->EuDistance();

    return 0;
}

