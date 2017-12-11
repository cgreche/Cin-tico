#ifndef __CINETICO_CORE_ACTION_H__
#define __CINETICO_CORE_ACTION_H__

#include <string>
#include <vector>
#include "Body.h"
#include "core/entity/SimpleGesture.h"
#include "core/entity/MovementGesture.h"
#include "core/lib/vector3.h"

namespace cinetico_core {

	class Exercise;
	class Body;

	class Action {
	public:
		enum ActionState {
			Idle,
			Running,
			Finished
		};

		enum ActionResult {
			Missed,
			Bad,
			Good,
			Excellent
		};

	protected:
		Exercise &m_owner;
		int m_id;
		//Definition attributes
		std::string m_name;
		float m_minExecutionTime;
		float m_maxExecutionTime;
		float m_timeToHold;
		std::vector<SimpleGesture *> m_gestures;

		//State attributes
		ActionState m_state;
		ActionResult m_result;
		float m_accuracy;
		std::vector<int> m_results;

		//
		bool evaluate();
	public:
		Action(Exercise &owner, int id = -1);
		~Action();

		int addGesture(SimpleGesture *gesture);

		void start();
		void stop();
		void step(Body &body);

		bool isCorrect() const { return m_state == Finished && m_result != Missed; }

		void setName(const std::string &name) { m_name = name; }
		void setMinExecutionTime(float minExecutionTime) { m_minExecutionTime = minExecutionTime; }
		void setMaxExecutionTime(float maxExecutionTime) { m_maxExecutionTime = maxExecutionTime; }
		void setTimeToHold(float timeToHold) { m_timeToHold = timeToHold; }

		void setResult(int gestureId, int result) { m_results[gestureId] = result; }

		Exercise &ownerExercise() const { return m_owner; }
		int id() const { return m_id; }
		const std::string& name() const { return m_name; }
		float minExecutionTime() const { return m_minExecutionTime; }
		float maxExecutionTime() const { return m_maxExecutionTime; }
		float timeToHold() const { return m_timeToHold; }

		ActionState state() const { return m_state; }
		ActionResult result() const { return m_result; }
		float accuracy() const { return m_accuracy; }

		int gestureCount() const { return m_gestures.size(); }
		SimpleGesture* gesture(int index) const { return m_gestures[index]; }
	};

}

#endif