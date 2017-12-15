#pragma once

#ifndef __RENDER3D_RENDERENGINE_H__
#define __RENDER3D_RENDERENGINE_H__

#include <vector>

#include "vector3.h"
#include "renderobject.h"
#include "resourcedata.h"
#include "viewport.h"
#include "camera.h"
#include "resourceinstance.h"
#include "fontresource.h"
#include "textresource.h"

namespace render3d {
	struct Adapter {
		std::string name;
		void *internalData;
	};

	struct DisplayMode {
		int width;
		int height;
		int frequency;
	};

	class Material : public RenderObject {

	public:
		enum DirtyFlags {
			DIFFUSE_DIRTY = 0x01,
		};

	private:
		Material(RenderEngine *engine, int resId)
			: RenderObject(engine, resId) {

		}

		Color m_diffuse;
	public:
		friend class RenderEngine;
		~Material();

		void setDiffuse(Color color) {
			m_diffuse = color;
			m_dirtyFlags |= DIFFUSE_DIRTY;
		}

		const Color &diffuse() const { return m_diffuse; }
	};

	class RenderEngine
	{
	public:
		class Config {
			int m_adapter;
			DisplayMode m_displayMode;
			bool m_fullscreen;
			bool m_antialiasing;

		public:
			Config() {
				m_adapter = 0;
				m_displayMode = { 1024,768,60 };
				m_fullscreen = false;
				m_antialiasing = false;
			}

			void setAdapter(int adapter) { m_adapter = adapter; }
			void setDisplayMode(const struct DisplayMode &displayMode) { m_displayMode = displayMode; }
			void setFullscreen(bool fullscreen) { m_fullscreen = fullscreen; }
			void setAntialiasing(bool antialiasing) { m_antialiasing = antialiasing; }

			int adapter() const { return m_adapter; }
			const DisplayMode &displaymode() const { return m_displayMode; }
			bool fullscreen() const { return m_fullscreen; }
			bool antialiasing() const { return m_antialiasing; }
		};

		Config m_config;
		Config& config() { return m_config; }

	protected:
		std::vector<ResourceData*> m_resources;
		std::vector<Camera *> m_cameras;
		std::vector<Viewport *> m_viewports;
		std::vector<ResourceInstance*> m_instances;
		std::vector<FontResource*> m_fontResources;
		std::vector<TextResource*> m_textResources;
		std::vector<Material*> m_materials;

		Camera *m_currentCamera;
		Viewport *m_currentViewport;
		FontResource *m_currentFont;

	public:
		RenderEngine();
		virtual ~RenderEngine();

		virtual void configure(void *) = 0;
		virtual void init() = 0;
		virtual void destroy() = 0;
		virtual void beginScene() = 0;
		virtual void endScene() = 0;

		virtual std::vector<Adapter> getAdapterList() = 0;
		virtual std::vector<DisplayMode> getDisplayModeList() = 0;

		//Resource allocation
		virtual int newResource(unsigned int vertexCount, Vector3 *vertices, unsigned int indexCount = 0, int *indices = NULL);
		virtual int newMaterial(Color diffuse);
		virtual int newCamera(const Vector3 &pos, const Vector3 &rot, float zoom = 1.f);
		virtual int newViewport(int x, int y, int width, int height);
		virtual int newLight(const Vector3 &position, const Vector3 &direction, const Color &color) { return 1; }
		virtual int newResourceInstance(int resDataId);
		virtual int newFontResource(const char *face, int width, int height, unsigned long flags);
		virtual int newTextResource(int x, int y, int width, int height);
		//
		virtual void drawResource(int resInstanceId);
		virtual void setCurrentCamera(int cameraId);
		virtual void setCurrentViewport(int viewportId);
		virtual void addLight(int lightId) { }
		virtual void setCurrentFont(int fontId);
		virtual void drawText(const char *text, int x, int y, const Color &color);
		virtual void drawResourceDirect(render3d::Vector3 vertices[], int vertexCount, render3d::Color colorList[]); //temp
		virtual void printScreen(const char *fileName) = 0; //temp
		virtual void clear(const Color &clearColor) = 0;

		ResourceData *resourceData(int id) const { return m_resources[id]; }
		Camera *camera(int id) const { return m_cameras[id]; }
		Viewport *viewport(int id) const { return m_viewports[id]; }
		ResourceInstance *resourceInstance(int id) const { return m_instances[id]; }

	protected:
		virtual void *newInternalResource(ResourceData *resData) { return NULL; }
		virtual void *newInternalMaterial(Material *material) { return NULL; }
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