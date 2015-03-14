// CaptureTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CaptureTest.h"
#include "CaptureTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4482)
#pragma warning(disable:4311)
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCaptureTestDlg 对话框

typedef int (*FnStartScreenCapture)(const char* szAuth, const char* szDefaultSavePath, void* pCallBack, unsigned long hWndNotice, unsigned int noticeMsg);
FnStartScreenCapture gl_StartScreenCapture = NULL;
typedef int (*FnInitScreenCapture)(unsigned long trackerColor, unsigned long editBorderColor, int nTransparent, int flag);
FnInitScreenCapture gl_InitCapture = NULL;

typedef int (*FnInitCaptureParam)(int flag, unsigned long flagvalue);
FnInitCaptureParam gl_InitCaptureParam = NULL;

//此接口无用 
typedef int (*FnSetExtendFlag)(int flag, int flagvalue);
FnSetExtendFlag gl_SetExtendFlag = NULL;

typedef enum ExtendFlagTypeEnum
{
	emPensize = 1,		//设置画笔大小
	emDrawType,			//设置是腾讯风格还是360风格
	emTrackColor,		//自动识别的边框的颜色
	emEditBorderColor,	//文本输入的边框颜色
	emTransparent,		//工具栏的透明度
	emWindowAware,		//设置是否禁用随着DPI放大
	emDetectSubWindowRect,	//是否自动检测子窗口，暂时无用 
	emSetSaveName,		//设置保存时的开始文字
	emSetMagnifierBkColor, //设置放大镜的背景色，不设置则透明
	emSetMagnifierLogoText, //设置放大镜上的LOGO字符，可提示快捷键，如： 牛牛截图(CTRL + SHIFT + A)
};

CCaptureTestDlg::CCaptureTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptureTestDlg::IDD, pParent)
{
	//加载截图控件
	m_hModule = LoadLibrary("NiuniuCapture.dll");
	gl_StartScreenCapture = (FnStartScreenCapture)GetProcAddress(m_hModule, "StartScreenCapture");
	gl_InitCapture = (FnInitScreenCapture)GetProcAddress(m_hModule, "InitScreenCapture");
	gl_InitCaptureParam = (FnInitCaptureParam)GetProcAddress(m_hModule, "InitCaptureParam");


	//设置截图边框的样式【为兼容老版本，新版本可不这样调用 】  
	//gl_InitCapture(RGB(255, 0, 0), RGB(0, 174, 255), 230, 2);	//最后一个参数设置画笔线宽

	
	gl_InitCaptureParam(ExtendFlagTypeEnum::emPensize, 2);	//画笔线宽
	gl_InitCaptureParam(ExtendFlagTypeEnum::emDrawType, 0);	//设置放大镜风格：0： 腾讯风格   1： 360风格 
	gl_InitCaptureParam(ExtendFlagTypeEnum::emTrackColor, RGB(255, 0, 0));	//自动识别的边框颜色
	gl_InitCaptureParam(ExtendFlagTypeEnum::emEditBorderColor, RGB(0, 174, 255));	//文字编辑框边框颜色
	gl_InitCaptureParam(ExtendFlagTypeEnum::emTransparent, 240); //设置工具栏窗口透明度

	const char* szSavePath = "牛牛截图";
	gl_InitCaptureParam(ExtendFlagTypeEnum::emSetSaveName, (unsigned long)szSavePath); //设置保存时的开始文字
	gl_InitCaptureParam(ExtendFlagTypeEnum::emSetMagnifierBkColor, RGB(255, 255, 255)); //设置放大镜的背景色，不设置则透明
	
	//以下可以设置放大镜上的LOGO文字，如果不设置，默认显示“牛牛截图” 
	//gl_InitCaptureParam(ExtendFlagTypeEnum::emSetMagnifierLogoText, (unsigned long)"牛牛截图(Ctrl+Shift+A)");
	gl_InitCaptureParam(ExtendFlagTypeEnum::emSetMagnifierLogoText, (unsigned long)"  可通过接口设置名称");
	
	gl_InitCaptureParam(ExtendFlagTypeEnum::emWindowAware, 1); //此函数必需窗口创建前调用，其等同于如下代码
	//通过这段代码调用后，应用程序将不随DPI进行放大，在设置了DPI放大的机器上，需要调用此API； 一定要在窗口创建前进行调用，建议放在应用程序最开始初始化的地方 
	/*
	HINSTANCE hUser32 = LoadLibrary( "user32.dll" );
	if( hUser32 )
	{
		typedef BOOL ( WINAPI* LPSetProcessDPIAware )( void );
		LPSetProcessDPIAware pSetProcessDPIAware = ( LPSetProcessDPIAware )GetProcAddress(hUser32,
			"SetProcessDPIAware" );
		if( pSetProcessDPIAware )
		{
			pSetProcessDPIAware();
		}
		FreeLibrary( hUser32 );
	}
	*/

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
}

void CCaptureTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCaptureTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER + 1111,OnCaptureFinish)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCaptureTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CCaptureTestDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CCaptureTestDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()



// CCaptureTestDlg 消息处理程序

BOOL CCaptureTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CenterWindow();
	ShowWindow(SW_SHOW);

	CButton* btn = (CButton*)GetDlgItem(IDC_RADIO1);
	btn->SetCheck(TRUE);
	
	

	SetWindowText("牛牛截图");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCaptureTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCaptureTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCaptureTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HWND gl_hWnd = NULL;
void CaptureNotice(int nType)
{
	::PostMessage(gl_hWnd, WM_USER + 1111, 1, 1);
}

void CCaptureTestDlg::OnBnClickedOk()
{
	SetDlgItemText(IDC_STATIC_NOTICE, "");

	/*
	参数1的定义如下 
	typedef enum ExtendFlagTypeEnum
	{
	emPensize = 1,
	emDrawType,	
	};
	*/
	CButton* btn = (CButton*)GetDlgItem(IDC_RADIO2);
	BOOL bCheck = btn->GetCheck();

	gl_InitCaptureParam(2, bCheck);	//参数2的意义，0： 腾讯风格   1： 360风格 

	//通过回调函数来通知截图完成事件
	gl_hWnd = m_hWnd;
	gl_StartScreenCapture("niuniu", "", CaptureNotice, 0, 0);

	//通过 WINDOWS 消息来通知截图完成事件
	//gl_StartScreenCapture("niuniu", "", NULL, (unsigned long)m_hWnd, WM_USER + 1111);
}


LRESULT CCaptureTestDlg::OnCaptureFinish(WPARAM wParam,LPARAM lParam)
{
	if (wParam == lParam && wParam == 1)
	{
		SetDlgItemText(IDC_STATIC_NOTICE, "截图完毕");
	}	
	return 0;
}

void CCaptureTestDlg::OnBnClickedRadio1()
{
	CButton* btn = (CButton*)GetDlgItem(IDC_RADIO1);
	btn->SetCheck(TRUE);

	btn = (CButton*)GetDlgItem(IDC_RADIO2);
	btn->SetCheck(FALSE);
}

void CCaptureTestDlg::OnBnClickedRadio2()
{
	CButton* btn = (CButton*)GetDlgItem(IDC_RADIO1);
	btn->SetCheck(FALSE);

	btn = (CButton*)GetDlgItem(IDC_RADIO2);
	btn->SetCheck(TRUE);
}
