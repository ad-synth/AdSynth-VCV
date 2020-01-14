#include "plugin.hpp"


struct Adsynth_5xVCA : Module {
	enum ParamIds {
		ENUMS(KNOB_PARAM, 5),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(IN_INPUT, 5),
		ENUMS(CV_INPUT, 5),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 5),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_5xVCA() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		for (int i = 0; i < 5; i++) {
			configParam(KNOB_PARAM + i, 0.f, 1.f, 0.f, string::f("Ch %d gain", i + 1), "%", 0, 100);
		}
	}

	void process(const ProcessArgs& args) override {
		for (int i = 0; i < 5; i++) {
			float cv_in[5];
			float gain[5];
			float input[5];
			float out[5];

			input[i] = inputs[IN_INPUT + i].getVoltage();
			gain[i] = params[KNOB_PARAM + i].getValue();

			if (inputs[CV_INPUT + i].isConnected()) {
				cv_in[i] = clamp(inputs[CV_INPUT + i].getVoltage(), 0.f, 10.f);
			}
			else {
				cv_in[i] = 10.f;
			}


			out[i] = input[i] * (gain[i] * cv_in[i] / 10);
			out[i] = clamp(out[i], -11.4f, 11.4f);
			outputs[OUT_OUTPUT + i].setVoltage(out[i]);
		}
	}
};



struct Adsynth_5xVCAWidget : ModuleWidget {
	Adsynth_5xVCAWidget(Adsynth_5xVCA* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/5xVCA.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(29.448, 19.7)), module, Adsynth_5xVCA::KNOB_PARAM+0));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(29.448, 41.2)), module, Adsynth_5xVCA::KNOB_PARAM+1));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(29.448, 62.7)), module, Adsynth_5xVCA::KNOB_PARAM+2));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(29.448, 84.2)), module, Adsynth_5xVCA::KNOB_PARAM+3));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(29.448, 105.7)), module, Adsynth_5xVCA::KNOB_PARAM+4));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 19.7)), module, Adsynth_5xVCA::IN_INPUT+0));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(17.967, 19.7)), module, Adsynth_5xVCA::CV_INPUT+0));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 41.2)), module, Adsynth_5xVCA::IN_INPUT+1));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(17.967, 41.2)), module, Adsynth_5xVCA::CV_INPUT+1));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 62.7)), module, Adsynth_5xVCA::IN_INPUT+2));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(17.967, 62.7)), module, Adsynth_5xVCA::CV_INPUT+2));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 84.2)), module, Adsynth_5xVCA::IN_INPUT+3));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(17.967, 84.2)), module, Adsynth_5xVCA::CV_INPUT+3));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 105.7)), module, Adsynth_5xVCA::IN_INPUT+4));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(17.967, 105.7)), module, Adsynth_5xVCA::CV_INPUT+4));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.347, 19.7)), module, Adsynth_5xVCA::OUT_OUTPUT+0));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.347, 41.2)), module, Adsynth_5xVCA::OUT_OUTPUT+1));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.347, 62.7)), module, Adsynth_5xVCA::OUT_OUTPUT+2));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.347, 84.2)), module, Adsynth_5xVCA::OUT_OUTPUT+3));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.347, 105.7)), module, Adsynth_5xVCA::OUT_OUTPUT+4));
	}
};


Model* modelAdsynth_5xVCA = createModel<Adsynth_5xVCA, Adsynth_5xVCAWidget>("Adsynth_5xVCA");