#include "plugin.hpp"


struct _4xVCA : Module {
	enum ParamIds {
		ENUMS(KNOB_PARAM, 4),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(IN_INPUT, 4),
		ENUMS(CV_INPUT, 4),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(OUT_OUTPUT, 4),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	_4xVCA() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		for (int i = 0; i < 4; i++) {
			configParam(KNOB_PARAM+i, 0.f, 1.f, 0.f, string::f("Ch %d gain", i + 1), "%", 0, 100);
		}		
	}

	void process(const ProcessArgs& args) override {
		for (int i = 0; i < 4; i++) {
					float cv_in[4];
					float gain[4];
					float input[4]; 
					float out[4];
					
					input[i] = inputs[IN_INPUT + i].getVoltage();
					gain[i] = params[KNOB_PARAM + i].getValue();

					if (inputs[CV_INPUT + i].isConnected()) { 
						cv_in[i] = clamp(inputs[CV_INPUT + i].getVoltage(), 0.f, 10.f); 
					}
					else { 
						cv_in[i] = 10.f; 
					}

					
					out[i] = input[i] * (gain[i] * cv_in[i] / 10);
					out[i] = clamp(out[i], -11.4f, 11.4f);
					outputs[OUT_OUTPUT + i].setVoltage(out[i]);
		}
	}
};


struct _4xVCAWidget : ModuleWidget {
	_4xVCAWidget(_4xVCA* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/4xVCA.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 32.548)), module, _4xVCA::KNOB_PARAM + 0));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 52.949)), module, _4xVCA::KNOB_PARAM + 1));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 73.35)), module, _4xVCA::KNOB_PARAM + 2));
		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(30.384, 93.751)), module, _4xVCA::KNOB_PARAM + 3));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 29.497)), module, _4xVCA::IN_INPUT + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 49.898)), module, _4xVCA::IN_INPUT + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 70.299)), module, _4xVCA::IN_INPUT + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.0, 90.7)), module, _4xVCA::IN_INPUT + 3));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 29.497)), module, _4xVCA::CV_INPUT + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 49.898)), module, _4xVCA::CV_INPUT + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 70.299)), module, _4xVCA::CV_INPUT + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.967, 90.7)), module, _4xVCA::CV_INPUT + 3));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 29.497)), module, _4xVCA::OUT_OUTPUT + 0));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 49.898)), module, _4xVCA::OUT_OUTPUT + 1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 70.299)), module, _4xVCA::OUT_OUTPUT + 2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(42.347, 90.7)), module, _4xVCA::OUT_OUTPUT + 3));
	}
};


Model* model_4xVCA = createModel<_4xVCA, _4xVCAWidget>("4xVCA");