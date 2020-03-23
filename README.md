<<<<<<< HEAD
# adsynth VCV 1.0.01b

 *simple, hp friendly VCV rack modules. download adsynth-1.0.0.zip and extract to VCV plugin lbrary*
=======
# adsynth VCV alpha

 *simple, hp friendly VCV rack modules. download adsynth-VCV.zip and extract to VCV plugin lbrary*
>>>>>>> 8d316315378305bcc7c4247cbd50523d60b6d304
 
 <img src="https://user-images.githubusercontent.com/35740399/77367698-0154af00-6d5b-11ea-928e-bd25b7e7c082.png" alt="adsynth vcv plugins"> 
 
## logic

*treshold* - sets the treshold value for logic gates, in volts

inputs A and B goes into:

logic gates: *and*, *nand*, *or*, *nor*

*max/min* outputs respective voltage - suitable for audio/cv signals


## miniSeq

8-step sequencer equipped with three CV-switchable modes: up, down, pendulum (up-down)

inputs:

*clk* - clock in

*rst* - reset sequence

*run* - when trigger is received, the sequence starts running

*mode* - sets running mode by incoming CV signal

outputs:

*clk* - passes on internal clock

*eoc* - sends trigger when cycle finishes 

useful in creating longer than 8-step sequences with multiple instances of plugin:

_example:_ both seq: loop off. seq1 eoc -> seq2 run & seq2 eoc -> seq1 run 

_creates 16 step sequence when in up/down mode_

*CV*- sequence control voltage output

*gate* - gate output


## miniOSC

hp friendly audio oscillator with four simultaneous output waves: sine, triangle, pulse, sawtooth

*pwm* - sets pulse width modulation for pulse wave

*polar* - switches range from 0V - 10V to -5V - 5V


## miniLFO

hp friendly low frequency oscillator with four simultaneous output waves: sine, triangle, pulse, sawtooth

*pw* - sets pulse width for pulse wave

*+/-* - switches range from 0V - 10V to -5V - 5V

*phase* - inverts phase


## uEnv

simple hp friendly ADSR envelope generator.

exp/lin/log curves included.


## 2mult4

dual audio & CV multiples

twice: one input - four outputs


## mult8

one input, eight outputs signal multiples


## stripmix

hp friendly signal merger, with two operating modes:

*sum* - adds the signals

*avg* - adds signals then divises the signal with value of used inputs


## uPots

4xVCA in minimal hp environment


## StereoVCA

quad voltage controlled attenuator

both channels are controlled by single CV signal


## 5xVCA

linear voltage controlled attenuator times five.

less cramped layout than uPots.


## Offset

simple DC offset, range -10V - 10V


## ClockDiv

clock divider