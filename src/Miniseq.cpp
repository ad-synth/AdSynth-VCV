#include "plugin.hpp"


struct Adsynth_Miniseq : Module {
	enum ParamIds {
		ENUMS(SWITCH_PARAM, 8),
		ENUMS(KNOB_PARAM, 8),
		CLOCK_PARAM,
		DIR_PARAM,
		STEPS_PARAM,
		RUN_PARAM,
		LOOP_PARAM,
		GATE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		GATEPW_INPUT,
		CLOCK_INPUT,
		RESET_INPUT,
		RUN_INPUT,
		MODE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		CV_OUTPUT,
		GATE_OUTPUT,
		CLOCK_OUTPUT,
		EOC_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(LED_LIGHT, 8),
		ENUMS(ON_LIGHT, 8),
		ENUMS(MODE_LIGHT, 3),
		NUM_LIGHTS
	};

	//variables=======================================================

	bool runState = true,
		eocCache = false;


	int index = 0,
		modeSwitch = 1,
		direction = 1,
		seqLength = 8;
	float outCache = 0,
		clockPhase = 0.5f;
	dsp::SchmittTrigger gateInput;
	dsp::SchmittTrigger resetInput;
	dsp::SchmittTrigger runInput;
	dsp::PulseGenerator internalClock, externalClock;
	dsp::PulseGenerator eocGate, outputGate;

	//functions=======================================================

	int voltageToSwitch(float voltage) {
		int switchValue = 0;
		voltage = 3 * (voltage + 5.f);
		if (voltage <= 10.f) switchValue = -1;
		else if (voltage > 10.f && voltage <= 20.f) switchValue = 0;
		else if (voltage > 20.f) switchValue = 1;
		return switchValue;
	}

	void resetSequence() {
		switch (modeSwitch) {
		case 1: index = 0; direction = 1; break;
		case 2: index = seqLength - 1; break;
		case 3: index = 0; direction = 1; break;
		}
		//(modeSwitch == 2) ? index = seqLength - 1 : index = 0, direction = 1;
	}
	void stopSequence() {
		runState = false;
		params[RUN_PARAM].setValue(0.f);
		resetSequence();
	}
	void muteOuts() {
		outputGate.reset();
		outputs[CV_OUTPUT].setVoltage(0.f);
	}
	void setModeLight() {
		for (int i = 0; i < 3; i++) {
			lights[MODE_LIGHT + i].setBrightness(0.f);
		}
		switch (clamp(modeSwitch, 1, 3))
		{
		case 1: lights[MODE_LIGHT + 0].setBrightness(10.f); break;
		case 2: lights[MODE_LIGHT + 1].setBrightness(10.f); break;
		case 3: lights[MODE_LIGHT + 2].setBrightness(10.f); break;
		default: break;
		}
	}
	void killSeqLights() {
		for (int i = 0; i < 8; i++) {
			lights[LED_LIGHT + i].setBrightness(0.f);
		}
	}
	void killActiveLights() {
		for (int i = 0; i < 8; i++) {
			lights[ON_LIGHT + i].setBrightness(0.f);
		}
	}
	void setActiveLights (){
		for (int i = 0; i < seqLength; i++) {
			lights[ON_LIGHT + i].setBrightness(1.f);
		}
	}

	//=======================================================================

