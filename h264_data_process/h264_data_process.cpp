// h264_data_process.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include <cstdio>
#include "function.h"

int main(int argc, char* argv[])
{
	const char* pUrl = "..\\Resource\\sintel.h264";

	FILE* pIn = fopen(pUrl, "rb");
	if (nullptr == pIn)
	{
		fclose(pIn);

		return -1;
	}

	ParseH264Data(pUrl);

	system("pause");
}
