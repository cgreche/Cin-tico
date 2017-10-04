
#include "ExerciseRealizationController.h"
#include "cinetico.h"
#include "mainwindow.h"

namespace cinetico {

	extern Cinetico g_cinetico;

	ExerciseRealizationController::ExerciseRealizationController() {

	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter(ViewParams *params) {
		g_cinetico.mainWindow()->setVisible(false);
		g_cinetico.cinetico3D()->startExercise(*(Exercise*)(*params)["exercise"]);
	}

	void ExerciseRealizationController::onViewTick() {
		g_cinetico.cinetico3D()->update();
		g_cinetico.cinetico3D()->render();
	}

	void ExerciseRealizationController::onViewQuit() {
		g_cinetico.mainWindow()->setVisible(true);
	}

}