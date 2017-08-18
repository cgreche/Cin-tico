#include <windows.h>
#include <math.h>
#include <time.h>
#include "render/d3d9engine.h"
#include "render/renderenginehelper.h"

float aspectRatio43 = 1.33334f; //4:3
float aspectRatio51 = 1.25; //5:4
float aspectRatio169 = 1.77778f; //16:9

float currentAspectRatio = aspectRatio169;

int winWidth = 1280;
int winHeight = (int)ceil(winWidth / currentAspectRatio);

HWND g_hwnd;

RenderEngine *renderEngine;
RenderEngineHelper *renderEngineHelper;

int resTriangle;
int resCube;
int resTriangleType2;
int resCircle;
int resTerrain;

int instanceTriangle;
int instanceCube;
int instanceTriangleType2;
int instanceCircle;
int instanceCircle2;
int instanceTerrain;
int instanceWall;

int cam1;
int cam2;

int viewport1;
int viewport2;

bool keyStates[256] = { 0 };
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

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch(msg) {
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

void setupWindow() {

	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	const char wndClassName[] = "MainWindow";
	const char wndName[] = "MainWindow";

	WNDCLASS wndClass = {0};
	wndClass.style = 0;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wndClass.lpszClassName = wndClassName;
	wndClass.lpfnWndProc = MainWndProc;

	ATOM res = ::RegisterClass(&wndClass);

	DWORD winStyle = WS_OVERLAPPEDWINDOW;
	RECT rc = { 0,0, winWidth,winHeight };
	::AdjustWindowRect(&rc, winStyle, FALSE);

	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	HWND hwnd = ::CreateWindow(wndClassName, wndName, winStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, w, h,
		NULL, NULL, hInstance, 0);

	g_hwnd = hwnd;
}

void setupRenderEngine()
{
	renderEngine = new D3D9Engine();
	renderEngine->configure(g_hwnd);
	renderEngine->init();
	renderEngineHelper = new RenderEngineHelper(*renderEngine);
}


void setupDrawables() {
	::srand((unsigned int)::time(0));

	Vertex3 triangle1[] = {
		{ 0.3f, 0.0f, 1 },
		{ 0.5f, 0.0f, 1 },
		{ 0, -0.5f, 1 },
	};

	Vertex3 triangle3[] = {
		{ -1, 1, 0 },
		{ 1, 1, 0 },
		{ -1, -1, 0 }
	};

	const int numPoints = 30;
	Color circleColors[numPoints + 1];
	

	int indices1[] = {
		0,1,2
	};

	int indices3[] = {
		0,1,2
	};

	for(int i = 0; i < numPoints + 1; ++i) {
		circleColors[i] = Color(rand()%255, rand()%255, rand()%255, 122);
	}



	resTriangle = renderEngine->newResource(sizeof(triangle1) / sizeof(triangle1[0]), triangle1, 3, indices1,&Color(255,0,0));
	resCube = renderEngineHelper->createCube(2);
	resTriangleType2 = renderEngine->newResource(sizeof(triangle3)/sizeof(triangle3[0]),triangle3,3,indices3);
	resCircle = renderEngineHelper->createCircle(0.3f, numPoints + 1,circleColors);

	instanceTriangle = renderEngine->newResourceInstance(resTriangle);
	instanceCube = renderEngine->newResourceInstance(resCube);
	renderEngine->resourceInstance(resCube)->setPos(Vector3(0.f, 1.f, 0.f));
	instanceTriangleType2 = renderEngine->newResourceInstance(resTriangleType2);
	instanceCircle = renderEngine->newResourceInstance(resCircle);
	instanceCircle2 = renderEngine->newResourceInstance(resCircle);

	ResourceInstance *circle2 = renderEngine->resourceInstance(instanceCircle2);
	circle2->setPos(Vector3(2.0f, 0.0f, 0.0f));


	for(int i = 0; i < NUM_CUBES; ++i) {
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
		inst->setPos(Vector3(x, y, z));
		inst->setRot(Vector3(rx, ry, rz));
		inst->setScale(s);
	}

	//setup terrain
	Vertex3 *terrainVertices;
	int *terrainIndices;
	int terrainVertexCount;
	int terrainIndexCount;

	float quadSize = 3.2f;
	float quadCountH = 128;
	float quadCountV = 32;
	resTerrain = renderEngineHelper->generateTerrain(quadSize, quadCountH, quadCountV);
	instanceTerrain = renderEngine->newResourceInstance(resTerrain);
	instanceWall = renderEngine->newResourceInstance(resTerrain);

	ResourceInstance *instance = (ResourceInstance*)renderEngine->resourceInstance(instanceWall);
	instance->setPos(Vector3(0.f, quadCountV/2*quadSize, quadCountV/2*quadSize));
	instance->setRot(Vector3(-3.14159f * 0.5f, 0.f,  0.f));
}

void setupCameras() {
	cam1 = renderEngine->newCamera(Vector3(0.f, 3.f, -6.4f), Vector3(-0.3f, 0.f, 0.f));
	cam2 = renderEngine->newCamera(Vector3(8.f, 8.f, -8.f), Vector3(0.f, 0.f, 0.f));
}

void setupViewports() {
	viewport1 = renderEngine->newViewport(0, 0, winWidth, winHeight);
	float percent = 0.25f;
	viewport2 = renderEngine->newViewport(20, 20, (int)(winWidth*percent), (int)(winHeight*percent));
}

void setup()
{
	setupWindow();
	setupRenderEngine();
	setupDrawables();
	setupCameras();
	setupViewports();
}


void update()
{
	float d = 0.1f;
	ResourceInstance *instance = renderEngine->resourceInstance(instanceCube);
	Vector3 newPos = instance->pos();
	Vector3 newRot = instance->rot();

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

	static Vector3 camPos(0.f, 0.f, -8.0f);

	if(spaceDown) {
		currentCameraId = cam2;
	}
	else {
		currentCameraId = cam1;
	}

	if(mouse2Down) {
		Camera *camera = renderEngine->camera(currentCameraId);
		Vector3 camRot = camera->rot();

		if(dx != 0) {
			static  float ang = 0.f;
			const float PI = 3.14159f;
			ang += 0.001* PI / 180;
			camRot.setY(camRot.y() - 0.001*sinf(ang));
			camRot.setZ(camRot.z() + 0.001*cosf(ang));
		}

		if(dy != 0) {
			float camRotX = camRot.x();
			camRot.setX(camRot.x() - 0.01f*dy);
		}

		if(dz != 0)
			camera->setZoom(camera->zoom()-dz*0.1f);

		camera->setRot(camRot);
	}

	if(tabDown) {
		float d = 0.5f;
		Camera *camera = renderEngine->camera(currentCameraId);
		Vector3 camPos = camera->pos();
		if(rightDown) {
			camPos.setX(camPos.x() + d);
		}
		
		if(leftDown) {
			camPos.setX(camPos.x() - d);
		}

		if(upDown) {
			if(ctrlDown)
				camPos.setZ(camPos.z() + d);
			else
				camPos.setY(camPos.y() + d);
		}

		if(downDown) {
			if(ctrlDown)
				camPos.setZ(camPos.z() - d);
			else
				camPos.setY(camPos.y() - d);
		}

		camera->setPos(camPos);
		return;
	}

	if(leftDown) {
		if(shiftDown) {
			newRot.setY(newRot.y() + d);
		}
		else {
			newPos.setX(newPos.x() - d);
		}
	}

	if(rightDown) {
		if(shiftDown) {
			newRot.setY(newRot.y() - d);
		}
		else {
			newPos.setX(newPos.x() + d);
		}
	}

	if(downDown) {
		if(shiftDown) {
			newRot.setX(newRot.x() - d);
		}
		else if(ctrlDown) {
			newPos.setZ(newPos.z() - d);
		}
		else {
			newPos.setY(newPos.y() - d);
		}
	}

	if(upDown) {
		if(shiftDown) {
			newRot.setX(newRot.x() + d);
		}
		else if(ctrlDown) {
			newPos.setZ(newPos.z() + d);
		}
		else {
			newPos.setY(newPos.y() + d);
		}
	}

	instance->setPos(newPos);
	instance->setRot(newRot);
}


void render()
{
	renderEngine->beginScene();

	ResourceInstance *instance = renderEngine->resourceInstance(instanceCube);

	renderEngine->setCurrentCamera(currentCameraId);
	renderEngine->setCurrentViewport(viewport1);
	renderEngine->clear(Color(30, 30, 30));
	renderEngine->drawResource(instanceTerrain);
	renderEngine->drawResource(instanceWall);
	renderEngine->drawResource(instanceTriangle);
	renderEngine->drawResource(instanceCube);
	renderEngine->drawResource(instanceTriangleType2);
	renderEngine->drawResource(instanceCircle);
	renderEngine->drawResource(instanceCircle2);
	
	
	for(int i = 0; i < NUM_CUBES; ++i) {
//		d3d9.drawResource(instancedCubes[i]);
	}

	renderEngine->setCurrentCamera(cam2);
	renderEngine->setCurrentViewport(viewport2);
	renderEngine->clear(Color(0, 40, 100));
	instance->setScale(0.5f);
	renderEngine->drawResource(instanceCube);
	
	renderEngine->endScene();
}







int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	setup();
	::ShowWindow(g_hwnd, nCmdShow);
	::UpdateWindow(g_hwnd);

	while(true) {
		
		MSG msg;
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
				break;
		}

		update();
		render();
	}

	return 0;
}