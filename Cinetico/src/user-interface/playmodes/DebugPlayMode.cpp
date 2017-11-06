
#include <time.h>
#include "DebugPlayMode.h"
#include "cinetico.h"
#include "cineticoui.h"
#include "input.h"
#include "entity/core/quaternion.h"

namespace cinetico {

	int mouseX = 0;
	int mouseY = 0;
	int mouseZ = 0;
	int lastMouseX = 0;
	int lastMouseY = 0;
	int lastMouseZ = 0;

	int resQuad1;
	int resQuad2;
	int line1;
	int line2;

	int resCube;
	int resCircle;
	int resTerrain;
	int resPrism;

	int instanceQuad1;
	int instanceQuad2;
	int instanceCube;
	int instanceCircle;
	int instanceCircle2;
	int instanceTerrain;
	int instanceWall;
	int instancePrism;

	int cam1;
	int cam2;

	int viewport1;
	int viewport2;

	int instCubeTest;


#define NUM_CUBES 1000
	int instancedCubes[NUM_CUBES];

	int currentCameraId;


	DebugPlayMode::DebugPlayMode(Cinetico &cinetico)
		: PlayMode(cinetico, DEBUG_MODE) {
	}

	void DebugPlayMode::setup() {
		::srand((unsigned int)::time(0));

		render3d::Color cubeColors[] =
		{
			//Front
			render3d::Color(255,0,0),
			render3d::Color(255,0,0),
			render3d::Color(255,0,0),
			render3d::Color(255,0,0),

			//Back
			render3d::Color(0,255,0),
			render3d::Color(0,255,0),
			render3d::Color(0,255,0),
			render3d::Color(0,255,0),

			//Top
			render3d::Color(255,255,255),
			render3d::Color(255,255,255),
			render3d::Color(255,255,255),
			render3d::Color(255,255,255),

			//Bottom
			render3d::Color(0,0,0),
			render3d::Color(0,0,0),
			render3d::Color(0,0,0),
			render3d::Color(0,0,0),

			//Left
			render3d::Color(0,0,255),
			render3d::Color(0,0,255),
			render3d::Color(0,0,255),
			render3d::Color(0,0,255),

			//Right
			render3d::Color(255,0,255),
			render3d::Color(255,0,255),
			render3d::Color(255,0,255),
			render3d::Color(255,0,255),
		};

		const int numPoints = 30;
		render3d::Color circleColors[numPoints + 1];
		for (int i = 0; i < numPoints + 1; ++i) {
			circleColors[i] = render3d::Color(rand() % 255, rand() % 255, rand() % 255, 122);
		}

		resCube = m_renderEngineHelper->createCube(2);
		m_renderEngine->resourceData(resCube)->setColors(cubeColors);
		instanceCube = m_renderEngine->newResourceInstance(resCube);
		instCubeTest = m_renderEngine->newResourceInstance(resCube);
		//m_renderEngine->resourceInstance(instCubeTest)->setPos(render3d::Vector3(-2.5f, 1.5f, 0.f));

		resQuad1 = m_renderEngineHelper->createQuad(10, 0.25);
		resQuad2 = m_renderEngineHelper->createQuad(100, 200);

		resCircle = m_renderEngineHelper->createCircle(0.3f, numPoints + 1);
		m_renderEngine->resourceData(resCircle)->setColors(circleColors);
		resPrism = m_renderEngineHelper->createRectangularPrism(0.3f, 2.f, 0.5f);
		m_renderEngine->resourceData(resPrism)->setColors(cubeColors);

		instanceQuad1 = m_renderEngine->newResourceInstance(resQuad1);
		m_renderEngine->resourceInstance(instanceQuad1)->setPos(render3d::Vector3(10, 10, 0));
		instanceQuad2 = m_renderEngine->newResourceInstance(resQuad2);
		m_renderEngine->resourceInstance(instanceQuad2)->setPos(render3d::Vector3(120, 10, 0));
		instanceCube = m_renderEngine->newResourceInstance(resCube);
		m_renderEngine->resourceInstance(instanceCube)->setPos(render3d::Vector3(-2.5, 0., 0.));
		instanceCircle = m_renderEngine->newResourceInstance(resCircle);
		instanceCircle2 = m_renderEngine->newResourceInstance(resCircle);
		instancePrism = m_renderEngine->newResourceInstance(resPrism);

		ResourceInstance *circle2 = m_renderEngine->resourceInstance(instanceCircle2);
		circle2->setPos(render3d::Vector3(2.0f, 0.0f, 0.0f));


		for (int i = 0; i < NUM_CUBES; ++i) {
			instancedCubes[i] = m_renderEngine->newResourceInstance(resCube);
			ResourceInstance* inst = m_renderEngine->resourceInstance(instancedCubes[i]);
			float x, y, z;
			float rx, ry, rz;
			float s;
			x = -100.f + 200.f * ((rand() % 101) / 100.f);
			y = -10.f + 20.f * ((rand() % 101) / 100.f);
			z = -10.f + 20.f * ((rand() % 101) / 100.f);
			rx = ((rand() % 100 + 1) / 100.f) * 6.28f;
			ry = ((rand() % 100 + 1) / 100.f) * 6.28f;
			rz = ((rand() % 100 + 1) / 100.f) * 6.28f;
			s = 0.1f + ((rand() % 101) / 100.f)* 0.9f;
			inst->setPos(render3d::Vector3(x, y, z));
			inst->setRot(render3d::Vector3(rx, ry, rz));
			inst->setScale(s);
		}

		float quadSize = 3.2f;
		int quadCountH = 128;
		int quadCountV = 32;
		resTerrain = m_renderEngineHelper->generateTerrain(quadSize, quadCountH, quadCountV);
		instanceTerrain = m_renderEngine->newResourceInstance(resTerrain);
		instanceWall = m_renderEngine->newResourceInstance(resTerrain);

		ResourceInstance *instance = (ResourceInstance*)m_renderEngine->resourceInstance(instanceWall);
		instance->setPos(render3d::Vector3(0.f, quadCountV / 2 * quadSize, quadCountV / 2 * quadSize));
		instance->setRot(render3d::Vector3(-3.14159f * 0.5f, 0.f, 0.f));

		ResourceInstance *terrain = (ResourceInstance*)m_renderEngine->resourceInstance(instanceTerrain);
		terrain->setPos(render3d::Vector3(0, -2, 0));

		//Setup cameras
		cam1 = m_renderEngine->newCamera(render3d::Vector3(0.f, 3.f, -6.4f), render3d::Vector3(-0.3f, 0.f, 0.f));
		cam2 = m_renderEngine->newCamera(render3d::Vector3(8.f, 8.f, -8.f), render3d::Vector3(0.f, 0.f, 0.f));
		currentCameraId = cam1;

		//Setup viewports
		uilib::Size size = m_cinetico.cineticoUI()->mainWindow()->size();
		float percent = 0.25f;
		viewport2 = m_renderEngine->newViewport(20, 20, (int)(size.width()*percent), (int)(size.height()*percent));
	}

