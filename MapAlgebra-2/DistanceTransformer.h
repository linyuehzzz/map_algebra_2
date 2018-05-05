#pragma once
#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include <math.h>
#include "OctTmp.h"
#include "Eu5Tmp.h"

#define MaxFloat 9999999999999.0
#define MinFloat 0.0000000001
class DistanceTransformer
{
public:
	DistanceTransformer(const char* readPath, const char *writeBMPPath,const char* writeLocPath);
	~DistanceTransformer();
	void OctDistance();
	void EuDistance();
	int Bmp8to32b(const char* SourceFileName,const char* IdxFileName);  //将源8bit位图转为32bit位图
	int TmpDistanceTransform(char *SrcBmpName, CDistanceTemplet *Templet);
	int TmpDistanceTransform32b(const char *SrcBmpName, CDistanceTemplet *pTemplet,
		const char *OutLocName, const char *OutDisName);
	int PoweredTrans(char *SrcBmpName, CDistanceTemplet *pTemplet, float Powers[255] = NULL);

	unsigned char ** LocMtx;
	float ** DisMtx;

private:
	COctTmp* OTmp;
	CEu5Tmp* E5Tmp;
	const char* readPath;
	const char* writeBMPPath;
	const char* writeLocPath;
};

