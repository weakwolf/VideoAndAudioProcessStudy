
// audio_capture_test.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号


/**
 * 此程序是为了使用SDL来获取原始音频数据，仅此而已
 */

// CAudioCaptureTestApp:
// 有关此类的实现，请参阅 audio_capture_test.cpp
//

class CAudioCaptureTestApp : public CWinApp
{
public:
	CAudioCaptureTestApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CAudioCaptureTestApp theApp;
