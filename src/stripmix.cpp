#include "plugin.hpp"


struct Adsynth_Stripmix : Module {
	enum ParamIds {
		MODE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(IN_INPUT, 4),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 2),
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(ON_LIGHT, 4),
		NUM_LIGHTS
	};

	Adsynth_Stripmix() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(MODE_PARAM, 0, 1, 0, "Sum / Average");
	}

	int checkInputs() {
		float activeInputs = 0.f;
		for (int i = 0; i < 4; i++)
		{
			inputs[IN_INPUT + i].isConnected() ? activeInputs+=1.f : activeInputs = activeInputs;
		}
		return clamp(activeInputs, 1.f, 4.f);
	}
	float sumAllInputs() {
		float sumAllInputs = 0;
		for (int i = 0; i < 4; i++)
		{
			sumAllInputs += inputs[IN_INPUT + i].getVoltage();
		}
		return sumAllInputs;
	}

	void process(const ProcessArgs& args) override {
		
		float modeParam,
			inputValue[4],
			outputValue;
		for (int i = 0; i < 4; i++)
		{
			inputValue[i] = inputs[IN_INPUT + i].getVoltage();
			lights[ON_LIGHT+i].setSmoothBrightness(inputValue[i]/10.f, 36 * args.sampleTime);
		}
		modeParam = std::round(params[MODE_PARAM].getValue());
		if (modeParam) {
			outputValue = clamp(sumAllInputs(), -10.f, 10.f);
		}
		else {
			outputValue = clamp(sumAllInputs() / checkInputs(), -10.f, 10.f);
		}
		outputs[OUT_OUTPUT+0].setVoltage(outputValue);
		outputs[OUT_OUTPUT+1].setVoltage(outputValue);
	}
};


struct Adsynth_StripmixWidget : ModuleWidget {
	Adsynth_StripmixWidget(Adsynth_Stripmix* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/stripmix.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthGreenSeqButton>(mm2px(Vec(5.08, 75.451)), module, Adsynth_Stripmix::MODE_PARAM));

		for (int i = 0; i < 4; i++)
		{
			float posX = 5.08;
			addInput(createInputCentered<AdsynthJack>(mm2px(Vec(posX, 25.5 + i * 13)), module, Adsynth_Stripmix::IN_INPUT + i));
			addChild(createLightCentered<SmallLight<GreenLight>>(mm2px(Vec(8, 21 + i * 13)), module, Adsynth_Stripmix::ON_LIGHT + i));
		}

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 93.5)), module, Adsynth_Stripmix::OUT_OUTPUT + 0));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 105.5)), module, Adsynth_Stripmix::OUT_OUTPUT + 1));
	}
};


Model* modelAdsynth_Stripmix = createModel<Adsynth_Stripmix, Adsynth_StripmixWidget>("Adsynth_Stripmix");