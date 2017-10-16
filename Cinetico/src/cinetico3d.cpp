#include <windows.h>
#include <time.h>
#include "cinetico.h"
#include "cinetico3d.h"
#include "render3d/d3d9engine.h"
#include "kinectsensor.h"

#include "dummycharacter.h"
#include "humancharacter.h"

using namespace cinetico_core;
using namespace render3d;

HWND g_world3DWindow = NULL;

namespace cinetico {

	float aspectRatio43 = 1.33334f; //4:3
	float aspectRatio51 = 1.25; //5:4
	float aspectRatio169 = 1.77778f; //16:9

	float currentAspectRatio = aspectRatio169;

	int winWidth = 1280;
	int winHeight = (int)ceil(winWidth / currentAspectRatio);

	int g_frameCount = 0;

	
	RenderEngineHelper *renderEngineHelper;

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

	bool keyStates[256] = { 0 };
	bool lastKeyStates[256] = { 0 };
	bool mouse[3] = { 0 };

	int mouseX = 0;
	int mouseY = 0;
	int mouseZ = 0;
	int lastMouseX = 0;
	int lastMouseY = 0;
	int lastMouseZ = 0;

	int currentCameraId = cam1;

	int terrainId;


#define NUM_CUBES 1000

	int instancedCubes[NUM_CUBES];


	int resFontArial;
	int resFontVerdana;	

	class SensorFactory
	{
	public:
		static Sensor *getSystemSensor() {
#ifdef WIN32
			return new KinectSensor();
#endif
		}
	};


	Cinetico3D::Cinetico3D(Cinetico &cinetico)
		: m_application(cinetico) {
		m_playingMode = NOT_PLAYING;
		m_playingExercise = NULL;
		m_globalFrameCount = 0;
		m_exerciseFrameCount = 0;
		setup();
	}

	Cinetico3D::~Cinetico3D() {
		cleanUp();
	}

	void Cinetico3D::setupBody() {



	}

	LRESULT CALLBACK world3D_MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

		switch (msg) {
		case WM_SYSKEYDOWN:
			keyStates[wParam] = true;
			break;

		case WM_CHAR:
		{
			int c = wParam;
			break;
		}

		case WM_KEYDOWN:
		{
			keyStates[wParam] = true;
			break;
		}

		case WM_KEYUP:
		{
			keyStates[wParam] = false;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			mouse[0] = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			mouse[0] = false;
			break;
		}

		case WM_RBUTTONDOWN:
		{
			mouse[1] = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			mouse[1] = false;
			break;
		}

		case WM_MBUTTONDOWN:
		{
			mouse[2] = true;
			break;
		}

		case WM_MBUTTONUP:
		{
			mouse[2] = false;
			break;
		}

		case WM_MOUSEWHEEL:
		{
			mouseZ = (int)(short)HIWORD(wParam);
			mouseZ /= WHEEL_DELTA;
			break;
		}

		case WM_MOUSEMOVE:
		{
			mouseX = LOWORD(lParam);
			mouseY = HIWORD(lParam);
			break;
		}

		case WM_CLOSE:
			::PostQuitMessage(0);
			break;
		case WM_DESTROY:
			int a = 1;
			break;
		}


		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void Cinetico3D::setupWindow() {

		HINSTANCE hInstance = ::GetModuleHandle(NULL);

		const char wndClassName[] = "World3DWindow";
		const char wndName[] = "World3DWindow";

		WNDCLASS wndClass = { 0 };
		wndClass.style = 0;
		wndClass.hInstance = hInstance;
		wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndClass.lpszClassName = wndClassName;
		wndClass.lpfnWndProc = world3D_MainWndProc;

		ATOM res = ::RegisterClass(&wndClass);

		DWORD winStyle = WS_OVERLAPPEDWINDOW;
		RECT rc = { 0,0, winWidth, winHeight };
		::AdjustWindowRect(&rc, winStyle, FALSE);

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;
		HWND hwnd = ::CreateWindow(wndClassName, wndName, winStyle,
			CW_USEDEFAULT, CW_USEDEFAULT, w, h,
			NULL, NULL, hInstance, 0);

		g_world3DWindow = hwnd;
	}

