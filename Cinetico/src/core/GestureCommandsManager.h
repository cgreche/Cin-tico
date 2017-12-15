#ifndef __CINETICO_CORE_GESTURE_COMMANDS_MANAGER_H__
#define __CINETICO_CORE_GESTURE_COMMANDS_MANAGER_H__

#include <vector>
#include "BodyPointState.h"
#include "GestureCommand.h"
#include "lib/vector3.h"
#include "uilib/lib/time.h"

#define GAP 0.06f

namespace cinetico_core {

	class Body;
	class Action;
	class SimpleGesture;

	class GestureCommandsManager {
		Body *m_body;
		std::vector<GestureCommand*> m_gestureCommands;
		std::vector<BodyPointState> m_trackedBps;

		uilib::u64 m_curTime;

		bool meetConditions(SimpleGesture *gesture, GestureCommand *command, float distThreshold);
	public:
		GestureCommandsManager();
		~GestureCommandsManager();

		void reset();
		void step(uilib::u64 curTime);

		unsigned int commandCount() const { return m_gestureCommands.size(); }
		GestureCommand *actionCommand(int i) const { return m_gestureCommands[i]; }
		void setBody(Body *body);

		std::vector<GestureCommand *> filterCommands(int transitionType, BodyPoint::BodyPart bp);

		void checkConditions(uilib::u64 curTime, Action &action, float distThreshold);
	};

}

#endif