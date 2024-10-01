#include "function.h"
#include "GlobalDefine.h"

int main(int argc, char* argv[])
{
#ifdef TEST_2
	HalfPcm16Volume("..\\Resource\\audio_data.pcm");
#endif

#ifdef TEST_1
	SplitPcm16LE("..\\Resource\\audio_data_half.pcm");
#endif

#ifdef TEST_3
	DoublePcm16Speed("..\\Resource\\audio_data.pcm");
#endif

#ifdef TEST_4
	Pcm16ToPcm8("..\\Resource\\audio_data.pcm");
#endif

#ifdef TEST_5
	WavePcmData("..\\Resource\\audio_data.pcm", "..\\Resource\\test1.wav");
#endif

	return 0;
}