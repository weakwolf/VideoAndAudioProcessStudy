#pragma once

/**
 * ��һ��yuv420p��ʽͼƬ��YUV��������Ϊ����ͼƬ��ʹ�ô�Cʵ��
 *
 * @param pUrl		ָ��Դ�ļ���ַ
 * @param nWidth	ͼƬ���
 * @param nHeight	ͼƬ�߶�
 * @param nNum		��Ҫ�����֡����Ĭ��Ϊ1
 */
void SplitYUV420P(const char* pUrl, int nWidth, int nHeight, int nNum = 1);