	void DebugPlayMode::step() {
		bool leftDown = m_cinetico.input()->keyboard.key('A');
		bool rightDown = m_cinetico.input()->keyboard.key('D');
		bool downDown = m_cinetico.input()->keyboard.key('S');
		bool upDown = m_cinetico.input()->keyboard.key('W');
		bool shiftDown = m_cinetico.input()->keyboard.key(VK_SHIFT);
		bool ctrlDown = m_cinetico.input()->keyboard.key(VK_CONTROL);
		bool tabDown = m_cinetico.input()->keyboard.key(VK_TAB);
		bool spaceDown = m_cinetico.input()->keyboard.key(VK_SPACE);
		bool escDown = m_cinetico.input()->keyboard.key(VK_ESCAPE);

		float d = 0.1f;
		float e = 0.005f;
		float max = 1.f;

		static float a = 0.f;
		static bool invert = false;

		cinetico_core::Quaternion q, q2, q3;

		if (invert)
			a -= e;
		else
			a += e;

		if (a > max) {
			a = max;
			invert = true;
		}
		if (a < 0) {
			a = 0;
			invert = false;
		}

		//a = 0.05;
		q = Quaternion::fromEuler(0,0,0);
		q2 = Quaternion::fromEuler(0,0,1);
		q3 = Quaternion::nlerp(q, q2, a);
		cinetico_core::Vector3 testRotation = q3.toEuler();
		//testRotation = cinetico_core::Vector3(a, 0, 0);

		ResourceInstance *instance;
		instance = m_renderEngine->resourceInstance(instCubeTest);
		instance->setRot(render3d::Vector3(testRotation.x(), testRotation.y(), testRotation.z()));



		if (!shiftDown) {
			//Update quads (billboarding)
			processCamera();
			Camera *cam = m_renderEngine->camera(cam1);
			render3d::Vector3 faceCam = render3d::Vector3(-cam->rot().x(), -cam->rot().y(), -cam->rot().z());
			m_renderEngine->resourceInstance(instanceQuad1)->setRot(faceCam);
			m_renderEngine->resourceInstance(instanceQuad2)->setRot(faceCam);
		}


		instance = m_renderEngine->resourceInstance(instanceCube);
		render3d::Vector3 newPos = instance->pos();
		render3d::Vector3 newRot = instance->rot();

		if (leftDown) {
			if (shiftDown) {
				if (ctrlDown) {
					newRot.setZ(newRot.z() + d);
				}
				else {
					newRot.setY(newRot.y() + d);
				}
			}
			else {
				newPos.setX(newPos.x() - d);
			}
		}

		if (rightDown) {
			if (shiftDown) {
				if (ctrlDown) {
					newRot.setZ(newRot.z() - d);
				}
				else {
					newRot.setY(newRot.y() - d);
				}
			}
			else {
				newPos.setX(newPos.x() + d);
			}
		}

		if (downDown) {
			if (shiftDown) {
				newRot.setX(newRot.x() - d);
			}
			else if (ctrlDown) {
				newPos.setZ(newPos.z() - d);
			}
			else {
				newPos.setY(newPos.y() - d);
			}
		}

		if (upDown) {
			if (shiftDown) {
				newRot.setX(newRot.x() + d);
			}
			else if (ctrlDown) {
				newPos.setZ(newPos.z() + d);
			}
			else {
				newPos.setY(newPos.y() + d);
			}
		}

		instance->setPos(newPos);
		instance->setRot(newRot);
	}

