#include "function.h"

#include <cstdio>

#include <cstdlib>

int main()
{
	// 使用雷神给的flv文件出来的结果前面的帧是正确的，到后面视频帧就有点问题了
	// 但是我没看出这个视频有什么问题，暂时先不管吧，重点是理解flv格式
	PasrseFLV("..\\Resource\\cuc_ieschool.flv");

	system("pause");

	return 0;
}
