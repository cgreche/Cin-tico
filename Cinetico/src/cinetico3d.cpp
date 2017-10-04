#include <windows.h>
#include <time.h>
#include "cinetico.h"
#include "cinetico3d.h"
#include "render3d/d3d9engine.h"
#include "render3d/renderenginehelper.h"
#include "bodytracker.h"
#include "kinectsensor.h"

using namespace cinetico_core;
using namespace render3d;

HWND g_world3DWindow = NULL;

#define GRAY_COLOR 80

namespace cinetico {

	Cinetico3D::Cinetico3D(Cinetico &cinetico)
	: m_application(cinetico) {
		m_playingExercise = NULL;
		setup();
	}

	Cinetico3D::~Cinetico3D() {
		cleanUp();
	}

	KinectSensor kinectSensor;
	BodyTracker *m_bodyTracker;

	float aspectRatio43 = 1.33334f; //4:3
	float aspectRatio51 = 1.25; //5:4
	float aspectRatio169 = 1.77778f; //16:9

	float currentAspectRatio = aspectRatio169;

	int winWidth = 1280;
	int winHeight = (int)ceil(winWidth / currentAspectRatio);

	int g_frameCount = 0;

	RenderEngine *renderEngine;
	RenderEngineHelper *renderEngineHelper;

	int resQuad1;
	int resQuad2;
	int line1;
	int line2;

	int resTriangle;
	int resCube;
	int resTriangleType2;
	int resCircle;
	int resTerrain;
	int resPrism;

	int instanceQuad1;
	int instanceQuad2;
	int instanceTriangle;
	int instanceCube;
	int instanceTriangleType2;
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


#define HEAD_SIZE 4.f/10.f
#define FOOT_SIZE HEAD_SIZE/2.f
#define HAND_SIZE HEAD_SIZE/2.f
#define ELBOW_SIZE HEAD_SIZE/6.f
#define KNEE_SIZE HEAD_SIZE/6.f


	struct BodyResourceIds {
		int head;
		int spine;
		int elbow;
		int hand;
		int knee;
		int foot;
	};


	struct BodyInstanceIds {
		int head;
		int spine;
		int leftElbow;
		int rightElbow;
		int leftHand;
		int rightHand;
		int leftKnee;
		int rightKnee;
		int leftFoot;
		int rightFoot;
	};

	BodyResourceIds g_bodyResourceIds;
	BodyInstanceIds g_bodyInstanceIds;


	void Cinetico3D::setupBody() {
		BodyResourceIds &resId = g_bodyResourceIds;
		BodyInstanceIds &instId = g_bodyInstanceIds;

		Color bodyColors[] =
		{
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),

			//Back
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),

