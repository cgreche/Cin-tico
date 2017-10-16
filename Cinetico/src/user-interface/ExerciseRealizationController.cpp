
#include "ExerciseRealizationController.h"
#include "cinetico.h"
#include "mainwindow.h"

namespace cinetico {

	extern Cinetico g_cinetico;

	ExerciseRealizationController::ExerciseRealizationController(Cinetico &cinetico)
		: Controller(cinetico) {

	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter(ViewParams *params) {
		g_cinetico.mainWindow()->setVisible(false);

		Exercise *exercise = NULL;
		if(params)
			exercise = (Exercise*)(*params)["exercise"];

		if (exercise) {
			g_cinetico.cinetico3D()->startExercise(*exercise);
		}
		else {
			g_cinetico.cinetico3D()->startPlayground();
		}
	}

	void ExerciseRealizationController::onViewTick() {
		g_cinetico.cinetico3D()->update();
		g_cinetico.cinetico3D()->render();
	}

	void ExerciseRealizationController::onViewQuit() {
		g_cinetico.mainWindow()->setVisible(true);
	}

}