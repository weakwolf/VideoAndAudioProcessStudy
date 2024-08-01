#pragma once

/**
 * 将一张yuv420p格式图片的YUV分量分离为三张图片
 *
 * @param pUrl		指向源文件地址
 * @param nWidth	图片宽度
 * @param nHeight	图片高度
 * @param nNum		需要处理的帧数，默认为1
 */
void SplitYUV420P(const char* pUrl, int nWidth, int nHeight, int nNum = 1);


/**
 * 将一张yuv420p格式图片的亮度减半
 *
 * @param pUrl		指向源文件地址
 * @param nWidth	图片宽度
 * @param nHeight	图片高度
 * @param nNum		需要处理的帧数，默认为1
 */
void HalfLuminance(const char* pUrl, int nWidth, int nHeight, int nNum = 1);


/**
 * 分离rgb24图片的分量
 *
 * @param pUrl		指向源文件地址
 * @param nWidth	图片宽度
 * @param nHeight	图片高度
 * @param nNum		需要处理的帧数，默认为1
 */
void SplitRGB24(const char* pUrl, int nWidth, int nHeight, int nNum = 1);


/**
 * rgb24图片转为yuv420p
 *
 * @param pIn		指向源文件地址
 * @param nWidth	图片宽度
 * @param nHeight	图片高度
 * @param pOut		输出路径
 * @param nNum		需要处理的帧数，默认为1
 */
void RGB24ToYUV420P(const char* pIn, int nWidth, int nHeight, const char* pOut, int num = 1);
// 裁剪数据
unsigned char ClipValue(unsigned char x, unsigned char minVal, unsigned char maxVal);
// RGB24 to YUV420P
bool RGB24ToYUV420P(unsigned char* pRgb, int nWidth, int nHeight, unsigned char* pYuv);
