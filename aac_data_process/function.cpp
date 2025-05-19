#include "function.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_BUFFER_SIZE	1024*1024// 缓冲区大小
#define ADTS_FRAME_SIZE	1024*5// ADTS帧大小

int GetADTSFrame(unsigned char* pBuf, int& iBufSize, unsigned char* pFrame, int* pFrameSize)
{
	if (!pBuf || !pFrame || !pFrameSize)
		return -1;

	// ADTS长度
	int iSize = 0;

	while (1)
	{
		// @see https://blog.csdn.net/weixin_41910694/article/details/107735932 AAC格式介绍
		// 如果读取到缓冲区中还剩下7个不到的字节还没有找到同步字，则认为当前缓冲区中不存在ADTS
		// 为什么是7？AAC协议规定，ADTS的头部字段大部分情况是7个字节，也有可能为9，详细参考上面的链接
		if (iBufSize < 7)
			return -1;

		// 判断同步字
		if ((0xff == pBuf[0]) && (0xf0 == (pBuf[1] & 0xf0)))
		{
			// 13个bit表示长度
			// 1100000000000
			//   01010101000
			//      00000111
			iSize |= ((pBuf[3] & 0x03) << 11);	// 高位2bit
			iSize |= pBuf[4] << 3;				// 中间8bit
			iSize |= ((pBuf[5] & 0xe0) >> 5);	// 低位3bit

			break;
		}
		--iBufSize;
		++pBuf;
	}

	// 如果pBuf剩余的位置不足以存储完整的ADTS帧
	if (iBufSize < iSize)
		return 1;

	memcpy(pFrame, pBuf, iSize);
	*pFrameSize = iSize;

	return 0;
}

int ParseAacData(const char* pUrl /*= nullptr*/)
{
	if (nullptr == pUrl) return -1;

	FILE* pInput = fopen(pUrl, "rb");
	if (nullptr == pInput) return -1;
	
	// 存放ADTS帧的缓冲区
	unsigned char* pFrame = (unsigned char*)malloc(ADTS_FRAME_SIZE);
	if (nullptr == pFrame) return -1;
	// 存放一段aac码流的缓冲区
	unsigned char* pBuf = (unsigned char*)malloc(MAX_BUFFER_SIZE);
	if (nullptr == pBuf) return -1;

	printf("-----+- ADTS Frame Table -+------+\n");
	printf(" NUM | Profile | Frequency| Size |\n");
	printf("-----+---------+----------+------+\n");

	int iOffset = 0;
	// 记录ADTS帧的大小
	int iSize = 0;
	// 帧数量
	int iCount = 1;
	while (!feof(pInput))
	{
		fread(pBuf + iOffset, 1, MAX_BUFFER_SIZE - iOffset, pInput);
		unsigned char* pTempBuf = pBuf;
		// 缓冲区中剩余可读数据的长度
		int iRestDataSize = MAX_BUFFER_SIZE;

		// 对于当前读取出的一段aac码流，尝试读取出其中的aac帧，如果有的话
		while (1)
		{
			int iRet = GetADTSFrame(pTempBuf, iRestDataSize, pFrame, &iSize);
			if (-1 == iRet)
			{
				break;
			}
			else if (1 == iRet)
			{
				// 如果ADTS被截断了，先把前半段复制复制到缓冲区开始处
				memcpy(pBuf, pTempBuf, iRestDataSize);
				iOffset = iRestDataSize;
				break;
			}

			char strProfile[10] = { 0 };
			unsigned char profile = pFrame[2] & 0xc0;
			profile = profile >> 6;
			switch (profile)
			{
			case 0:sprintf(strProfile, "Main"); break;
			case 1:sprintf(strProfile, "LC"); break;
			case 2:sprintf(strProfile, "SSR"); break;
			default:sprintf(strProfile, "unknown");break;
			}
			char strFrequence[10] = { 0 };
			unsigned char frequency = pFrame[2] & 0x3c;
			frequency = frequency >> 2;
			switch (frequency)
			{
			case 0:sprintf(strFrequence, "96000Hz"); break;
			case 1: sprintf(strFrequence, "88200Hz"); break;
			case 2: sprintf(strFrequence, "64000Hz"); break;
			case 3: sprintf(strFrequence, "48000Hz"); break;
			case 4: sprintf(strFrequence, "44100Hz"); break;
			case 5: sprintf(strFrequence, "32000Hz"); break;
			case 6: sprintf(strFrequence, "24000Hz"); break;
			case 7: sprintf(strFrequence, "22050Hz"); break;
			case 8: sprintf(strFrequence, "16000Hz"); break;
			case 9: sprintf(strFrequence, "12000Hz"); break;
			case 10: sprintf(strFrequence, "11025Hz"); break;
			case 11: sprintf(strFrequence, "8000Hz"); break;
			default:sprintf(strFrequence, "unknown"); break;
			}
			fprintf(stdout, "%5d| %8s|  %8s| %5d|\n", iCount, strProfile, strFrequence, iSize);

			pTempBuf += iSize;
			iRestDataSize -= iSize;
		}
	}

	free(pFrame);
	free(pBuf);
	fclose(pInput);

	return 0;
}