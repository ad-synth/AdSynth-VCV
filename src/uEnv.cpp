#include "plugin.hpp"


struct Adsynth_uEnv : Module {
	enum ParamIds {
		ATTACK_PARAM,
		DECAY_PARAM,
		SUSTAIN_PARAM,
		RELEASE_PARAM,
		MODE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		RESET_INPUT,
		GATE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		GATE_OUTPUT,
		OUT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_uEnv() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(ATTACK_PARAM, 0.f, 10.f, 5.f, "");
		configParam(DECAY_PARAM, 0.f, 10.f, 5.f, "");
		configParam(SUSTAIN_PARAM, 0.f, 10.f, 5.f, "");
		configParam(RELEASE_PARAM, 0.f, 10.f, 5.f, "");
		configParam(MODE_PARAM, 0.f, 2.f, 1.f, "");
	
	}

	struct ENV {

		float slope = 1 / std::pow(parameter / 2 + 0.01, 2);

		float target = 1,
			parameter,
			attackVal,
			decayVal,
			sustainVal,
			releaseVal,
			output = 0;

		int modeState;

		bool gateState,			
			resetState,
			attack = true,
			decay = false,
			sustain = false,
			release = false;

		void setValues(float attack, float decay, float sustain, float release, bool gate, int mode, bool reset) {
			attackVal = attack;
			decayVal = decay;
			sustainVal = sustain;
			releaseVal = release;
			gateState = gate;
			modeState = mode;
			resetState = reset;
		}
		
		void step(float dt) {
			//slope = (24 / std::pow(parameter / 2 + 0.08, 2)) - 0.2;
			slope = dsp::approxExp2_taylor5(10-parameter)/10;
			if (modeState <= 0 && modeState < 1) {
				float expSlope = (std::pow((target - output), 2) + 0.01) *  30 * slope * dt;
				(target > output) ? output += expSlope : output -= expSlope;
			}
			else if (modeState >= 1 && modeState < 2) {
				(target > output) ? output += slope * dt : output -= slope * dt;
				}
			else {
				//float logSlope = (std::pow(output, 2) + 0.01) * slope * dt;
				float logSlope = (std::pow(output/2, 2) + 0.01) * 30 * slope * dt;
				if (target > output) {
					output += logSlope;
				}
				else {
					output -= logSlope;
				}
			}
			
			if (resetState) attack = true;
		}

		void setValue() {
			if (gateState) {				
				if (attack) {
					parameter = attackVal;
					target = 1.f;
					(output >= target) ? decay = true, attack = false: decay = false;
				}
				else if (decay) {
					parameter = decayVal;
					target = sustainVal / 10;
					attack = false;
					
				}
				else if (!decay) {
					output = sustainVal / 10;
					decay = false;
				}
			}
			else {
				parameter = releaseVal;
				target = 0.f;
				attack = true;
			}
		}

		float getOutputValue() {
			return output * 10;
		}
	};
	
	ENV envelope;
	//dsp::SchmittTrigger gateInput;


	void process(const ProcessArgs& args) override {
		float attack,
			decay,
			sustain,
			release,
			output;

		int mode;

		bool gate,		
			reset;


		attack = params[ATTACK_PARAM].getValue();
		decay = params[DECAY_PARAM].getValue();
		sustain = params[SUSTAIN_PARAM].getValue();
		release = params[RELEASE_PARAM].getValue();
		mode = params[MODE_PARAM].getValue();
		gate = clampSafe(0.f, 1.f, inputs[GATE_INPUT].getVoltage());
		reset = clampSafe(0.f, 1.f, inputs[RESET_INPUT].getVoltage());
		envelope.setValues(attack, decay, sustain, release, gate, mode, reset);
		envelope.setValue();
		envelope.step(args.sampleTime);

		output = envelope.getOutputValue();

		outputs[GATE_OUTPUT].setVoltage(inputs[GATE_INPUT].getVoltage());
		outputs[OUT_OUTPUT].setVoltage(output);


	}
};


struct Adsynth_uEnvWidget : ModuleWidget {
	Adsynth_uEnvWidget(Adsynth_uEnv* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/uEnv.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 15.5)), module, Adsynth_uEnv::ATTACK_PARAM));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 26.833)), module, Adsynth_uEnv::DECAY_PARAM));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 38.167)), module, Adsynth_uEnv::SUSTAIN_PARAM));
		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 49.5)), module, Adsynth_uEnv::RELEASE_PARAM));
		addParam(createParamCentered<AdsynthTriKnobMini>(mm2px(Vec(5.08, 60.5)), module, Adsynth_uEnv::MODE_PARAM));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 70.5)), module, Adsynth_uEnv::RESET_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 82.5)), module, Adsynth_uEnv::GATE_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 97.5)), module, Adsynth_uEnv::GATE_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 109.5)), module, Adsynth_uEnv::OUT_OUTPUT));
	}
};


Model* modelAdsynth_uEnv = createModel<Adsynth_uEnv, Adsynth_uEnvWidget>("Adsynth_uEnv");