
#include "database/database.h"
#include "ActionDAO.h"
#include "core/entity/Action.h"

namespace cinetico {

	ActionDAO::ActionDAO(Database &db)
		: m_db(db) {
	}

	void ActionDAO::create(Action &action) {
		const char *actionSql = "INSERT INTO ACTION("
			"exercise_id"
			",name"
			",min_execution_time,max_execution_time"
			",time_to_hold)"
			" VALUES(?,?,?,?,?);";

		SQLStatement *actionStmt;
		actionStmt = m_db.prepare(actionSql);
		actionStmt->bind(1, (int)action.ownerExercise().id());
		actionStmt->bind(2, action.name().c_str());
		actionStmt->bind(3, action.minExecutionTime());
		actionStmt->bind(4, action.maxExecutionTime());
		actionStmt->bind(5, action.timeToHold());
		int rc = actionStmt->execute();
		actionStmt->close();

		if (rc == 1) {
			int lastRowId = m_db.getLastRowId();

			const char *childClassSql;
			SQLStatement *childClassStmt;
			childClassSql = "INSERT INTO SIMPLE_GESTURE(action_id,transition_type,body_point,ref_point,operation,value_x,value_y,value_z) VALUES(?,?,?,?,?,?,?,?);";
			childClassStmt = m_db.prepare(childClassSql);

			for (int i = 0; i < action.gestureCount(); ++i) {
				SimpleGesture *gesture = action.gesture(i);

				childClassStmt->bind(1, lastRowId);
				childClassStmt->bind(2, (int)gesture->transitionType());
				childClassStmt->bind(3, (int)gesture->bodyPoint());
				childClassStmt->bind(4, gesture->refPoint());
				childClassStmt->bind(5, (int)gesture->operation());
				childClassStmt->bind(6, gesture->value().x());
				childClassStmt->bind(7, gesture->value().y());
				childClassStmt->bind(8, gesture->value().z());
				int rc = childClassStmt->execute();
				if (rc == 1) {
					if (gesture->transitionType() == SimpleGesture::FixedMovement) {
						const char *movementGestureSql = "INSERT INTO MOVEMENT_GESTURE(simple_gesture_id,movement_type,min_speed,max_speed) VALUES(?,?,?,?);";
						SQLStatement *movementGestureStmt = m_db.prepare(movementGestureSql);
						int gestureRowId = m_db.getLastRowId();
						movementGestureStmt->bind(1, gestureRowId);
						movementGestureStmt->bind(2, ((MovementGesture*)gesture)->movementType());
						movementGestureStmt->bind(3, ((MovementGesture*)gesture)->minSpeed());
						movementGestureStmt->bind(4, ((MovementGesture*)gesture)->maxSpeed());
						movementGestureStmt->execute();
						movementGestureStmt->close();
					}
				}
			}
			childClassStmt->close();
		}		
	}

	void ActionDAO::update(Action &action) {
		const char *actionSql = "UPDATE ACTION SET name = ?"
			", min_execution_time = ?, max_execution_time = ?"
			", time_to_hold = ? WHERE id = ?";

		SQLStatement *actionStmt;
		actionStmt = m_db.prepare(actionSql);
		actionStmt->bind(1, action.name().c_str());
		actionStmt->bind(2, action.minExecutionTime());
		actionStmt->bind(3, action.maxExecutionTime());
		actionStmt->bind(4, action.timeToHold());
		actionStmt->bind(5, action.id());
		int rc = actionStmt->execute();
		actionStmt->close();

		if (rc == 1) {
			//Delete All Movement Gestures
			const char *sql = "DELETE FROM MOVEMENT_GESTURE WHERE simple_gesture_id IN (SELECT id FROM SIMPLE_GESTURE WHERE action_id = ?);";
			SQLStatement *stmt = m_db.prepare(sql);
			stmt->bind(1, action.id());
			stmt->execute();
			//Delete All Simple Gestures
			sql = "DELETE FROM SIMPLE_GESTURE WHERE action_id = ?;";
			stmt = m_db.prepare(sql);
			stmt->bind(1, action.id());
			stmt->execute();

			for (int i = 0; i < action.gestureCount(); ++i) {
				SimpleGesture *gesture = action.gesture(i);
				const char *childClassSql;
				SQLStatement *childClassStmt;

				childClassSql = "INSERT INTO SIMPLE_GESTURE(action_id,transition_type,body_point,ref_point,operation,value_x,value_y,value_z) VALUES(?,?);";
				childClassStmt = m_db.prepare(childClassSql);
				childClassStmt->bind(1, action.id());
				childClassStmt->bind(2, (int)gesture->transitionType());
				childClassStmt->bind(3, (int)gesture->bodyPoint());
				childClassStmt->bind(4, gesture->refPoint());
				childClassStmt->bind(5, (int)gesture->operation());
				childClassStmt->bind(6, gesture->value().x());
				childClassStmt->bind(7, gesture->value().y());
				childClassStmt->bind(8, gesture->value().z());
				int gestureRowId = m_db.getLastRowId();
				if (gesture->transitionType() == SimpleGesture::FixedMovement) {
					childClassSql = "INSERT INTO MOVEMENT_GESTURE(simple_gesture_id,movement_type,min_speed,max_speed) VALUES(?,?,?,?);";
					childClassStmt = m_db.prepare(childClassSql);
					childClassStmt->bind(1, gestureRowId);
					childClassStmt->bind(2, ((MovementGesture*)gesture)->movementType());
					childClassStmt->bind(3, ((MovementGesture*)gesture)->minSpeed());
					childClassStmt->bind(4, ((MovementGesture*)gesture)->maxSpeed());
				}

				int rc = childClassStmt->execute();
				childClassStmt->close();
			}
		}
	}

