
#include "cinetico.h"
#include "cineticoui.h"
#include "input.h"
#include "ExercisePlayMode.h"
#include "core/entity/Exercise.h"
#include "entity/GeneralSettings.h"
#include "core/GestureCommandsManager.h"
#include "humancharacter.h"
#include "dummycharacter.h"
#include "HadoukenController.h"
#include "RelOrientChar.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include <uilib/lib/time.h>

namespace cinetico {

	using namespace cinetico_core;
	using namespace render3d;

	static bool f4pressed = false;
	static bool debug = false;
	
	ExercisePlayMode::ExercisePlayMode(Cinetico &cinetico, Exercise &exercise)
		: PlayMode(cinetico, EXERCISE_MODE)
		, m_exercise(exercise) {
	}

	void ExercisePlayMode::setup() {

		m_humanChar = new HumanCharacter(*m_cinetico.cineticoUI());
		//m_dummyChar = new DummyCharacter(*m_cinetico.cineticoUI());
		//m_roChar = new RelOrientChar(*m_cinetico.cineticoUI());
		m_commandsManager = new GestureCommandsManager(m_cinetico.generalSettings()->posDistThreshold(),m_cinetico.generalSettings()->posMinHoldtime());

		m_hadoukenController = new HadoukenController(*m_cinetico.cineticoUI(),*m_humanChar);

		m_cinetico.bodyTracker()->setTrackableBodyPoints(m_exercise.trackableBodyPoints());

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

		//m_dummyChar->setPosition(cinetico_core::Vector3(0, terrain->pos().y(), 5));
	}

	void ExercisePlayMode::step() {

		static bool f6pressed = false;
		bool hadouken = false;
		bool f4Down = m_cinetico.input()->keyboard.key(VK_F4);
		bool f6Down = m_cinetico.input()->keyboard.key(VK_F6);
		if (!f4pressed && f4Down) {
			debug = !debug;
		}
		f4pressed = f4Down;

		if (!f6pressed && f6Down) {
			hadouken = true;
		}
		f6pressed = f6Down;

		m_cinetico.bodyTracker()->track();
		m_commandsManager->step(m_cinetico.currentTime());

		Body *body;
		body = m_cinetico.bodyTracker()->body();

		if (body) {
			if (m_exercise.state() == Exercise::Idle) {
				m_exercise.start(*body);
				m_commandsManager->setBody(body);
			}

			m_humanChar->update();

			if(m_exercise.state() == Exercise::Running) {
				int curAction = m_exercise.currentActionIndex();
				if (curAction >= 0)
					m_commandsManager->checkConditions(*m_exercise.actionList()[curAction]);
				m_exercise.step();
			}

			if (m_cinetico.input()->keyboard.key(VK_F5)) {
				m_exercise.reset();
				m_commandsManager->reset();
			}
		}

		if (hadouken) {
			m_hadoukenController->createHadouken(cinetico_core::Vector3(0, 0, 0), cinetico_core::Vector3(0, 0.2f, 0),0.5f);
		}

		m_hadoukenController->step();
		processCamera();
	}

#ifdef DEBUGGING
	inline void printVector3(string &str, const cinetico_core::Vector3 &v) {
		str += "V: [";
		str += string::fromFloat(v.x());
		str += ", ";
		str += string::fromFloat(v.y());
		str += ", ";
		str += string::fromFloat(v.z());
		str += "]\n";
	}

	inline void printVector3(string &str, const render3d::Vector3 &v) {
		str += "V: [";
		str += string::fromFloat(v.x());
		str += ", ";
		str += string::fromFloat(v.y());
		str += ", ";
		str += string::fromFloat(v.z());
		str += "]\n";
	}
#endif

