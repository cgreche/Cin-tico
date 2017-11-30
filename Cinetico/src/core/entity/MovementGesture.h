#ifndef __CINETICO_CORE_MOVEMENTGESTURE_H__
#define __CINETICO_CORE_MOVEMENTGESTURE_H__

#include <vector>
#include "SimpleGesture.h"

namespace cinetico_core {

	class MovementGesture : public SimpleGesture {
	public:
		enum MovementType {
			Linear,
			Smooth //uses Besier
		};

	private:
		MovementType m_movementType;
		float m_minSpeed;
		float m_maxSpeed;
		std::vector<Vector3> m_extraPoints; //for Besier only

	public:
		MovementGesture(BodyPoint::BodyPart bodyPoint, MovementType movementType);

		void addPoint(const Vector3 &point) {
			m_extraPoints.push_back(point);
		}

		MovementType movementType() const { return m_movementType; }
		void setMinSpeed(float minSpeed) { m_minSpeed = minSpeed; }
		void setMaxSpeed(float maxSpeed) { m_maxSpeed = maxSpeed; }
		float minSpeed() const { return m_minSpeed; }
		float maxSpeed() const { return m_maxSpeed; }
	};

}

#endif