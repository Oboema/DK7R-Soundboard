# Het Grote DK7R Soundboard

## Hoe het environment klaar te maken

1. Download en installeer [Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. Installeer in de Arduino IDE de volgende libraries:  
	  * TMRpcm (by TMRh20)
	  * sdfat  (by Bill Greiman)
`(library manager staat onder Sketch -> Include Library -> Manage Libraries)`
4. Ga naar de folder waar de Arduino libs staan (op windows: `C:\Users\YOURUSERNAME\Documents\Arduino\libraries`)
5. Uncomment de regel `#define SDFAT` in TMRpcm/pcmConfig.h
6. ???
7. Profit
       
## Format sound files

Sound files moeten in het formaat 8 bit unsigned pcm zijn gezet.
Zie [Deze random guide](https://www.techwalla.com/articles/how-to-make-8-bit-sounds-with-audacity) om dat te doen met [Audacity](https://www.audacityteam.org/download/)

## Vragen?
Wil je de hardware ff lenen om te testen?
Is er iets niet duidelijk?
Stuur Daniel ff een appje
