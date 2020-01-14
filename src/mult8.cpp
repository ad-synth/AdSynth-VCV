#include "plugin.hpp"


struct Adsynth_Mult8 : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		IN_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 16),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_Mult8() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}

	void process(const ProcessArgs& args) override {
		float input;
		float output[8];
		

		for (int i = 0; i < 8; i++){
			input = inputs[IN_INPUT].getVoltage();
			output[i] = input;
			outputs[OUT_OUTPUT + i].setVoltage(output[i]);
		}
	}
};


struct Adsynth_Mult8Widget : ModuleWidget {
	Adsynth_Mult8Widget(Adsynth_Mult8* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Mult8.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(10.16, 17.5)), module, Adsynth_Mult8::IN_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 32.494)), module, Adsynth_Mult8::OUT_OUTPUT +0));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 43.255)), module, Adsynth_Mult8::OUT_OUTPUT + 1));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 54.016)), module, Adsynth_Mult8::OUT_OUTPUT + 2));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 64.777)), module, Adsynth_Mult8::OUT_OUTPUT + 3));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 75.539)), module, Adsynth_Mult8::OUT_OUTPUT + 4));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 86.3)), module, Adsynth_Mult8::OUT_OUTPUT + 5));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 97.061)), module, Adsynth_Mult8::OUT_OUTPUT + 6));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 107.822)), module, Adsynth_Mult8::OUT_OUTPUT + 7));
	}
};


Model* modelAdsynth_Mult8 = createModel<Adsynth_Mult8, Adsynth_Mult8Widget>("Adsynth_Mult8");