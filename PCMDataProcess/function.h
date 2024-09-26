#pragma once

/**
 * 分离PCM-16位有符号整数-双声道音频采样数据的左声道和右声道
 *
 * @param pUrl	文件路径
 */
void SplitPcm16LE(const char* pUrl);
