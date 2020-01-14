#include "plugin.hpp"


struct Adsynth_Offset : Module {
	enum ParamIds {
		ENUMS(KNOB_PARAM, 5),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(IN_INPUT, 5),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 5),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_Offset() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		for (int i = 0; i < 5; i++)
		{
			configParam(KNOB_PARAM + i, -10.f, 10.f, 0.f, "DC Offset (V)");
		}
	}

	void process(const ProcessArgs& args) override {
		float
			inputValue[5],
			outputValue[5],
			offsetValue[5];

		for (int i = 0; i < 5; i++)
		{
			if (inputs[IN_INPUT + i].isConnected()) {
				inputValue[i] = inputs[IN_INPUT + i].getVoltage();
				offsetValue[i] = params[KNOB_PARAM + i].getValue();
				outputValue[i] = clampSafe(inputValue[i] + offsetValue[i], -10.f, 10.f);
				outputs[OUT_OUTPUT + i].setVoltage(outputValue[i]);
			}
			else
			{
				outputs[OUT_OUTPUT + i].setVoltage(params[KNOB_PARAM + i].getValue());
			}
		}

	}
};


struct Adsynth_OffsetWidget : ModuleWidget {
	Adsynth_OffsetWidget(Adsynth_Offset* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Offset.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		for (int i = 0; i < 5; i++) {
			addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(15.24, 25.7 + (i * 21.5))), module, Adsynth_Offset::KNOB_PARAM + i));
			addInput(createInputCentered<AdsynthJack>(mm2px(Vec(6.0, 19.7 + (i * 21.5))), module, Adsynth_Offset::IN_INPUT + i));
			addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(23.98, 19.7 + (i * 21.5))), module, Adsynth_Offset::OUT_OUTPUT + i));
		}
	}
};


Model* modelAdsynth_Offset = createModel<Adsynth_Offset, Adsynth_OffsetWidget>("Adsynth_Offset");