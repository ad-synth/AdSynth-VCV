#include "plugin.hpp"


struct Adsynth_uPots : Module {
	enum ParamIds {
		ENUMS(KNOB_PARAM, 3),
		NUM_PARAMS
	};

	enum InputIds {
		ENUMS(IN_INPUT, 3),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 3),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_uPots() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(KNOB_PARAM, 0.f, 1.f, 1.f, "Gain 1");
		configParam(KNOB_PARAM, 0.f, 1.f, 1.f, "Gain 2");
		configParam(KNOB_PARAM, 0.f, 1.f, 1.f, "Gain 3");
	}

	void process(const ProcessArgs& args) override {
		for (int i = 0; i < 3; i++) {
			float gain[3],
				input[3],
				out[3];

			input[i] = inputs[IN_INPUT + i].getVoltage();
			gain[i] = params[KNOB_PARAM + i].getValue();

			out[i] = input[i] * gain[i];
			out[i] = clamp(out[i], -11.4f, 11.4f);
			outputs[OUT_OUTPUT + i].setVoltage(out[i]);
		}
	}
};


struct Adsynth_uPotsWidget : ModuleWidget {
	Adsynth_uPotsWidget(Adsynth_uPots* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/uPots.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 27.5)), module, Adsynth_uPots::KNOB_PARAM+0));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 52.641)), module, Adsynth_uPots::KNOB_PARAM+1));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 77.59)), module, Adsynth_uPots::KNOB_PARAM+2));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 17.5)), module, Adsynth_uPots::IN_INPUT+0));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 42.5)), module, Adsynth_uPots::IN_INPUT+1));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 67.5)), module, Adsynth_uPots::IN_INPUT+2));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 92.5)), module, Adsynth_uPots::OUT_OUTPUT+0));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 101.5)), module, Adsynth_uPots::OUT_OUTPUT+1));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 110.5)), module, Adsynth_uPots::OUT_OUTPUT+2));
	}
};


Model* modelAdsynth_uPots = createModel<Adsynth_uPots, Adsynth_uPotsWidget>("Adsynth_uPots");