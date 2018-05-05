#include "stdafx.h"
#include "BMPBasics.h"


BMPBasics::BMPBasics(const char* readPath1, const char *readPath2,
	const char* writePath1, const char *writePath2, const char *writePath3)
{
	this->readPath1 = readPath1;
	this->readPath2 = readPath2;
	this->writePath1 = writePath1;
	this->writePath2 = writePath2;
	this->writePath3 = writePath3;
}


BMPBasics::~BMPBasics()
{

}

/// <summary>
/// 执行操作入口
/// </summary>
void BMPBasics::ProcessBMP()
{
	ReverseBMP();
	printf("finish reversing.\n");
	OverlayBMP();
	printf("finish overlaying.\n");
	MeanFilterBMP();
	printf("finish filtering.\n");
}

/// <summary>
/// 反色操作：255-当前颜色
/// </summary>
void BMPBasics::ReverseBMP()
{
	/* Read source file.*/
	BITMAPFILEHEADER srcBmpHead;
	BITMAPINFOHEADER srcBmpInfo;
	unsigned char ColorTab[1024];

	FILE *srcBmp = fopen(this->readPath1, "rb");

	fread(&srcBmpHead, sizeof(BITMAPFILEHEADER), 1, srcBmp);
	fread(&srcBmpInfo, sizeof(BITMAPINFOHEADER), 1, srcBmp);
	fread(ColorTab, 1024, 1, srcBmp);

	unsigned MtxHeight = srcBmpInfo.biHeight;
	unsigned MtxWidth = srcBmpInfo.biWidth;
	unsigned BufWidth = (MtxWidth + 3) / 4 * 4;

	unsigned char *srcLineBuf = new unsigned char[BufWidth];
	unsigned char **destBmpMtx = new unsigned char *[MtxHeight];

	/* Reverse.*/
	unsigned i, j;
	for (i = 0; i < MtxHeight; i++)
	{
		fread(srcLineBuf, BufWidth, 1, srcBmp);
		destBmpMtx[i] = new unsigned char[BufWidth];

		for (j = 0; j < MtxWidth; j++)
		{
			if (srcLineBuf[j] == 0xFF)
				destBmpMtx[i][j] = 0xFF;
			destBmpMtx[i][j] = 255 - srcLineBuf[j];
		}
	}

	/* Writing destination file.*/
	FILE *destBmp = fopen(writePath1, "wb");

	fwrite(&srcBmpHead, sizeof(BITMAPFILEHEADER), 1, destBmp);
	fwrite(&srcBmpInfo, sizeof(BITMAPINFOHEADER), 1, destBmp);
	fwrite(ColorTab, 1024, 1, destBmp);

	for (i = 0; i<MtxHeight; i++)
	{
		fwrite(destBmpMtx[i], BufWidth, 1, destBmp);
		delete[]destBmpMtx[i];
		destBmpMtx[i] = NULL;
	}

	delete[]destBmpMtx;
	delete[]srcLineBuf;
	srcLineBuf = NULL;
	fclose(srcBmp);
	fclose(destBmp);
}

/// <summary>
/// 叠置处理
/// </summary>
void BMPBasics::OverlayBMP()
{
	/* Read source file1.*/
	BITMAPFILEHEADER srcBmpHead1;
	BITMAPINFOHEADER srcBmpInfo1;
	unsigned char ColorTab1[1024];

	FILE *srcBmp1 = fopen(readPath1, "rb");

	fread(&srcBmpHead1, sizeof(BITMAPFILEHEADER), 1, srcBmp1);
	fread(&srcBmpInfo1, sizeof(BITMAPINFOHEADER), 1, srcBmp1);
	fread(ColorTab1, 1024, 1, srcBmp1);

	unsigned MtxHeight1 = srcBmpInfo1.biHeight;
	unsigned MtxWidth1 = srcBmpInfo1.biWidth;

	/* Reading source file2.*/
	BITMAPFILEHEADER srcBmpHead2;
	BITMAPINFOHEADER srcBmpInfo2;
	unsigned char ColorTab2[1024];

	FILE *srcBmp2 = fopen(readPath2, "rb");

	fread(&srcBmpHead2, sizeof(BITMAPFILEHEADER), 1, srcBmp2);
	fread(&srcBmpInfo2, sizeof(BITMAPINFOHEADER), 1, srcBmp2);
	fread(ColorTab2, 1024, 1, srcBmp2);

	unsigned MtxHeight2 = srcBmpInfo2.biHeight;
	unsigned MtxWidth2 = srcBmpInfo2.biWidth;

	/* Overlay.*/
	if (MtxHeight1 == MtxHeight2 && MtxWidth1 == MtxWidth2)
	{
		unsigned BufWidth = (MtxWidth1 + 3) / 4 * 4;
		unsigned MtxHeight = MtxHeight1;

		unsigned char *srcLineBuf1 = new unsigned char[BufWidth];
		unsigned char *srcLineBuf2 = new unsigned char[BufWidth];
		unsigned char **destBmpMtx = new unsigned char *[MtxHeight];

		unsigned i, j;
		for (i = 0; i < MtxHeight; i++)
		{
			fread(srcLineBuf1, BufWidth, 1, srcBmp1);
			fread(srcLineBuf2, BufWidth, 1, srcBmp2);

			destBmpMtx[i] = new unsigned char[BufWidth];

			for (j = 0; j < MtxWidth1; j++)
			{
				if (srcLineBuf1[j] == 0xFF && srcLineBuf2[j] == 0xFF)
					destBmpMtx[i][j] = 0xFF;
				else if (srcLineBuf1[j] != 0xFF && srcLineBuf2[j] == 0xFF)
					destBmpMtx[i][j] = srcLineBuf1[j];
				else if (srcLineBuf1[j] == 0xFF && srcLineBuf2[j] == 0xFF)
					destBmpMtx[i][j] = srcLineBuf2[j];

				destBmpMtx[i][j] = srcLineBuf1[j] + srcLineBuf2[j];
			}
		}

		/* Write destination file.*/
		FILE *destBmp = fopen(writePath2, "wb");

		fwrite(&srcBmpHead1, sizeof(BITMAPFILEHEADER), 1, destBmp);
		fwrite(&srcBmpInfo1, sizeof(BITMAPINFOHEADER), 1, destBmp);
		fwrite(ColorTab1, 1024, 1, destBmp);

		for (i = 0; i<MtxHeight; i++)
		{
			fwrite(destBmpMtx[i], BufWidth, 1, destBmp);
			delete[]destBmpMtx[i];
			destBmpMtx[i] = NULL;
		}

		delete[]destBmpMtx;
		delete[]srcLineBuf1;
		srcLineBuf1 = NULL;
		delete[]srcLineBuf2;
		srcLineBuf2 = NULL;

		fclose(destBmp);
	}
	fclose(srcBmp1);
	fclose(srcBmp2);
}

