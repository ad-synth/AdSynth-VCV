#include "plugin.hpp"


struct _4xVCA : Module {
	enum ParamIds {
		KNOB1_PARAM,
		KNOB2_PARAM,
		KNOB3_PARAM,
		KNOB4_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		CV1_INPUT,
		IN2_INPUT,
		CV2_INPUT,
		IN3_INPUT,
		CV3_INPUT,
		IN4_INPUT,
		CV4_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	_4xVCA() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(KNOB1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(KNOB2_PARAM, 0.f, 1.f, 0.f, "");
		configParam(KNOB3_PARAM, 0.f, 1.f, 0.f, "");
		configParam(KNOB4_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct _4xVCAWidget : ModuleWidget {
	_4xVCAWidget(_4xVCA* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/4xVCA.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 32.548)), module, _4xVCA::KNOB1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 52.949)), module, _4xVCA::KNOB2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 73.35)), module, _4xVCA::KNOB3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 93.751)), module, _4xVCA::KNOB4_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 29.497)), module, _4xVCA::IN1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 29.497)), module, _4xVCA::CV1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 49.898)), module, _4xVCA::IN2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 49.898)), module, _4xVCA::CV2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 70.299)), module, _4xVCA::IN3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 70.299)), module, _4xVCA::CV3_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 90.7)), module, _4xVCA::IN4_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 90.7)), module, _4xVCA::CV4_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 29.497)), module, _4xVCA::OUT1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 49.898)), module, _4xVCA::OUT2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 70.299)), module, _4xVCA::OUT3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 90.7)), module, _4xVCA::OUT4_OUTPUT));
	}
};


Model* model_4xVCA = createModel<_4xVCA, _4xVCAWidget>("4xVCA");