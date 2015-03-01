#include "ZSPlugin.h"




//--------Global Functions------------
NPError NS_PluginInitialize()
{
	return NPERR_NO_ERROR;
}
void NS_PluginShutdown()
{

}
nsPluginInstanceBase* NS_NewPluginInstance(nsPluginCreateData* aCreateDataStruct)
{
	if(!aCreateDataStruct)
		return NULL;
	CZSPlugin* plugin=new CZSPlugin(aCreateDataStruct->instance);
	return plugin;
}
void NS_DestroyPluginInstance(nsPluginInstanceBase* aPlugin)
{
	if(aPlugin)
		delete (CZSPlugin*)aPlugin;
}

CZSPlugin::CZSPlugin(NPP pNPInstance):nsPluginInstanceBase(),m_pNPInstance(pNPInstance),m_bInitialized(FALSE)
{
}

CZSPlugin::~CZSPlugin(void)
{
}

NPBool CZSPlugin::init(NPWindow* pNPWindow)
{
	m_bInitialized=TRUE;
	return TRUE;
}
void  CZSPlugin::shut()
{
	m_bInitialized=FALSE;

}
NPBool  CZSPlugin::isInitialized()
{
	return m_bInitialized;
}