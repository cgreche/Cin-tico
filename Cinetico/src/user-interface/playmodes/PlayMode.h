
#ifndef __CINETICO_PLAYMODE_H__
#define __CINETICO_PLAYMODE_H__

namespace render3d {
	class RenderEngine;
	class RenderEngineHelper;
}

namespace cinetico {
	class Cinetico;
	class Dictionary;

	class PlayMode {
	public:
		enum PlayModeID {
			NOT_PLAYING,
			EXERCISE_MODE,
			FREE_MODE,
			REPLAY_MODE,
			DEBUG_MODE
		};

	protected:
		Cinetico &m_cinetico;
		Dictionary &m_dictionary;

		PlayModeID m_playingMode;
		render3d::RenderEngine *m_renderEngine;
		render3d::RenderEngineHelper *m_renderEngineHelper;

	public:
		PlayMode(Cinetico &cinetico, PlayModeID playMode);
		virtual ~PlayMode();

		virtual void setup() { }
		virtual void step() { }
		virtual void render() { }
		virtual void cleanUp() { }

		PlayModeID tyoe() const { return m_playingMode; }
	};

}

#endif