#ifndef __CINETICO_3D_H__
#define __CINETICO_3D_H__

#include "entity/core/Exercise.h"

namespace cinetico {

	class Cinetico;

	class Character {
	protected:
		std::vector<int> m_instanceIds;
		cinetico_core::Vector3 m_position;

	public:
		Character() { };

		virtual void setPosition(const cinetico_core::Vector3 &position) { m_position = position; }

		virtual void update() = 0;
		virtual void render() = 0;
	};

	class DummyCharacter : public Character {


	public:
		DummyCharacter();

		virtual void update();
		virtual void render();
	};

	class HumanCharacter : public Character {
		Body *m_body;

		void mapBodyPointToWorldPoint(int instId, BodyPoint::BodyPart bodyPoint);
	public:
		HumanCharacter();

		virtual void update();
		virtual void render();
	};

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
	};

}

#endif