#include "plugin.hpp"
#include <cmath>

struct Adsynth_MiniLFO : Module {
	enum ParamIds {
		COARSE_PARAM,
		PW_PARAM,
		POLAR_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CV_INPUT,
		RESET_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		SINE_OUTPUT,
		TRI_OUTPUT,
		SAW_OUTPUT,
		SQU_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Adsynth_MiniLFO() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(COARSE_PARAM, 0.f, 10.f, 0.f, "Rate");
		configParam(PW_PARAM, 0.f, 1.f, 0.5f, "Pulse width");
		configParam(POLAR_PARAM, 0.f, 1.f, 0.f, "Polarity");
	}

	struct LFO {
		float phase = 0.f,
			frequency = 0.f,
			amplitude = 0.f,
			pulseWidth = 0.5f;

		bool bipolar = true;
				
		
		void setFrequency(float voltOct) {
			voltOct = clamp(voltOct, 0.f, 10.f);
			frequency = dsp::approxExp2_taylor5(voltOct+2)/200;
		}
		void setPulseWidth(float pw) {
			pulseWidth = pw;
		}
		void setPolarity(float pol) {
			bipolar = pol;
		}
		void resetCycle() {
			phase = 0.f;
		}

		void step(float dt) {
			phase >= 1.f ? phase = 0.f: phase += frequency * dt;
		}
		float sine() {
			float volt;
			volt = 5.f * sin(2 * M_PI * phase);
			if (!bipolar) volt += 5.f;
			return volt;
		}
		float saw() {
			float volt;
			volt = 5.f - 10*phase;
			if (!bipolar) volt += 5.f;
			return volt;
		}
		float pulse() {
			float volt;
			(phase <= pulseWidth) ? volt = 5.f : volt = -5.f;
			if (!bipolar) volt += 5.f;
			return volt;
		}
		float triangle() {
			float volt;
			if (phase <= 0.25f) volt = phase;
			else if (phase > 0.25f && phase <= 0.75f) volt = 0.25f - (phase - 0.25f);
			else volt = -0.25f + (phase - 0.75f);
			volt *= 20;
			if (!bipolar) volt += 5.f;
			return volt;
		}
	};

	LFO oscillator;
	dsp::SchmittTrigger resetInput;

	void process(const ProcessArgs& args) override {
		float sine, saw, squ, tri, 
			reset = resetInput.process(inputs[RESET_INPUT].getVoltage()),
			freq = params[COARSE_PARAM].getValue(),
			pwm = params[PW_PARAM].getValue();

		if (inputs[CV_INPUT].isConnected()) freq += inputs[CV_INPUT].getVoltage();
		if (reset) oscillator.resetCycle();

		oscillator.setFrequency(freq);
		oscillator.setPulseWidth(pwm);
		oscillator.setPolarity(params[POLAR_PARAM].getValue());
		oscillator.step(args.sampleTime);

		sine = oscillator.sine();
		saw = oscillator.saw();
		squ = oscillator.pulse();
		tri = oscillator.triangle();

		outputs[SINE_OUTPUT].setVoltage(sine);
		outputs[SAW_OUTPUT].setVoltage(saw);
		outputs[SQU_OUTPUT].setVoltage(squ);
		outputs[TRI_OUTPUT].setVoltage(tri);
	}
};


struct Adsynth_MiniLFOWidget : ModuleWidget {
	Adsynth_MiniLFOWidget(Adsynth_MiniLFO* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MiniLFO.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(5.08, 18.5)), module, Adsynth_MiniLFO::COARSE_PARAM));
		addParam(createParamCentered<AdsynthTrimpot>(mm2px(Vec(5.08, 28.5)), module, Adsynth_MiniLFO::PW_PARAM));
		addParam(createParamCentered<AdsynthButton>(mm2px(Vec(5.08, 36)), module, Adsynth_MiniLFO::POLAR_PARAM));

		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 45.5)), module, Adsynth_MiniLFO::CV_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(5.08, 57.5)), module, Adsynth_MiniLFO::RESET_INPUT));


		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 72.5)), module, Adsynth_MiniLFO::SINE_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 84.5)), module, Adsynth_MiniLFO::TRI_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 96.5)), module, Adsynth_MiniLFO::SAW_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(5.08, 108.5)), module, Adsynth_MiniLFO::SQU_OUTPUT));
	}
};


Model* modelAdsynth_MiniLFO = createModel<Adsynth_MiniLFO, Adsynth_MiniLFOWidget>("Adsynth_MiniLFO");