
#include "cinetico.h"
#include "cineticoui.h"

#include "DebugController.h"

namespace cinetico {
	using namespace cinetico_core;

	void buttonToEuler_onAction(cButton& button) {
		DebugController *controller = (DebugController*)button.param();
		float w, x, y, z;
		w = controller->tbQW.text().toFloat();
		x = controller->tbQX.text().toFloat();
		y = controller->tbQY.text().toFloat();
		z = controller->tbQZ.text().toFloat();

		Quaternion q = Quaternion(w, x, y, z);
		cinetico_core::Vector3 euler = q.toEuler();
		controller->tbEX.setText(string::fromFloat(euler.x()));
		controller->tbEY.setText(string::fromFloat(euler.y()));
		controller->tbEZ.setText(string::fromFloat(euler.z()));
	}

	void buttonToQuaternion_onAction(cButton& button) {
		DebugController *controller = (DebugController*)button.param();
		float x, y, z;
		x = controller->tbEX.text().toFloat();
		y = controller->tbEY.text().toFloat();
		z = controller->tbEZ.text().toFloat();

		Quaternion q = Quaternion::fromEuler(x, y, z);
		controller->tbQW.setText(string::fromFloat(q.w()));
		controller->tbQX.setText(string::fromFloat(q.x()));
		controller->tbQY.setText(string::fromFloat(q.y()));
		controller->tbQZ.setText(string::fromFloat(q.z()));
	}

	void DebugController::onViewUpdate() {

	}

	DebugController::DebugController(CineticoUI &cineticoUI)
		: Controller(cineticoUI)
	{
		//
		labelQuaternion.setText("Quaternion");	
		tbQW.setLabel("W");
		tbQX.setLabel("X");
		tbQY.setLabel("Y");
		tbQZ.setLabel("Z");
		buttonToEuler.setText("To Euler");
		buttonToEuler.setParam(this);
		buttonToEuler.addOnAction(buttonToEuler_onAction);

		layoutQValue.append(tbQW);
		layoutQValue.append(tbQX);
		layoutQValue.append(tbQY);
		layoutQValue.append(tbQZ);
		layoutQValue.append(buttonToEuler);

		//
		labelEuler.setText("Euler");
		tbEX.setLabel("X");
		tbEY.setLabel("Y");
		tbEZ.setLabel("Z");
		buttonToQuaternion.setText("To Quaternion");
		buttonToQuaternion.setParam(this);
		buttonToQuaternion.addOnAction(buttonToQuaternion_onAction);
		
		layoutEValue.append(tbEX);
		layoutEValue.append(tbEY);
		layoutEValue.append(tbEZ);
		layoutEValue.append(buttonToQuaternion);


		//
		layout.append(labelQuaternion);
		layout.append(layoutQValue);
		layout.append(labelEuler);
		layout.append(layoutEValue);
	}

	Layout *DebugController::viewDefinition() {
		return &layout;
	}

	void DebugController::onViewEnter(ViewParams params) {
	}

	void DebugController::onViewTick() {
	}

	void DebugController::onViewQuit() {
	}

}