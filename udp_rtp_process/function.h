#pragma once

#pragma pack(1)

// RTP包固定头信息
// 注意，雷神这里使用位域来声明，又学到一个知识点
struct SRTPFixedHeader
{	
	// byte 1
	unsigned char csrc_lem : 4;
	unsigned char extension : 1;
	unsigned char padding : 1;
	unsigned char version : 2;

	// byte 2
	unsigned char payLoad : 7;
	unsigned char marker : 1;

	// byte 3,4
	unsigned short seqNum;

	// byte 5-8
	unsigned long timeStamp;

	// byte 9-12
	unsigned long ssrc;
};

struct SMPEGTSFixedHeader 
{
	unsigned char syncByte;
	unsigned char transportErrorIndicator : 1;
	unsigned char payloadUnitStartIndicator : 1;
	unsigned char transportPriority : 1;
	unsigned int pid : 13;
	unsigned char scramblingControl : 2;
	unsigned char adaptationFieldExist : 2;
	unsigned char continuityCounter : 4;
};

#pragma pack()

/**
 * 监听特定端口，解析rtp数据
 * 
 * @param iPort	端口号
 * @return		错误码
 */
int ParserRTPMpegTS(int iPort);