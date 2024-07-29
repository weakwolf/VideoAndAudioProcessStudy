#include "function.h"

#include "GlobalDefine.h"

int main(int argc, char* argv[])
{
#ifdef TEST_ONE
	SplitYUV420P("..\\Resource\\wcr_yuv_420p.yuv", 1920, 1080);
#endif

#ifdef TEST_TWO
	HalfLuminance("..\\Resource\\wcr_yuv_420p.yuv", 1920, 1080);
#endif

#ifdef TEST_THREE
	SplitRGB24("..\\Resource\\wcr_yuv_420p.yuv", 1920, 1080);
#endif

	return 0;
}