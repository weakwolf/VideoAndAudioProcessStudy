#include "function.h"

#include <cstdio>
#include <cstdlib>

void SplitPcm16LE(const char* pUrl)
{
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOutLeft = fopen("..\\Resource\\audio_data_left.pcm", "wb+");
	FILE* pOutRight = fopen("..\\Resource\\audio_data_right.pcm", "wb+");

	// 一次读取两个样本，共四个字节
	unsigned char* pbuf = (unsigned char*)malloc(4);
	while (!feof(pIn))
	{
		fread(pbuf, 1, 4, pIn);
		fwrite(pbuf, 1, 2, pOutLeft);
		fwrite(pbuf + 2, 1, 2, pOutRight);
	}

	free(pbuf);
	fclose(pIn);
	fclose(pOutLeft);
	fclose(pOutRight);
}