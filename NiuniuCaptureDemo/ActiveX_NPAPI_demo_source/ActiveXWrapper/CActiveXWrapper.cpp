// CActiveXWrapper.cpp : CCActiveXWrapper 的实现
#include "stdafx.h"
#include "CActiveXWrapper.h"


typedef int (*FnStartScreenCapture)(const char* szAuth, const char* szDefaultSavePath, void* pCallBack, unsigned long hWndNotice, unsigned int noticeMsg);
FnStartScreenCapture gl_StartScreenCapture = NULL;
typedef int (*FnInitScreenCapture)(unsigned long trackerColor, unsigned long editBorderColor, int nTransparent, int flag);
FnInitScreenCapture gl_InitCapture = NULL;

HMODULE m_hModule;

extern HINSTANCE gl_hInstance1;
// CCActiveXWrapper

STDMETHODIMP CCActiveXWrapper::Test(void)
{
	// TODO: 在此添加实现代码
	return S_OK;
}

CCActiveXWrapper::CCActiveXWrapper()
{
	m_bWindowOnly = true;
	string path = GetDllPath() + "\\NiuniuCapture.dll";
	//::MessageBox(NULL, path.c_str(), "1", 0);

	//加载截图控件[在控件中这部分需要注意，定位到DLL的绝对路径后调用才能确保正确]
	m_hModule = LoadLibrary(path.c_str());
	DWORD dret = GetLastError();
	gl_StartScreenCapture = (FnStartScreenCapture)GetProcAddress(m_hModule, "StartScreenCapture");
	gl_InitCapture = (FnInitScreenCapture)GetProcAddress(m_hModule, "InitScreenCapture");

	//设置截图边框的样式  
	gl_InitCapture(RGB(255, 0, 0), RGB(0, 174, 255), 180, 0);
}

string CCActiveXWrapper::GetDllPath()
{
	char szPath[MAX_PATH] = {0};
	::GetModuleFileName(gl_hInstance1, szPath, MAX_PATH);
	int len = strlen(szPath);
	if( len == 0) return "";
	while (len>0)
	{
		if(szPath[len-1] == '\\')
		{	
			szPath[len-1] = '\0';
			break;
		}
		else
		{
			szPath[len-1] = '\0';
			len--;
		}
	}
	return szPath;
}

STDMETHODIMP CCActiveXWrapper::TestControl(void)
{
	//可以添加新的接口，由WEB传入地址，截图后，通过控件自动将图片发往服务器 
	//此处一定要找一个可写的路径来保存截图
	
	gl_StartScreenCapture("niuniu", "", NULL, 0, 0);
	return S_OK;
}
 