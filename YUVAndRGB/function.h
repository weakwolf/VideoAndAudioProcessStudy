#pragma once

/**
 * 将一张yuv420p格式图片的YUV分量分离为三张图片，使用纯C实现
 *
 * @param pUrl		指向源文件地址
 * @param nWidth	图片宽度
 * @param nHeight	图片高度
 * @param nNum		需要处理的帧数，默认为1
 */
void SplitYUV420P(const char* pUrl, int nWidth, int nHeight, int nNum = 1);

