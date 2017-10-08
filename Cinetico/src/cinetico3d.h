#ifndef __CINETICO_3D_H__
#define __CINETICO_3D_H__

#include "entity/core/Exercise.h"

namespace cinetico {

	class Cinetico;

	class Cinetico3D {
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
		void mapBodyPointToWorldPoint(int instId, BodyPoint::BodyPart bodyPoint);
		void updateBody();
		void renderBody();
		void processCamera();

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