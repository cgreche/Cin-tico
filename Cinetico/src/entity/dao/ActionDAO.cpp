
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
			"exercise_id,type,order"
			",name,min_time,max_time"
			",body_point,ref_point"
			",position_x,position_y,position_z"
			",orientation_x,orientation_y,orientation_z)"
			" VALUES(?,?,?,?, ?,?, ?,?, ?,?,?, ?,?,?);";

		SQLStatement *actionStmt;
		actionStmt = m_db.prepare(actionSql);
		actionStmt->bind(1, (int)action.ownerExercise().id());
		actionStmt->bind(2, (int)action.type());
		actionStmt->bind(3, (int)action.order());
		actionStmt->bind(4, action.name().c_str());
		actionStmt->bind(5, action.minTime());
		actionStmt->bind(6, action.maxTime());
		actionStmt->bind(7, (int)action.bodyPoint());
		actionStmt->bind(8, (int)action.refPoint());
		actionStmt->bind(9, action.position().x());
		actionStmt->bind(10, action.position().y());
		actionStmt->bind(11, action.position().z());
		actionStmt->bind(12, action.orientation().x());
		actionStmt->bind(13, action.orientation().y());
		actionStmt->bind(14, action.orientation().z());
		int rc = actionStmt->execute();
		actionStmt->close();

		if (rc == 0) {
			const char *childClassSql;
			SQLStatement *childClassStmt;

			int lastRowId = m_db.getLastRowId();
			if (action.type() == Action::Position) {
				childClassSql = "INSERT INTO POSITION_ACTION(action_id,min_hold_time) VALUES(?,?);";
				childClassStmt = m_db.prepare(actionSql);
				childClassStmt->bind(1, lastRowId); //safer to use the created user properties
				childClassStmt->bind(2, ((PositionAction&)action).minHoldTime());
			}
			else if (action.type() == Action::Movement) {
				childClassSql = "INSERT INTO POSITION_ACTION(action_id,movement_type,min_speed,max_speed) VALUES(?,?,?,?);";
				childClassStmt = m_db.prepare(actionSql);
				childClassStmt->bind(1, lastRowId); //safer to use the created user properties
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
		/*
		const char *sql = "UPDATE EXERCISE SET name = ?, author = ?, trackable_body_points = ?, is_public = ? WHERE id = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, exercise.name());
		stmt->bind(2, exercise.author());
		stmt->bind(3, exercise.trackableBodyPoints());
		stmt->bind(4, exercise.isPublic());
		stmt->bind(5, exercise.id());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo
		}
		stmt->close();
		*/
	}

	std::vector<Action *> ActionDAO::getAllActionsByExercise(Exercise &exercise) {
		const char *sql = "SELECT id,type FROM ACTION WHERE exercise_id = ?;";
		SQLStatement *stmt;
		std::vector<Action *> exerciseList;
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
						"a.id,a.exercise_id,a.type,a.order"
						",a.name,a.min_time,a.max_time"
						",a.body_point,a.ref_point"
						",a.position_x,a.position_y,a.position_z"
						",a.orientation_x,a.orientation_y,a.orientation_z"

						",p.min_hold_time"
						" FROM ACTION AS a, POSITION_ACTION AS p WHERE a.id = ? AND p.action_id = ?;";

					pStmt = m_db.prepare(sql);
					if (!pStmt)
						continue;
					pRS = pStmt->query();
					if (!pRS) {
						pStmt->close();
						continue;
					}

					positionAction->setMinHoldTime(rs->getFloat(15));

					action = positionAction;

				}
				else if (actionType == Action::Movement) {
					MovementAction *movementAction = new MovementAction(exercise, id);
					sql = "SELECT "
						"a.id,a.exercise_id,a.type,a.order"
						",a.name,a.min_time,a.max_time"
						",a.body_point,a.ref_point"
						",a.position_x,a.position_y,a.position_z"
						",a.orientation_x,a.orientation_y,a.orientation_z"

						",m.movement_type,m.min_speed,m.max_speed"
						" FROM ACTION AS a, MOVEMENT_ACTION AS m WHERE a.id = ? AND m.action_id = ?;";

					pStmt = m_db.prepare(sql);
					if (!pStmt)
						continue;
					pRS = pStmt->query();
					if (!pRS) {
						pStmt->close();
						continue;
					}

					movementAction->setMovementType((MovementAction::MovementType)rs->getInt(15));
					movementAction->setMinSpeed(rs->getFloat(16));
					movementAction->setMaxSpeed(rs->getFloat(17));

					action = movementAction;
				}
				else {
					continue;
				}

				action->setOrder((Action::ActionOrder)rs->getInt(3));
				action->setName(rs->getString(4).c_str());
				action->setMinTime(rs->getFloat(3));
				action->setMaxTime(rs->getFloat(4));
				action->setBodyPoint((BodyPoint::BodyPart)rs->getInt(5));
				action->setRefPoint(rs->getInt(6));
				action->setRefPoint(rs->getInt(7));
				action->setPosition(Vector3(rs->getFloat(8), rs->getFloat(9), rs->getFloat(10)));
				action->setOrientation(Vector3(rs->getFloat(11), rs->getFloat(12), rs->getFloat(13)));

				pRS->close();
				pStmt->close();
			}
			rs->close();
		}
		stmt->close();
		return exerciseList;
	}

	void ActionDAO::exclude(Action &action) {
		const char *sql = "DELETE FROM ACTION WHERE id = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, action.id());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo
		}
		stmt->close();
	}

}