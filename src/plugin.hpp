#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
extern Model* modelTriggerburst;
extern Model* model_4xVCA;
extern Model* modelMult8;
extern Model* model_2mult4;