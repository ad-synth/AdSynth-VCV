#pragma once
#include <rack.hpp>


using namespace rack;

extern Plugin* pluginInstance;

extern Model* modelTriggerburst;
extern Model* modelAdsynth_Mult8;
extern Model* modelAdsynth_2mult4;
extern Model* modelAdsynth_Logic;
extern Model* modelAdsynth_5xVCA;
extern Model* modelAdsynth_Miniseq;
extern Model* modelAdsynth_Offset;
extern Model* modelAdsynth_Stripmix;
extern Model* modelAdsynth_StereoVCA;
extern Model* modelAdsynth_MiniLFO;
extern Model* modelAdsynth_MiniOSC;
extern Model* modelAdsynth_uPots;
extern Model* modelAdsynth_uEnv;
extern Model* modelAdsynth_filter;
extern Model* modelAdsynth_VUvca;


//-----jacks

struct AdsynthJack : SVGPort {
	AdsynthJack() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthJack.svg")));
	}
};
struct AdsynthJackTeal : SVGPort {
	AdsynthJackTeal() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthJackTeal.svg")));
	}
};
struct AdsynthJackRed : SVGPort {
	AdsynthJackRed() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthJackRed.svg")));
	}
};

//-----knobs

struct AdsynthSmallKnob : SVGKnob {
	AdsynthSmallKnob() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSmallKnob.svg")));
	}
};
struct AdsynthKnobSmallRed : SVGKnob {
	AdsynthKnobSmallRed() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthKnobSmallRed.svg")));
	}
};
struct AdsynthKnobSmallTeal : SVGKnob {
	AdsynthKnobSmallTeal() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthKnobSmallTeal.svg")));
	}
};
struct AdsynthBigKnobRed : SVGKnob {
	AdsynthBigKnobRed() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthBigKnobRed.svg")));
	}
};
struct AdsynthBigKnobTeal : SVGKnob {
	AdsynthBigKnobTeal() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthBigKnobTeal.svg")));
	}
};

struct AdsynthKnobHugeRed : SVGKnob {
	AdsynthKnobHugeRed() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthKnobHugeRed.svg")));
	}
};


struct AdsynthBigKnob : SVGKnob {
	AdsynthBigKnob() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthBigKnob.svg")));
	}
};

struct AdsynthTrimpot : SVGKnob {
	AdsynthTrimpot() {
		box.size = Vec(60, 60);
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthTrimpot.svg")));
	}
};



struct AdsynthSmallSnapKnob : SVGKnob {

	AdsynthSmallSnapKnob ()	{
		box.size = Vec(40, 40);
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		Knob::snap = true;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSmallTriKnob.svg")));
	
	}
};

struct AdsynthSmallTriKnob : SVGKnob {

	AdsynthSmallTriKnob() {
		minAngle = -0.4 * M_PI;
		maxAngle = 0.4 * M_PI;
		Knob::snap = true;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSmallTriKnob.svg")));
	}
};


struct AdsynthTriKnobMini : SVGKnob {

	AdsynthTriKnobMini() {
		minAngle = -0.4 * M_PI;
		maxAngle = 0.4 * M_PI;
		Knob::snap = true;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthTriKnobMini.svg")));
	}
};

//-----buttons, switches

struct AdsynthSwitch : SVGSwitch {

	AdsynthSwitch() {
		momentary = false;
		box.size = Vec(60, 60);
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSwitch_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSwitch_1.svg")));
		shadow->visible = false;
	}
};

struct AdsynthButton : SVGSwitch {

	AdsynthButton() {
		momentary = false;
		box.size = Vec(30, 30);
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthButton_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthButton_1.svg")));
		shadow->visible = false;
	}

};
struct AdsynthGreenSeqButton : SVGSwitch {

	AdsynthGreenSeqButton() {
		momentary = false;
		shadow->visible = false;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthGreenSeqButton_0.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthGreenSeqButton_1.svg")));

	}
};

//======experimental

struct AdsynthVUtick : SvgKnob {
	AdsynthVUtick() {
		minAngle = -0.2 * M_PI;
		maxAngle = 0.2 * M_PI;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthVUtick.svg")));
	}
};

struct AdsynthFader : SvgSlider {
	AdsynthFader() {
		this->box.size = Vec(4, 30);
		this->setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSliederSlot.svg")));
		this->setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSliderHandle.svg")));
	}
};