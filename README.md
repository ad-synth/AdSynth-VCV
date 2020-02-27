 	2mult4
		
		dual audio & CV multiples
		twice: one input - four outputs
		
	5xVCA
		
		linear voltage controlled attenuator times five.
	
	Logic
		
		treshold parameter sets the treshold value for logic gates, in volts
		inputs A and B goes into:
		logic gates: and, nand, or, nor
		max/min outputs respective voltage - suitable for audio/cv signals
		
	MiniLFO
		
		hp friendly low frequency oscillator with four simultaneous output waves: sine, triangle, pulse, sawtooth
		pw - sets pulse width for pulse wave
		polar - switches range from 0V - 10V to -5V - 5V
	
	MiniOSC

		hp friendly audio oscillator with four simultaneous output waves: sine, triangle, pulse, sawtooth
		pwm - sets pulse width modulation for pulse wave
		polar - switches range from 0V - 10V to -5V - 5V
	
	Miniseq
		
		8-step sequencer equipped with three CV-switchable modes: up, down, pendulum (up-down)
		inputs:
			clk - clock in
			rst - reset sequence
			run - when trigger is received, the sequence starts running
			mode - sets running mode by incoming CV signal		
		outputs:
			clk - passes on internal clock
			eoc - sends trigger when cycle finishes 
					useful in creating longer than 8-step sequences with multiple instances of plugin:
					eg: both: loop off. seq1 eoc -> seq2 run & seq2 eoc -> seq1 run creates 16 step sequence when in up/down mode
			CV - sequence control voltage output
			gate - gate output
		
	Offset
		
		simple DC offset, range -10V - 10V
	
	StereoVCA
	
		quad voltage controlled attenuator
		both channels are controlled by single CV signal

	
	VUvca
		
		dummy module, non functional
	
	filter.cpp 	

		dummy module, non functional
	
	mult8

		one input, eight outputs signal multiples
	
	stripmix

		hp friendly signal merger, with two operating modes:
		sum - simply adds the signals
		avg - adds signals then divises the signal with value of used inputs
	
	triggerburst

		//uhhhh that module build was just a IDE/SDK test but welp, peeps keep sayin that it comes in handy so its still here
		
		outputs burst of triggers with given frequency, likewise: (f1 * cv1) + (f2 * cv2) + (f3 * cv3)
		
	uEnv

		simple hp friendly ADSR envelope generator.
		exp/lin/log curves included.
	
	uPots
	
		passive linear attenuator. like, input -> potentiometer -> output.
	