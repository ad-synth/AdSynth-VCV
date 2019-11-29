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
		LED1_LIGHT,
		LED2_LIGHT,
		LED3_LIGHT,
		NUM_LIGHTS
	};

	dsp::PulseGenerator pulseGenerators[3];
	float phase1, phase2, phase3 = 0.f;

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

		//-- row 1

		float frequencyOne = params[FREQ1_PARAM].getValue();
		float modCVOne = params[MCV1_PARAM].getValue();

		modCVOne *= 0.2;
		modCVOne = clamp(modCVOne, 0.f, 1.f);

		frequencyOne += inputs[CV1_INPUT].getVoltage() * modCVOne;
		frequencyOne = clamp(frequencyOne, 0.f, 10.f);

		float freq1 = 0.2f * std::pow(4.f, frequencyOne);



		//---row 2

		float frequencyTwo = params[FREQ2_PARAM].getValue();
		float modCVTwo = params[MCV2_PARAM].getValue();

		modCVTwo *= 0.2;
		modCVTwo = clamp(modCVTwo, 0.f, 1.f);

		frequencyTwo += inputs[CV2_INPUT].getVoltage() * modCVTwo;
		frequencyTwo = clamp(frequencyTwo, 0.f, 10.f);

		float freq2 = 0.2f * std::pow(8.f, frequencyTwo);

		//--- row 3

		float frequencyThree = params[FREQ3_PARAM].getValue();
		float modCVThree = params[MCV3_PARAM].getValue();

		modCVThree *= 0.2;
		modCVThree = clamp(modCVThree, 0.f, 1.f);

		frequencyThree += inputs[CV3_INPUT].getVoltage() * modCVThree;
		frequencyThree = clamp(frequencyThree, 0.f, 10.f);

		float freq3 = 0.2f * std::pow(16.f, frequencyThree);

		//---

		phase1 += freq1 * args.sampleTime;
		phase2 += freq2 * args.sampleTime;
		phase3 += freq3 * args.sampleTime;

		if (phase1 >= 0.5f) {
			pulseGenerators[0].trigger(1e-3f);

			phase1 -= 1.f;
		}
		if (phase2 >= 0.5f) {
			pulseGenerators[1].trigger(1e-3f);
			phase2 -= 1.f;
		}
		if (phase3 >= 0.5f) {
			pulseGenerators[2].trigger(1e-3f);
			phase3 -= 1.f;
		}
			   		 
		bool pulse1 = pulseGenerators[0].process(args.sampleTime);
		bool pulse2 = pulseGenerators[1].process(args.sampleTime);
		bool pulse3 = pulseGenerators[2].process(args.sampleTime);

		bool pulse = ((pulse1 || pulse2 || pulse3) ? 1.f : 0.f);

		lights[LED1_LIGHT].setBrightness(pulse1 ? 1.f : 0.f);
		lights[LED2_LIGHT].setBrightness(pulse2 ? 1.f : 0.f);
		lights[LED2_LIGHT].setBrightness(pulse3 ? 1.f : 0.f);

		outputs[OUT1_OUTPUT].setVoltage(pulse ? 10.f : 0.f);
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

		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(43.204, 34.565)), module, Triggerburst::LED1_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(43.205, 58.084)), module, Triggerburst::LED2_LIGHT));
		addChild(createLightCentered<MediumLight<RedLight>>(mm2px(Vec(43.205, 81.594)), module, Triggerburst::LED3_LIGHT));
	}
};


Model* modelTriggerburst = createModel<Triggerburst, TriggerburstWidget>("triggerburst");