	unsigned long frameCount = 0;
	int print = 0;
	void ExercisePlayMode::render() {
		//todo: draw "Excellent, Good, Bad or Missed on actions and exercise
		++frameCount;
		m_renderEngine->setCurrentCamera(m_currentCameraId);
		m_renderEngine->clear(render3d::Color(30, 30, 30));

		//if (m_dummyChar)
		//	m_dummyChar->render();
		if (m_humanChar) {
			m_humanChar->render();
		}

		m_hadoukenController->render();
		
		//if (m_roChar) {
		//	m_roChar->setPosition(cinetico_core::Vector3(10, 0, 0));
		//	m_roChar->render();
		//}

		m_renderEngine->drawResource(m_instanceTerrain);

		m_renderEngine->setCurrentFont(m_resFontArial);
		
		uilib::string str = m_dictionary.getString(Dictionary::PlayModeExerciseRealizationSelectedExercise,m_exercise.name().c_str());
		m_renderEngine->drawText(str.data(), 500, 10, render3d::Color(255, 255, 255, 100));

		int drawX;
		int drawY;

		/*
		if(frameCount % 100) {
			string filename = "prints/test";
			filename += string::fromInteger(print);
			filename += ".png";
			++print;
			m_renderEngine->printScreen(filename.data());
		}
		*/

#ifdef DEBUGGING
		static int fuck = 0;
		if (debug) {
			drawX = 1000;
			drawY = 50;
			str = "Action Commands: ";
			m_renderEngine->drawText(str.data(), drawX, drawY, render3d::Color(255, 255, 255, 100));
			drawY += 25;
			for (unsigned int i = 0; i < m_commandsManager->commandCount(); ++i) {
				str = "";
				GestureCommand *command = m_commandsManager->actionCommand(i);
				if (command->bodyPoint().bodyPart() == BodyPoint::RightPalm) {
					if (command->positionGestureCommand()) {
						str += "Position action";
						str += "(";
						str += uilib::string::fromInteger(fuck);
						str += ")";
					}
					else {
						MovementGestureCommand *movCommand = command->movementGestureCommand();
						str += "Movement action (" + string::fromInteger(((MovementGestureCommand*)command)->pointCount()) + ")";
						render3d::Color bodyColors[] =
						{
							//Front
							render3d::Color(255,0,0),
							render3d::Color(255,0,0)
						};
						if (movCommand->pointCount() > 1) {
							for (int j = 0; j < movCommand->pointCount()-1; ++j) {
								render3d::Vector3 points[2];
								cinetico_core::Vector3 p = movCommand->point(j);
								p *= 2;

								cinetico_core::Vector3 p2 = movCommand->point(j+1);
								p2 *= 2;
								points[0] = render3d::Vector3(p.x(), p.y(),p.z());
								points[1] = render3d::Vector3(p2.x(), p2.y(),p2.z());
								m_renderEngine->drawResourceDirect(points, 2, bodyColors);
							}
						}
					}
					if (command->positionGestureCommand() && command->finished())
						++fuck;
					m_renderEngine->drawText(str.data(), drawX, drawY, render3d::Color(255, 255, 255, 100));
					drawY += 25;
				}
			}
		}
#endif

		m_renderEngine->setCurrentFont(m_resFontVerdana);
		
		std::vector<Action *> actionList = m_exercise.actionList();
		int currentActionIndex = m_exercise.currentActionIndex();

		render3d::Color drawColor = render3d::Color(200, 200, 200);

		if(m_exercise.state() == Exercise::Finished)
			drawColor = render3d::Color(0, 220, 0, 200);


		int drawIndexX = 36;
		int drawIndexY = 220;

		Exercise::ExerciseState exerciseState;
		Dictionary::StringID exerciseStateId;

		exerciseState = m_exercise.state();
		if (exerciseState == Exercise::Running)
			exerciseStateId = Dictionary::ExerciseStateRunning;
		else if (exerciseState == Exercise::Finished)
			exerciseStateId = Dictionary::ExerciseStateFinished;
		else
			exerciseStateId = Dictionary::ExerciseStateIdle;
		
		string exerciseInfo = m_dictionary.getString(Dictionary::PlayModeExerciseState, m_dictionary.getString(exerciseStateId));
		if (m_exercise.state() == Exercise::Finished) {
			float accuracy = m_exercise.accuracy();
			exerciseInfo += " (";
			exerciseInfo += string::fromFloat(accuracy,2) + '%';
			exerciseInfo += ") - ";

			Dictionary::StringID actionResultId;
			if (accuracy < 50.0f)
				actionResultId = Dictionary::ActionResultMissed;
			else if (accuracy < 75.0f)
				actionResultId = Dictionary::ActionResultBad;
			else if (accuracy < 95.0f)
				actionResultId = Dictionary::ActionResultGood;
			else
				actionResultId = Dictionary::ActionResultExcellent;
			exerciseInfo += m_dictionary.getString(actionResultId);
		}

		m_renderEngine->drawText(exerciseInfo.data(), 20, drawIndexY, drawColor);
		drawIndexY += 25;
		string s = m_dictionary.getString(Dictionary::PlayModeActionList) + ":";
		m_renderEngine->drawText(s.data(), 20, drawIndexY, drawColor);
		drawIndexY += 25;
		
		for (unsigned int i = 0; i < actionList.size(); ++i) {
			Action *action = actionList[i];
			if (action->isCorrect())
				drawColor = render3d::Color(0, 220, 0, 200);
			else {
				if (action->state() == Action::Finished)
					drawColor = render3d::Color(220, 0, 0, 200);
				else
					drawColor = render3d::Color(220, 220, 220, 200);
			}

			uilib::string curAction = action->name().c_str();
			curAction += " (";
			curAction += uilib::string::fromFloat(action->accuracy(), 2);
			curAction += "%)";
			m_renderEngine->drawText(curAction.data(), drawIndexX, drawIndexY, drawColor);
			drawIndexY += 30;

			if (action->state() == Action::Finished) {
				Dictionary::StringID actionResultId;
				Action::ActionResult result = action->result();
				if (result == Action::Missed)
					actionResultId = Dictionary::ActionResultMissed;
				else if (result == Action::Bad)
					actionResultId = Dictionary::ActionResultBad;
				else if (result == Action::Good)
					actionResultId = Dictionary::ActionResultGood;
				else
					actionResultId = Dictionary::ActionResultExcellent;
				exerciseInfo += m_dictionary.getString(actionResultId);
			}
		}
		

#ifdef DEBUGGING
		uilib::string strQ;
		Body *body = m_humanChar->body();
		if (body) {
			BodyPoint *bpTest = body->bodyPoint(BodyPoint::Head);
			Quaternion q = bpTest->orientation();
			cinetico_core::Vector3 e = q.toEuler();
			
			strQ += "Q: [";
			strQ += string::fromFloat(q.w());
			strQ += ", ";
			strQ += string::fromFloat(q.x());
			strQ += ", ";
			strQ += string::fromFloat(q.y());
			strQ += ", ";
			strQ += string::fromFloat(q.z());
			strQ += "]\n";
			strQ += "E: [";
			strQ += string::fromFloat(e.x());
			strQ += ", ";
			strQ += string::fromFloat(e.y());
			strQ += ", ";
			strQ += string::fromFloat(e.z());
			strQ += "]\n";
			printVector3(strQ, body->bodyPoint(actionList[0]->gesture(0)->bodyPoint())->position());
			printVector3(strQ, body->bodyPoint(BodyPoint::Spine)->position());
			strQ += string::fromInteger(m_cinetico.currentTime());
			if (m_humanChar->body()->bodyPoint(BodyPoint::RightPalm)->position().y() > m_humanChar->body()->bodyPoint(BodyPoint::Head)->position().y())
				strQ = "Fuck";			
		}

		Camera *camera = m_renderEngine->camera(m_currentCameraId);
		printVector3(strQ, camera->pos());
		printVector3(strQ, camera->rot());
		m_renderEngine->drawText(strQ.data(), 200, drawIndexY, drawColor);

#endif

		//m_renderEngine->setCurrentCamera(m_cam2);
		//m_renderEngine->setCurrentViewport(m_viewportActionPreview);
		//m_renderEngine->clear(render3d::Color(0, 40, 100));
	}

	void ExercisePlayMode::cleanUp() {
		if (m_humanChar)
			delete m_humanChar;

#ifdef DEBUGGING
		/*
		if(m_dummyChar)
			delete m_dummyChar;
		if (m_roChar)
			delete m_roChar;
			*/
#endif

		if(m_commandsManager)
			delete m_commandsManager;
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

		float fspeed = 0;
		float rspeed = 0;
		float uspeed = 0;
		float d = 0.5f;
		Camera *camera = m_renderEngine->camera(m_currentCameraId);
		render3d::Vector3 camPos = camera->pos();
		render3d::Vector3 camRot = camera->rot();
		render3d::Vector3 camUp = render3d::Vector3(0, 1, 0);
		render3d::Vector3 camForward = camRot;
		camForward.setX(-std::sin(camRot.y())*std::cos(camRot.x()));
		camForward.setY(std::sin(camRot.x()));
		camForward.setZ(std::cos(camRot.y())*std::cos(camRot.x()));
		render3d::Vector3 camRight = crossProduct(camUp, camForward);

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
}