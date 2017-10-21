
#include "cinetico.h"
#include "cineticoui.h"
#include "input.h"
#include "ExercisePlayMode.h"
#include "entity/core/Exercise.h"
#include "humancharacter.h"
#include "dummycharacter.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"


namespace cinetico {

	using namespace cinetico_core;
	using namespace render3d;

	ExercisePlayMode::ExercisePlayMode(Cinetico &cinetico, Exercise &exercise)
		: PlayMode(cinetico, EXERCISE_MODE)
		, m_exercise(exercise) {
	}

	void ExercisePlayMode::setup() {
		m_humanChar = new HumanCharacter(*m_cinetico.cineticoUI());
		m_dummyChar = new DummyCharacter(*m_cinetico.cineticoUI());

		m_cinetico.bodyTracker()->setTrackableBodyPoints(m_exercise.trackableBodyPoints());

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

		m_resCube = m_renderEngineHelper->createCube(1);
		m_renderEngine->resourceData(m_resCube)->setColors(cubeColors);
		m_instanceCube = m_renderEngine->newResourceInstance(m_resCube);
		m_renderEngine->resourceInstance(m_instanceCube)->setPos(render3d::Vector3(-2.5f,0,0));

		float quadSize = 3.2f;
		int quadCountH = 128;
		int quadCountV = 32;
		m_resTerrain = m_renderEngineHelper->generateTerrain(quadSize, quadCountH, quadCountV);
		m_instanceTerrain = m_renderEngine->newResourceInstance(m_resTerrain);

		ResourceInstance *terrain = (ResourceInstance*)m_renderEngine->resourceInstance(m_instanceTerrain);
		terrain->setPos(render3d::Vector3(0, -2, 0));

		//Setup fonts
		m_resFontArial = m_renderEngine->newFontResource("Arial", 15, 30, FontResource::BOLD);
		m_resFontVerdana = m_renderEngine->newFontResource("Verdana", 10, 25, FontResource::BOLD);

		//Setup cameras
		m_cam1 = m_renderEngine->newCamera(render3d::Vector3(0.f, 3.f, -6.4f), render3d::Vector3(-0.3f, 0.f, 0.f));
		m_cam2 = m_renderEngine->newCamera(render3d::Vector3(8.f, 8.f, -8.f), render3d::Vector3(0.f, 0.f, 0.f));
		m_currentCameraId = m_cam1;

		//Setup viewports
		float percent = 0.25f;
		Size size = m_cinetico.cineticoUI()->mainWindow()->size();
		m_viewportActionPreview = m_renderEngine->newViewport(20, 20, (int)(size.width()*percent), (int)(size.height()*percent));
	}

	void ExercisePlayMode::step() {
		m_cinetico.bodyTracker()->track();

		Body *body;
		body = m_cinetico.bodyTracker()->body();

		if (body) {
			if (m_exercise.state() == Exercise::Idle)
				m_exercise.start(*body);

			m_humanChar->update();
			if (m_exercise.step() == Exercise::Finished)
				m_exercise.reset();

		}
		if(m_cinetico.input()->keyboard.key(VK_SHIFT))
			processCube();
		else
			processCamera();
	}

	void ExercisePlayMode::render() {
		m_renderEngine->setCurrentCamera(m_currentCameraId);
		m_renderEngine->setCurrentViewport(m_cinetico.cineticoUI()->viewport());
		m_renderEngine->clear(render3d::Color(30, 30, 30));

		m_renderEngine->drawResource(m_instanceCube);

		if (m_dummyChar)
			m_dummyChar->render();
		if (m_humanChar)
			m_humanChar->render();

		m_renderEngine->drawResource(m_instanceTerrain);

		m_renderEngine->setCurrentFont(m_resFontArial);
		std::string str = "Exercício selecionado: ";
		str += m_exercise.name();
		m_renderEngine->drawText(str.c_str(), 500, 10, render3d::Color(255, 255, 255, 100));
		m_renderEngine->setCurrentFont(m_resFontVerdana);

		if (m_exercise.state() == Exercise::Running) {
			std::vector<Action *> actionList = m_exercise.actionList();
			int actionIndex = m_exercise.currentActionIndex();
			int nextActionindex = m_exercise.getNextActionsIndex();

			m_renderEngine->drawText("Ação atual:", 20, 220, render3d::Color(200, 200, 200));

			int drawIndexX = 36;
			int drawIndexY = 250;
			render3d::Color drawColor = render3d::Color(220, 220, 220, 200);
			for (; actionIndex < nextActionindex; ++actionIndex) {
				if (actionList[actionIndex]->isCorrect())
					drawColor = render3d::Color(0, 220, 0, 200);
				uilib::string curAction = actionList[actionIndex]->name().c_str();
				curAction += " (";
				curAction += uilib::string::fromFloat(actionList[actionIndex]->accuracy(), 2);
				curAction += "%)";
				m_renderEngine->drawText(curAction.data(), drawIndexX, drawIndexY, drawColor);
				drawIndexY += 30;
			}
		}

		m_renderEngine->setCurrentCamera(m_cam2);
		m_renderEngine->setCurrentViewport(m_viewportActionPreview);
		m_renderEngine->clear(render3d::Color(0, 40, 100));
	}

	void ExercisePlayMode::cleanUp() {
		if (m_humanChar)
			delete m_humanChar;
		if(m_dummyChar)
			delete m_dummyChar;
	}

	void ExercisePlayMode::processCamera() {
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

		float d = 0.5f;
		Camera *camera = m_renderEngine->camera(m_currentCameraId);
		render3d::Vector3 camPos = camera->pos();
		if (rightDown) {
			camPos.setX(camPos.x() + d);
		}

		if (leftDown) {
			camPos.setX(camPos.x() - d);
		}

		if (upDown) {
			if (ctrlDown)
				camPos.setZ(camPos.z() + d);
			else
				camPos.setY(camPos.y() + d);
		}

		if (downDown) {
			if (ctrlDown)
				camPos.setZ(camPos.z() - d);
			else
				camPos.setY(camPos.y() - d);
		}

		camera->setPos(camPos);

		if (spaceDown) {
			m_currentCameraId = m_cam2;
		}
		else {
			m_currentCameraId = m_cam1;
		}

		if (mouse2Down) {
			Camera *camera = m_renderEngine->camera(m_currentCameraId);
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

	void ExercisePlayMode::processCube() {
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
		bool escDown = kb.key(VK_ESCAPE);

		float d = 0.1f;

		ResourceInstance *instance = m_renderEngine->resourceInstance(m_instanceCube);
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
}