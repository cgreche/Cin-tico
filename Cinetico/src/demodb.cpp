#include <windows.h>
#include <math.h>
#include <time.h>
#include "lib/sqlite-amalgamation-3200100/sqlite3.h"
#include "render/d3d9engine.h"
#include "render/renderenginehelper.h"
#include "database.h"

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
int instanceTriangle;

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

void setupDB() {
	Database db("test.db");
	db.open();
	if(db.isOpen()) {
		time_t t = time(NULL);
		const char *createUserProfile = "CREATE TABLE USER_PROFILE(login_name TEXT, password TEXT, creation_date INTEGER, last_login_date INTEGER);";
		const char *clearDB = "DELETE FROM USER_PROFILE;";
		const char *insertUser = "INSERT INTO USER_PROFILE(login_name, password, creation_date, last_login_date) VALUES(?,?,?,?);";
		const char *getUsers = "SELECT * FROM USER_PROFILE;";
		SQLStatement *stmt1 = db.prepare(createUserProfile);
		SQLStatement *stmt2 = db.prepare(clearDB);
		SQLStatement *stmt3 = db.prepare(insertUser);
		SQLStatement *stmt4 = db.prepare(getUsers);

		int rc;
		if(stmt1) {
			rc = stmt1->execute();
			stmt1->close();
		}

		if(stmt2) {
			rc = stmt2->execute();
			stmt2->close();
		}

		if(stmt3) {
			stmt3->bind(1, "username");
			stmt3->bind(2, "password");
			stmt3->bind(3, t);
			stmt3->bind(4, 0);
			rc = stmt3->execute();
			stmt3->bind(1, "admin");
			stmt3->bind(2, "admin");
			stmt3->bind(3, t);
			stmt3->bind(4, 0);
			rc = stmt3->execute();
			stmt3->close();
		}

		if(stmt4) {
			rc = stmt4->execute();
			stmt4->close();
		}
	}

	db.close();

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
}

void setupCameras() {
	cam1 = renderEngine->newCamera(Vector3(0.f, 3.f, -6.4f), Vector3(-0.3f, 0.f, 0.f));
}

void setupViewports() {
	viewport1 = renderEngine->newViewport(0, 0, winWidth, winHeight);
}

void setup()
{
	setupDB();
	setupWindow();
	setupRenderEngine();
	setupDrawables();
	setupCameras();
	setupViewports();
}


void update()
{
}


void render()
{
	renderEngine->beginScene();

	renderEngine->setCurrentCamera(currentCameraId);
	renderEngine->setCurrentViewport(viewport1);
	renderEngine->clear(Color(30, 30, 30));
//	renderEngine->drawResource(instanceTerrain);
	
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