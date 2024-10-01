#pragma once

/**
 * ����PCM-16λ�з�������-˫������Ƶ�������ݵ���������������
 *
 * @param pUrl	�ļ�·��
 */
void SplitPcm16LE(const char* pUrl);

/**
 * ��PCM-16-�з�������-˫������Ƶ�������ݵ���������������һ��
 *
 * @param pUrl	�ļ�·��
 */
void HalfPcm16Volume(const char* pUrl);

/**
 * ��PCM-16-�з�������-˫������Ƶ�������ݵ������ٶ����һ��
 *
 * @param pUrl	�ļ�·��
 */
void DoublePcm16Speed(const char* pUrl);

/**
 * ��PCM-16-�з�������-˫������Ƶ��������ת��ΪPCM8-�޷�������
 *
 * @param pUrl	�ļ�·��
 */
void Pcm16ToPcm8(const char* pUrl);

/**
 * ��ԭʼ��Ƶ���ݷ�װΪwav��Ƶ�ļ�
 *
 * @param pUrl			ԭʼ��Ƶ�ļ�·��
 * @param pOut			�����wave�ļ�·��
 * @param nSampleRate	������
 * @param nChannels		ͨ����
 * @param nSampleDeep	����λ��
 * @param nAudioFormat	��Ƶ��ʽ��1����pcm
 */
void WavePcmData(const char* pUrl, const char* pOut, int nSampleRate = 44100, int nChannels = 2, int nSampleDeep = 16, int nAudioFormat = 1);