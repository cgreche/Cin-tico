#include "renderengine.h"

namespace render3d {

	RenderEngine::RenderEngine() {
		m_currentCamera = NULL;
		m_currentViewport = NULL;
		m_currentFont = NULL;
	}

	RenderEngine::~RenderEngine() {

	}

	void RenderEngine::destroy() {
		for (ResourceData *resourceData : m_resources) {
			releaseInternalResource(resourceData);
			delete resourceData;
		}
		for (Camera *camera : m_cameras) {
			releaseInternalCamera(camera);
			delete camera;
		}
		for (Viewport *viewport : m_viewports) {
			releaseInternalViewport(viewport);
			delete viewport;
		}
		for (ResourceInstance *resInstance : m_instances) {
			releaseInternalResourceInstance(resInstance);
			delete resInstance;
		}
		for (FontResource *fontResource : m_fontResources) {
			releaseInternalFontResource(fontResource);
			delete fontResource;
		}
		for (TextResource *textResource : m_textResources) {
			releaseInternalTextResource(textResource);
			delete textResource;
		}

		//temp
		if (!m_resources.empty())
			m_resources.clear();
		if (!m_materials.empty())
			m_materials.clear();
		m_cameras.clear();
		if (!m_viewports.empty())
			m_viewports.clear();
		if (!m_instances.empty())
			m_instances.clear();
		if (!m_fontResources.empty())
			m_fontResources.clear();
		if (!m_textResources.empty())
			m_textResources.clear();

		m_currentCamera = NULL;
	}

	int RenderEngine::newResource(unsigned int vertexCount, Vector3 *vertices, unsigned int indexCount, int *indices) {
		int curId = m_resources.size();
		ResourceData *resData = new ResourceData(this, curId);
		resData->setVertices(vertexCount, vertices);
		resData->setIndices(indexCount, indices);
		void *internalData = newInternalResource(resData);
		resData->m_internalData = internalData;
		m_resources.push_back(resData);
		return curId;
	}

	int RenderEngine::newMaterial(Color diffuse) {
		int curId = m_materials.size();
		Material *material = new Material(this, curId);
		material->setDiffuse(diffuse);
		void *internalData = newInternalMaterial(material);
		material->m_internalData = internalData;
		m_materials.push_back(material);
		return curId;
	}

	int RenderEngine::newCamera(const Vector3 &pos, const Vector3 &rot, float zoom) {
		int curId = m_cameras.size();

		Camera *camera = new Camera(this, curId, pos, rot, zoom);
		camera->m_internalData = newInternalCamera(camera);
		m_cameras.push_back(camera);
		if (!m_currentCamera)
			m_currentCamera = camera;
		return curId;
	}

	int RenderEngine::newViewport(int x, int y, int width, int height)
	{
		int curId = m_viewports.size();
		Viewport *viewport = new Viewport(this, curId, x, y, width, height);
		viewport->m_internalData = newInternalViewport(viewport);
		m_viewports.push_back(viewport);
		if (!m_currentViewport)
			m_currentViewport = viewport;
		return curId;
	}

	int RenderEngine::newResourceInstance(int resDataID)
	{
		int curId = m_instances.size();
		ResourceInstance *instance = new ResourceInstance(this, curId, resDataID);
		instance->m_internalData = newInternalResourceInstance(instance);
		m_instances.push_back(instance);
		return curId;
	}

	int RenderEngine::newFontResource(const char *face, int width, int height, unsigned long flags)
	{
		int curId = m_fontResources.size();
		FontResource *font = new FontResource(this, curId, face, width, height, flags);
		font->m_internalData = newInternalFontResource(font);
		m_fontResources.push_back(font);
		return curId;
	}

	int RenderEngine::newTextResource(int x, int y, int width, int height)
	{
		int curId = m_textResources.size();
		TextResource *text = new TextResource(this, curId, x, y, width, height);
		text->m_internalData = newInternalTextResource(text);
		m_textResources.push_back(text);
		return curId;
	}

	void RenderEngine::drawResource(int resInstanceId) {
		drawInternalResource(m_instances[resInstanceId]);
	}

	void RenderEngine::setCurrentCamera(int cameraId) {
		m_currentCamera = m_cameras[cameraId];
		setCurrentInternalCamera(m_currentCamera);
	}

	void RenderEngine::setCurrentViewport(int viewportId) {
		m_currentViewport = m_viewports[viewportId];
		setCurrentInternalViewport(m_currentViewport);
	}

	void RenderEngine::setCurrentFont(int fontId) {
		m_currentFont = m_fontResources[fontId];
		setCurrentInternalFont(m_currentFont);
	}

	void RenderEngine::drawText(const char *text, int x, int y, const Color &color) {
	}

	void RenderEngine::drawResourceDirect(render3d::Vector3 vertices[], int vertexCount, render3d::Color colorList[]) {

	}
}