	void DebugPlayMode::render() {
		m_renderEngine->setCurrentCamera(currentCameraId);
		m_renderEngine->setCurrentViewport(m_cinetico.cineticoUI()->viewport());
		m_renderEngine->clear(render3d::Color(30, 30, 30));

		m_renderEngine->drawResource(instanceTerrain);
		m_renderEngine->drawResource(instanceCube);
		m_renderEngine->drawResource(instCubeTest);

		/*
		m_renderEngine->drawResource(instanceQuad2);
		static int state = 1;
		if (g_frameCount % 30 == 0)
		state ^= 1;
		if (state == 1)
		m_renderEngine->drawResource(instanceQuad1);
		*/
		//m_renderEngine->drawResource(instanceWall);

		//m_renderEngine->drawResource(instanceTriangle);
		//m_renderEngine->drawResource(instanceTriangleType2);
		//m_renderEngine->drawResource(instanceCircle);
		//m_renderEngine->drawResource(instanceCircle2);
		//m_renderEngine->drawResource(instancePrism);

		for (int i = 0; i < NUM_CUBES; ++i) {
			//		d3d9.drawResource(instancedCubes[i]);
		}

		m_renderEngine->setCurrentCamera(cam2);
		m_renderEngine->setCurrentViewport(viewport2);
		m_renderEngine->clear(render3d::Color(0, 40, 100));

		ResourceInstance *instance = m_renderEngine->resourceInstance(instanceCube);


	}

	void DebugPlayMode::cleanUp() {
	}

	void DebugPlayMode::processCamera() {
		Input::Keyboard &kb = m_cinetico.input()->keyboard;
		Input::Mouse &mouse = m_cinetico.input()->mouse;
		bool leftDown = kb.key('A');
		bool rightDown = kb.key('D');
		bool downDown = kb.key('S');
		bool upDown = kb.key('W');
		bool shiftDown = kb.key(VK_SHIFT);
		bool ctrlDown = kb.key(VK_CONTROL);
		bool tabDown = kb.key(VK_TAB);
		bool spaceDown = kb.key(VK_SPACE);
		bool mouse2Down = mouse.button(1);

		static int lastMouseX = mouse.x();
		static int lastMouseY = mouse.y();
		static int lastMouseZ = mouse.z();

		int mouseX = mouse.x();
		int mouseY = mouse.y();
		int mouseZ = mouse.z();

		int dx = mouseX - lastMouseX;
		int dy = mouseY - lastMouseY;
		int dz = mouseZ - lastMouseZ;
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		lastMouseZ = 0;
		mouseZ = 0;

		float fspeed = 0;
		float rspeed = 0;
		float uspeed = 0;
		float d = 0.5f;
		Camera *camera = m_renderEngine->camera(cam1);
		render3d::Vector3 camPos = camera->pos();
		render3d::Vector3 camRot = camera->rot();
		render3d::Vector3 camUp = render3d::Vector3(0,1,0);
		render3d::Vector3 camForward = camRot;
		camForward.setX(-std::sin(camRot.y())*std::cos(camRot.x()));
		camForward.setY(std::sin(camRot.x()));
		camForward.setZ(std::cos(camRot.y())*std::cos(camRot.x()));
		render3d::Vector3 camRight = crossProduct(camUp,camForward);
		
		render3d::Vector3 camVel;

		camForward.normalize();
		if (rightDown) {
			rspeed = d;
		}

		if (leftDown) {
			rspeed = -d;
		}

		if (upDown) {
			if (ctrlDown)
				fspeed = d;
			else
				uspeed = d;
		}

		if (downDown) {
			if (ctrlDown)
				fspeed = -d;
			else
				uspeed = -d;
		}

		camVel = camForward*fspeed + camRight*rspeed + camUp*uspeed;
		camera->setPos(camPos + camVel);

		if (mouse2Down) {
			Camera *camera = m_renderEngine->camera(cam1);
			render3d::Vector3 camRot = camera->rot();

			if (dx != 0) {
				camRot.setY(camRot.y() - 0.01f*dx);
			}

			if (dy != 0) {
				camRot.setX(camRot.x() - 0.01f*dy);
			}

			if (dz != 0)
				camera->setZoom(camera->zoom() - dz*0.1f);

			camera->setRot(camRot);
		}
		return;
	}
}