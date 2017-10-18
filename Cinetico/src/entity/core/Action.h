#ifndef __CINETICO_CORE_ACTION_H__
#define __CINETICO_CORE_ACTION_H__

#include <string>
#include "vector3.h"
#include "Body.h"

namespace cinetico_core {

	class Exercise;
	
	class Action {
	public:
		enum ActionType {
			Position,
			Movement
		};

		enum ActionOrder {
			NewAction,
			SameLastAction,
			AllActions
		};

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
		ActionType m_actionType;
		ActionOrder m_order;

		//Definition attributes
		std::string m_name;
		float m_minTime;
		float m_maxTime;

		BodyPoint::BodyPart m_bodyPoint;
		int m_refPoint;

		Vector3 m_finalPosition;
		Vector3 m_finalOrientation;

		//State attributes
		ActionState m_state;
		ActionResult m_result;
		float m_accuracy;

	protected:
		Action(ActionType actionType, Exercise &owner, int id = -1)
			: m_actionType(actionType), m_owner(owner), m_id(id) {
			m_minTime = m_maxTime = 0.f;
			m_refPoint = -2;
			m_state = Idle;
			m_result = Missed;
			m_accuracy = 0.f;
		}

		virtual ActionResult avaliate(Body &body) = 0;
	public:
		void step(Body &body) {
			m_result = avaliate(body);
		}

		bool isCorrect() const { return m_state == Finished && m_result != Missed; }

		void setOrder(ActionOrder order) { m_order = order; }
		void setName(const char *name) { m_name = name; }
		void setMinTime(float minTime) { m_minTime = minTime; }
		void setMaxTime(float maxTime) { m_minTime = m_maxTime; }
		void setBodyPoint(BodyPoint::BodyPart bodyPoint) { m_bodyPoint = bodyPoint; }
		void setRefPoint(int refPoint) { m_refPoint = refPoint; }
		void setFinalPosition(const Vector3 &position) { m_finalPosition = position; }
		void setFinalOrientation(const Vector3 &orientation) { m_finalOrientation = orientation; }

		Exercise &ownerExercise() const { return m_owner; }
		int id() const { return m_id; }
		ActionType type() const { return m_actionType; }
		ActionOrder order() const { return m_order; }
		const std::string& name() const { return m_name; }
		float minTime() const { return m_minTime; }
		float maxTime() const { return m_maxTime; }
		BodyPoint::BodyPart bodyPoint() const { return m_bodyPoint; }
		int refPoint() const { return m_refPoint; }
		const Vector3 &finalPosition() const { return m_finalPosition; }
		const Vector3 &finalOrientation() const { return m_finalOrientation; }

		ActionState state() const { return m_state; }
		ActionResult result() const { return m_result; }
		float accuracy() const { return m_accuracy; }
	};

}

#endif