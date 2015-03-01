// CActiveXWrapper.h : CCActiveXWrapper 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>
#include "ActiveXWrapper.h"
#include "_ICActiveXWrapperEvents_CP.h"
#include <string>
using std::string;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif


// CCActiveXWrapper
class ATL_NO_VTABLE CCActiveXWrapper :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICActiveXWrapper, &IID_ICActiveXWrapper, &LIBID_ActiveXWrapperLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CCActiveXWrapper>,
	public IOleControlImpl<CCActiveXWrapper>,
	public IOleObjectImpl<CCActiveXWrapper>,
	public IOleInPlaceActiveObjectImpl<CCActiveXWrapper>,
	public IViewObjectExImpl<CCActiveXWrapper>,
	public IOleInPlaceObjectWindowlessImpl<CCActiveXWrapper>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CCActiveXWrapper>,
	public CProxy_ICActiveXWrapperEvents<CCActiveXWrapper>,
	public IPersistStorageImpl<CCActiveXWrapper>,
	public ISpecifyPropertyPagesImpl<CCActiveXWrapper>,
	public IQuickActivateImpl<CCActiveXWrapper>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CCActiveXWrapper>,
#endif
	public IProvideClassInfo2Impl<&CLSID_CActiveXWrapper, &__uuidof(_ICActiveXWrapperEvents), &LIBID_ActiveXWrapperLib>,
	public IObjectSafetyImpl<CCActiveXWrapper, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComCoClass<CCActiveXWrapper, &CLSID_CActiveXWrapper>,
	public CComControl<CCActiveXWrapper>
{
public:


	CCActiveXWrapper();

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_CACTIVEXWRAPPER)


BEGIN_COM_MAP(CCActiveXWrapper)
	COM_INTERFACE_ENTRY(ICActiveXWrapper)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CCActiveXWrapper)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CCActiveXWrapper)
	CONNECTION_POINT_ENTRY(__uuidof(_ICActiveXWrapperEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CCActiveXWrapper)
	CHAIN_MSG_MAP(CComControl<CCActiveXWrapper>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// 处理程序原型:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_ICActiveXWrapper,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ICActiveXWrapper
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
	STDMETHOD(Test)(void);


public:
	STDMETHOD(TestControl)(void);
	string GetDllPath();
};

OBJECT_ENTRY_AUTO(__uuidof(CActiveXWrapper), CCActiveXWrapper)
