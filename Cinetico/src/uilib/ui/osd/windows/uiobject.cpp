
#include "..\\..\\uibase.h"




OSDUIObj::OSDUIObj(UIObj &ref):m_ref(ref)
{
	uibase::UIInit();
	static u32 id = 0;
	m_id = id++;
	OSDBase::objects->push_back(this);
}

OSDUIObj::~OSDUIObj()
{

}




