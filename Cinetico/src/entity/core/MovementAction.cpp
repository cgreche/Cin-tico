
#include "MovementAction.h"

namespace cinetico_core {

	MovementAction::MovementAction(Exercise &owner, int id)
		: Action(Action::Movement, owner, id) {
		m_movementType = Linear;
		m_minSpeed = 0.f;
		m_maxSpeed = 0.f;
	}

	Action::ActionResult MovementAction::avaliate(Body &body) {
		return Missed;
	}
}
