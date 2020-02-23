#include "plugin.hpp"


struct Adsynth_VUvca : Module {
	enum ParamIds {
		TICK_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_VUvca() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(TICK_PARAM, 0.f, 1.f, 0.f, "");
	}

	float save;

	void process(const ProcessArgs& args) override {
		float input[2], output, release;

		release = 0.0000001;
		input[0] = save;
		input[1] = inputs[IN1_INPUT].getVoltage()/10;

		if (input[1] <= input[0]) {
			params[TICK_PARAM].setValue(input[1]);
		}
		else {
			params[TICK_PARAM].setValue(input[0]);

		}
		save = input[1]*0.9;

	}
};


struct Adsynth_VUvcaWidget : ModuleWidget {
	Adsynth_VUvcaWidget(Adsynth_VUvca* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/VUvca.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.08, 96.5)), module, Adsynth_VUvca::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.4, 96.5)), module, Adsynth_VUvca::IN2_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(5.08, 108.5)), module, Adsynth_VUvca::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.4, 108.5)), module, Adsynth_VUvca::OUT2_OUTPUT));

		// mm2px(Vec(17.006, 17.862))
		addParam(createParamCentered<AdsynthFader>(mm2px(Vec(10.16, 42.5)), module, Adsynth_VUvca::TICK_PARAM));
	}
};


Model* modelAdsynth_VUvca = createModel<Adsynth_VUvca, Adsynth_VUvcaWidget>("Adsynth_VUvca");