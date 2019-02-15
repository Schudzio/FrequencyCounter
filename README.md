# Frequency meter

Meter design of frequency and phase shift between two voltage signal
based on the Nucleo F103RB and the Nokia 5110 LCD.
The meter counts the frequency and phase shift between two signals by means of a counter in general-purpose mode.
Designed to accurately measure smaller frequencies (<1kHz).
Frequency counted to the accuracy of mHz. The angle of the phase shift between two signals is given in degrees.


####  The meter uses two microcontroller inputs:  
-  The first to measure the period of the signal on the basis of which the frequency is counted.    
-  Second to measure the phase shift between the signal from the first input and the second (assuming that the signals on both inputs have the same frequency)

The meter is based on measuring the period using interrupts from rising edges. The reference signal is external 8MHz quartz turned up to 72MHz with a pre-scaler equal to 720.

To measure the sinusoidal signal, you need to use a zero-cross detector for a condenser or optocoupler.

SPI protocols were used to communicate with the display