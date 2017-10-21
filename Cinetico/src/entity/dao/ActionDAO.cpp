
#include "database/database.h"
#include "ActionDAO.h"
#include "entity/core/PositionAction.h"
#include "entity/core/MovementAction.h"

namespace cinetico {

	ActionDAO::ActionDAO(Database &db)
		: m_db(db) {
	}

	void ActionDAO::create(Action &action) {
		const char *actionSql = "INSERT INTO ACTION("
			"exercise_id"
			",type,order_type,name"
			",min_time,max_time,body_point"
			",ref_point_x,ref_point_y,ref_point_z"
			",final_position_x,final_position_y,final_position_z"
			",final_orientation_x,final_orientation_y,final_orientation_z)"
			" VALUES(?,?,?, ?,?,?, ?, ?,?,?, ?,?,?, ?,?,?);";

		SQLStatement *actionStmt;
		actionStmt = m_db.prepare(actionSql);
		actionStmt->bind(1, (int)action.ownerExercise().id());
		actionStmt->bind(2, (int)action.type());
		actionStmt->bind(3, (int)action.order());
		actionStmt->bind(4, action.name().c_str());
		actionStmt->bind(5, action.minTime());
		actionStmt->bind(6, action.maxTime());
		actionStmt->bind(7, (int)action.bodyPoint());
		actionStmt->bind(8, (int)action.refPointX());
		actionStmt->bind(9, (int)action.refPointY());
		actionStmt->bind(10, (int)action.refPointZ());
		actionStmt->bind(11, action.finalPosition().x());
		actionStmt->bind(12, action.finalPosition().y());
		actionStmt->bind(13, action.finalPosition().z());
		actionStmt->bind(14, action.finalOrientation().x());
		actionStmt->bind(15, action.finalOrientation().y());
		actionStmt->bind(16, action.finalOrientation().z());
		int rc = actionStmt->execute();
		actionStmt->close();

		if (rc == 1) {
			const char *childClassSql;
			SQLStatement *childClassStmt;

			int lastRowId = m_db.getLastRowId();
			if (action.type() == Action::Position) {
				childClassSql = "INSERT INTO POSITION_ACTION(action_id,min_hold_time) VALUES(?,?);";
				childClassStmt = m_db.prepare(childClassSql);
				childClassStmt->bind(1, lastRowId);
				childClassStmt->bind(2, ((PositionAction&)action).minHoldTime());
			}
			else if (action.type() == Action::Movement) {
				childClassSql = "INSERT INTO MOVEMENT_ACTION(action_id,movement_type,min_speed,max_speed) VALUES(?,?,?,?);";
				childClassStmt = m_db.prepare(childClassSql);
				childClassStmt->bind(1, lastRowId);
				childClassStmt->bind(2, (int)((MovementAction&)action).movementType());
				childClassStmt->bind(3, ((MovementAction&)action).minSpeed());
				childClassStmt->bind(4, ((MovementAction&)action).maxSpeed());
			}
			else {
				return;
			}

			int rc = childClassStmt->execute();
			childClassStmt->close();
		}		
	}

	void ActionDAO::update(Action &action) {
		const char *actionSql = "UPDATE ACTION SET type = ?, order_type = ?, "
			"name = ?, min_time = ?, max_time = ?, "
			"body_point = ?, "
			"ref_point_x = ?, ref_point_y = ?, ref_point_z = ?, "
			"final_position_x = ?, final_position_y = ?, final_position_z = ?, "
			"final_orientation_x = ?, final_orientation_y = ?, final_orientation_z = ? WHERE id = ?;";

		SQLStatement *actionStmt;
		actionStmt = m_db.prepare(actionSql);
		actionStmt->bind(1, (int)action.type());
		actionStmt->bind(2, (int)action.order());
		actionStmt->bind(3, action.name().c_str());
		actionStmt->bind(4, action.minTime());
		actionStmt->bind(5, action.maxTime());
		actionStmt->bind(6, (int)action.bodyPoint());
		actionStmt->bind(7, (int)action.refPointX());
		actionStmt->bind(8, (int)action.refPointY());
		actionStmt->bind(9, (int)action.refPointZ());
		actionStmt->bind(10, action.finalPosition().x());
		actionStmt->bind(11, action.finalPosition().y());
		actionStmt->bind(12, action.finalPosition().z());
		actionStmt->bind(13, action.finalOrientation().x());
		actionStmt->bind(14, action.finalOrientation().y());
		actionStmt->bind(15, action.finalOrientation().z());
		actionStmt->bind(16, action.id());
		int rc = actionStmt->execute();
		actionStmt->close();

		if (rc == 1) {
			const char *childClassSql;
			SQLStatement *childClassStmt;

			if (action.type() == Action::Position) {
				childClassSql = "UPDATE POSITION_ACTION SET min_hold_time = ? WHERE action_id = ?;";
				childClassStmt = m_db.prepare(childClassSql);
				childClassStmt->bind(1, ((PositionAction&)action).minHoldTime());
				childClassStmt->bind(2, ((PositionAction&)action).id());
			}
			else if (action.type() == Action::Movement) {
				childClassSql = "UPDATE MOVEMENT_ACTION SET movement_type = ?, min_speed = ?, max_speed = ? WHERE action_id = ?";
				childClassStmt = m_db.prepare(childClassSql);
				childClassStmt->bind(1, (int)((MovementAction&)action).movementType());
				childClassStmt->bind(2, ((MovementAction&)action).minSpeed());
				childClassStmt->bind(3, ((MovementAction&)action).maxSpeed());
				childClassStmt->bind(4, ((MovementAction&)action).id());
			}
			else {
				return;
			}

			int rc = childClassStmt->execute();
			childClassStmt->close();
		}
	}

