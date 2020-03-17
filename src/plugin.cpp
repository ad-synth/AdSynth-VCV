#include "plugin.hpp"


Plugin* pluginInstance;


void init(Plugin* p) {
	pluginInstance = p;

	p->addModel(modelAdsynth_Mult8);
	p->addModel(modelAdsynth_2mult4);
	p->addModel(modelAdsynth_Stripmix);
	p->addModel(modelAdsynth_Offset);
	p->addModel(modelAdsynth_Logic);
	p->addModel(modelAdsynth_5xVCA);
	p->addModel(modelAdsynth_Miniseq);
	p->addModel(modelAdsynth_StereoVCA);
	p->addModel(modelAdsynth_MiniLFO);
	p->addModel(modelAdsynth_MiniOSC);
	p->addModel(modelAdsynth_uPots);
	p->addModel(modelAdsynth_uEnv);
	p->addModel(modelAdsynth_ClockDiv);
}