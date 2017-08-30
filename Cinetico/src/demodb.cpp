#include <windows.h>
#include <math.h>
#include <time.h>

HWND g_hwnd;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	extern void world3D_MainWndFilterProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	world3D_MainWndFilterProc(hwnd,msg,wParam,lParam);

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
	RECT rc = { 0,0, 800,600 };
	::AdjustWindowRect(&rc, winStyle, FALSE);

	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	HWND hwnd = ::CreateWindow(wndClassName, wndName, winStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, w, h,
		NULL, NULL, hInstance, 0);

	g_hwnd = hwnd;
}



void setup()
{
	extern void setupDB();
	setupDB();
	setupWindow();
	extern void setupWorld3D(HWND hwnd);
	setupWorld3D(g_hwnd);
}


void update()
{
}


void render()
{
	extern void renderWorld3D();
	renderWorld3D();

}

void cleanup() {
	extern void destroyDB();
	destroyDB();

	extern void destroyWorld3D();
	destroyWorld3D();
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

	cleanup();

	return 0;
}