#include "stdafx.h"
#include "DistanceTransformer.h"


DistanceTransformer::DistanceTransformer(const char* readPath, const char *writeBMPPath, const char* writeLocPath)
{
	this->readPath = readPath;
	this->writeLocPath = writeLocPath;
	this->writeBMPPath = writeBMPPath;
}


DistanceTransformer::~DistanceTransformer()
{
	if (OTmp != 0x00)
		delete OTmp;
	if (E5Tmp != 0x00)
		delete E5Tmp;

}

void DistanceTransformer::OctDistance()
{
	this->OTmp = new COctTmp();
	TmpDistanceTransform32b(readPath, OTmp, writeBMPPath, writeLocPath);
}

void DistanceTransformer::EuDistance()
{
	this->E5Tmp = new CEu5Tmp();
	TmpDistanceTransform32b(readPath, E5Tmp, writeBMPPath, writeLocPath);
}

int DistanceTransformer::TmpDistanceTransform(char *SrcBmpName, CDistanceTemplet *pTemplet)
{
	return 1;
};

int DistanceTransformer::Bmp8to32b(const char* SourceFileName, const char* IdxFileName)  //��Դ8bitλͼתΪ32bitλͼ
{
	unsigned i, j; // ѭ������
				   // *1 ����Դ�ļ�
	assert(SourceFileName != NULL);

	FILE* SrcFileStream = fopen(SourceFileName, "rb");
	if (SrcFileStream == NULL) return -1; // �������1���ļ�������

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}

	if (SrcFileInfo.biBitCount != 8) // 256ɫͼ����
	{
		fclose(SrcFileStream);
		return -2; // �������2���ļ���ʽ����Ҫ��
	}


	// *2 ����ʵ������
	BITMAPFILEHEADER IdxFileHead;
	BITMAPINFOHEADER IdxFileInfo;
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;
	int LineBytes2 = SrcFileInfo.biWidth;

	IdxFileHead.bfType = SrcFileHead.bfType;
	IdxFileHead.bfSize = 54 + LineBytes2 * 4 * SrcFileInfo.biHeight;
	IdxFileHead.bfReserved1 = 0;
	IdxFileHead.bfReserved2 = 0;
	IdxFileHead.bfOffBits = 54;

	IdxFileInfo.biSize = 40;
	IdxFileInfo.biWidth = SrcFileInfo.biWidth;
	IdxFileInfo.biHeight = SrcFileInfo.biHeight;
	IdxFileInfo.biPlanes = 1;
	IdxFileInfo.biBitCount = 32;
	IdxFileInfo.biCompression = 0;
	IdxFileInfo.biSizeImage = 0;
	IdxFileInfo.biXPelsPerMeter = 0;
	IdxFileInfo.biYPelsPerMeter = 0;
	IdxFileInfo.biClrUsed = 0;
	IdxFileInfo.biClrImportant = 0;


	// *2.1 ����ԭʼ����
	unsigned char * pSrcCellLineBuf = new unsigned char[LineBytes];   // Դ�ļ��л���
	unsigned **pIdxCellMatixBuf = new unsigned*[IdxFileInfo.biHeight]; //��������
	for (i = 0; i<IdxFileInfo.biHeight; i++)
	{
		fseek(SrcFileStream, SrcFileHead.bfOffBits + i * sizeof(char)*LineBytes, 0);
		fread(pSrcCellLineBuf, sizeof(char), LineBytes, SrcFileStream);
		pIdxCellMatixBuf[i] = new unsigned[LineBytes2];
		for (j = 0; j<LineBytes2; j++)
			if (pSrcCellLineBuf[j] == 0xFF) pIdxCellMatixBuf[i][j] = 0xFFFFFFFF;
			else
			{
				int k = pSrcCellLineBuf[j];
				unsigned uintTmp = k;
				/*  ȡ��Ӧ��ɫ*/

				uintTmp = ColorIdx[k * 4];
				uintTmp <<= 8;
				uintTmp += ColorIdx[k * 4 + 1];
				uintTmp <<= 8;
				uintTmp += ColorIdx[k * 4 + 2];
				uintTmp <<= 8;
				uintTmp += ColorIdx[k * 4 + 3];

				pIdxCellMatixBuf[i][j] = uintTmp;
			}
	}

	FILE *ObjIdxFile = fopen(IdxFileName, "wb");
	if (ObjIdxFile == NULL) return -3; // �������3��ʵ�����������ļ��޷�����
	fseek(ObjIdxFile, 0, 0);
	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER), 1, ObjIdxFile);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER), 1, ObjIdxFile);
	for (i = 0; i<IdxFileInfo.biHeight; i++)
	{
		fseek(ObjIdxFile, 54 + i * sizeof(unsigned)*LineBytes2, 0);
		fwrite(pIdxCellMatixBuf[i], sizeof(unsigned), LineBytes2, ObjIdxFile);
		delete[] pIdxCellMatixBuf[i];
		pIdxCellMatixBuf[i] = NULL;
	}

	delete[] pIdxCellMatixBuf;
	delete[] pSrcCellLineBuf;

	fclose(ObjIdxFile);
	fclose(SrcFileStream);
	return 1; // ���سɹ�����1
}



