#include "plugin.hpp"


struct Adsynth_2mult4 : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(IN_INPUT, 2),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 8),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_2mult4() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}


	void process(const ProcessArgs& args) override {
		
		float input[2];
		float output[8];


		for (int i = 0; i < 4; i++) {
			input[0] = inputs[IN_INPUT + 0].getVoltage();
			output[i] = input[0];
			outputs[OUT_OUTPUT + i].setVoltage(output[i]);
		}
		for (int i = 4; i < 8; i++) {
			input[1] = inputs[IN_INPUT + 1].getVoltage();
			output[i] = input[1];
			outputs[OUT_OUTPUT + i].setVoltage(output[i]);
		}
		
	}

};

struct Adsynth_2mult4Widget : ModuleWidget {
	Adsynth_2mult4Widget(Adsynth_2mult4* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/2mult4.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(10.16, 17.5)), module, Adsynth_2mult4::IN_INPUT+0));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(10.16, 72.098)), module, Adsynth_2mult4::IN_INPUT+1));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 30.215)), module, Adsynth_2mult4::OUT_OUTPUT+0));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 38.963)), module, Adsynth_2mult4::OUT_OUTPUT+1));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 47.712)), module, Adsynth_2mult4::OUT_OUTPUT+2));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 56.46)), module, Adsynth_2mult4::OUT_OUTPUT+3));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 84.715)), module, Adsynth_2mult4::OUT_OUTPUT+4));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 93.463)), module, Adsynth_2mult4::OUT_OUTPUT+5));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 102.212)), module, Adsynth_2mult4::OUT_OUTPUT+6));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(10.16, 110.96)), module, Adsynth_2mult4::OUT_OUTPUT+7));
	}
};


Model* modelAdsynth_2mult4 = createModel<Adsynth_2mult4, Adsynth_2mult4Widget>("Adsynth_2mult4");