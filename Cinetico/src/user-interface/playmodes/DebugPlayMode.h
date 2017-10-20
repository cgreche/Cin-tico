
#ifndef __CINETICO_DEBUGPLAYMODE_H__
#define __CINETICO_DEBUGPLAYMODE_H__

#include "PlayMode.h"

namespace cinetico {

	class DebugPlayMode : public PlayMode {
		void processCamera();

	public:
		DebugPlayMode(Cinetico &cinetico);

		virtual void setup();
		virtual void step();
		virtual void render();
		virtual void cleanUp();

	};

}

#endif