	std::vector<Action *> ActionDAO::getAllActionsByExercise(Exercise &exercise) {
		std::vector<Action *> actionList;

		const char *sql = "SELECT * FROM ACTION WHERE exercise_id = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		ResultSet *rs = stmt->query();
		
		while (rs->next()) {
			int actionId = rs->getInt(0);
			Action *action = new Action(exercise, actionId);
			action->setName(rs->getString(2).c_str());
			action->setMinExecutionTime(rs->getFloat(2));
			action->setMaxExecutionTime(rs->getFloat(3));
			action->setTimeToHold(rs->getFloat(4));

			SQLStatement *gestureStmt = m_db.prepare("SELECT * FROM SIMPLE_GESTURE WHERE action_id = ?;");
			gestureStmt->bind(1, actionId);
			ResultSet *gestureRS = gestureStmt->query();
			while (gestureRS->next()) {
				SimpleGesture *gesture;
				SimpleGesture::TransitionType transitionType = (SimpleGesture::TransitionType)gestureRS->getInt(2);
				BodyPoint::BodyPart bodyPoint = (BodyPoint::BodyPart)gestureRS->getInt(3);
				int refPoint = gestureRS->getInt(4);
				unsigned long operation = (unsigned long)gestureRS->getInt(5);
				float valueX = gestureRS->getFloat(6);
				float valueY = gestureRS->getFloat(7);
				float valueZ = gestureRS->getFloat(8);

				if (transitionType == SimpleGesture::FixedMovement) {
					MovementGesture::MovementType movementType = MovementGesture::Linear;
					float minSpeed = 0.f;
					float maxSpeed = 0.f;

					SQLStatement *movementGestureStmt = m_db.prepare("SELECT * FROM MOVEMENT_GESTURE WHERE simple_gesture_id = ?);");
					ResultSet *rs = movementGestureStmt->query();
					if (rs->next()) {
						movementType = (MovementGesture::MovementType)rs->getInt(1);
						minSpeed = rs->getFloat(2);
						maxSpeed = rs->getFloat(3);
					}
					rs->close();
					movementGestureStmt->close();

					MovementGesture *movementGesture = new MovementGesture(bodyPoint, movementType);
					movementGesture->setMinSpeed(minSpeed);
					movementGesture->setMaxSpeed(maxSpeed);
					gesture = movementGesture;
				}
				else {
					gesture = new SimpleGesture(transitionType, bodyPoint);
				}

				gesture->setRefPoint(refPoint);
				gesture->setOperation(operation);
				gesture->setValue(Vector3(valueX, valueY, valueZ));
				action->addGesture(gesture);
			}

			actionList.push_back(action);
			gestureRS->close();
			gestureStmt->close();
		}
			
		rs->close();
		stmt->close();
		return actionList;
	}

	void ActionDAO::exclude(Action &action) {
		SQLStatement *stmt;
		//Delete All Movement Gestures
		const char *sql = "DELETE FROM MOVEMENT_GESTURE WHERE simple_gesture_id IN (SELECT id FROM SIMPLE_GESTURE WHERE action_id = ?);";
		stmt = m_db.prepare(sql);
		stmt->bind(1, action.id());
		stmt->execute();
		//Delete All Simple Gestures
		sql = "DELETE FROM SIMPLE_GESTURE WHERE action_id = ?;";
		stmt = m_db.prepare(sql);
		stmt->bind(1, action.id());
		stmt->execute();
		//Delete the Action
		sql = "DELETE FROM ACTION WHERE id = ?;";
		stmt = m_db.prepare(sql);
		stmt->bind(1, action.id());
		stmt->execute();
		stmt->close();
	}

	int ActionDAO::getActionCountByExercise(Exercise &exercise) {
		const char *sql = "SELECT COUNT(*) FROM ACTION WHERE exercise_id = ?";
		SQLStatement *stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		ResultSet *rs = stmt->query();
		int count = 0;
		if (rs) {
			if (rs->next()) {
				count = rs->getInt(0);

			}
			rs->close();
		}
		stmt->close();
		return count;
	}

}