
#ifndef __CINETICO_EXERCISEPLAYMODE_H__
#define __CINETICO_EXERCISEPLAYMODE_H__

#include "PlayMode.h"

namespace cinetico_core {
	class Exercise;
	class GestureCommandsManager;
}

namespace cinetico {

	class HumanCharacter;
	class DummyCharacter;
	class RelOrientChar;
	class HadoukenController;

	class ExercisePlayMode : public PlayMode {

		enum PlayModeState {
			WAITING_FOR_BODY,
			ON_EXERCISE,
			EXERCISE_FINISHED
		};


		cinetico_core::Exercise &m_exercise;

		HumanCharacter *m_humanChar;
		//DummyCharacter *m_dummyChar;
		//RelOrientChar *m_roChar;
		GestureCommandsManager *m_commandsManager;
		HadoukenController *m_hadoukenController;

		int m_resTerrain;
		int m_instanceTerrain;
		int m_cam1;
		int m_cam2;
		int m_currentCameraId;

		int m_viewportActionPreview;

		int m_resFontArial;
		int m_resFontVerdana;

		void processCamera();
	public:
		ExercisePlayMode(Cinetico &cinetico, cinetico_core::Exercise &exercise);

		virtual void setup();
		virtual void step();
		virtual void render();
		virtual void cleanUp();
	};

}

#endif