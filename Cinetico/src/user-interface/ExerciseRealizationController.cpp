#include <windows.h>
#include <time.h>
#include "cinetico.h"
#include "cineticoui.h"
#include "ExerciseRealizationController.h"
#include "dummycharacter.h"
#include "humancharacter.h"

#include "playmodes/ExercisePlayMode.h"
#include "playmodes/FreePlayMode.h"

namespace cinetico {

	ExerciseRealizationController::ExerciseRealizationController(Cinetico &cinetico)
		: Controller(cinetico) {
		m_playingMode = NULL;
	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter(ViewParams *params) {

		PlayMode::PlayModeID playModeId = (PlayMode::PlayModeID)(int)(*params)["play_mode"];

		m_renderEngine = m_cinetico.cineticoUI()->renderEngine();
		m_renderEngineHelper = m_cinetico.cineticoUI()->renderEngineHelper();
		m_cinetico.cineticoUI()->setHeaderAndFooterVisible(false);

		PlayMode *playMode = NULL;

		if (playModeId == PlayMode::EXERCISE_MODE) {
			Exercise *exercise = (Exercise*)(*params)["exercise"];
			playMode = new ExercisePlayMode(m_cinetico, *exercise);
		}

		if (playMode)
			playMode->setup();
		m_playingMode = playMode;
	}

	void ExerciseRealizationController::onViewTick() {
		step();
		render();
	}

	void ExerciseRealizationController::onViewQuit() {
		m_cinetico.cineticoUI()->setHeaderAndFooterVisible(true);
		if (m_playingMode)
			m_playingMode->cleanUp();
	}



	/*
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
	*/

	void ExerciseRealizationController::step() {
		if (m_playingMode)
			m_playingMode->step();
	}

	void ExerciseRealizationController::render() {
		if (m_playingMode) {
			m_renderEngine->beginScene();
			m_playingMode->render();
			m_renderEngine->endScene();
		}
	}

}