	void Cinetico3D::setupRenderEngine()
	{
		m_renderEngine = new D3D9Engine();
		m_renderEngine->configure(g_world3DWindow);
		m_renderEngine->init();
		m_renderEngineHelper = new RenderEngineHelper(*m_renderEngine);
	}


	void Cinetico3D:: setupDrawables() {
		::srand((unsigned int)::time(0));
		setupBody();

		const int numPoints = 30;
		render3d::Color circleColors[numPoints + 1];
		for (int i = 0; i < numPoints + 1; ++i) {
			circleColors[i] = render3d::Color(rand() % 255, rand() % 255, rand() % 255, 122);
		}

		resQuad1 = m_renderEngineHelper->createQuad(10, 0.25);
		resQuad2 = m_renderEngineHelper->createQuad(100, 200);

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

		resCube = m_renderEngineHelper->createCube(2);
		m_renderEngine->resourceData(resCube)->setColors(cubeColors);
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


		resFontArial = m_renderEngine->newFontResource("Arial", 15, 30, FontResource::BOLD);
		resFontVerdana = m_renderEngine->newFontResource("Verdana", 20, 200, FontResource::ITALIC);

		m_dummyChar = new DummyCharacter(*this);
		m_humanChar = new HumanCharacter(*this);
	}

	void Cinetico3D::setupCameras() {
		cam1 = m_renderEngine->newCamera(render3d::Vector3(0.f, 3.f, -6.4f), render3d::Vector3(-0.3f, 0.f, 0.f));
		cam2 = m_renderEngine->newCamera(render3d::Vector3(8.f, 8.f, -8.f), render3d::Vector3(0.f, 0.f, 0.f));
	}

	void Cinetico3D::setupViewports() {
		viewport1 = m_renderEngine->newViewport(0, 0, winWidth, winHeight);
		float percent = 0.25f;
		viewport2 = m_renderEngine->newViewport(20, 20, (int)(winWidth*percent), (int)(winHeight*percent));
	}

	void Cinetico3D::setup() {
		m_sensor = SensorFactory::getSystemSensor();
		m_sensor->initialize();
		m_bodyTracker = new BodyTracker(*m_sensor);
		setupWindow();
		setupRenderEngine();
		setupDrawables();
		setupCameras();
		setupViewports();
	}

	void Cinetico3D::cleanUp() {
		if (m_bodyTracker)
			delete m_bodyTracker;

		if (m_sensor) {
			m_sensor->finalize();
			delete m_sensor;
		}

		if (m_renderEngineHelper)
			delete m_renderEngineHelper;
		if (m_renderEngine)
			delete m_renderEngine;
	}

	void Cinetico3D::processCamera() {

		bool leftDown = keyStates['A'];
		bool rightDown = keyStates['D'];
		bool downDown = keyStates['S'];
		bool upDown = keyStates['W'];
		bool shiftDown = keyStates[VK_SHIFT];
		bool ctrlDown = keyStates[VK_CONTROL];
		bool tabDown = keyStates[VK_TAB];
		bool spaceDown = keyStates[VK_SPACE];
		bool mouse2Down = mouse[1];

		int dx = mouseX - lastMouseX;
		int dy = mouseY - lastMouseY;
		int dz = mouseZ - lastMouseZ;
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		lastMouseZ = 0;
		mouseZ = 0;

		float d = 0.5f;
		Camera *camera = m_renderEngine->camera(currentCameraId);
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
			currentCameraId = cam2;
		}
		else {
			currentCameraId = cam1;
		}

