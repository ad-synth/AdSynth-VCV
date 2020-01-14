#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
extern Model* modelTriggerburst;
extern Model* modelAdsynth_Mult8;
extern Model* modelAdsynth_2mult4;
extern Model* modelAdsynth_Logic;
extern Model* modelAdsynth_5xVCA;
extern Model* modelAdsynth_Miniseq;
extern Model* modelAdsynth_Offset;
extern Model* modelAdsynth_Stripmix;

struct AdsynthJack : SVGPort {
	AdsynthJack() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthJack.svg")));
	}
};

struct AdsynthSmallKnob : SVGKnob {
	AdsynthSmallKnob() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		shadow->visible = false;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/components/AdsynthSmallKnob.svg")));
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
		box.size = Vec(60, 60);
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