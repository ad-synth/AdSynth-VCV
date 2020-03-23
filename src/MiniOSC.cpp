#include "plugin.hpp"


struct Adsynth_MiniOSC : Module {
	enum ParamIds {
		PWM_PARAM,
		FINE_PARAM,
		SEMI_PARAM,
		COARSE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		VOCT_INPUT,
		SYNC_INPUT,
		PITCH_INPUT,
		PWM_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SAW_OUTPUT,
		SINE_OUTPUT,
		PULSE_OUTPUT,
		TRI_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_MiniOSC() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(SEMI_PARAM, -12, 12, 0, "Semitones");
		configParam(FINE_PARAM, -1, 1, 0, "Fine tune");
		configParam(COARSE_PARAM, -5, 5, 0, "Coarse");
		configParam(PWM_PARAM, 0, 2, 1, "Pulse Width", "%");
	}
	struct OSC {
		float phase = 0.f,
			baseFrequency = dsp::FREQ_C4,
			semiFrequency = 0.f,
			fineFrequency = 0.f,
			outputFrequency = 1.f,
			amplitude = 0.f,
			pulseWidth = 0.5f;

		void setBaseFrequency(float voltOct) {
			baseFrequency = voltOct;
		}
		void setSemiFrequency(float voltOct) {
			semiFrequency = voltOct;
		}
		void setFineFrequency(float voltOct) {
			fineFrequency = voltOct;
		}
		void setOutputFrequency() {
			// Compute frequency, pitch is 1V/oct
			float pitch = 1.f + baseFrequency + semiFrequency / 12.f + fineFrequency / 12.f;
			outputFrequency = dsp::FREQ_C4 * std::pow(2.f, pitch);
			outputFrequency = clamp(outputFrequency, 0.f, 20000.f);
		}
		void setPulseWidth(float pw) {
			pulseWidth = pw;
		}
		void resetCycle() {
			phase = 0.f;
		}

		void step(float dt) {
			phase >= 1.f ? phase = 0.f : phase += outputFrequency * dt;
		}
		float analogSine() {
			//-x*(x-1.4)*(x+1.4)
			float volt, transform;
			volt = sin(2 * M_PI * phase);
			transform = -volt * (volt - 2.2) * (volt + 2.2);
			volt = 1.3f * transform;
			return volt;
		}
		float analogSaw() {
			float volt, transform;
			volt = 1.f - 2 * phase;
			transform = -volt * 2 * (volt - 1.8)*(volt+2);
			return transform;
		}
		//=========================todo: PWM behavior, find dem polynominals again
		float analogPulse() {
			float volt, transform;
			volt = 20 - (40* sin(2 * M_PI * phase * (phase*(pulseWidth+0.19))));
			volt = clamp(volt, -1.f, 1.f);
			transform = -volt * 1.3 * (volt - 2.2) * (volt + 2.2);
			return transform;
		}
		float analogTriangle() {
			float volt;
			float transform;
			if (phase <= 0.25f) volt = phase;
			else if (phase > 0.25f && phase <= 0.75f) volt = 0.25f - (phase - 0.25f);
			else volt = -0.25f + (phase - 0.75f);
			volt *= 50;
			transform = (phase - 1.f)*2*phase;
			volt *= transform;
			return volt;
		}
	};

	OSC oscillator;
	dsp::SchmittTrigger syncInput;

	void process(const ProcessArgs& args) override {
		float sine, saw, squ, tri,
			reset = syncInput.process(inputs[SYNC_INPUT].getVoltage()),
			coarse = params[COARSE_PARAM].getValue(),
			semi = params[SEMI_PARAM].getValue(),
			fine = params[FINE_PARAM].getValue(),
			pwm = params[PWM_PARAM].getValue();

		if (inputs[PITCH_INPUT].isConnected()) coarse += inputs[PITCH_INPUT].getVoltage()/12;
		if (inputs[VOCT_INPUT].isConnected()) coarse += inputs[VOCT_INPUT].getVoltage();
		if (inputs[PWM_INPUT].isConnected()) pwm += inputs[PWM_INPUT].getVoltage();
		if (reset) oscillator.resetCycle();

		oscillator.setBaseFrequency(coarse);
		oscillator.setSemiFrequency(semi);
		oscillator.setFineFrequency(fine);
		oscillator.setOutputFrequency();
		oscillator.setPulseWidth(pwm);
		oscillator.step(args.sampleTime);

		sine = oscillator.analogSine();
		saw = oscillator.analogSaw();
		squ = oscillator.analogPulse();
		tri = oscillator.analogTriangle();

		outputs[SINE_OUTPUT].setVoltage(sine);
		outputs[SAW_OUTPUT].setVoltage(saw);
		outputs[PULSE_OUTPUT].setVoltage(squ);
		outputs[TRI_OUTPUT].setVoltage(tri);
	}
};


struct Adsynth_MiniOSCWidget : ModuleWidget {
	Adsynth_MiniOSCWidget(Adsynth_MiniOSC* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MiniOSC.svg")));

		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthKnobSmallTeal>(mm2px(Vec(10.16, 18.5)), module, Adsynth_MiniOSC::PWM_PARAM));
		addParam(createParamCentered<AdsynthTrimpot>(mm2px(Vec(10.16, 29.679)), module, Adsynth_MiniOSC::FINE_PARAM));
		addParam(createParamCentered<AdsynthKnobSmallRed>(mm2px(Vec(10.16, 38.76)), module, Adsynth_MiniOSC::SEMI_PARAM));
		addParam(createParamCentered<AdsynthSmallTriKnob>(mm2px(Vec(10.16, 52.5)), module, Adsynth_MiniOSC::COARSE_PARAM));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 70.5)), module, Adsynth_MiniOSC::VOCT_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(15.4, 70.5)), module, Adsynth_MiniOSC::SYNC_INPUT));
		addInput(createInputCentered<AdsynthJackRed>(mm2px(Vec(5.08, 82.5)), module, Adsynth_MiniOSC::PITCH_INPUT));
		addInput(createInputCentered<AdsynthJackTeal>(mm2px(Vec(15.4, 82.5)), module, Adsynth_MiniOSC::PWM_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 97)), module, Adsynth_MiniOSC::SAW_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(15.4, 97)), module, Adsynth_MiniOSC::SINE_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 109)), module, Adsynth_MiniOSC::PULSE_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(15.4, 109)), module, Adsynth_MiniOSC::TRI_OUTPUT));
	}
};


Model* modelAdsynth_MiniOSC = createModel<Adsynth_MiniOSC, Adsynth_MiniOSCWidget>("Adsynth_MiniOSC");