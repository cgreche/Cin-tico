#include "renderengine.h"

namespace render3d {

	RenderEngine::RenderEngine() {
		m_currentCamera = NULL;
	}

	int RenderEngine::newResource(unsigned int vertexCount, Vertex3 *vertices, unsigned int indexCount, int *indices, Color *colors) {
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

}