#pragma once

/**
 * 雷博士给的AAC码流解析的代码，相比之前的代码，注释要少的多
 * 所以看起来相对难懂一些，可能是当时写的时候太累了，我这里尽量
 * 根据自己的理解加注释
 */

/**
 * 从某段码流中获取adts帧
 *
 * @param pBuf			码流缓冲区
 * @param iBufSize		缓冲区大小
 * @param pFrame		存储ADTS的缓冲区
 * @param pFrameSize	ADTS帧大小，需要返还给调用方
 * @return				0代表成功，其他代表失败，-1代表当前缓冲区没有同步字，1代表当前的frame还有一部分数据在pBuf之外
 */
int	GetADTSFrame(unsigned char* pBuf, int iBufSize, unsigned char* pFrame, int* pFrameSize);

/**
 * 解析AAC码流（ADTS）
 *
 * @param pUrl	资源路径
 * @return		-1代表失败,0代表成功
 */
int	ParseAacData(const char* pUrl = nullptr);