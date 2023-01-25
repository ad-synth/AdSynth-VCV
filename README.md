# adsynth VCV alpha

 *simple, hp friendly VCV rack modules. download adsynth-VCV.zip and extract to VCV plugin lbrary*
 
 <img src="https://user-images.githubusercontent.com/35740399/214573598-885e8305-d919-4131-b527-97823de878c1.png" alt="adsynth vcv plugins"> 
 
 note: all plugins work in VCV 1.1.4++ environment, but are NOT suitable for VCV2.0 operation.
 
## logic

treshold parameter sets the critical value for logic gates (0 - 10 V)  
and, nand, or, nor max/min - outputs respective voltage - suitable for audio/cv signals  

## miniSeq

8-step sequencer equipped with three CV-switchable modes of operation: up, down, pendulum (up-down)  
inputs:  
clk - clock/trigger input  
rst - reset sequence  
run - when trigger is received, the sequence starts running  
mode - sets running mode by incoming CV signal  
clk - passes on internal or external clock  
eoc - sends trigger when cycle finishes, useful in creating longer than 8-step sequences with multiple instances of plugin  
CV - sequence CV output  
gate - gate output

## miniOSC

slim audio oscillator with hard sync and four simultaneous output waves: sine, triangle, pulse, sawtooth  
fine, semi, coarse - tuning kobs, respectively: +/-1 semitone, +/-1 octave, +/-10 octaves.  
pwm - sets pulse width modulation for pulse wave  
pitch - additional linear FM CV input  

## miniLFO

hp friendly low frequency oscillator with four simultaneous output waves: sine, triangle, pulse, sawtooth  
pw - sets pulse width for pulse wave  
polar - switches range from 0V - 10V to -5V - 5V  

## uEnv

simple hp friendly ADSR envelope generator.  
exp/lin/log curves included.  

## 2mult4

dual audio & CV multiples  

## mult8

one input, eight outputs signal multiples  

## stripmix

hp friendly signal merger, with two operating modes:  
sum - adds the signals  
avg - adds signals then divises the signal with value of used inputs  

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
