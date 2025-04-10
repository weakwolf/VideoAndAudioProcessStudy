#pragma once

#include <cstdio>

using byte = unsigned char;

enum ENaluType
{
	ENT_None		= 0,
	ENT_Slice		= 1,	// 帧被分成的片
	ENT_DPA			= 2,
	ENT_DPB			= 3,
	ENT_DPC			= 4,
	ENT_IDR			= 5,	// 关键帧
	ENT_SEI			= 6,
	ENT_SPS			= 7,	// 序列参数集
	ENT_PPS			= 8,	// 图像参数集
	ENT_AUD			= 9,
	ENT_EOSEQ		= 10,
	ENT_EOSTREAM	= 11,
	ENT_FILL		= 12,
};

// 指定nalu重要性
enum ENaluPriority
{
	ENP_Disposable	= 0,
	ENP_Low			= 1,
	ENP_High		= 2,
	ENP_Highest		= 3,
};

// 用于存储NALU的结构体
struct SNALU
{
	int				iStartCodeLen;	// 起始码，如果是一帧的第一个slice则为4，其他的都为3
	unsigned int	iLen;			// NALU单元的总体长度，包括起始码
	unsigned int	iMaxSize;		// Nalu Buffer size
	int				iForbidden;		// 永远为false
	int				iPriority;		// 优先级
	int				iType;			// 类型
	byte*			pBuf;			// 数据
};

// 判断是否是起始码0x000001
bool IsStartCodeOne(unsigned char* pBuf);
// 判断是否是起始码0x00000001
bool IsStartCodeTwo(unsigned char* pBuf);

/**
 * 从h264码流数据解析出NALU
 *
 * @param pNalu	由外界传入的指针，用于存储搜索到的NAL
 * @param pFile	h264码流文件
 * @return		返回NALU大小，单位字节，-1代表失败
 */
int	GetNextNALU(SNALU* pNalu, FILE* pFile);

/**
 * 解析h264码流并打印相关信息
 * @param pUrl	h264码流文件的地址
 */
int	ParseH264Data(const char* pUrl = nullptr);