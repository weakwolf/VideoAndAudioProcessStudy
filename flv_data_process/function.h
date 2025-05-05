#pragma once

using byte = unsigned char;
using uint = unsigned int;

// 结构体按照1字节对齐，这是为了和flv格式对应
#pragma pack(1)
// FLV头部
struct SFLVHeader
{
	byte signature[3];		// FLV签名，其实就是代表FLV三个字母的字节，注意结构体中的数组是一块连续的内存空间
	byte version;			// 版本
	byte flages;			// 标志位，前5位保留，第6位表示是否有音频tag，第7位保留，第8位表示是否有视频tag
	byte iDataOffset[4];	// 头部占用的字节数，3+1+1+4=9
};
// Tag头部
struct STagHeader
{
	byte tagType;		// tag类型
	byte dataSize[3];	// tag大小
	byte timeStamp[3];	// tag的时间戳
	uint iReserved;		// 保留字段
};
#pragma pack()

/**
 * 解析flv文件
 *
 * @param pUrl		输入文件地址
 * @param pAudioOut	输出的音频数据的保存地址，如果为空代表不保存
 * @param pVideoOut	输出的视频数据的保存地址，如果为空代表不保存
 * @return			-1失败，0成功
 */
int	PasrseFLV(const char* pUrl = nullptr,const char* pAudioOutUrl = nullptr,const char* pVideoOutUrl = nullptr);