
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"
#include "GeneralConfigController.h"

namespace cinetico {
	
	static void GeneralConfigController_buttonSave_onClick(Button &button) {
		GeneralConfigController *controller = (GeneralConfigController*)button.param();
		RenderEngine::Config &config = controller->m_cinetico.cineticoUI()->renderEngine()->config();

		Adapter *adapter = controller->cbAdapter.selectedItem() ? (Adapter*)controller->cbAdapter.selectedItem()->data() : NULL;
		DisplayMode *displayMode = controller->cbResolution.selectedItem() ? (DisplayMode*)controller->cbResolution.selectedItem()->data() : NULL;
		bool fullscreen = controller->chkFullscreen.checked();
		bool antialiasing = controller->chkAntialiasing.checked();

		if(adapter)
			config.setAdapter(*adapter);
		if(displayMode)
			config.setDisplayMode(*displayMode);
		config.setFullscreen(fullscreen);
		config.setAntialiasing(antialiasing);
	}

	static void GeneralConfigController_buttonBack_onClick(Button &button) {
		GeneralConfigController *controller = (GeneralConfigController*)button.param();
		controller->m_cineticoUI.goTo(controller->m_cineticoUI.lastView());
	}

	void GeneralConfigController::onViewUpdate() {
		title.setTitle(m_dictionary.getString(Dictionary::GeneralConfigViewTitle));
		title.setDesc(m_dictionary.getString(Dictionary::GeneralConfigViewDesc));

		buttonSave.setText(m_dictionary.getString(Dictionary::DefaultActionSave));
		buttonBack.setText(m_dictionary.getString(Dictionary::DefaultActionBack));

		tbPosThreshold.setLabel("Position Threshold");

		cbAdapter.setLabel(m_dictionary.getString(Dictionary::GeneralConfigAdapter));
		cbResolution.setLabel(m_dictionary.getString(Dictionary::GeneralConfigResolution));
		chkFullscreen.setText(m_dictionary.getString(Dictionary::GeneralConfigFullscreen));
		chkAntialiasing.setText(m_dictionary.getString(Dictionary::GeneralConfigAntialiasing));
	}

	GeneralConfigController::GeneralConfigController(CineticoUI &cineticoUI)
		: Controller(cineticoUI)
	{
		buttonSave.setParam(this);
		buttonSave.setOnClick(GeneralConfigController_buttonSave_onClick);
		buttonBack.setParam(this);
		buttonBack.setOnClick(GeneralConfigController_buttonBack_onClick);
		layoutActionButtons.append(buttonSave);
		layoutActionButtons.append(buttonBack);

		layoutSensor.append(tbPosThreshold);

		layoutGraphics.append(cbAdapter);
		layoutGraphics.append(cbResolution);
		layoutGraphics.append(chkFullscreen);
		layoutGraphics.append(chkAntialiasing);

		layout.append(title);
		layout.append(layoutActionButtons);
		layout.append(sepSensor);
		layout.append(layoutSensor);
		layout.append(sepGraphics);
		layout.append(layoutGraphics);
		layout.setMargin(10);
	}

	Layout *GeneralConfigController::viewDefinition() {
		return &layout;
	}

	void GeneralConfigController::onViewEnter(ViewParams params) {
		cbAdapter.clear();
		cbResolution.clear();
		m_adapters = m_cinetico.cineticoUI()->renderEngine()->getAdapterList();
		m_displayModes = m_cinetico.cineticoUI()->renderEngine()->getDisplayModeList();
		for (Adapter &adapter : m_adapters) {
			cbAdapter.appendItem(adapter.name.c_str(), (void*)&adapter);
		}
		for (DisplayMode &displayMode : m_displayModes) {
			string resolution;
			resolution += string::fromInteger(displayMode.width);
			resolution += "x";
			resolution += string::fromInteger(displayMode.height);
			resolution += ":";
			resolution += string::fromInteger(displayMode.frequency);
			cbResolution.appendItem(resolution, (void*)&displayMode);
		}
	}

	void GeneralConfigController::onViewQuit() {

	}

}