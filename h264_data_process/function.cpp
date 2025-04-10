#include "function.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

bool IsStartCodeOne(unsigned char* pBuf)
{
	if (0 == pBuf[0] && 0 == pBuf[1] && 1 == pBuf[2])
	{
		return true;
	}

	return false;
}

bool IsStartCodeTwo(unsigned char* pBuf)
{
	if (0 == pBuf[0] && 0 == pBuf[1] && 0 == pBuf[2] && 1 == pBuf[3])
	{
		return true;
	}

	return false;
}

int GetNextNALU(SNALU* pNalu,FILE* pFile)
{
	if (nullptr == pFile)
	{
		printf("invalid file!!!\n");

		return -1;
	}
	if (nullptr == pNalu)
	{
		printf("nalu is nullptr!!!]n");

		return -1;
	}

	// 用于暂时存储读取字节数据的缓冲区
	byte* pBuf = (byte*)calloc(pNalu->iMaxSize, sizeof(byte));
	if (nullptr == pBuf)
	{
		printf("allocate buffer failed!!!\n");

		return -1;
	}

	// 判断起始码的类型
	bool bIsCodeOne = false;
	bool bIsCodeTwo = false;
	int iPos = 0;
	pNalu->iStartCodeLen = 3;
	if (3 != fread(pBuf, 1, 3, pFile))
	{
		free(pBuf);
		
		return -1;
	}
	bIsCodeOne = IsStartCodeOne(pBuf);
	if (!bIsCodeOne)
	{
		if (1 != fread(pBuf + 1, 1, 1, pFile))
		{
			free(pBuf);

			return -1;
		}
		bIsCodeTwo = IsStartCodeTwo(pBuf);
		if (bIsCodeTwo)
		{
			iPos = 4;
			pNalu->iStartCodeLen = iPos;
		}
		else
		{
			// 两种起始码都不属于，说明h264码流有问题
			free(pBuf);

			return -1;
		}
	}
	else
	{
		iPos = 3;
	}

	// 至此我们已经找到了一个起始码，这个变量用于标识下一个起始码是否找到
	bool bStartCodeFound = false;
	while (!bStartCodeFound)
	{
		// 按照雷博士这里的代码逻辑，到文件末尾前面一定有一个nalu
		// 这里看起来没有考虑H264码流被破坏的情况
		if (feof(pFile))
		{
			pNalu->iLen = (iPos - 1) - pNalu->iStartCodeLen;
			memcpy(pNalu->pBuf, &pBuf[pNalu->iStartCodeLen], pNalu->iLen);

			free(pBuf);
			return iPos - 1;
		}

		pBuf[iPos++] = fgetc(pFile);
		// 由于下一个起始码必须要整个读取完才能判断，所以这里要倒着判断
		bIsCodeOne = IsStartCodeOne(&pBuf[iPos - 3]);
		if (bIsCodeOne)
		{
			bStartCodeFound = true;
		}
		else
		{
			bIsCodeTwo = IsStartCodeTwo(&pBuf[iPos - 4]);
			if (bIsCodeTwo)
			{
				bStartCodeFound = true;
			}
		}
	}
	// 退出循环代表已经找到了下一个起始码，也就代表已经读取完了一个nalu，此时需要回退文件指针
	int iRewind = bIsCodeOne ? -3 : -4;
	if (0 != fseek(pFile, iRewind, SEEK_CUR))
	{
		free(pBuf);
		printf("seek failed!!!\n");
	}

	// 填充nalu字段
	pNalu->iLen = (iPos + iRewind) - pNalu->iStartCodeLen;
	memcpy(pNalu->pBuf, &pBuf[pNalu->iStartCodeLen], pNalu->iLen);
	// nalu的头只占一个字节
	pNalu->iForbidden = pNalu->pBuf[0] & 0x80;// 1 bit，1000 0000
	// 右移五位获取实际的值
	pNalu->iPriority = (pNalu->pBuf[0] & 0x60) >> 5;// 2 bit,0110 0000
	pNalu->iType = pNalu->pBuf[0] & 0x1f;// 5 bit,0001 1111

	free(pBuf);

	return iPos + iRewind;
}

int ParseH264Data(const char* pUrl /*= nullptr*/)
{
	if (nullptr == pUrl)
	{
		printf("invalid path!!!\n");

		return -1;
	}

	FILE* pFile = fopen(pUrl, "rb+");
	if (nullptr == pFile)
	{
		printf("fail to open file!!!\n");

		return -1;
	}

	SNALU nalu;
	nalu.iMaxSize = 100000;
	nalu.pBuf = (byte*)calloc(nalu.iMaxSize,sizeof(byte));
	if (nullptr == nalu.pBuf)
	{
		printf("alloc failed!!!\n");

		return -1;
	}

	int data_offset = 0;
	int nal_num = 0;
	printf("-----+-------- NALU Table ------+---------+\n");
	printf(" NUM |    POS  |    IDC |  TYPE |   LEN   |\n");
	while (!feof(pFile))
	{
		int iDataLen = GetNextNALU(&nalu, pFile);
		if(-1 == iDataLen) continue;
		
		char type[20] = { 0 };
		switch (nalu.iType)
		{
		case ENT_Slice:sprintf(type, "SLICE"); break;
		case ENT_DPA:sprintf(type, "DPA"); break;
		case ENT_DPB:sprintf(type, "DPB"); break;
		case ENT_DPC:sprintf(type, "DPC"); break;
		case ENT_IDR:sprintf(type, "IDR"); break;
		case ENT_SEI:sprintf(type, "SEI"); break;
		case ENT_SPS:sprintf(type, "SPS"); break;
		case ENT_PPS:sprintf(type, "PPS"); break;
		case ENT_AUD:sprintf(type, "AUD");break;
		case ENT_EOSEQ:sprintf(type, "EOSEQ"); break;
		case ENT_EOSTREAM:sprintf(type, "EOSTREAM"); break;
		case ENT_FILL:sprintf(type, "FILL"); break;
		}
		char idc_str[20] = { 0 };
		switch (nalu.iPriority)
		{
		case ENP_Disposable:sprintf(idc_str, "DISPOS"); break;
		case ENP_Low:sprintf(idc_str, "LOW"); break;
		case ENP_High:sprintf(idc_str, "HIGH"); break;
		case ENP_Highest:sprintf(idc_str, "HIGHEST"); break;
		}
		fprintf(stdout, "%5d| %8d| %7s| %6s| %8d\n", nal_num, data_offset, idc_str, type, nalu.iLen);
		data_offset += iDataLen;
		++nal_num;
	}

	free(nalu.pBuf);

	return 0;
}
