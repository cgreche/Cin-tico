#pragma once

#ifndef __RENDER3D_D3D9ENGINE_H__
#define __RENDER3D_D3D9ENGINE_H__

#include "renderengine.h"
#include <d3d9.h>
#include <lib/d3dx9.h>

namespace render3d {

	class D3D9Engine : public RenderEngine
	{
		HWND m_hwnd;
		LPDIRECT3D9 m_d3d9;
		LPDIRECT3DDEVICE9 m_device;

		void updateResourceInstanceData(ResourceInstance *instance);
		void updateCamera(Camera *camera);

		void setViewFromCamera(Camera *camera);

	protected:
		virtual void *newInternalResource(ResourceData *resData);
		virtual void *newInternalCamera(Camera *camera);
		virtual void *newInternalViewport(Viewport *viewport);
		virtual void *newInternalResourceInstance(ResourceInstance *instance);
		virtual void drawInternalResource(ResourceInstance *resData);
		virtual void setCurrentInternalCamera(Camera *camera);
		virtual void setCurrentInternalViewport(Viewport *viewport);

	public:
		D3D9Engine();
		virtual void configure(void *options);
		virtual void init();
		virtual void destroy();
		virtual void beginScene();
		virtual void endScene();
		virtual void clear(const Color &clearColor);
	};

}

#endif