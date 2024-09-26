
// audio_capture_testDlg.h: 头文件
//

#pragma once

#include <vector>


// CaudiocapturetestDlg 对话框
class CaudiocapturetestDlg : public CDialogEx
{
// 构造
public:
	CaudiocapturetestDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CaudiocapturetestDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUDIO_CAPTURE_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void	InitComboBox();
	void	InitSDL();
	void	UnInitSDL();
	void	EnumDevice();

private:
	CComboBox					m_deviceList;
	std::vector<std::string>	m_devices;
	SDL_AudioSpec				m_audioSpec;	// 当前音频设备描述
	SDL_AudioDeviceID			m_deviceId;		// 当前音频设备ID

public:
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonStart();

public:
	static void OnSdlCallback(void* pUserdata, Uint8* pStream, int nLen);

public:
	void DoSdlCallback(Uint8* pStream, int nLen);
	afx_msg void OnBnClickedButtonEnd();
};
