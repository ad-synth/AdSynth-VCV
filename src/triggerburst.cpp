#include "plugin.hpp"


struct Triggerburst : Module {
	enum ParamIds {
		FREQ1_PARAM,
		MCV1_PARAM,
		FREQ2_PARAM,
		MCV2_PARAM,
		FREQ3_PARAM,
		MCV3_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CV1_INPUT,
		CV2_INPUT,
		CV3_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		LED2_LIGHT,
		LED3_LIGHT,
		NUM_LIGHTS
	};

	Triggerburst() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(FREQ1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MCV1_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQ2_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MCV2_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQ3_PARAM, 0.f, 1.f, 0.f, "");
		configParam(MCV3_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct TriggerburstWidget : ModuleWidget {
	TriggerburstWidget(Triggerburst* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/triggerburst.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(11.086, 34.573)), module, Triggerburst::FREQ1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.691, 34.573)), module, Triggerburst::MCV1_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(11.086, 58.084)), module, Triggerburst::FREQ2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.691, 58.084)), module, Triggerburst::MCV2_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(11.086, 81.594)), module, Triggerburst::FREQ3_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.691, 81.594)), module, Triggerburst::MCV3_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 112.0)), module, Triggerburst::CV1_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.2, 112.0)), module, Triggerburst::CV2_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(26.4, 112.0)), module, Triggerburst::CV3_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(40.6, 112.0)), module, Triggerburst::OUT1_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(43.205, 58.084)), module, Triggerburst::LED2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(43.205, 81.594)), module, Triggerburst::LED3_LIGHT));
	}
};


Model* modelTriggerburst = createModel<Triggerburst, TriggerburstWidget>("triggerburst");