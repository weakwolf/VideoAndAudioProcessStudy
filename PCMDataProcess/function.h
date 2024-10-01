#pragma once

/**
 * 分离PCM-16位有符号整数-双声道音频采样数据的左声道和右声道
 *
 * @param pUrl	文件路径
 */
void SplitPcm16LE(const char* pUrl);

/**
 * 将PCM-16-有符号整数-双声道音频采样数据的左声道音量降低一半
 *
 * @param pUrl	文件路径
 */
void HalfPcm16Volume(const char* pUrl);

/**
 * 将PCM-16-有符号整数-双声道音频采样数据的声音速度提高一倍
 *
 * @param pUrl	文件路径
 */
void DoublePcm16Speed(const char* pUrl);

/**
 * 将PCM-16-有符号整数-双声道音频采样数据转换为PCM8-无符号整数
 *
 * @param pUrl	文件路径
 */
void Pcm16ToPcm8(const char* pUrl);

/**
 * 将原始音频数据封装为wav音频文件
 *
 * @param pUrl			原始音频文件路径
 * @param pOut			输出的wave文件路径
 * @param nSampleRate	采样率
 * @param nChannels		通道数
 * @param nSampleDeep	样本位深
 * @param nAudioFormat	音频格式，1代表pcm
 */
void WavePcmData(const char* pUrl, const char* pOut, int nSampleRate = 44100, int nChannels = 2, int nSampleDeep = 16, int nAudioFormat = 1);