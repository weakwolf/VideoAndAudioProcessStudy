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
