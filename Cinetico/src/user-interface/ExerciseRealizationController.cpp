
#include "ExerciseRealizationController.h"
#include "cinetico.h"

namespace cinetico {

	extern Cinetico g_cinetico;
	extern void setupWorld3D();

	ExerciseRealizationController::ExerciseRealizationController() {

	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter() {
		
		setupWorld3D();
		g_cinetico.setOnWorld3D(true);
	}

	void ExerciseRealizationController::onViewQuit() {

	}

}