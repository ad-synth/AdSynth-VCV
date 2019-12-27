#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelTriggerburst);
	p->addModel(modelMult8);
	p->addModel(model_2mult4);
	p->addModel(modelLogic);
	p->addModel(model_5xVCA);

	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
