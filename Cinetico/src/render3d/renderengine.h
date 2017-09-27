#pragma once

#ifndef __RENDER3D_RENDERENGINE_H__
#define __RENDER3D_RENDERENGINE_H__

#include <vector>

#include "vector3.h"
#include "vertex3.h"
#include "renderobject.h"
#include "resourcedata.h"
#include "viewport.h"
#include "camera.h"
#include "resourceinstance.h"
#include "fontresource.h"
#include "textresource.h"

namespace render3d {

	class RenderEngine
	{
	protected:
		std::vector<ResourceData*> m_resources;
		std::vector<Camera *> m_cameras;
		std::vector<Viewport *> m_viewports;
		std::vector<ResourceInstance*> m_instances;
		std::vector<FontResource*> m_fontResources;
		std::vector<TextResource*> m_textResources;

		Camera *m_currentCamera;
		Viewport *m_currentViewport;
		FontResource *m_currentFont;

	public:
		RenderEngine();

		virtual void configure(void *) = 0;
		virtual void init() = 0;
		virtual void destroy() = 0;
		virtual void beginScene() = 0;
		virtual void endScene() = 0;

		//Resource allocation
		virtual int newResource(unsigned int vertexCount, Vertex3 *vertices, unsigned int indexCount = 0, int *indices = NULL, Color *colors = NULL);
		virtual int newCamera(const Vector3 &pos, const Vector3 &rot, float zoom = 1.f);
		virtual int newViewport(int x, int y, int width, int height);
		virtual int newResourceInstance(int resDataId);
		virtual int newFontResource(const char *face, int width, int height, unsigned long flags);
		virtual int newTextResource(int x, int y, int width, int height);
		//
		virtual void drawResource(int resInstanceId);
		virtual void setCurrentCamera(int cameraId);
		virtual void setCurrentViewport(int viewportId);
		virtual void setCurrentFont(int fontId);
		virtual void drawText(const char *text, int x, int y, const Color &color);
		virtual void clear(const Color &clearColor) = 0;

		ResourceData *resourceData(int id) const { return m_resources[id]; }
		Camera *camera(int id) const { return m_cameras[id]; }
		Viewport *viewport(int id) const { return m_viewports[id]; }
		ResourceInstance *resourceInstance(int id) const { return m_instances[id]; }

	protected:
		virtual void *newInternalResource(ResourceData *resData) { return NULL; }
		virtual void *newInternalCamera(Camera *camera) { return NULL; }
		virtual void *newInternalViewport(Viewport *viewport) { return NULL; }
		virtual void *newInternalResourceInstance(ResourceInstance *instance) { return NULL; }
		virtual void *newInternalFontResource(FontResource *font) { return NULL; }
		virtual void *newInternalTextResource(TextResource *text) { return NULL; }
		virtual void drawInternalResource(ResourceInstance *instance) = 0;
		virtual void setCurrentInternalCamera(Camera *camera) { }
		virtual void setCurrentInternalViewport(Viewport *viewport) { }
		virtual void setCurrentInternalFont(FontResource *font) { }
	};

}

#endif