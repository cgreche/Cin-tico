
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"
#include "GeneralSettingsController.h"

namespace cinetico {

	static void GeneralSettingsController_buttonSave_onClick(Button &button) {
		GeneralSettingsController *controller = (GeneralSettingsController*)button.param();
		GeneralSettings &settings = *controller->m_cinetico.generalSettings();

		DisplayMode *displayMode = controller->cbResolution.selectedItem() ? (DisplayMode*)controller->cbResolution.selectedItem()->data() : NULL;
		bool fullscreen = controller->chkFullscreen.checked();
		bool antialiasing = controller->chkAntialiasing.checked();

		int adapter = controller->cbAdapter.selection();
		settings.setPosDistThreshold(controller->tbDistThreshold.text().toFloat());
		settings.setPosMinHoldtime(controller->tbMinHoldtime.text().toFloat());
		settings.setAdapter(adapter == -1 ? 0 : adapter);
		settings.setResolutionWidth(displayMode->width);
		settings.setResolutionHeight(displayMode->height);
		settings.setRefreshRate(displayMode->frequency);
		settings.setFullscreen(fullscreen);
		settings.setAntialiasing(antialiasing);

		controller->m_cinetico.cineticoDB()->generalSettingsDAO()->save(settings);
	}

	static void GeneralSettingsController_buttonBack_onClick(Button &button) {
		GeneralSettingsController *controller = (GeneralSettingsController*)button.param();
		controller->m_cineticoUI.goTo(controller->m_cineticoUI.lastView());
	}

	static void GeneralSettingsController_buttonTestSensor_onClick(Button &button) {
		GeneralSettingsController *controller = (GeneralSettingsController*)button.param();
		int result = controller->m_cinetico.sensor()->test();
		if (result == 1) {
			controller->labelTestSensor.setText(controller->m_dictionary.getString(Dictionary::GeneralSettingsViewTestSensorErrorSuccess));
			controller->labelTestSensor.setTextColor(uilib::Color(0, 220, 0));
		}
		else if (result == 0) {
			controller->labelTestSensor.setText(controller->m_dictionary.getString(Dictionary::GeneralSettingsViewTestSensorErrorNotDetected));
			controller->labelTestSensor.setTextColor(uilib::Color(240, 0, 0));
		}
		controller->labelTestSensor.setVisible(true);
		controller->layoutTestSensor.setSize(controller->layoutTestSensor.getAutoSize());
	}

	void GeneralSettingsController::onViewUpdate() {
		title.setTitle(m_dictionary.getString(Dictionary::GeneralSettingsViewTitle));
		title.setDesc(m_dictionary.getString(Dictionary::GeneralSettingsViewDesc));

		buttonSave.setText(m_dictionary.getString(Dictionary::DefaultActionSave));
		buttonBack.setText(m_dictionary.getString(Dictionary::DefaultActionBack));

		sepCore.setText(m_dictionary.getString(Dictionary::GeneralSettingsViewSectionCore));
		tbDistThreshold.setLabel(m_dictionary.getString(Dictionary::GeneralSettingsCorePosDistThreshold));
		tbMinHoldtime.setLabel(m_dictionary.getString(Dictionary::GeneralSettingsCorePosMinHoldtime));
		buttonTestSensor.setText(m_dictionary.getString(Dictionary::GeneralSettingsViewTestSensor));

		sepGraphics.setText(m_dictionary.getString(Dictionary::GeneralSettingsViewSectionGraphics));
		cbAdapter.setLabel(m_dictionary.getString(Dictionary::GeneralSettingsAdapter));
		cbResolution.setLabel(m_dictionary.getString(Dictionary::GeneralSettingsResolution));
		chkFullscreen.setText(m_dictionary.getString(Dictionary::GeneralSettingsFullscreen));
		chkAntialiasing.setText(m_dictionary.getString(Dictionary::GeneralSettingsAntialiasing));
	}

	GeneralSettingsController::GeneralSettingsController(CineticoUI &cineticoUI)
		: Controller(cineticoUI)
	{
		buttonSave.setParam(this);
		buttonSave.setOnClick(GeneralSettingsController_buttonSave_onClick);
		buttonBack.setParam(this);
		buttonBack.setOnClick(GeneralSettingsController_buttonBack_onClick);
		layoutActionButtons.append(buttonSave);
		layoutActionButtons.append(buttonBack);

		layoutCore.append(tbDistThreshold);
		layoutCore.append(tbMinHoldtime);

		layoutGraphics.append(cbAdapter);
		layoutGraphics.append(cbResolution);
		layoutGraphics.append(chkFullscreen);
		layoutGraphics.append(chkAntialiasing);

		buttonTestSensor.setParam(this);
		buttonTestSensor.setOnClick(GeneralSettingsController_buttonTestSensor_onClick);
		layoutTestSensor.append(buttonTestSensor);
		layoutTestSensor.append(labelTestSensor);
		layoutTestSensor.setAlignment(Layout::center_align);
		labelTestSensor.setVisible(false);

		layout.append(title);
		layout.append(layoutActionButtons);
		layout.append(sepCore);
		layout.append(layoutCore);
		layout.append(layoutTestSensor);
		layout.append(sepGraphics);
		layout.append(layoutGraphics);
		layout.setMargin(10);
	}

	Layout *GeneralSettingsController::viewDefinition() {
		return &layout;
	}

	void GeneralSettingsController::onViewEnter(ViewParams params) {
		cbAdapter.clear();
		cbResolution.clear();
		m_adapters = m_cinetico.cineticoUI()->renderEngine()->getAdapterList();
		m_displayModes = m_cinetico.cineticoUI()->renderEngine()->getDisplayModeList();
		GeneralSettings &settings = *m_cinetico.generalSettings();

		for (Adapter &adapter : m_adapters) {
			cbAdapter.appendItem(adapter.name.c_str(), (void*)&adapter);
		}

		cbAdapter.setSelection(settings.adapter());

		for (DisplayMode &displayMode : m_displayModes) {

			if ( (displayMode.width % 16 != 0 || displayMode.height % 9 != 0)
				&& (displayMode.width % 16 != 0 || displayMode.height % 10 != 0)
				&& (displayMode.width % 5 != 0 || displayMode.height % 4 != 0)
				&& (displayMode.width % 4 != 0 || displayMode.height % 3 != 0) )
				continue;

			string resolution;
			resolution += string::fromInteger(displayMode.width);
			resolution += "x";
			resolution += string::fromInteger(displayMode.height);
			resolution += ":";
			resolution += string::fromInteger(displayMode.frequency);
			cbResolution.appendItem(resolution, (void*)&displayMode);
			if (displayMode.width == settings.resolutionWidth()
				&& displayMode.height == settings.resolutionHeight()
				&& displayMode.frequency == settings.refreshRate())
				cbResolution.setSelectionByData(&displayMode);

		}

		tbDistThreshold.setText(string::fromFloat(settings.posDistThreshold()));
		tbMinHoldtime.setText(string::fromFloat(settings.posMinHoldtime()));
		chkFullscreen.setChecked(settings.fullscreen());
		chkAntialiasing.setChecked(settings.antialiasing());
	}

	void GeneralSettingsController::onViewQuit() {

	}

}