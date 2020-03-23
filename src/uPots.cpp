#include "plugin.hpp"


struct Adsynth_uPots : Module {
	enum ParamIds {
		ENUMS(KNOB_PARAM, 4),
		NUM_PARAMS
	};

	enum InputIds {
		ENUMS(IN_INPUT, 4),
		ENUMS(CV_INPUT, 4),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 4),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_uPots() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(KNOB_PARAM + 0, 0.f, 1.f, 1.f, "Gain 1");
		configParam(KNOB_PARAM + 1, 0.f, 1.f, 1.f, "Gain 2");
		configParam(KNOB_PARAM + 2, 0.f, 1.f, 1.f, "Gain 3");
		configParam(KNOB_PARAM + 3, 0.f, 1.f, 1.f, "Gain 4");
	}

	void process(const ProcessArgs& args) override {
		for (int i = 0; i < 4; i++) {
			float gain[4],
				cvInput[4],
				input[4],
				out[4];

			if (inputs[CV_INPUT + i].isConnected()) {
				cvInput[i] = clamp(inputs[CV_INPUT + i].getVoltage(), 0.f, 10.f);
			}
			else {
				cvInput[i] = 10.f;
			}
			input[i] = inputs[IN_INPUT + i].getVoltage();
			gain[i] = cvInput[i] * params[KNOB_PARAM + i].getValue() / 10;

			out[i] = input[i] * gain[i];
			out[i] = clamp(out[i], -10.f, 10.f);
			outputs[OUT_OUTPUT + i].setVoltage(out[i]);
		}
	}
};


struct Adsynth_uPotsWidget : ModuleWidget {
	Adsynth_uPotsWidget(Adsynth_uPots* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/uPots.svg")));

		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		for (int i = 0; i < 4; i++) {
			addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(15.24, 22.5 + i * 12)), module, Adsynth_uPots::KNOB_PARAM + i));
			addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 22.5 + i * 12)), module, Adsynth_uPots::CV_INPUT + i));
		}
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 72.5)), module, Adsynth_uPots::IN_INPUT + 0));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 84.5)), module, Adsynth_uPots::IN_INPUT+1));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 97)), module, Adsynth_uPots::IN_INPUT+2));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 109)), module, Adsynth_uPots::IN_INPUT+3));
		
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(15.24, 72.5)), module, Adsynth_uPots::OUT_OUTPUT + 0));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(15.24, 84.5)), module, Adsynth_uPots::OUT_OUTPUT+1));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(15.24, 97)), module, Adsynth_uPots::OUT_OUTPUT+2));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(15.24, 109)), module, Adsynth_uPots::OUT_OUTPUT + 3));
	}
};


Model* modelAdsynth_uPots = createModel<Adsynth_uPots, Adsynth_uPotsWidget>("Adsynth_uPots");