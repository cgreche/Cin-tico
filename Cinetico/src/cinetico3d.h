#ifndef __CINETICO_3D_H__
#define __CINETICO_3D_H__

namespace cinetico {

	class Cinetico;

	class Cinetico3D {
		Cinetico &m_application;

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
		void update();
		void render();
	};

}

#endif