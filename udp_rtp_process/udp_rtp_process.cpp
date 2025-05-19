
#include "function.h"

int main()
{
	// 使用ffmpeg -re -i sintel.ts -f mpegts udp://127.0.0.1:8880推流
	ParserRTPMpegTS(8880);
}
