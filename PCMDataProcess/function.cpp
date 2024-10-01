#include "function.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

void SplitPcm16LE(const char* pUrl)
{
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOutLeft = fopen("..\\Resource\\audio_data_left.pcm", "wb+");
	FILE* pOutRight = fopen("..\\Resource\\audio_data_right.pcm", "wb+");

	// һ�ζ�ȡ�������������ĸ��ֽ�
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

void HalfPcm16Volume(const char* pUrl)
{
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOut = fopen("..\\Resource\\audio_data_half.pcm", "wb+");

	unsigned char* pBuf = (unsigned char*)malloc(4);
	while (!feof(pIn))
	{
		fread(pBuf, 1, 4, pIn);
		short* pLeft = (short*)pBuf;
		short nHalf = (*pLeft) / 2;
		fwrite(&nHalf, 1, 2, pOut);
		fwrite(pBuf + 2, 1, 2, pOut);
	}

	free(pBuf);
	fclose(pIn);
	fclose(pOut);
}

void DoublePcm16Speed(const char* pUrl)
{
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOut = fopen("..\\Resource\\audio_data_double.pcm", "wb+");

	unsigned char* pBuf = (unsigned char*)malloc(4);
	int nCount = 0;
	while (!feof(pIn))
	{
		fread(pBuf, 1, 4, pIn);
		if (0 == nCount % 2)
		{
			fwrite(pBuf, 1, 2, pOut);
			fwrite(pBuf + 2, 1, 2, pOut);
		}

		++nCount;
	}

	free(pBuf);
	fclose(pIn);
	fclose(pOut);
}

void Pcm16ToPcm8(const char* pUrl)
{
	FILE* pIn = fopen(pUrl, "rb+");
	FILE* pOut = fopen("..\\Resource\\audio_data_pcm8.pcm", "wb+");

	unsigned char* pBuf = (unsigned char*)malloc(4);
	while (!feof(pIn))
	{
		fread(pBuf, 1, 4, pIn);

		// Left
		short* pLeft = (short*)pBuf;
		short nLeft = *pLeft;
		char nLeft8 = nLeft >> 8;
		// ע��ת���������ֻ��Ҫһ���ֽھ��ܴ洢
		fwrite(&nLeft8, 1, 1, pOut);

		// Right
		short* pRight = (short*)(pBuf + 2);
		short nRight = *pRight;
		char nRight8 = nRight >> 8;
		fwrite(&nRight8, 1, 1, pOut);
	}

	free(pBuf);
	fclose(pIn);
	fclose(pOut);
}

void WavePcmData(const char* pUrl, const char* pOut, int nSampleRate, int nChannels, int nSampleDeep, int nAudioFormat)
{
	// TODO:��������������

	// ���½ṹ���Ӧwava�ļ��е����ݿ�
	// !!!�������ʹ��ƽ̨�޹����ͣ�������ܻ��Ԥ�ڵ��г���!!!
	// header��12�ֽ�
	struct WAVE_HEADER
	{
		char		fccID[4];
		uint32_t	nSize;		// �ļ��ܴ�С
		char		fccType[4];
	};
	// format��24�ֽ�
	struct WAVE_FORMAT
	{
		char		fccID[4];
		uint32_t	nSize;				// ��ȥǰ�����ֶΣ������ֶε��ֽ���֮�ͣ�Ϊ16
		uint16_t	nFormatTag;			// ��ʽ���ͣ�1����pcm
		uint16_t	nChannels;			// ͨ����
		uint32_t	nSamplesPerSec;		// ������
		uint32_t	nAvgBytesPerSec;	// ÿ���ֽ���
		uint16_t	nBlockAlign;		// ����nChannels * nBitsPerSample / 8
		uint16_t	nBitsPerSample;		// һ��ͨ����������λ��������16λ����Ϊ16
	};
	// data��8�ֽ�
	struct WAVE_DATA 
	{
		char		fccID[4];
		uint32_t	nSize;
	};

	FILE* pIn = fopen(pUrl, "rb");
	FILE* pFileOut = fopen(pOut, "wb+");
	WAVE_HEADER header;
	memset(header.fccID, 0, 4);
	memset(header.fccType, 0, 4);
	WAVE_FORMAT format;
	memset(format.fccID, 0, 4);
	WAVE_DATA data;
	memset(data.fccID, 0, 4);

	// ���header
	memcpy(header.fccID, "RIFF", strlen("RIFF"));
	header.nSize = 0;
	memcpy(header.fccType, "WAVE", strlen("WAVE"));
	// ע�⣬���������Ȳ����nSize�ֶ�
	fseek(pFileOut, sizeof(WAVE_HEADER), SEEK_CUR);

	// ���format
	format.nChannels = nChannels;
	format.nSamplesPerSec = nSampleRate;
	format.nBitsPerSample = nSampleDeep;
	format.nBlockAlign = format.nChannels * (format.nBitsPerSample / 8);
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	// !!!��Ҫ����fmt����Ŀո������ĸ��ֽ�
	memcpy(format.fccID, "fmt ", strlen("fmt "));
	// WAVE_FORMAT�г�ȥfccid��nsize�������ֶε������ֶε��ܴ�С
	format.nSize = 16;
	// ��Ƶ��ʽ������pcm
	format.nFormatTag = nAudioFormat;
	fwrite(&format, sizeof(WAVE_FORMAT), 1, pFileOut);

	// ���data
	memcpy(data.fccID, "data", strlen("data"));
	data.nSize = 0;
	// ͬ���ģ����������Ȳ����nSize�ֶ�
	fseek(pFileOut, sizeof(WAVE_DATA), SEEK_CUR);

	// ���pcm����
	int nSize = format.nBlockAlign;
	unsigned char* pBuf = (unsigned char*)malloc(nSize);
#if 0
	while (!feof(pIn))
	{
		fread(pBuf, 1, nSize, pIn);
		fwrite(pBuf, 1, nSize, pFileOut);
		data.nSize += nSize;
	}
#else
	fread(pBuf, nSize, 1, pIn);
	while (!feof(pIn))
	{
		data.nSize += nSize;
		fwrite(pBuf, nSize, 1, pFileOut);
		fread(pBuf, nSize, 1, pIn);
	}
#endif

	// 36��44�����ԣ�����36�����Ϲ淶
	header.nSize = 36 + data.nSize;
	// rewind�������ڽ��ļ���λ��ָ�����õ��ļ��Ŀ�ͷ
	rewind(pFileOut);
	fwrite(&header, sizeof(WAVE_HEADER), 1, pFileOut);
	fseek(pFileOut, sizeof(WAVE_FORMAT), SEEK_CUR);
	fwrite(&data, sizeof(WAVE_DATA), 1, pFileOut);

	free(pBuf);
	fclose(pIn);
	fclose(pFileOut);
}
