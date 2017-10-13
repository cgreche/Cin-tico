#ifndef __CINETICO_3D_H__
#define __CINETICO_3D_H__

#include "entity/core/Exercise.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include "bodytracker.h"

namespace cinetico {

	class Cinetico;
	class DummyCharacter;
	class HumanCharacter;

	using namespace cinetico_core;
	using namespace render3d;

	class render3d::RenderEngine;
	class render3d::RenderEngineHelper;

	class Cinetico3D {

		enum PlayingMode {
			NOT_PLAYING,
			EXERCISE_MODE,
			FREE_MODE,
			REPLAY_MODE,
			DEBUG_MODE
		};

		Cinetico &m_application;
		cinetico_core::Exercise *m_playingExercise;

		void setup();
		void cleanUp();

		void setupWindow();
		void setupRenderEngine();
		void setupDrawables();
		void setupCameras();
		void setupViewports();

		void setupBody();
		void processCamera();

		void renderExerciseMode();
		void renderFreeMode();
		void renderReplayMode();
		void renderDebugMode();

		Sensor *m_sensor;
		BodyTracker *m_bodyTracker;
		RenderEngine *m_renderEngine;
		RenderEngineHelper *m_renderEngineHelper;

		PlayingMode m_playingMode;
		unsigned int m_globalFrameCount;
		unsigned int m_exerciseFrameCount;

		DummyCharacter *m_dummyChar;
		HumanCharacter *m_humanChar;

	public:
		Cinetico3D(Cinetico &cinetico);
		~Cinetico3D();
		void startExercise(cinetico_core::Exercise &exercise);
		void startPlayground();
		void update();
		void render();

		Sensor *sensor() const { return m_sensor; }
		BodyTracker *bodyTracker() const { return m_bodyTracker; }
		RenderEngine *renderEngine() const { return m_renderEngine; }
		RenderEngineHelper *renderEngineHelper() const { return m_renderEngineHelper; }
	};

}

#endif