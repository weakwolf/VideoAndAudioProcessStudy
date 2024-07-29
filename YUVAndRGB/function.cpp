#include "function.h"

#include <cstdio>
#include <cstdlib>

void SplitYUV420P(const char* pUrl, int nWidth, int nHeight, int nNum /*= 1*/)
{
	// 打开文件
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOutY = fopen("..\\Resource\\output_420_y.y", "wb+");
	FILE* pOutU = fopen("..\\Resource\\output_420_u.y", "wb+");
	FILE* pOutV = fopen("..\\Resource\\output_420_v.y", "wb+");

	unsigned char* pPic = (unsigned char*)malloc(nWidth * nHeight * 3 / 2);

	for (int nIndex = 0; nIndex < nNum; ++nIndex)
	{
		fread(pPic, 1, nWidth * nHeight * 3 / 2, pIn);
		fwrite(pPic, 1, nWidth * nHeight, pOutY);
		fwrite(pPic + nWidth * nHeight, 1, nWidth * nHeight / 4, pOutU);
		fwrite(pPic + nWidth * nHeight * 5 / 4, 1, nWidth * nHeight / 4, pOutV);
	}

	free(pPic);
	fclose(pIn);
	fclose(pOutY);
	fclose(pOutU);
	fclose(pOutV);
}


void HalfLuminance(const char* pUrl, int nWidth, int nHeight, int nNum)
{
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOut = fopen("..\\Resource\\half_y.yuv", "wb+");

	unsigned char* pData = (unsigned char*)malloc(nWidth * nHeight * 3 / 2);
	
	for (int i = 0; i < nNum; ++i)
	{
		fread(pData, 1, nWidth * nHeight * 3 / 2, pIn);
		for (int j = 0; j < nWidth * nHeight; ++j)
		{
			unsigned char temp = pData[j] / 2;
			pData[j] = temp;
		}
	}
	fwrite(pData, 1, nWidth * nHeight * 3 / 2, pOut);

	// 释放资源
	free(pData);
	fclose(pIn);
	fclose(pOut);
}


void SplitRGB24(const char* pUrl, int nWidth, int nHeight, int nNum /*= 1*/)
{
	// 打开文件
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOutR = fopen("..\\Resource\\r.y", "wb+");
	FILE* pOutG = fopen("..\\Resource\\g.y", "wb+");
	FILE* pOutB = fopen("..\\Resource\\b.y", "wb+");

	unsigned char* pPic = (unsigned char*)malloc(nWidth * nHeight * 3);

	for (int nIndex = 0; nIndex < nNum; ++nIndex)
	{
		fread(pPic, 1, nWidth * nHeight * 3, pIn);
		fwrite(pPic, 1, nWidth * nHeight, pOutR);
		fwrite(pPic + nWidth * nHeight, 1, nWidth * nHeight, pOutG);
		fwrite(pPic + nWidth * nHeight * 2, 1, nWidth * nHeight, pOutB);
	}

	free(pPic);
	fclose(pIn);
	fclose(pOutR);
	fclose(pOutG);
	fclose(pOutB);
}
