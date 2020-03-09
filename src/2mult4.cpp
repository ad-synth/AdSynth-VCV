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
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<AdsynthJackRed>(mm2px(Vec(5.08, 17)), module, Adsynth_2mult4::IN_INPUT+0));
		addInput(createInputCentered<AdsynthJackTeal>(mm2px(Vec(5.08, 69)), module, Adsynth_2mult4::IN_INPUT+1));


		float spacing = 9.5;
		for (int i = 0; i < 4; i++) {
			addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 29.5 + i * spacing)), module, Adsynth_2mult4::OUT_OUTPUT + i));
		}
		for (int i = 4; i < 8; i++) {
			addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 80.5 + (i-4) * spacing)), module, Adsynth_2mult4::OUT_OUTPUT + i));
		}
	}
};


Model* modelAdsynth_2mult4 = createModel<Adsynth_2mult4, Adsynth_2mult4Widget>("Adsynth_2mult4");