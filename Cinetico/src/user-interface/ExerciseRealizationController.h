
#ifndef __CINETICO_EXERCISEREALIZATIONCONTROLLER_H__
#define __CINETICO_EXERCISEREALIZATIONCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {
	using namespace uilib;

	class PlayMode;

	class ExerciseRealizationController : public Controller
	{
	public:
		VerticalLayout layout;

		//
		DummyCharacter *m_dummyChar;
		HumanCharacter *m_humanChar;

		void step();
		void render();
		RenderEngine *m_renderEngine;
		RenderEngineHelper *m_renderEngineHelper;

		PlayMode *m_playingMode;

		int m_oldWidth;
		int m_oldHeight;
		bool m_oldFullscreen;
	public:
		ExerciseRealizationController(CineticoUI &cineticoUI);

		Layout *viewDefinition();
		void onViewEnter(ViewParams params);
		void onViewTick();
		void onViewQuit();
	};

}

#endif