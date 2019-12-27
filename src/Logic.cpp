#include "plugin.hpp"


struct Logic : Module {
	enum ParamIds {
		TRESHOLD_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		INA_INPUT,
		INB_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OR_OUTPUT,
		NOR_OUTPUT,
		AND_OUTPUT,
		NAND_OUTPUT,
		MAX_OUTPUT,
		MIN_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Logic() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(TRESHOLD_PARAM, 0.f, 10.f, 5.f, "Treshold for logic gates");
	}

	void process(const ProcessArgs& args) override {

		float treshold, inputa, inputb, outand, outnand, outor, outnor, outmax, outmin;

		treshold = params[TRESHOLD_PARAM].getValue();

		inputa = inputs[INA_INPUT].getVoltage();
		inputb = inputs[INB_INPUT].getVoltage();

		(inputa >= treshold && inputb >= treshold) ? outand = 10.f : outand = 0.f;
		(inputa <= treshold && inputb <= treshold) ? outnand = 0.f : outnand = 10.f;

		(inputa >= treshold || inputb >= treshold) ? outor = 10.f : outor = 0.f;
		(inputa <= treshold || inputb <= treshold) ? outnor = 10.f : outnor = 0.f;

		(inputa >= inputb) ? outmax = inputa : outmax = inputb;
		(inputa <= inputb) ? outmin = inputa : outmin = inputb;

		outputs[AND_OUTPUT].setVoltage(outand);
		outputs[NAND_OUTPUT].setVoltage(outnand);
		outputs[OR_OUTPUT].setVoltage(outor);
		outputs[NOR_OUTPUT].setVoltage(outnor);
		outputs[MAX_OUTPUT].setVoltage(outmax);
		outputs[MIN_OUTPUT].setVoltage(outmin);
	}
};


struct LogicWidget : ModuleWidget {
	LogicWidget(Logic* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Logic.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthTrimpot>(mm2px(Vec(10.16, 39.0)), module, Logic::TRESHOLD_PARAM));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(12.806, 18.482)), module, Logic::INA_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(12.806, 28.828)), module, Logic::INB_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 54.04)), module, Logic::OR_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 64.624)), module, Logic::NOR_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 75.208)), module, Logic::AND_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 85.792)), module, Logic::NAND_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 96.376)), module, Logic::MAX_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 106.96)), module, Logic::MIN_OUTPUT));
	}
};


Model* modelLogic = createModel<Logic, LogicWidget>("Logic");