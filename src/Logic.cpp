#include "plugin.hpp"


struct Adsynth_Logic : Module {
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
		LEDOR_LIGHT,
		LEDNOR_LIGHT,
		LEDAND_LIGHT,
		LEDNAND_LIGHT,
		LEDMAX_LIGHT,
		LEDMIN_LIGHT,
		NUM_LIGHTS
	};

	Adsynth_Logic() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(TRESHOLD_PARAM, 0.f, 1.f, 5, "Logic gates treshold");
	}

	void process(const ProcessArgs& args) override {

		float treshold, 
			inputa, 
			inputb, 
			outand, 
			outnand, 
			outor,
			outnor,
			outmax, 
			outmin, 
			ledmin;

		treshold = params[TRESHOLD_PARAM].getValue();

		inputa = inputs[INA_INPUT].getVoltage();
		inputb = inputs[INB_INPUT].getVoltage();

		(inputa >= treshold && inputb >= treshold) ? outand = 10.f : outand = 0.f;
		 outnand = 10.f - outand;

		(inputa >= treshold || inputb >= treshold) ? outor = 10.f : outor = 0.f;
		(inputa <= treshold || inputb <= treshold) ? outnor = 10.f : outnor = 0.f;

		(inputa >= inputb) ? outmax = inputa : outmax = inputb;
		(inputa <= inputb) ? outmin = inputa : outmin = inputb;

		ledmin = outmin;

		(outmin < 0.f) ? ledmin = ledmin * -1 : ledmin = ledmin;

		outputs[AND_OUTPUT].setVoltage(outand);
		outputs[NAND_OUTPUT].setVoltage(outnand);
		outputs[OR_OUTPUT].setVoltage(outor);
		outputs[NOR_OUTPUT].setVoltage(outnor);
		outputs[MAX_OUTPUT].setVoltage(outmax);
		outputs[MIN_OUTPUT].setVoltage(outmin);

		lights[LEDAND_LIGHT].setBrightness(outand / 10);
		lights[LEDNAND_LIGHT].setBrightness(outnand / 10);
		lights[LEDOR_LIGHT].setBrightness(outor / 10);
		lights[LEDNOR_LIGHT].setBrightness(outnor / 10);
		lights[LEDMAX_LIGHT].setBrightness(outmax / 5);
		lights[LEDMIN_LIGHT].setBrightness(ledmin / 5);
	}
};


struct Adsynth_LogicWidget : ModuleWidget {
	Adsynth_LogicWidget(Adsynth_Logic* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Logic.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthTrimpot>(mm2px(Vec(10.16, 39.0)), module, Adsynth_Logic::TRESHOLD_PARAM));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(12.806, 18.482)), module, Adsynth_Logic::INA_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(12.806, 28.828)), module, Adsynth_Logic::INB_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 54.04)), module, Adsynth_Logic::OR_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 64.624)), module, Adsynth_Logic::NOR_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 75.208)), module, Adsynth_Logic::AND_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 85.792)), module, Adsynth_Logic::NAND_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 96.376)), module, Adsynth_Logic::MAX_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(12.806, 106.96)), module, Adsynth_Logic::MIN_OUTPUT));

		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.0, 54.04)), module, Adsynth_Logic::LEDOR_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.0, 64.624)), module, Adsynth_Logic::LEDNOR_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.0, 75.208)), module, Adsynth_Logic::LEDAND_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.0, 85.792)), module, Adsynth_Logic::LEDNAND_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.0, 96.376)), module, Adsynth_Logic::LEDMAX_LIGHT));
		addChild(createLightCentered<MediumLight<GreenLight>>(mm2px(Vec(6.0, 106.96)), module, Adsynth_Logic::LEDMIN_LIGHT));
	}
};


Model* modelAdsynth_Logic = createModel<Adsynth_Logic, Adsynth_LogicWidget>("Adsynth_Logic");