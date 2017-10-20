
#ifndef __CINETICO_EXERCISEPLAYMODE_H__
#define __CINETICO_EXERCISEPLAYMODE_H__

#include "PlayMode.h"

namespace cinetico_core {
	class Exercise;
}

namespace cinetico {

	class HumanCharacter;
	class DummyCharacter;

	class ExercisePlayMode : public PlayMode {

		cinetico_core::Exercise &m_exercise;

		HumanCharacter *m_humanChar;
		DummyCharacter *m_dummyChar;

		int m_resTerrain;
		int m_instanceTerrain;
		int m_resCube;
		int m_instanceCube;
		int m_cam1;
		int m_cam2;
		int m_currentCameraId;

		int m_viewportActionPreview;

		int m_resFontArial;
		int m_resFontVerdana;

	public:
		ExercisePlayMode(Cinetico &cinetico, cinetico_core::Exercise &exercise);

		virtual void setup();
		virtual void step();
		virtual void render();
		virtual void cleanUp();
	};

}

#endif