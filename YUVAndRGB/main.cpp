#include "function.h"

int main(int argc, char* argv[])
{
	// YUV420P�ֽ�YUV����
	SplitYUV420P("..\\Resource\\wcr_yuv_420p.yuv", 1920, 1080);

	return 0;
}