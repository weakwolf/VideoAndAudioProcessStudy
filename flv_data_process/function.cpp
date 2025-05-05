#include "function.h"

#include <cstdio>
#include <cstring>

#define TAG_TYPE_SCRIPT	18	// script tag
#define TAG_TYPE_AUDIO	8	// audio tag
#define TAG_TYPE_VIDEO	9	// video tag

int	PasrseFLV(const char* pUrl, const char* pAudioOutUrl, const char* pVideoOutUrl)
{
	if (nullptr == pUrl) 
		return -1;

	FILE* pIn = fopen(pUrl, "rb");
	if (nullptr == pIn) return -1;
	FILE* pAudioOut = pAudioOutUrl?fopen(pAudioOutUrl, "wb+"):nullptr;
	FILE* pVideoOut = pVideoOutUrl?fopen(pVideoOutUrl, "wb+"):nullptr;

	// 解析flv头部字段
	SFLVHeader flvHeader;
	fread(&flvHeader, 1, sizeof(flvHeader), pIn);
	printf("============== FLV Header ==============\n");
	printf("Signature:	%c%c%c\n", flvHeader.signature[0], flvHeader.signature[1], flvHeader.signature[2]);
	printf("Version:	%X\n", flvHeader.version);// %X表示以大写十六进制表示
	printf("Has Audio?:	%s\n", (flvHeader.flages & 0x04)?"yes":"no");
	printf("Has Video?:	%s\n", (flvHeader.flages & 0x01) ? "yes" : "no");
	printf("HeaderSize:	%X\n", flvHeader.iDataOffset[3]);
	printf("========================================\n");


	//fseek(pIn, flvHeader.iDataOffset[3], SEEK_SET);


	// 解析每一个tag
	int iPreSize = 0;
	STagHeader tagHeader;
	int iTagDataSize = 0;
	int iTagTimeStamp = 0;
	int iTagSize = sizeof(STagHeader);
	do 
	{
		// 读取前一个tag的大小，四字节
		iPreSize = _getw(pIn);
#if 0
		printf("%d", iPreSize);
#endif
		
		fread(&tagHeader, 1, iTagSize, pIn);

		// 解析tagdata大小和时间戳
		iTagDataSize = 0;
		iTagDataSize |= (tagHeader.dataSize[0] << 16);
		iTagDataSize |= (tagHeader.dataSize[1] << 8);
		iTagDataSize |= (tagHeader.dataSize[2]);
		iTagTimeStamp = 0;
		iTagTimeStamp |= (tagHeader.timeStamp[0] << 16);
		iTagTimeStamp |= (tagHeader.timeStamp[1] << 8);
		iTagTimeStamp |= (tagHeader.timeStamp[2]);
		char tagType[10] = { 0 };
		switch (tagHeader.tagType)
		{
		case TAG_TYPE_AUDIO:sprintf(tagType, "AUDIO"); break;
		case TAG_TYPE_VIDEO:sprintf(tagType, "VIDEO"); break;
		case TAG_TYPE_SCRIPT:sprintf(tagType, "SCRIPT"); break;
		default:sprintf(tagType, "UNKNOWN"); break;
		}
		printf("[%6s] %6d %6d", tagType, iTagDataSize, iTagTimeStamp);
		
		switch (tagHeader.tagType)
		{
		case TAG_TYPE_AUDIO:
		{
			// 解析音频帧参数，总共8bit
			char audioTag[100] = { 0 };
			strcat(audioTag, "| ");
			byte audioInfo = fgetc(pIn);

			// 音频编码类型，4bit
			int iCodeType = (audioInfo & 0xf0) >> 4;
			switch (iCodeType)
			{
			case 0:strcat(audioTag, "Linear PCM, platform endian"); break;
			case 1:strcat(audioTag, "ADPCM"); break;
			case 2:strcat(audioTag, "MP3"); break;
			case 3:strcat(audioTag, "Linear PCM, little endian"); break;
			case 4:strcat(audioTag, "Nellymoser 16-kHz mono"); break;
			case 5:strcat(audioTag, "Nellymoser 8-kHz mono"); break;
			case 6:strcat(audioTag, "Nellymoser"); break;
			case 7:strcat(audioTag, "G.711 A-law logarithmic PCM"); break;
			case 8:strcat(audioTag, "G.711 mu-law logarithmic PCM"); break;
			case 9:strcat(audioTag, "reserved"); break;
			case 10:strcat(audioTag, "AAC"); break;
			case 11:strcat(audioTag, "Speex"); break;
			case 14:strcat(audioTag, "MP3 8-Khz"); break;
			case 15:strcat(audioTag, "Device-specific sound"); break;
			default:strcat(audioTag, "UNKNOWN"); break;
			}
			strcat(audioTag, "| ");

			// 采样率，2bit
			int iSampleRate = (audioInfo & 0x0c) >> 2;
			switch (iSampleRate)
			{
			case 0:strcat(audioTag, "5.5-kHz"); break;
			case 1:strcat(audioTag, "1-kHz"); break;
			case 2:strcat(audioTag, "22-kHz"); break;
			case 3:strcat(audioTag, "44-kHz"); break;
			default:strcat(audioTag, "UNKNOWN"); break;
			}
			strcat(audioTag, "| ");

			// 精度，1bit
			int iDepth = (audioInfo & 0x02) >> 1;
			switch (iDepth)
			{
			case 0:strcat(audioTag, "8Bit"); break;
			case 1:strcat(audioTag, "16Bit"); break;
			default:strcat(audioTag, "UNKNOWN"); break;
			}
			strcat(audioTag, "| ");

			// 类型，1bit
			int iChannels = audioInfo & 0x01;
			switch (iChannels)
			{
			case 0:strcat(audioTag, "Mono"); break;// 单声道
			case 1:strcat(audioTag, "Stereo"); break;// 立体声，左右声道
			default:strcat(audioTag, "UNKNOWN"); break;
			}

			printf("%s\n", audioTag);

			// 减去已经读取的存储元数据的一个字节
			iTagDataSize -= 1;
			if (nullptr != pAudioOut)
			{
				while (iTagDataSize--)
				{
					fputc(fgetc(pIn), pAudioOut);
				}
			}
			else
			{
				fseek(pIn, iTagDataSize, SEEK_CUR);
			}

			break;
		}
		case TAG_TYPE_VIDEO:
		{
			// 解析视频参数，8bit
			char videoTag[100] = { 0 };
			strcat(videoTag, "| ");
			byte videoInfo = fgetc(pIn);
			
			// 帧类型
			int iFrameType = (videoInfo & 0xf0) >> 4;
			switch (iFrameType)
			{
			case 1:strcat(videoTag, "key frame  "); break;
			case 2:strcat(videoTag, "inter frame"); break;
			case 3:strcat(videoTag, "disposable inter frame"); break;
			case 4:strcat(videoTag, "generated keyframe"); break;
			case 5:strcat(videoTag, "video info/command frame"); break;
			default:strcat(videoTag, "UNKNOWN"); break;
			}
			strcat(videoTag, "| ");

			// 视频编码类型
			int iCodeType = (videoInfo & 0x0f);
			switch (iCodeType)
			{
			case 1:strcat(videoTag, "JPEG (currently unused)"); break;
			case 2:strcat(videoTag, "Sorenson H.263"); break;
			case 3:strcat(videoTag, "Screen video"); break;
			case 4:strcat(videoTag, "On2 VP6"); break;
			case 5:strcat(videoTag, "On2 VP6 with alpha channel"); break;
			case 6:strcat(videoTag, "Screen video version 2"); break;
			case 7:strcat(videoTag, "AVC"); break;
			default:strcat(videoTag, "UNKNOWN"); break;
			}
			strcat(videoTag, "| ");
			printf("%s\n", videoTag);

			iTagDataSize -= 1;
			if (nullptr != pVideoOut)
			{
				while (iTagDataSize--)
				{
					fputc(fgetc(pIn), pVideoOut);
				}
			}
			else
			{
				fseek(pIn, iTagDataSize, SEEK_CUR);
			}

			break;
		}
		case TAG_TYPE_SCRIPT:
		{
			printf("\n");
			fseek(pIn, iTagDataSize, SEEK_CUR);

			break;
		}
		}

	} while (!feof(pIn));

	if(nullptr != pIn) fclose(pIn);
	if (nullptr != pVideoOut) fclose(pVideoOut);
	if (nullptr != pAudioOut) fclose(pAudioOut);

	return 0;
}