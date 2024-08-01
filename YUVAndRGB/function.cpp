#include "function.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

void SplitYUV420P(const char* pUrl, int nWidth, int nHeight, int nNum /*= 1*/)
{
	// ���ļ�
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

	// �ͷ���Դ
	free(pData);
	fclose(pIn);
	fclose(pOut);
}


void SplitRGB24(const char* pUrl, int nWidth, int nHeight, int nNum /*= 1*/)
{
	// ���ļ�
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


void RGB24ToYUV420P(const char* pIn, int nWidth, int nHeight, const char* pOut, int num /*= 1*/)
{
	int nRgbLen = nWidth * nHeight * 3;
	int nYuvLen = nWidth * nHeight * 3 / 2;

	FILE* pFileIn = fopen(pIn, "rb+");
	FILE* pFileOut = fopen(pOut, "wb+");
	unsigned char* pDataRgb = (unsigned char*)malloc(nRgbLen);
	unsigned char* pDataYuv = (unsigned char*)malloc(nYuvLen);
	memset(pDataRgb, 0, nRgbLen);
	memset(pDataYuv, 0, nYuvLen);

	for (int n = 0; n < num; ++n)
	{
		fread(pDataRgb, 1, nRgbLen, pFileIn);
		RGB24ToYUV420P(pDataRgb, nWidth, nHeight, pDataYuv);
		fwrite(pDataYuv, 1, nYuvLen, pFileOut);
	}

	// �ͷ���Դ
	free(pDataRgb);
	free(pDataYuv);
	fclose(pFileIn);
	fclose(pFileOut);
}

bool RGB24ToYUV420P(unsigned char* pRgb, int nWidth, int nHeight, unsigned char* pYuv)
{
	// ָ��y,u,v��ʼ��ָ��
	unsigned char* pY = pYuv;
	unsigned char* pU = pY + nWidth * nHeight;
	unsigned char* pV = pU + nWidth * nHeight / 4;
	// rgb������ʼ
	unsigned char* pRgbData = pRgb;
	// r,g,b,y,u,v
	unsigned char r, g, b, y, u, v;

	// һ��һ�еĴ���RGB����
	for (int j = 0; j < nHeight; ++j)
	{
		// һ�������������ֽ�RGB��ɣ�����Ҫ����3
		pRgbData = pRgb + nWidth * j * 3;
		for (int i = 0; i < nWidth; ++i)
		{
			r = *(pRgbData++);
			g = *(pRgbData++);
			b = *(pRgbData++);
			// ����rgb�ֱ����yuv������ǹ�ʽ����������
			// ע�⹫ʽ��������ţ�һ����������ͼƬ�������
			y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			*(pY++) = ClipValue(y, 0, 255);
			// yuv420pÿ�ĸ�Y����һ��UV
			if ((0 == (j + 1) % 2) && 0 == (i + 1) % 2)
			{
				u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
				v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
				*(pU++) = ClipValue(u,0,255);
				*(pV++) = ClipValue(v, 0, 255);
			}
		}
	}

	return true;
}

unsigned char ClipValue(unsigned char x, unsigned char minVal, unsigned char maxVal)
{
	if (x < minVal)
		return minVal;
	else if (x > maxVal)
		return maxVal;
	else
		return x;
}