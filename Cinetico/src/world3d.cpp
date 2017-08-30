#include <windows.h>
#include <time.h>
#include "render3d/d3d9engine.h"
#include "render3d/renderenginehelper.h"

float aspectRatio43 = 1.33334f; //4:3
float aspectRatio51 = 1.25; //5:4
float aspectRatio169 = 1.77778f; //16:9

float currentAspectRatio = aspectRatio169;

int winWidth = 1280;
int winHeight = (int)ceil(winWidth / currentAspectRatio);

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



void setupRenderEngine(HWND hwnd)
{
	renderEngine = new D3D9Engine();
	renderEngine->configure(hwnd);
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

void setupWorld3D(HWND hwnd) {
	setupRenderEngine(hwnd);
	setupDrawables();
	setupCameras();
	setupViewports();
}

void destroyWorld3D() {

}

void renderWorld3D() {
	renderEngine->beginScene();

	renderEngine->setCurrentCamera(currentCameraId);
	renderEngine->setCurrentViewport(viewport1);
	renderEngine->clear(Color(30, 30, 30));
//	renderEngine->drawResource(instanceTerrain);
	
	renderEngine->endScene();
}


void world3D_MainWndFilterProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

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
}