	Adsynth_Miniseq() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		for (int i = 0; i < 8; i++) {
			configParam(SWITCH_PARAM + i, 0.f, 1.f, 0.f, "");
			configParam(KNOB_PARAM + i, 0.f, 1.f, 0.f, "");
			lights[LED_LIGHT + i].setBrightness(0.f);
			lights[ON_LIGHT + i].setBrightness(1.f);
		}
		configParam(DIR_PARAM, 1.f, 3.f, 1, "1: Forward 2: Backward 3: Pendulum");
		configParam(STEPS_PARAM, 2.f, 8.f, 8, "Step Count");
		configParam(CLOCK_PARAM, 0.f, 10.f, 5, "Clock Rate");
		configParam(RUN_PARAM, 0.f, 1.f, 1.f, "Run");
		configParam(LOOP_PARAM, 0.f, 1.f, 1, "Loop / OneShot");
		configParam(GATE_PARAM, 0, 1, 1, "Gatelength");
	}

	void process(const ProcessArgs& args) override {

		float
			input = 10,			//output stage gain for all signals
			out,
			gain[8],
			tempoMod = 0.1,		//clock tempo linear modifier
			clockRate,
			extClock,
			trigger,
			eoc,
			modeParamValue,
			gateOut,
			gateLength = 0.1,  //in seconds, init value only
			gatePW = params[GATE_PARAM].getValue();

		bool
			pendulum,
			switchState,
			runInValue,
			runButtonValue,
			reset,
			loop,
			pulse;
		//fetch parameters;
		
		reset = resetInput.process(inputs[RESET_INPUT].getVoltage());
		loop = params[LOOP_PARAM].getValue();
		seqLength = std::round(params[STEPS_PARAM].getValue());				
		runButtonValue = params[RUN_PARAM].getValue();
		runInValue = runInput.process(inputs[RUN_INPUT].getVoltage());
		runState = runButtonValue || runInValue;
			
		modeSwitch = std::round(params[DIR_PARAM].getValue());

		if (inputs[MODE_INPUT].isConnected()) {
			modeParamValue = inputs[MODE_INPUT].getVoltage();
			modeSwitch += voltageToSwitch(modeParamValue);
		}
		else modeParamValue = 0;

		//lights init

		setModeLight();		
		killActiveLights();
		setActiveLights();

		//generate internal clock signal
		
		clockRate = tempoMod * std::pow(2.f, params[CLOCK_PARAM].getValue());
		clockPhase += clockRate * args.sampleTime;
		
		if (clockPhase >= 0.5f) {
			internalClock.trigger(1e-6f);
			externalClock.trigger(1e-3f);
			clockPhase -= 1.f;
		}
		extClock = externalClock.process(args.sampleTime);
		pulse = internalClock.process(args.sampleTime);
		

		if (inputs[GATEPW_INPUT].isConnected()) {
			gatePW += inputs[GATEPW_INPUT].getVoltage() / input;
		}
		gateLength = gatePW / clockRate;

		//choosing clock source
		
		if (inputs[CLOCK_INPUT].isConnected()) {
			trigger = gateInput.process(inputs[CLOCK_INPUT].getVoltage());
			outputs[CLOCK_OUTPUT].setVoltage(inputs[CLOCK_INPUT].getVoltage());
			
			gateLength = 8; 

			// if external clock is connected - max gatetime equals fixed 8 sec
			// (bypasses gate param, sets permanent legato mode)
			// todo: gatelength for ext clx
		}
		else {
			trigger = pulse ? 10.f : 0.f;
			outputs[CLOCK_OUTPUT].setVoltage(extClock);
		}

		runState += clampSafe(inputs[RUN_INPUT].getVoltage(), 0.f, 10.f);
		runState ? params[RUN_PARAM].setValue(10.f) : params[RUN_PARAM].setValue(0.f);	//button light on/off

		if (reset) resetSequence();

		//check direction

		switch (modeSwitch) {
		case 1: direction = 1; pendulum = false; break;
		case 2: direction = -1; pendulum = false; break;
		case 3: pendulum = true; break;
		default: direction = 1; pendulum = false; break;
		}

		if (trigger && eocCache) {
			muteOuts();
			eocCache = 0;
		}

		if (trigger && runState) {

			killSeqLights();

			//process row

			switchState = params[SWITCH_PARAM + index].getValue();
			gain[index] = params[KNOB_PARAM + index].getValue();

			if (switchState) {
				outCache = gain[index];
				outputGate.trigger(gateLength);
			}
			else {
				outputGate.reset();
			}

			out = input * outCache;

			outputs[CV_OUTPUT].setVoltage(out);
			lights[LED_LIGHT + index].setSmoothBrightness(1.f, 5e3);

			index += direction;
		}

		eoc = eocGate.process(args.sampleTime) * 10.f;
		outputs[EOC_OUTPUT].setVoltage(eoc);
		
		gateOut = input * outputGate.process(args.sampleTime);

		outputs[GATE_OUTPUT].setVoltage(gateOut);

		//one shot or loop

		if (!loop && eoc) {
			stopSequence();
			eocCache++;
		}
		
		//check if on edge of sequence

		if (pendulum) {
			if (index <= 0 && direction == -1) direction = 1, eocGate.trigger(1e-3f);
			if (index >= seqLength - 1 && direction == 1) direction = -1;
		}
		else {
			if (index < 0) resetSequence(), eocGate.trigger(5e-3f);
			if (index > seqLength-1) resetSequence(), eocGate.trigger(5e-3f);
		}
	}
};

