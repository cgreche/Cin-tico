#include "renderengine.h"

namespace render3d {

	RenderEngine::RenderEngine() {
		m_currentCamera = NULL;
		m_currentViewport = NULL;
		m_currentFont = NULL;
	}

	int RenderEngine::newResource(unsigned int vertexCount, Vector3 *vertices, unsigned int indexCount, int *indices, Color *colors) {
		int curId = m_resources.size();
		ResourceData *resData = new ResourceData(this, curId);
		resData->setVertices(vertexCount, vertices);
		resData->setIndices(indexCount, indices);
		resData->setColors(colors);
		void *internalData = newInternalResource(resData);
		resData->m_internalData = internalData;
		m_resources.push_back(resData);
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

}