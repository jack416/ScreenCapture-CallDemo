#pragma once
#include "pluginbase.h"


class CZSPlugin :
	public nsPluginInstanceBase
{
private:
	NPP m_pNPInstance;
	NPBool m_bInitialized;
public:
	CZSPlugin(NPP pNPInstance);
	~CZSPlugin(void);

	NPBool init(NPWindow* pNPWindow);
	//{
	//	m_bInitialized=TRUE;
	//	return TRUE;
	//}
	void shut();
	//{
	//	m_bInitialized=FALSE;

	//}
	NPBool isInitialized();
	//{
	//	return m_bInitialized;
	//}
};
