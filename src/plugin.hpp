#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
extern Model* modelTriggerburst;
extern Model* modelMult8;
extern Model* model_2mult4;
extern Model* modelLogic;
extern Model* model_5xVCA;

struct AdsynthSmallKnob : SVGKnob {
	AdsynthSmallKnob() {
		box.size = Vec(80, 80);
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AdsynthSmallKnob.svg")));
	}
};
struct AdsynthTrimpot : SVGKnob {
	AdsynthTrimpot() {
		box.size = Vec(60, 60);
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		maxAngle = 0.83 * M_PI;
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AdsynthTrimpot.svg")));
	}
};

struct AdsynthJack : SVGPort {
	AdsynthJack() {
		box.size = Vec(84, 84);
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/AdsynthJack.svg")));
	}
};