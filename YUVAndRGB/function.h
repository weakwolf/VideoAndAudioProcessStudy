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


/**
 * rgb24ͼƬתΪyuv420p
 *
 * @param pIn		ָ��Դ�ļ���ַ
 * @param nWidth	ͼƬ���
 * @param nHeight	ͼƬ�߶�
 * @param pOut		���·��
 * @param nNum		��Ҫ�����֡����Ĭ��Ϊ1
 */
void RGB24ToYUV420P(const char* pIn, int nWidth, int nHeight, const char* pOut, int num = 1);
// �ü�����
unsigned char ClipValue(unsigned char x, unsigned char minVal, unsigned char maxVal);
// RGB24 to YUV420P
bool RGB24ToYUV420P(unsigned char* pRgb, int nWidth, int nHeight, unsigned char* pYuv);
