# AGCP
This is where the code is located for my implementation of the AGCP microcontroller. It is subdivided into several sections.

- AGCP Code

This folder contains the original AGCP code from Bruce as well as a modified version which contains the delay timer to measure response time.

- comms

This folder contains all of the files necessary for the communication library to be used between the vehicle computer and the ATmega328p

- steering

This folder contains an implementation of the steering control code using the communications library as well as a steering library.

- driving

This folder contains an implementation of the driving control code using the communications library as well as a driving library.


- python

This folder contains scripts which run in python using the pyserial library to test and benchmark the microcontroller's steering and driving implementations