			//Top
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),

			//Bottom
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),

			//Left
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),

			//Right
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
			Color(GRAY_COLOR,GRAY_COLOR,GRAY_COLOR),
		};

		resId.head = renderEngineHelper->createCube(HEAD_SIZE, bodyColors);
		resId.spine = renderEngineHelper->createRectangularPrism(HEAD_SIZE / 3.5f, HEAD_SIZE * 1.5, HEAD_SIZE / 3.5f, bodyColors);
		resId.elbow = renderEngineHelper->createCube(ELBOW_SIZE, bodyColors);
		resId.hand = renderEngineHelper->createCube(HAND_SIZE, bodyColors);
		resId.knee = renderEngineHelper->createCube(KNEE_SIZE, bodyColors);
		resId.foot = renderEngineHelper->createCube(FOOT_SIZE, bodyColors);

		instId.head = renderEngine->newResourceInstance(resId.head);
		instId.spine = renderEngine->newResourceInstance(resId.spine);
		instId.leftElbow = renderEngine->newResourceInstance(resId.elbow);
		instId.rightElbow = renderEngine->newResourceInstance(resId.elbow);
		instId.leftHand = renderEngine->newResourceInstance(resId.hand);
		instId.rightHand = renderEngine->newResourceInstance(resId.hand);
		instId.leftKnee = renderEngine->newResourceInstance(resId.knee);
		instId.rightKnee = renderEngine->newResourceInstance(resId.knee);
		instId.leftFoot = renderEngine->newResourceInstance(resId.foot);
		instId.rightFoot = renderEngine->newResourceInstance(resId.foot);
	}

	void Cinetico3D::mapBodyPointToWorldPoint(int instId, BodyPoint::BodyPart bodyPoint) {
		ResourceInstance *instance;
		Body *body = m_bodyTracker->body();
		if (!body)
			return;
		instance = renderEngine->resourceInstance(instId);
		cinetico_core::Vector3 pos = body->bodyPoint(bodyPoint)->position();
		instance->setPos(render3d::Vector3(pos.x()*2, pos.y()*2, pos.z()*2));
	}

	void Cinetico3D::updateBody() {
		m_bodyTracker->track();
		BodyInstanceIds &instId = g_bodyInstanceIds;

		mapBodyPointToWorldPoint(instId.head, BodyPoint::Head);
		mapBodyPointToWorldPoint(instId.spine, BodyPoint::Spine);
		mapBodyPointToWorldPoint(instId.leftElbow, BodyPoint::LeftElbow);
		mapBodyPointToWorldPoint(instId.rightElbow, BodyPoint::RightElbow);
		mapBodyPointToWorldPoint(instId.leftHand, BodyPoint::LeftPalm);
		mapBodyPointToWorldPoint(instId.rightHand, BodyPoint::RightPalm);
		mapBodyPointToWorldPoint(instId.leftKnee, BodyPoint::LeftKnee);
		mapBodyPointToWorldPoint(instId.rightKnee, BodyPoint::RightKnee);
		mapBodyPointToWorldPoint(instId.leftFoot, BodyPoint::LeftFoot);
		mapBodyPointToWorldPoint(instId.rightFoot, BodyPoint::RightFoot);
	}

	void Cinetico3D::renderBody() {
		BodyInstanceIds &instId = g_bodyInstanceIds;

		Body *body = m_bodyTracker->body();

		if (!body || body->identifiedBodyPointCount() == 0)
			return;

		renderEngine->drawResource(instId.head);
		renderEngine->drawResource(instId.spine);
		renderEngine->drawResource(instId.leftElbow);
		renderEngine->drawResource(instId.rightElbow);
		renderEngine->drawResource(instId.leftHand);
		renderEngine->drawResource(instId.rightHand);
		renderEngine->drawResource(instId.leftKnee);
		renderEngine->drawResource(instId.rightKnee);
		renderEngine->drawResource(instId.leftFoot);
		renderEngine->drawResource(instId.rightFoot);
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
		renderEngine = new D3D9Engine();
		renderEngine->configure(g_world3DWindow);
		renderEngine->init();
		renderEngineHelper = new RenderEngineHelper(*renderEngine);
	}


	void Cinetico3D:: setupDrawables() {
		::srand((unsigned int)::time(0));
		setupBody();

		render3d::Vector3 triangle1[] = {
			{ 0.3f, 0.0f, 1 },
			{ 0.5f, 0.0f, 1 },
			{ 0, -0.5f, 1 },
		};

		render3d::Vector3 triangle3[] = {
			{ -1, 1, 0 },
			{ 1, 1, 0 },
			{ -1, -1, 0 }
		};

		int indicesTriangle1[] = {
			0,1,2
		};

		Color triangle1Colors[3] = {
			Color(255,0,0),
			Color(255,0,0),
			Color(255,0,0)
		};

		int indicesTriangle3[] = {
			0,1,2
		};

		Color triangle3Colors[3] = {
			Color(255,0,0),
			Color(255,0,0),
			Color(255,0,0)
		};


		const int numPoints = 30;
		Color circleColors[numPoints + 1];
		for (int i = 0; i < numPoints + 1; ++i) {
			circleColors[i] = Color(rand() % 255, rand() % 255, rand() % 255, 122);
		}

		resQuad1 = renderEngineHelper->createQuad(10, 0.25);
		resQuad2 = renderEngineHelper->createQuad(100, 200);
		resTriangle = renderEngine->newResource(sizeof(triangle1) / sizeof(triangle1[0]), triangle1, 3, indicesTriangle1, triangle1Colors);

		Color cubeColors[] =
		{
			Color(255,0,0),
			Color(255,0,0),
			Color(255,0,0),
			Color(255,0,0),

			//Back
			Color(255,0,0),
			Color(255,0,0),
			Color(255,0,0),
			Color(255,0,0),

			//Top
			Color(0,0,0),
			Color(0,0,0),
			Color(0,0,0),
			Color(0,0,0),

			//Bottom
			Color(255,255,0),
			Color(255,255,0),
			Color(255,255,0),
			Color(255,255,0),

			//Left
			Color(255,255,255),
			Color(255,255,255),
			Color(255,255,255),
			Color(255,255,255),

			//Right
			Color(0,0,255),
			Color(0,0,255),
			Color(0,0,255),
			Color(0,0,255),
		};

		resCube = renderEngineHelper->createCube(2, cubeColors);
		resTriangleType2 = renderEngine->newResource(sizeof(triangle3) / sizeof(triangle3[0]), triangle3, 3, indicesTriangle3, triangle3Colors);
		resCircle = renderEngineHelper->createCircle(0.3f, numPoints + 1, circleColors);
		resPrism = renderEngineHelper->createRectangularPrism(0.3f, 2.f, 0.5f,cubeColors);

		instanceQuad1 = renderEngine->newResourceInstance(resQuad1);
		renderEngine->resourceInstance(instanceQuad1)->setPos(render3d::Vector3(10, 10, 0));
		instanceQuad2 = renderEngine->newResourceInstance(resQuad2);
		renderEngine->resourceInstance(instanceQuad2)->setPos(render3d::Vector3(120, 10, 0));
		instanceTriangle = renderEngine->newResourceInstance(resTriangle);
		instanceCube = renderEngine->newResourceInstance(resCube);
		renderEngine->resourceInstance(resCube)->setPos(render3d::Vector3(0.f, 1.f, 0.f));
		instanceTriangleType2 = renderEngine->newResourceInstance(resTriangleType2);
		instanceCircle = renderEngine->newResourceInstance(resCircle);
		instanceCircle2 = renderEngine->newResourceInstance(resCircle);
		instancePrism = renderEngine->newResourceInstance(resPrism);

		ResourceInstance *circle2 = renderEngine->resourceInstance(instanceCircle2);
		circle2->setPos(render3d::Vector3(2.0f, 0.0f, 0.0f));


		for (int i = 0; i < NUM_CUBES; ++i) {
			instancedCubes[i] = renderEngine->newResourceInstance(resCube);
			ResourceInstance* inst = renderEngine->resourceInstance(instancedCubes[i]);
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
		resTerrain = renderEngineHelper->generateTerrain(quadSize, quadCountH, quadCountV);
		instanceTerrain = renderEngine->newResourceInstance(resTerrain);
		instanceWall = renderEngine->newResourceInstance(resTerrain);

		ResourceInstance *instance = (ResourceInstance*)renderEngine->resourceInstance(instanceWall);
		instance->setPos(render3d::Vector3(0.f, quadCountV / 2 * quadSize, quadCountV / 2 * quadSize));
		instance->setRot(render3d::Vector3(-3.14159f * 0.5f, 0.f, 0.f));

		ResourceInstance *terrain = (ResourceInstance*)renderEngine->resourceInstance(instanceTerrain);
		terrain->setPos(render3d::Vector3(0, -2, 0));


		resFontArial = renderEngine->newFontResource("Arial", 15, 30, FontResource::BOLD);
		resFontVerdana = renderEngine->newFontResource("Verdana", 20, 200, FontResource::ITALIC);
	}

	void Cinetico3D::setupCameras() {
		cam1 = renderEngine->newCamera(render3d::Vector3(0.f, 3.f, -6.4f), render3d::Vector3(-0.3f, 0.f, 0.f));
		cam2 = renderEngine->newCamera(render3d::Vector3(8.f, 8.f, -8.f), render3d::Vector3(0.f, 0.f, 0.f));
	}

	void Cinetico3D::setupViewports() {
		viewport1 = renderEngine->newViewport(0, 0, winWidth, winHeight);
		float percent = 0.25f;
		viewport2 = renderEngine->newViewport(20, 20, (int)(winWidth*percent), (int)(winHeight*percent));
	}

	void Cinetico3D::setup() {
		kinectSensor.initialize();
		m_bodyTracker = new BodyTracker(kinectSensor);
		setupWindow();
		setupRenderEngine();
		setupDrawables();
		setupCameras();
		setupViewports();
	}

	void Cinetico3D::cleanUp() {
		if (m_bodyTracker)
			delete m_bodyTracker;
		kinectSensor.finalize();
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
		Camera *camera = renderEngine->camera(currentCameraId);
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
			Camera *camera = renderEngine->camera(currentCameraId);
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
		m_bodyTracker->setTrackableBodyPoints(exercise.trackableBodyPoints());
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

		updateBody();
		//Update quads (billboarding)
		processCamera();
		Camera *cam = renderEngine->camera(cam1);
		render3d::Vector3 faceCam = render3d::Vector3(-cam->rot().x(), -cam->rot().y(), -cam->rot().z());
		renderEngine->resourceInstance(instanceQuad1)->setRot(faceCam);
		renderEngine->resourceInstance(instanceQuad2)->setRot(faceCam);

		return;

		ResourceInstance *instance = renderEngine->resourceInstance(instanceCube);
		render3d::Vector3 newPos = instance->pos();
		render3d::Vector3 newRot = instance->rot();
		
		if (leftDown) {
			if (shiftDown) {
				newRot.setY(newRot.y() + d);
			}
			else {
				newPos.setX(newPos.x() - d);
			}
		}

		if (rightDown) {
			if (shiftDown) {
				newRot.setY(newRot.y() - d);
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


	void Cinetico3D::render() {
		g_frameCount++;

		renderEngine->beginScene();

		ResourceInstance *instance = renderEngine->resourceInstance(instanceCube);

		renderEngine->setCurrentCamera(currentCameraId);
		renderEngine->setCurrentViewport(viewport1);
		renderEngine->clear(Color(30, 30, 30));

		renderEngine->drawResource(instanceTerrain);
		renderBody();

		
		/*
		renderEngine->drawResource(instanceQuad2);
		static int state = 1;
		if (g_frameCount % 30 == 0)
			state ^= 1;
		if (state == 1)
			renderEngine->drawResource(instanceQuad1);
		*/
		//renderEngine->drawResource(instanceWall);
		/*
		renderEngine->drawResource(instanceTriangle);
		renderEngine->drawResource(instanceCube);
		renderEngine->drawResource(instanceTriangleType2);
		renderEngine->drawResource(instanceCircle);
		renderEngine->drawResource(instanceCircle2);
		renderEngine->drawResource(instancePrism);

		for(int i = 0; i < NUM_CUBES; ++i) {
	//		d3d9.drawResource(instancedCubes[i]);
		}
		*/


		if (m_playingExercise) {
			renderEngine->setCurrentFont(resFontArial);
			std::string str = "Exerc�cio selecionado: ";
			str += m_playingExercise->name();
			renderEngine->drawText(str.c_str(), 500, 10, Color(255, 255, 255, 100));
			renderEngine->setCurrentFont(resFontVerdana);
		}


		//
		renderEngine->setCurrentCamera(cam2);
		renderEngine->setCurrentViewport(viewport2);
		renderEngine->clear(Color(0, 40, 100));
		instance->setScale(0.5f);
		renderEngine->drawResource(instanceCube);

		renderEngine->endScene();
	}

}