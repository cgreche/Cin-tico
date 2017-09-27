#ifndef __CINETICO_3D_H__
#define __CINETICO_3D_H__

namespace cinetico {

	class Cinetico;
	class Exercise;

	class Cinetico3D {
		Cinetico &m_application;
		Exercise *m_playingExercise;

		void setup();
		void cleanUp();

		void setupWindow();
		void setupRenderEngine();
		void setupDrawables();
		void setupCameras();
		void setupViewports();

		void setupBody();
		void updateBody();
		void renderBody();
		void processCamera();

	public:
		Cinetico3D(Cinetico &cinetico);
		~Cinetico3D();
		void startExercise(Exercise &exercise);
		void update();
		void render();
	};

}

#endif