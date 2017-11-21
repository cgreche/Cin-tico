#ifndef __CINETICO_CORE_SIMPLEGESTURE_H__
#define __CINETICO_CORE_SIMPLEGESTURE_H__

#include "BodyPoint.h"

namespace cinetico_core {

	class SimpleGesture {
	public:
		enum TransitionType {
			Free,
			FixedMovement
		};

		enum Operation {
			FixedPosition = 0,
			InFront,
			Behind,
			ToRight,
			ToLeft,
			Above,
			Below,
			FixedOrientation,
			OrientationLookingTo
		};

	protected:
		TransitionType m_type;
		BodyPoint::BodyPart m_bodyPoint;
		int m_refPoint;
		unsigned long m_operation;
		Vector3 m_value;

	public:
		SimpleGesture(TransitionType type, BodyPoint::BodyPart bodyPoint);

		void setRefPoint(int refPoint) { m_refPoint = refPoint; }
		void setOperation(unsigned long operation) { m_operation = operation; }
		void setValue(const Vector3 &value) { m_value = value; }

		TransitionType transitionType() const { return m_type; }
		BodyPoint::BodyPart bodyPoint() const { return m_bodyPoint; }
		int refPoint() const { return m_refPoint; }
		unsigned long operation() const { return m_operation; }
		Vector3 value() const { return m_value; }

		static unsigned long makeOperationFlags(Operation operation, bool useX, bool useY, bool useZ) {
			return ((int)operation & 255) | (useX << 8) | (useY << 9) | (useZ << 10);
		}

		static Operation getOperation(unsigned long operationFlags) {
			return (Operation)(operationFlags & 255);
		}

		static bool useX(unsigned long operationFlags) {
			return (operationFlags >> 8) & 1;
		}

		static bool useY(unsigned long operationFlags) {
			return (operationFlags >> 9) & 1;
		}

		static bool useZ(unsigned long operationFlags) {
			return (operationFlags >> 10) & 1;
		}
	};

}

#endif