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
		// 注意转换后的数据只需要一个字节就能存储
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
	// TODO:函数参数错误检查

	// 以下结构体对应wava文件中的数据块
	// !!!整型最好使用平台无关类型，否则可能会跟预期的有出入!!!
	// header，12字节
	struct WAVE_HEADER
	{
		char		fccID[4];
		uint32_t	nSize;		// 文件总大小
		char		fccType[4];
	};
	// format，24字节
	struct WAVE_FORMAT
	{
		char		fccID[4];
		uint32_t	nSize;				// 除去前两个字段，其余字段的字节数之和，为16
		uint16_t	nFormatTag;			// 格式类型，1代表pcm
		uint16_t	nChannels;			// 通道数
		uint32_t	nSamplesPerSec;		// 采样率
		uint32_t	nAvgBytesPerSec;	// 每秒字节数
		uint16_t	nBlockAlign;		// 等于nChannels * nBitsPerSample / 8
		uint16_t	nBitsPerSample;		// 一个通道中样本的位数，例如16位整型为16
	};
	// data，8字节
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

	// 填充header
	memcpy(header.fccID, "RIFF", strlen("RIFF"));
	header.nSize = 0;
	memcpy(header.fccType, "WAVE", strlen("WAVE"));
	// 注意，我们这里先不填充nSize字段
	fseek(pFileOut, sizeof(WAVE_HEADER), SEEK_CUR);

	// 填充format
	format.nChannels = nChannels;
	format.nSamplesPerSec = nSampleRate;
	format.nBitsPerSample = nSampleDeep;
	format.nBlockAlign = format.nChannels * (format.nBitsPerSample / 8);
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	// !!!不要忘记fmt后面的空格，这是四个字节
	memcpy(format.fccID, "fmt ", strlen("fmt "));
	// WAVE_FORMAT中除去fccid和nsize这两个字段的其余字段的总大小
	format.nSize = 16;
	// 音频格式，例如pcm
	format.nFormatTag = nAudioFormat;
	fwrite(&format, sizeof(WAVE_FORMAT), 1, pFileOut);

	// 填充data
	memcpy(data.fccID, "data", strlen("data"));
	data.nSize = 0;
	// 同样的，我们这里先不填充nSize字段
	fseek(pFileOut, sizeof(WAVE_DATA), SEEK_CUR);

	// 填充pcm数据
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

	// 36和44都可以，但是36更符合规范
	header.nSize = 36 + data.nSize;
	// rewind函数用于将文件的位置指针重置到文件的开头
	rewind(pFileOut);
	fwrite(&header, sizeof(WAVE_HEADER), 1, pFileOut);
	fseek(pFileOut, sizeof(WAVE_FORMAT), SEEK_CUR);
	fwrite(&data, sizeof(WAVE_DATA), 1, pFileOut);

	free(pBuf);
	fclose(pIn);
	fclose(pFileOut);
}
