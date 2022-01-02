//========= Copyright(c) Valve Corporation, All rights reserved. ============//

#include "itooldictionary.h"
#include "../tier1/utlvector.h"

class CToolDictionary : public IToolDictionary
{
public:
	virtual int	GetToolCount() const
	{
		return m_Tools.Count();
	}

	virtual IToolSystem* GetTool(int index)
	{
		if (index < 0 || index >= m_Tools.Count())
		{
			return NULL;
		}

		return m_Tools[index];
	}

public:
	void RegisterTool(IToolSystem* tool)
	{
		m_Tools.AddToTail(tool);
	}

private:
	CUtlVector<IToolSystem*> m_Tools;
};

//inline CToolDictionary g_ToolDictionary;

//EXPOSE_SINGLE_INTERFACE_GLOBALVAR(IToolDictionary, CToolDictionary, VTOOLDICTIONARY_INTERFACE_VERSION, g_ToolDictionary);

//void RegisterTool(IToolSystem* tool)
//{
//	g_ToolDictionary.RegisterTool(tool);
//}