		if (mouse2Down) {
			Camera *camera = m_renderEngine->camera(currentCameraId);
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

	void Cinetico3D::startExercise(Exercise &exercise) {
		m_playingExercise = &exercise;
		m_exerciseFrameCount = 0;
		m_playingMode = Cinetico3D::EXERCISE_MODE;
		m_bodyTracker->setTrackableBodyPoints(exercise.trackableBodyPoints());

		::ShowWindow(g_world3DWindow, SW_SHOWNORMAL);
		::UpdateWindow(g_world3DWindow);
	}

	void Cinetico3D::startPlayground() {
		m_playingExercise = NULL;
		m_exerciseFrameCount = 0;
		m_playingMode = Cinetico3D::FREE_MODE;

		//m_bodyTracker->setTrackableBodyPoints(exercise.trackableBodyPoints());
		::ShowWindow(g_world3DWindow, SW_SHOWNORMAL);
		::UpdateWindow(g_world3DWindow);
	}

	void Cinetico3D::update() {

		bool leftDown = keyStates['A'];
		bool rightDown = keyStates['D'];
		bool downDown = keyStates['S'];
		bool upDown = keyStates['W'];
		bool shiftDown = keyStates[VK_SHIFT];
		bool ctrlDown = keyStates[VK_CONTROL];
		bool tabDown = keyStates[VK_TAB];
		bool spaceDown = keyStates[VK_SPACE];
		bool escDown = keyStates[VK_ESCAPE];

		bool quit3D = false;
		if (keyStates[VK_ESCAPE] == 1 && lastKeyStates[VK_ESCAPE] == 0) {
			quit3D = true;
		}

		lastKeyStates[VK_ESCAPE] = keyStates[VK_ESCAPE];

		if (quit3D) {
			::ShowWindow(g_world3DWindow, SW_HIDE);
			m_application.goTo(Cinetico::EXERCISES);
			return;
		}

		float d = 0.1f;

		if (m_playingMode == Cinetico3D::EXERCISE_MODE) {
			updateExerciseMode();
		}
		else if (m_playingMode == Cinetico3D::FREE_MODE) {
			m_bodyTracker->track();
			m_humanChar->update();
		}
		if (!shiftDown) {
			//Update quads (billboarding)
			processCamera();
			Camera *cam = m_renderEngine->camera(cam1);
			render3d::Vector3 faceCam = render3d::Vector3(-cam->rot().x(), -cam->rot().y(), -cam->rot().z());
			m_renderEngine->resourceInstance(instanceQuad1)->setRot(faceCam);
			m_renderEngine->resourceInstance(instanceQuad2)->setRot(faceCam);
		}


		ResourceInstance *instance = m_renderEngine->resourceInstance(instanceCube);
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


	void Cinetico3D::updateExerciseMode() {
		m_bodyTracker->track();

		Body *body;
		body = m_bodyTracker->body();
		if (body) {
			if (m_playingExercise->state() == Exercise::Idle)
				m_playingExercise->start(*body);

			m_humanChar->update();
			if (m_playingExercise->step() == Exercise::Finished)
				m_playingExercise->start(*body);
		}
	}


	void Cinetico3D::renderExerciseMode() {
		m_renderEngine->setCurrentCamera(currentCameraId);
		m_renderEngine->setCurrentViewport(viewport1);
		m_renderEngine->clear(render3d::Color(30, 30, 30));

		m_renderEngine->drawResource(instanceCube);

		if (m_dummyChar)
			m_dummyChar->render();
		if (m_humanChar)
			m_humanChar->render();

		m_renderEngine->drawResource(instanceTerrain);

		m_renderEngine->setCurrentFont(resFontArial);
		std::string str = "Exercício selecionado: ";
		str += m_playingExercise->name();
		m_renderEngine->drawText(str.c_str(), 500, 10, render3d::Color(255, 255, 255, 100));
		m_renderEngine->setCurrentFont(resFontVerdana);

		m_renderEngine->setCurrentCamera(cam2);
		m_renderEngine->setCurrentViewport(viewport2);
		m_renderEngine->clear(render3d::Color(0, 40, 100));
	}

	void Cinetico3D::renderFreeMode() {
		m_renderEngine->setCurrentCamera(currentCameraId);
		m_renderEngine->setCurrentViewport(viewport1);
		m_renderEngine->clear(render3d::Color(30, 30, 30));
		m_renderEngine->drawResource(instanceTerrain);
	}

	void Cinetico3D::renderDebugMode() {
		ResourceInstance *instance = m_renderEngine->resourceInstance(instanceCube);

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
	}

	void Cinetico3D::render() {
		g_frameCount++;

		m_renderEngine->beginScene();

		if (m_playingMode == Cinetico3D::EXERCISE_MODE)
			renderExerciseMode();
		else if (m_playingMode == Cinetico3D::FREE_MODE)
			renderFreeMode();

		//
		m_renderEngine->endScene();
	}

}