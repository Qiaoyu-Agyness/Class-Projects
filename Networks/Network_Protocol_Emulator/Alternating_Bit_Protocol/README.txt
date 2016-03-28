Qiaoyu Liao
qliao@wpi.edu
CS2516 Project2
README for Alternating Bit Protocol
----------------------------------
Command Line Argument
----------------------------------
Compile the program:
	make

Run the simulator for Go Back N Protocol
	./abp

----------------------
File List
----------------------
project2.c
project2.h
student2.c
qliao_ABP_trace.pdf
makefile
README.txt

----------------------------------
After running the program, follow the instruction to emulate the protocol:

Enter the number of messages to simulate: 30
	Number greater than 0

Packet loss probability [enter number between 0.0 and 1.0]: 0.2
	Number between 0.0 and 1.0

Packet corruption probability [0.0 for no corruption]: 0.2
	Number between 0.0 and 1.0

Packet out-of-order probability [0.0 for no out-of-order]: 0
	Number between 0.0 and 1.0
	But for this assignment, only run it with 0

Average time between messages from sender's layer5 [ > 0.0]: 1000
	Number greater than 0
	suggest to use number around 1000

Enter Level of tracing desired: 2
	Integer from 0 to larger than 5,  0 means little or no tracing, 5 means detailed tracing

Do you want actions randomized: (1 = yes, 0 = no)? 
	0 for no randomaztion, 1 for randomazation

Do you want Bidirectional: (1 = yes, 0 = no)? 
	0 for unidirectional, 1 for bidirectional
	But for this assignment, only run it with 0

----------------------------------





