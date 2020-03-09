#include "plugin.hpp"


struct Adsynth_StereoVCA : Module {
	enum ParamIds {
		ENUMS(KNOB_PARAM, 4),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(INL_INPUT, 4),
		ENUMS(INR_INPUT, 4),
		ENUMS(CV_INPUT, 4),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUTL_OUTPUT, 4),
		ENUMS(OUTR_OUTPUT, 4),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_StereoVCA() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(KNOB_PARAM, 0.f, 1.f, 0.f, "Gain");
	}

	void process(const ProcessArgs& args) override {
		float inputL[4],
			inputR[4],
			inputCV[4],
			outputL[4],
			outputR[4],
			gain[4];

		for (int i = 0; i < 4; i++) {
			inputL[i] = inputs[INL_INPUT + i].getVoltage();
			inputR[i] = inputs[INR_INPUT + i].getVoltage();
			gain[i] = params[KNOB_PARAM + i].getValue();

			if (inputs[CV_INPUT + i].isConnected()) {
				inputCV[i] = clamp(inputs[CV_INPUT + i].getVoltage(), 0.f, 10.f);
			}
			else {
				inputCV[i] = 10.f;
			}

			outputL[i] = inputL[i] * (gain[i] * inputCV[i] / 10);
			outputR[i] = inputR[i] * (gain[i] * inputCV[i] / 10);

			outputL[i] = clamp(outputL[i], -11.4f, 11.4f);
			outputR[i] = clamp(outputR[i], -11.4f, 11.4f);

			outputs[OUTL_OUTPUT + i].setVoltage(outputL[i]);
			outputs[OUTR_OUTPUT + i].setVoltage(outputR[i]);
		}
	}
};


struct Adsynth_StereoVCAWidget : ModuleWidget {
	Adsynth_StereoVCAWidget(Adsynth_StereoVCA* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/StereoVCA.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		for (int i = 0; i < 4; i++)
		{
			int spacing = 26,
				ytop = 19,
				ybot = 31;
			addParam(createParamCentered<AdsynthKnobSmallRed>(mm2px(Vec(14.24, ytop + i * spacing)), module, Adsynth_StereoVCA::KNOB_PARAM +i));

			addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, ytop + i * spacing)), module, Adsynth_StereoVCA::INL_INPUT+i));
			addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, ybot + i * spacing)), module, Adsynth_StereoVCA::INR_INPUT+i));
			addInput(createInputCentered<AdsynthJackRed>(mm2px(Vec(14.24, ybot + i * spacing)), module, Adsynth_StereoVCA::CV_INPUT+i));

			addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(24, ytop + i * spacing)), module, Adsynth_StereoVCA::OUTL_OUTPUT+i));
			addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(24, ybot + i * spacing)), module, Adsynth_StereoVCA::OUTR_OUTPUT+i));
		}

		
	}
};


Model* modelAdsynth_StereoVCA = createModel<Adsynth_StereoVCA, Adsynth_StereoVCAWidget>("Adsynth_StereoVCA");