int DistanceTransformer::TmpDistanceTransform32b(const char *SrcBmpName, CDistanceTemplet *pTemplet, const char *OutLocName, const char *OutDisName)
{

	// 1. ��ȡbmp������Allocation Field
	unsigned i, j; // ѭ������

	FILE* SrcFileStream = fopen(SrcBmpName, "rb");
	if (SrcFileStream == NULL) return -1; // �������1���ļ�������

	BITMAPFILEHEADER SrcFileHead;
	BITMAPINFOHEADER SrcFileInfo;
	unsigned char ColorIdx[1024];

	if (fread(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}
	if (fread(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}
	if (fread(ColorIdx, 1024, 1, SrcFileStream) != 1)
	{
		fclose(SrcFileStream);
		return -1;
	}

	if (SrcFileInfo.biBitCount != 8) // ����8bitλͼ
	{
		fclose(SrcFileStream);
		return -2; // �������2���ļ���ʽ����Ҫ��
	}

	int Height = SrcFileInfo.biHeight;
	int Width = SrcFileInfo.biWidth;
	int LineBytes = (SrcFileInfo.biWidth + 3) / 4 * 4;

	// 3. ������ʼ���볡�����䳡
	LocMtx = new unsigned char *[Height];
	DisMtx = new float *[Height];

	for (j = 0; j < Height; j++)
	{
		LocMtx[j] = new unsigned char[LineBytes];
		DisMtx[j] = new float[Width];
		fread(LocMtx[j], sizeof(char), LineBytes, SrcFileStream);
		for (i = 0; i < Width; i++)
		{
			if (LocMtx[j][i] == 255) DisMtx[j][i] = MaxFloat;
			else DisMtx[j][i] = 0;
		}
	}

	// 4. ģ�����任
	// 4.1 �����ϵ����·���Forward mask��
	for (j = 0; j < Height; j++)
		for (i = 0; i < Width; i++)
		{
			if (fabs(DisMtx[j][i]) < MinFloat) continue;
			float MinDis = DisMtx[j][i];

			for (int k = 0; k < pTemplet->TmpSize() / 2; k++)
			{
				int OffX = pTemplet->GetOffX(k);
				int OffY = pTemplet->GetOffY(k);
				float TmpDis = pTemplet->GetTmpDis(k);

				int x = i + OffX;
				int y = j + OffY;

				if (x < 0 || y < 0 || x > Width - 1 || y > Height - 1) continue;

				if (fabs(DisMtx[y][x] - MaxFloat) < MinFloat) continue; // ��λ�����������󲻿ɼ���

				if (MinDis > DisMtx[y][x] + TmpDis)
				{
					MinDis = DisMtx[y][x] + TmpDis;
					DisMtx[j][i] = MinDis;
					LocMtx[j][i] = LocMtx[y][x];
				}
			}
		}
	// 4.2 �����µ����Ϸ���Backward mask��
	for (j = Height - 1; j > 0; j--)
		for (i = Width - 1; i > 0; i--)
		{
			if (fabs(DisMtx[j][i]) < MinFloat) continue;
			float MinDis = DisMtx[j][i];

			for (int k = pTemplet->TmpSize() / 2 + 1; k < pTemplet->TmpSize(); k++)
			{
				int OffX = pTemplet->GetOffX(k);
				int OffY = pTemplet->GetOffY(k);
				float TmpDis = pTemplet->GetTmpDis(k);

				int x = i + OffX;
				int y = j + OffY;

				if (x < 0 || y < 0 || x > Width - 1 || y > Height - 1) continue;

				if (fabs(DisMtx[y][x] - MaxFloat) < MinFloat) continue; // ��λ�����������󲻿ɼ���

				if (MinDis > DisMtx[y][x] + TmpDis)
				{
					MinDis = DisMtx[y][x] + TmpDis;
					DisMtx[j][i] = MinDis;
					LocMtx[j][i] = LocMtx[y][x];
				}
			}
		}

	// 5. д�����ļ�
	// 5.1 д��������32bitfloat��
	BITMAPFILEHEADER IdxFileHead; // 32bitλͼͷ�ṹ
	BITMAPINFOHEADER IdxFileInfo;

	IdxFileHead.bfType = SrcFileHead.bfType;
	IdxFileHead.bfSize = 54 + SrcFileInfo.biWidth * 4 * SrcFileInfo.biHeight;//
	IdxFileHead.bfReserved1 = 0;
	IdxFileHead.bfReserved2 = 0;
	IdxFileHead.bfOffBits = 54;//

	IdxFileInfo.biSize = 40;
	IdxFileInfo.biWidth = SrcFileInfo.biWidth;//
	IdxFileInfo.biHeight = SrcFileInfo.biHeight;//
	IdxFileInfo.biPlanes = 1;
	IdxFileInfo.biBitCount = 32;//
	IdxFileInfo.biCompression = 0;
	IdxFileInfo.biSizeImage = 0;
	IdxFileInfo.biXPelsPerMeter = 0;
	IdxFileInfo.biYPelsPerMeter = 0;
	IdxFileInfo.biClrUsed = 0;
	IdxFileInfo.biClrImportant = 0;

	FILE * bmpWrite = fopen(OutDisName, "wb");
	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER), 1, bmpWrite);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER), 1, bmpWrite);
	for (int y = 0; y < Height; y++)
	{
		fwrite(DisMtx[y], sizeof(float), Width, bmpWrite);
	}
	fclose(bmpWrite);

	// 5.2 д����䳡����8bit unsigned char��
	FILE *locWrite = fopen(OutLocName, "wb");
	fwrite(&SrcFileHead, sizeof(BITMAPFILEHEADER), 1, locWrite);
	fwrite(&SrcFileInfo, sizeof(BITMAPINFOHEADER), 1, locWrite);
	fwrite(ColorIdx, 1024, 1, locWrite);
	for (int y = 0; y < Height; y++)
	{
		fwrite(LocMtx[y], sizeof(char), LineBytes, locWrite);
	}
	fclose(locWrite);

	// 6. �����ͷ��ڴ�
	for (j = 0; j<Height; j++)
	{
		delete[]LocMtx[j];
		delete[]DisMtx[j];
	}
	delete[]LocMtx;
	delete[]DisMtx;
	return 1;

};


int DistanceTransformer::PoweredTrans(char *SrcBmpName, CDistanceTemplet *pTemplet, float Powers[255])
{
	return 1;
};