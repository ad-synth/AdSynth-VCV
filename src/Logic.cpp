#include "plugin.hpp"


struct Logic : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		INA_INPUT,
		INB_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AND_OUTPUT,
		NAND_OUTPUT,
		OR_OUTPUT,
		NOR_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Logic() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	void process(const ProcessArgs& args) override {
		float inputa, inputb, outand, outnand, outor, outnor;

		inputa = inputs[INA_INPUT].getVoltage();
		inputb = inputs[INB_INPUT].getVoltage();

		(inputa >= 0.1f && inputb >= 0.1f) ? outand = 10.f : outand = 0.f;
		(inputa <= 1.f && inputb <= 1.f) ? outnand = 0.f : outnand = 10.f;

		(inputa >= inputb) ? outor = inputa : outor = inputb;
		(inputa <= inputb) ? outnor = inputa : outnor = inputb;

		outputs[AND_OUTPUT].setVoltage(outand);
		outputs[NAND_OUTPUT].setVoltage(outnand);
		outputs[OR_OUTPUT].setVoltage(outor);
		outputs[NOR_OUTPUT].setVoltage(outnor);

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

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(10.16, 21.128)), module, Logic::INA_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(10.16, 36.237)), module, Logic::INB_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 58.04)), module, Logic::AND_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 74.347)), module, Logic::NAND_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 90.653)), module, Logic::OR_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 106.96)), module, Logic::NOR_OUTPUT));
	}
};


Model* modelLogic = createModel<Logic, LogicWidget>("Logic");