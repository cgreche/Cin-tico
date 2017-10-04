#ifndef __CINETICO_CORE_MOVEMENTACTION_H__
#define __CINETICO_CORE_MOVEMENTACTION_H__

#include "action.h"

namespace cinetico_core {

	class MovementAction : public Action {
	public:
		enum MovementType {
			Linear,
			Angular
		};

	private:
		MovementType m_movementType;
		float m_minSpeed;
		float m_maxSpeed;

	public:
		MovementAction(Exercise &owner, int id = -1)
			: Action(Action::Movement, owner, id) {
		}

		void setMovementType(MovementType movementType) { m_movementType = movementType; }
		void setMinSpeed(float minSpeed) { m_minSpeed = minSpeed; }
		void setMaxSpeed(float maxSpeed) { m_maxSpeed = maxSpeed; }
		MovementType movementType() const { return m_movementType; }
		float minSpeed() const { return m_minSpeed; }
		float maxSpeed() const { return m_maxSpeed; }
	};

}

#endif