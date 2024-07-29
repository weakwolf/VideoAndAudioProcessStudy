#pragma once

/**
 * ��һ��yuv420p��ʽͼƬ��YUV��������Ϊ����ͼƬ
 *
 * @param pUrl		ָ��Դ�ļ���ַ
 * @param nWidth	ͼƬ���
 * @param nHeight	ͼƬ�߶�
 * @param nNum		��Ҫ�����֡����Ĭ��Ϊ1
 */
void SplitYUV420P(const char* pUrl, int nWidth, int nHeight, int nNum = 1);


/**
 * ��һ��yuv420p��ʽͼƬ�����ȼ���
 *
 * @param pUrl		ָ��Դ�ļ���ַ
 * @param nWidth	ͼƬ���
 * @param nHeight	ͼƬ�߶�
 * @param nNum		��Ҫ�����֡����Ĭ��Ϊ1
 */
void HalfLuminance(const char* pUrl, int nWidth, int nHeight, int nNum = 1);


/**
 * ����rgb24ͼƬ�ķ���
 *
 * @param pUrl		ָ��Դ�ļ���ַ
 * @param nWidth	ͼƬ���
 * @param nHeight	ͼƬ�߶�
 * @param nNum		��Ҫ�����֡����Ĭ��Ϊ1
 */
void SplitRGB24(const char* pUrl, int nWidth, int nHeight, int nNum = 1);
