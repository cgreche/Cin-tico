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
		LPD3DXFONT m_currentFont;

		void updateInternalResourceData(ResourceData *data);
		void updateInternalResourceInstanceData(ResourceInstance *instance);
		void updateInternalCamera(Camera *camera);
		void updateInternalViewport(Viewport *viewport);

		void setViewFromCamera(Camera *camera);

		std::vector<Adapter> m_adapterList;
		std::vector<DisplayMode> m_displayModeList;

	protected:
		virtual void *newInternalResource(ResourceData *resData);
		virtual void releaseInternalResource(ResourceData *resData);
		virtual void *newInternalCamera(Camera *camera);
		virtual void releaseInternalCamera(Camera *camera);
		virtual void *newInternalViewport(Viewport *viewport);
		virtual void releaseInternalViewport(Viewport *camera);
		virtual void *newInternalResourceInstance(ResourceInstance *instance);
		virtual void releaseInternalResourceInstance(ResourceInstance *instance);
		virtual void *newInternalFontResource(FontResource *font);
		virtual void releaseInternalFontResource(FontResource *font);
		virtual void *newInternalTextResource(TextResource *text);
		virtual void releaseInternalTextResource(TextResource *text);
		virtual void drawInternalResource(ResourceInstance *resData);
		virtual void drawText(const char *text, int x, int y, const Color &color);
		virtual void drawResourceDirect(render3d::Vector3 vertices[], int vertexCount, render3d::Color colorList[]); //temp
		virtual void printScreen(const char *fileName); //temp
		virtual void setCurrentInternalCamera(Camera *camera);
		virtual void setCurrentInternalViewport(Viewport *viewport);
		virtual void setCurrentInternalFont(FontResource *font);

	public:
		D3D9Engine();
		~D3D9Engine();
		virtual void configure(void *options);
		virtual void init();
		virtual void destroy();
		virtual void beginScene();
		virtual void endScene();
		virtual void clear(const Color &clearColor);

		virtual std::vector<Adapter> getAdapterList() { return m_adapterList; }
		virtual std::vector<DisplayMode> getDisplayModeList() { return m_displayModeList; }
	};

}

#endif