	std::vector<Action *> ActionDAO::getAllActionsByExercise(Exercise &exercise) {
		const char *sql = "SELECT id,type FROM ACTION WHERE exercise_id = ?;";
		SQLStatement *stmt;
		std::vector<Action *> actionList;
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		ResultSet *rs = stmt->query();
		if (rs) {
			while (rs->next()) {
				int id = rs->getInt(0);
				Action::ActionType actionType = (Action::ActionType)rs->getInt(1);
				Action *action;
				SQLStatement *pStmt;
				ResultSet *pRS;
				if (actionType == Action::Position) {
					PositionAction *positionAction = new PositionAction(exercise, id);

					sql = "SELECT "
						"a.id,a.exercise_id,a.type,a.order_type"
						",a.name,a.min_time,a.max_time"
						",a.body_point"
						",a.ref_point_x,a.ref_point_y,a.ref_point_z"
						",a.final_position_x,a.final_position_y,a.final_position_z"
						",a.final_orientation_x,a.final_orientation_y,a.final_orientation_z"

						",p.min_hold_time"
						" FROM ACTION AS a, POSITION_ACTION AS p WHERE a.id = ? AND p.action_id = ?;";

					pStmt = m_db.prepare(sql);
					if (!pStmt)
						continue;
					pStmt->bind(1, id);
					pStmt->bind(2, id);
					pRS = pStmt->query();
					if (!pRS) {
						pStmt->close();
						continue;
					}
					pRS->next();

					positionAction->setMinHoldTime(pRS->getFloat(17));

					action = positionAction;

				}
				else if (actionType == Action::Movement) {
					MovementAction *movementAction = new MovementAction(exercise, id);
					sql = "SELECT "
						"a.id,a.exercise_id"
						",a.type,a.order_type,a.name"
						",a.min_time,a.max_time,a.body_point,"
						",a.ref_point_x,a.ref_point_y,a.ref_point_z"
						",a.final_position_x,a.final_position_y,a.final_position_z"
						",a.final_orientation_x,a.final_orientation_y,a.final_orientation_z"

						",m.movement_type,m.min_speed,m.max_speed"
						" FROM ACTION AS a, MOVEMENT_ACTION AS m WHERE a.id = ? AND m.action_id = ?;";

					pStmt = m_db.prepare(sql);
					if (!pStmt)
						continue;
					pStmt->bind(1, id);
					pStmt->bind(2, id);
					pRS = pStmt->query();
					if (!pRS) {
						pStmt->close();
						continue;
					}
					pRS->next();

					movementAction->setMovementType((MovementAction::MovementType)pRS->getInt(17));
					movementAction->setMinSpeed(pRS->getFloat(18));
					movementAction->setMaxSpeed(pRS->getFloat(19));

					action = movementAction;
				}
				else {
					continue;
				}

				action->setOrder((Action::ActionOrder)pRS->getInt(3));
				action->setName(pRS->getString(4).c_str());
				action->setMinTime(pRS->getFloat(5));
				action->setMaxTime(pRS->getFloat(6));
				action->setBodyPoint((BodyPoint::BodyPart)pRS->getInt(7));
				action->setRefPointX(pRS->getInt(8));
				action->setRefPointY(pRS->getInt(9));
				action->setRefPointZ(pRS->getInt(10));
				action->setFinalPosition(Vector3(pRS->getFloat(11), pRS->getFloat(12), pRS->getFloat(13)));
				action->setFinalOrientation(Vector3(pRS->getFloat(14), pRS->getFloat(15), pRS->getFloat(16)));
				actionList.push_back(action);
				pRS->close();
				pStmt->close();
			}
			rs->close();
		}
		stmt->close();
		return actionList;
	}

	void ActionDAO::exclude(Action &action) {
		const char *sql = "";
		SQLStatement *stmt;
		int rc;

		//Delete child table
		Action::ActionType actionType = action.type();
		if (actionType == Action::Position || actionType == Action::Movement) {
			if (actionType == Action::Position) {
				sql = "DELETE FROM POSITION_ACTION WHERE action_id = ?";
			}
			else if (actionType == Action::Movement) {
				sql = "DELETE FROM MOVEMENT_ACTION WHERE action_id = ?";
			}
			stmt = m_db.prepare(sql);
			stmt->bind(1, action.id());
			rc = stmt->execute();
			stmt->close();
		}

		//Delete base table
		sql = "DELETE FROM ACTION WHERE id = ?;";
		stmt = m_db.prepare(sql);
		stmt->bind(1, action.id());
		rc = stmt->execute();
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