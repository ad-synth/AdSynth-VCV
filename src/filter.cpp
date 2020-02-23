#include "plugin.hpp"


struct Adsynth_filter : Module {
	enum ParamIds {
		FREQ_PARAM,
		DRIVE_PARAM,
		RES_PARAM,
		FREQMOD_PARAM,
		CV_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		DRIVE_INPUT,
		RES_INPUT,
		FREQMOD_INPUT,
		CV_INPUT,
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

	Adsynth_filter() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(FREQ_PARAM, 0.f, 1.f, 0.f, "");
		configParam(DRIVE_PARAM, 0.f, 1.f, 0.f, "");
		configParam(RES_PARAM, 0.f, 1.f, 0.f, "");
		configParam(FREQMOD_PARAM, 0.f, 1.f, 0.f, "");
		configParam(CV_PARAM, 0.f, 1.f, 0.f, "");
	}

	void process(const ProcessArgs& args) override {
	}
};


struct Adsynth_filterWidget : ModuleWidget {
	Adsynth_filterWidget(Adsynth_filter* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/filter.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthKnobHugeRed>(mm2px(Vec(25.4, 42.5)), module, Adsynth_filter::FREQ_PARAM));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(18.4, 66.551)), module, Adsynth_filter::DRIVE_PARAM));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(32.4, 66.551)), module, Adsynth_filter::RES_PARAM));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(18.4, 85.5)), module, Adsynth_filter::FREQMOD_PARAM));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(32.4, 85.5)), module, Adsynth_filter::CV_PARAM));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 66.5)), module, Adsynth_filter::DRIVE_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(42.8, 66.5)), module, Adsynth_filter::RES_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 85.5)), module, Adsynth_filter::FREQMOD_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(42.8, 85.5)), module, Adsynth_filter::CV_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8.0, 100.5)), module, Adsynth_filter::IN1_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(18.4, 100.5)), module, Adsynth_filter::IN2_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(32.4, 100.5)), module, Adsynth_filter::OUT1_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.8, 100.5)), module, Adsynth_filter::OUT2_OUTPUT));
	}
};


Model* modelAdsynth_filter = createModel<Adsynth_filter, Adsynth_filterWidget>("Adsynth_filter");