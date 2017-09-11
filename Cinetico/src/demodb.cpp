#include <windows.h>
#include <math.h>
#include <time.h>

#include "uilib/ui/uibase.h"
#include "user-interface/LoginController.h"

using namespace cinetico;

LoginController *g_loginController;

MainWindow *g_mainWindow;

bool g_onWorld3D = false;


class MainWindow : public Window
{
	VerticalLayout vLayout;
	ComboBox test;
	Button b;
public:
	MainWindow() {
		setTitle("Cin�tico");
		setFrameSize(Size(1024, 768));
		setVisible(true);
	}
	void onCloseEvent() {
		this->setVisible(false);
		if (!g_onWorld3D) {
			uibase::UIQuit();
		}
	}
};




LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
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
	g_mainWindow = new MainWindow();

}



void setup()
{
	extern void setupDB();
	setupDB();
	setupWindow();

#if 0
	g_loginController = new LoginController();
	g_loginController->onViewEnter();
	g_mainWindow->setLayout(g_loginController->viewDefinition());
#else
	extern void setupWorld3D();
	setupWorld3D();
	g_onWorld3D = true;
#endif
}


void update()
{
	if(g_onWorld3D) {
		extern void updateWorld3D();
		updateWorld3D();
	}
}


void render()
{
	if(g_onWorld3D) {
		extern void renderWorld3D();
		renderWorld3D();
	}
}

void cleanup() {
	delete g_loginController;

	extern void destroyDB();
	destroyDB();

	extern void destroyWorld3D();
	destroyWorld3D();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	setup();

	while(uibase::UIProcess()) {
		update();
		render();
	}

	cleanup();

	return 0;
}