struct Adsynth_MiniseqWidget : ModuleWidget {
	Adsynth_MiniseqWidget(Adsynth_Miniseq* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Miniseq.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		const int jackTop = 97, jackBot = 109;

		for (int i = 0; i < 8; i++)
		{
			const float mult = 9.5;
			addParam(createParamCentered<AdsynthSmallKnob>(mm2px(Vec(17, 18 + i * mult)), module, Adsynth_Miniseq::KNOB_PARAM + i));
			addParam(createParamCentered<AdsynthGreenSeqButton>(mm2px(Vec(8, 18 + i * mult)), module, Adsynth_Miniseq::SWITCH_PARAM + i));
			addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(23, 16 + i * mult)), module, Adsynth_Miniseq::LED_LIGHT + i));
			addChild(createLightCentered<SmallLight<BlueLight>>(mm2px(Vec(23, 20 + i * mult)), module, Adsynth_Miniseq::ON_LIGHT + i));

		}

		addParam(createParamCentered<AdsynthBigKnobRed>(mm2px(Vec(37.8, 22)), module, Adsynth_Miniseq::CLOCK_PARAM));
		addParam(createParamCentered<AdsynthSmallSnapKnob>(mm2px(Vec(41.8, 47)), module, Adsynth_Miniseq::STEPS_PARAM));
		addParam(createParamCentered<AdsynthKnobSmallTeal>(mm2px(Vec(32.8, 35)), module, Adsynth_Miniseq::GATE_PARAM));

		addParam(createParamCentered<AdsynthSmallTriKnob>(mm2px(Vec(37, 70.5)), module, Adsynth_Miniseq::DIR_PARAM));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(32, 62.5)), module, Adsynth_Miniseq::MODE_LIGHT + 0));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(37, 59.5)), module, Adsynth_Miniseq::MODE_LIGHT + 1));
		addChild(createLightCentered<SmallLight<RedLight>>(mm2px(Vec(42, 62.5)), module, Adsynth_Miniseq::MODE_LIGHT + 2));

		addParam(createParamCentered<AdsynthGreenSeqButton>(mm2px(Vec(31.8, 82.5)), module, Adsynth_Miniseq::RUN_PARAM));
		addParam(createParamCentered<AdsynthGreenSeqButton>(mm2px(Vec(42.8, 82.5)), module, Adsynth_Miniseq::LOOP_PARAM));


		addInput(createInputCentered<AdsynthJackTeal>(mm2px(Vec(31.8, 44.5)), module, Adsynth_Miniseq::GATEPW_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8, jackTop)), module, Adsynth_Miniseq::CLOCK_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(8, jackBot)), module, Adsynth_Miniseq::RUN_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(19, jackTop)), module, Adsynth_Miniseq::RESET_INPUT));
		addInput(createInputCentered<AdsynthJack>(mm2px(Vec(19, jackBot)), module, Adsynth_Miniseq::MODE_INPUT));

		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(31.8, jackTop)), module, Adsynth_Miniseq::CLOCK_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(31.8, jackBot)), module, Adsynth_Miniseq::EOC_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.8, jackTop)), module, Adsynth_Miniseq::CV_OUTPUT));
		addOutput(createOutputCentered<AdsynthJack>(mm2px(Vec(42.8, jackBot)), module, Adsynth_Miniseq::GATE_OUTPUT));

	}
};

Model* modelAdsynth_Miniseq = createModel<Adsynth_Miniseq, Adsynth_MiniseqWidget>("Adsynth_Miniseq");