/// <summary>
/// 聚焦操作：平滑处理
/// </summary>
void BMPBasics::MeanFilterBMP()
{
	/* Read source file.*/
	BITMAPFILEHEADER srcBmpHead;
	BITMAPINFOHEADER srcBmpInfo;
	unsigned char ColorTab[1024];

	FILE *srcBmp = fopen(writePath2, "rb");

	fread(&srcBmpHead, sizeof(BITMAPFILEHEADER), 1, srcBmp);
	fread(&srcBmpInfo, sizeof(BITMAPINFOHEADER), 1, srcBmp);
	fread(ColorTab, 1024, 1, srcBmp);

	unsigned MtxHeight = srcBmpInfo.biHeight;
	unsigned MtxWidth = srcBmpInfo.biWidth;
	unsigned BufWidth = (MtxWidth + 3) / 4 * 4;

	unsigned char **srcBmpMtxIn = new unsigned char *[MtxHeight];
	unsigned char **srcBmpMtxOut = new unsigned char *[MtxHeight];

	/* Copy.*/
	unsigned i, j;
	for (i = 0; i < MtxHeight; i++)
	{
		srcBmpMtxIn[i] = new unsigned char[BufWidth];
		fread(srcBmpMtxIn[i], BufWidth, 1, srcBmp);

		srcBmpMtxOut[i] = new unsigned char[BufWidth];
		memcpy(srcBmpMtxOut[i], srcBmpMtxIn[i], BufWidth);
	}

	/* Filter.*/
	unsigned mid = 0;
	for (i = 1; i < MtxHeight - 1; i++)
	{
		for (j = 1; j < BufWidth - 1; j++)//（i，j）is the center of the window
		{
			mid = (srcBmpMtxIn[i - 1][j - 1] + srcBmpMtxIn[i - 1][j]
				+ srcBmpMtxIn[i - 1][j + 1] + srcBmpMtxIn[i][j - 1]
				+ srcBmpMtxIn[i - 1][j - 1] + srcBmpMtxIn[i - 1][j + 1]
				+ srcBmpMtxIn[i + 1][j - 1] + srcBmpMtxIn[i + 1][j]
				+ srcBmpMtxIn[i + 1][j + 1]) / 9;

			srcBmpMtxOut[i][j] = mid;
		}
	}

	/* Process edges */
	unsigned k;
	for (k = 0; k < BufWidth; k++)
		srcBmpMtxOut[0][k] = srcBmpMtxIn[0][k];
	for (k = 0; k < BufWidth; k++)
		srcBmpMtxOut[MtxHeight - 1][k] = srcBmpMtxIn[MtxHeight - 1][k];
	for (k = 0; k < MtxHeight; k++)
		srcBmpMtxOut[k][0] = srcBmpMtxIn[k][0];
	for (k = 0; k < MtxHeight; k++)
		srcBmpMtxOut[k][BufWidth - 1] = srcBmpMtxIn[k][BufWidth - 1];

	/* Writing destination file.*/
	FILE *destBmp = fopen(writePath3, "wb");

	fwrite(&srcBmpHead, sizeof(BITMAPFILEHEADER), 1, destBmp);
	fwrite(&srcBmpInfo, sizeof(BITMAPINFOHEADER), 1, destBmp);
	fwrite(ColorTab, 1024, 1, destBmp);

	for (i = 0; i < MtxHeight; i++)
	{
		fwrite(srcBmpMtxOut[i], BufWidth, 1, destBmp);
		delete[]srcBmpMtxOut[i];
		srcBmpMtxOut[i] = NULL;
	}
	delete[]srcBmpMtxOut;

	for (i = 0; i<MtxHeight; i++)
	{
		delete[]srcBmpMtxIn[i];
		srcBmpMtxIn[i] = NULL;
	}
	delete[]srcBmpMtxIn;

	fclose(srcBmp);
	fclose(destBmp);
}
