
// Last edit: 11/10/2013 09:35 (UTC-3)

#include "uibase.h"

namespace uilib {

	UIObj::UIObj() : m_osdRef(*new OSDUIObj(*this))
	{
		uibase::UIInit(); //ensure it is init
		osdRef().create();
	}

	UIObj::UIObj(OSDUIObj& osdRef) : m_osdRef(osdRef)
	{
		uibase::UIInit(); //ensure it is init
	}

	UIObj::~UIObj()
	{
		osdRef().destroy();
		delete &m_osdRef;
	}

	void UIObj::update()
	{
		osdRef().update();
	}





	OSDUIObj& UIObj::osdRef() const
	{
		return m_osdRef;
	}

}