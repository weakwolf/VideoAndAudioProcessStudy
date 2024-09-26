
// audio_capture_testDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "audio_capture_test.h"
#include "audio_capture_testDlg.h"
#include "afxdialogex.h"

#include <fstream>
#include <string>

#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef DEBUG
#pragma comment(lib,"SDL2d.lib")
#pragma comment(lib,"manual-link/SDL2maind.lib")
#else
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"manual-link/SDL2main.lib")
#endif

// CaudiocapturetestDlg 对话框



CaudiocapturetestDlg::CaudiocapturetestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AUDIO_CAPTURE_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CaudiocapturetestDlg::~CaudiocapturetestDlg()
{
	UnInitSDL();
}

void CaudiocapturetestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_deviceList);
}

BEGIN_MESSAGE_MAP(CaudiocapturetestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CaudiocapturetestDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_START, &CaudiocapturetestDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_END, &CaudiocapturetestDlg::OnBnClickedButtonEnd)
END_MESSAGE_MAP()


// CaudiocapturetestDlg 消息处理程序

BOOL CaudiocapturetestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitSDL();
	InitComboBox();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CaudiocapturetestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CaudiocapturetestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CaudiocapturetestDlg::InitComboBox()
{
	OnBnClickedButtonRefresh();
}

void CaudiocapturetestDlg::InitSDL()
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void CaudiocapturetestDlg::UnInitSDL()
{
	if (m_deviceId < 2)
		return;

	SDL_PauseAudioDevice(m_deviceId, SDL_TRUE);
	SDL_CloseAudioDevice(m_deviceId);
	SDL_zero(m_audioSpec);

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	SDL_Quit();
}

void CaudiocapturetestDlg::EnumDevice()
{
	// 清空旧数据
	m_devices.clear();

	int nCount = SDL_GetNumAudioDevices(SDL_TRUE);
	for (int i = 0; i < nCount; ++i)
	{
		// SDL_GetAudioDeviceName返回的是utf-8编码的字符
		std::string szName = SDL_GetAudioDeviceName(i, SDL_TRUE);
		m_devices.push_back(szName);
	}
}



void CaudiocapturetestDlg::OnBnClickedButtonRefresh()
{
	// 清空旧数据
	while (m_deviceList.GetCount() > 0)
	{
		m_deviceList.DeleteString(0);
	}

	EnumDevice();
	for (const auto& item : m_devices)
	{
		m_deviceList.AddString(convert::UTF8ToUnicode16(item));
	}
	m_deviceList.SetCurSel(0);
}


void CaudiocapturetestDlg::OnBnClickedButtonStart()
{
	// 定义预期的音频格式
	SDL_AudioSpec wanted;
	wanted.freq = 44100;
	wanted.channels = 2;
	// 16位有符号整数
	wanted.format = AUDIO_S16LSB;
	wanted.samples = 480;
	wanted.userdata = this;
	wanted.callback = CaudiocapturetestDlg::OnSdlCallback;

	// 打开音频设备
	CString szTemp;
	m_deviceList.GetLBText(m_deviceList.GetCurSel(), szTemp);
	std::string szName = convert::Unicode16ToUtf8(szTemp);
	SDL_zero(m_audioSpec);
	m_deviceId = SDL_OpenAudioDevice(szName.c_str(), SDL_TRUE, &wanted, &m_audioSpec, SDL_AUDIO_ALLOW_SAMPLES_CHANGE);
	SDL_PauseAudioDevice(m_deviceId, SDL_FALSE);
}

void CaudiocapturetestDlg::OnSdlCallback(void* pUserdata, Uint8* pStream, int nLen)
{
	if (nullptr == pUserdata)
		return;

	CaudiocapturetestDlg* pDlg = reinterpret_cast<CaudiocapturetestDlg*>(pUserdata);
	pDlg->DoSdlCallback(pStream, nLen);
}

void CaudiocapturetestDlg::DoSdlCallback(Uint8* pStream, int nLen)
{
#if 1
	char* pBuf = (char*)pStream;
	std::ofstream os;
	os.open("E://temp//audio_data.pcm", std::ofstream::binary | std::ofstream::app);
	os.write(pBuf, nLen);
	os.close();
#endif
}


void CaudiocapturetestDlg::OnBnClickedButtonEnd()
{
	if (m_deviceId < 2)
		return;

	SDL_PauseAudioDevice(m_deviceId, SDL_TRUE);
	SDL_CloseAudioDevice(m_deviceId);
	SDL_zero(m_audioSpec);
}
