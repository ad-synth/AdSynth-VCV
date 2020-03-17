#include "plugin.hpp"


struct Adsynth_ClockDiv : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		IN_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 8),
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(ON_LIGHT, 8),
		NUM_LIGHTS
	};

	Adsynth_ClockDiv() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	}


	//globals

	int length = 210, //2 x 3 x 5 x 7
		index = 1;

	dsp::TSchmittTrigger<> clockInput;
	dsp::PulseGenerator internalTrigger, activeLight;

	void process(const ProcessArgs& args) override {
		float input = 10,
			output[8],
			light[8];

		bool trigger = clockInput.process(inputs[IN_INPUT].getVoltage());

		//pulse counter

		if (trigger) {
			internalTrigger.trigger(1e-3);
			activeLight.trigger(1);
			index++;
		}
		if (index > length) index = 1;
		
		//----------------

		for (int i = 0; i < 8; i++)
		{	
			output[i] = clamp(1-(index % (i + 1)), 0, 1) * input * internalTrigger.process(args.sampleTime);
			light[i] = clamp(1 - (index % (i + 1)), 0, 1) * input * activeLight.process(args.sampleTime);
			outputs[OUT_OUTPUT + i].setVoltage(output[i]);
			lights[ON_LIGHT + i].setBrightness(light[i]);
		}
		
	}
};


struct Adsynth_ClockDivWidget : ModuleWidget {
	Adsynth_ClockDivWidget(Adsynth_ClockDiv* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ClockDiv.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 17)), module, Adsynth_ClockDiv::IN_INPUT));

		for (int i = 0; i < 8; i++) {
			int spacing = 11;
			addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 32 + spacing *i)), module, Adsynth_ClockDiv::OUT_OUTPUT + i));
			addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(3, 27 + spacing * i)), module, Adsynth_ClockDiv::ON_LIGHT + i));
		}
	}
};


Model* modelAdsynth_ClockDiv = createModel<Adsynth_ClockDiv, Adsynth_ClockDivWidget>("Adsynth_ClockDiv");