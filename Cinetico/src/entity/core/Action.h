#ifndef __CINETICO_CORE_ACTION_H__
#define __CINETICO_CORE_ACTION_H__

#include <string>
#include "vector3.h"
#include "BodyPoint.h"

namespace cinetico_core {

	class Exercise;

	enum ActionResult {
		Missed,
		Bad,
		Good,
		Excelent
	};

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

		Vector3 m_position;
		Vector3 m_orientation;

		//State attributes
		bool m_correct;
	protected:
		Action(ActionType actionType, Exercise &owner, int id = -1)
			: m_actionType(actionType), m_owner(owner), m_id(id) {
			m_minTime = m_maxTime = 0.f;
			m_refPoint = -2;
		}

	public:
		bool isCorrect() const { return m_correct; }

		void setOrder(ActionOrder order) { m_order = order; }
		void setName(const char *name) { m_name = name; }
		void setMinTime(float minTime) { m_minTime = minTime; }
		void setMaxTime(float maxTime) { m_minTime = m_maxTime; }
		void setBodyPoint(BodyPoint::BodyPart bodyPoint) { m_bodyPoint = bodyPoint; }
		void setRefPoint(int refPoint) { m_refPoint = refPoint; }
		void setPosition(const Vector3 &position) { m_position = position; }
		void setOrientation(const Vector3 &orientation) { m_orientation = orientation; }

		Exercise &ownerExercise() const { return m_owner; }
		int id() const { return m_id; }
		ActionType type() const { return m_actionType; }
		ActionOrder order() const { return m_order; }
		const std::string& name() const { return m_name; }
		float minTime() const { return m_minTime; }
		float maxTime() const { return m_maxTime; }
		BodyPoint::BodyPart bodyPoint() const { return m_bodyPoint; }
		int refPoint() const { return m_refPoint; }
		const Vector3 &position() const { return m_position; }
		const Vector3 &orientation() const { return m_orientation; }
	};

}

#endif