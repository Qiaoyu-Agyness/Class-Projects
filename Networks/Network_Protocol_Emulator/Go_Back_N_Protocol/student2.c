/*
 CS3516-B01 Networks
 Qiaoyu Liao 
 qliao@wpi.edu
 
 THIS is using the Go-Back-N Protocol
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

#define N 8 //window size
#define BUFSIZE 50 //buffer size
#define WAIT 100 //timer timeout time
 
// global variable used in A
int A_BASE = 1; //base number
int A_NEXTSEQNUM = 1;//expect next sequence number
int A_BUF = 0;//current size of the buffer
int windowsize = 0;//current size of the window
struct pkt window[N];//window
struct msg buffer[BUFSIZE];//buffer

//global variable used in B
int B_EXPECTEDSEQNUM = 1;//expected sequence number of packet
struct pkt B_prev_packet;
/* ***************************************************************************
 ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose

   This code should be used for Project 2, unidirectional or bidirectional
   data transfer protocols from A to B and B to A.
   Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets may be delivered out of order.

   Compile as gcc -g project2.c student2.c -o p2
**********************************************************************/



/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
/* 
 * The routines you will write are detailed below. As noted above, 
 * such procedures in real-life would be part of the operating system, 
 * and would be called by other procedures in the operating system.  
 * All these routines are in layer 4.
 */


//generate the checksum of the packet
int get_checksum(struct pkt packet){
	const char *buf = packet.payload;
	int size = MESSAGE_LENGTH;

	int sum = 0;
	int i;

	//add seqnum in the sum
	sum = (sum >> 1) + ((sum & 1) << 15);
	sum += packet.seqnum;
    sum &= 0xffff; 

    //add acknum in the sum
    sum = (sum >> 1) + ((sum & 1) << 15);
	sum += packet.acknum;
    sum &= 0xffff; 

	//accumulate the checksum of payload
	for(i = 0; i < size -1; i+= 2){
		sum = (sum >> 1) + ((sum & 1) << 15);
		sum += (unsigned short) (buf[i] << 8 | buf[i + 1]); 
		sum &= 0xffff; 
	}
	//handle the odd size
	if(size & 1){
		sum = (sum >> 1) + ((sum & 1) << 15);
		sum += (unsigned short) buf[i];
		sum &= 0xffff; 
	}
	
	//return the checksum	
	return sum;
}


//check the checksum of the packet
int check_checksum(struct pkt packet){
	//compare the checksum to the calculated checksum
	return (packet.checksum == get_checksum(packet));
}


// process to make the packet and send the packet out
void A_process(struct msg message){
	//make the packet
	struct pkt packet;
	strncpy(packet.payload,message.data,MESSAGE_LENGTH);
	packet.seqnum = A_NEXTSEQNUM;
	packet.acknum = A_NEXTSEQNUM; 
	//get the checksum
	packet.checksum = get_checksum(packet);

	//store the packet in the window
	window[windowsize] = packet;
	//increment the window size
	windowsize++;

	//send the packet to layer 3
	tolayer3(AEntity,packet);

	//start timer when first packet in the window being send out
	if(A_BASE == A_NEXTSEQNUM){
		if(getTimerStatus(AEntity)){
			stopTimer(AEntity);
		}
		startTimer(AEntity,WAIT);
	}
	//increment the next sequence number
	A_NEXTSEQNUM++;	
	
}

/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
void A_output(struct msg message) {
	// if the window is not full yet, buffer is empty, and receive a non-empty message
	// process the message
	if(windowsize >= 0 && windowsize < N && A_BUF == 0 && message.data[0] != 0){
		A_process(message);
	}

	//if the widow is not full, empty buffer, empty message
	//ignore
	else if(windowsize >= 0 && windowsize < N && A_BUF == 0 && message.data[0] == 0){}

	//if the window size is full, receive an empty message
	//ignore
	else if(windowsize == N && message.data[0] == 0){}

	//if the window size is full, receive a non-empty message
	else if(windowsize == N && message.data[0] != 0){
		//if used up all 50 buffer space
		if(A_BUF >= BUFSIZE){
			//exit program
			printf("\nUSED UP ALL 50 BUFFER SPACE, EXIT PROGRAM\n");
			exit(0);
		}
		//if not full, put message in the buffer
		buffer[A_BUF] = message;
		A_BUF++;
	}

	//if the window is not full yet, buffer not empty, and receive a non-empty message
	//store the message and process the first message in the buffer
	else if(windowsize >= 0 && windowsize < N && A_BUF > 0 && message.data[0] != 0){
		//if used up all 50 buffer space
		if(A_BUF >= BUFSIZE){
			printf("USED UP ALL 50 BUFFER SPACE, EXIT PROGRAM\n");
			exit(0);
		}
		//store the messge
		buffer[A_BUF] = message;
		A_BUF++;
		//construct the message from the head of buffer
		struct msg temp;
		temp = buffer[0];
		if(A_BUF > 1){
        	//loop around to move the msg in the buffer forward
        	for(int i = 1; i < A_BUF; i++){
       			buffer[i-1] = buffer[i];
       		}
      	}
       	//clear the last one in the buffer
       	memset(&buffer[A_BUF-1],0,sizeof(struct msg));
       	A_BUF--;
       	A_process(temp);
	}

	//if the window is not full yer, the buffer is not empty and receive an empty message
	//process the first message in the buffer
	else if(windowsize >= 0 && windowsize < N && A_BUF > 0 && message.data[0] == 0){
		struct msg temp;
		temp = buffer[0];
		if(A_BUF > 1){
        		//loop around to move the msg in the buffer forward
        		for(int i = 1; i < A_BUF; i++){
        			buffer[i-1] = buffer[i];
        		}
       		}
       		//clear the last one in the buffer
       		memset(&buffer[A_BUF-1],0,sizeof(struct msg));
       		A_BUF--;
       		//send the first message in buffer to process
       		A_process(temp);
	}
	
	//if windowsize become out of bound
	//overflow happen, exit
	else if(windowsize > N || windowsize < 0){
		printf("the window buffer overflowed, please restart with lower corruption rate");
		exit(0);
	}

	//shouldn't enter this field, all occurance should be included
	else{printf("THIS IS ABSOLUTLY WRONG\n");}
}

/*
 * Just like A_output, but residing on the B side.  USED only when the 
 * implementation is bi-directional.
 */
void B_output(struct msg message)  {
}

/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
	// double check that the packet is not corupted
	if(check_checksum(packet) == 1 && packet.acknum <= A_BASE + N){
		
		//sliding the sending window forward when the ACK number is greater than the sending base	
		while(window[0].seqnum <= packet.acknum && window[0].seqnum != 0 && windowsize > 0){
			for(int i = 1; i < windowsize; i++){
				window[i-1] = window[i];
			}
			memset(&window[windowsize-1],0,sizeof(struct pkt));
			windowsize--;

			//send an empty message to A_output 
			struct msg message = {0};
       		A_output(message);			
		}

		//increment the base number
		A_BASE = packet.acknum + 1;

		//check to see if the timer needed to be reset
		if(A_BASE == A_NEXTSEQNUM){
			if(getTimerStatus(AEntity)){
				stopTimer(AEntity);
			}
		}
		else{
			if(getTimerStatus(AEntity)){
				stopTimer(AEntity);
			}
			startTimer(AEntity,WAIT);
		}			
	}
}

/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
	//restart the timer
	stopTimer(AEntity);
	startTimer(AEntity,WAIT);
	//printf("\nresend all the packet start with %d to %d\n",A_BASE, A_NEXTSEQNUM-1);
	//loop to send all the package in the sending window
	for(int i=0; i< N; i++){
		if(window[i].seqnum != 0){
			tolayer3(AEntity,window[i]);
		}
		else{
			break;
		}
	}
}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
	// initialize the base and nextsequn as 1, and the buffer size as 0
	A_BASE = 1;
	A_NEXTSEQNUM = 1;
	A_BUF = 0;
	windowsize = 0;
}


/* 
 * Note that with simplex transfer from A-to-B, there is no routine  B_output() 
 */

/*
 * B_input(packet),where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the A-side (i.e., as a result 
 * of a tolayer3() being done by a A-side procedure) arrives at the B-side. 
 * packet is the (possibly corrupted) packet sent from the A-side.
 */
void B_input(struct pkt packet) {
	if(check_checksum(packet) && packet.seqnum == B_EXPECTEDSEQNUM){
		//construct the message
		struct msg message;
		strncpy(message.data, packet.payload, MESSAGE_LENGTH);
		//construct the ack packet
		struct pkt ack_packet;
		ack_packet.seqnum = 0;
		ack_packet.acknum = packet.seqnum;
		strncpy(ack_packet.payload, packet.payload,MESSAGE_LENGTH);

		//get the checksum of ack packet
		ack_packet.checksum = get_checksum(ack_packet);

		//set the previous packet to this ack packer
		B_prev_packet = ack_packet;

		//deliver the message to B layer5
		tolayer5(BEntity,message);
		//send back the ack packet
		tolayer3(BEntity,ack_packet);

		// expected sequence number increment
		B_EXPECTEDSEQNUM++;
	}
	// if B received a corrupted packet or wrong sequnce number packet
	else{
		// as long as B already received the right packet before
		if(B_EXPECTEDSEQNUM > 1){
			//resend the ACK packet
			tolayer3(BEntity,B_prev_packet);
		}
	}
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */

// this should not happen, but incase it happened from the project2.c
void  B_timerinterrupt() {
	tolayer3(BEntity,B_prev_packet);
}

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
	// initialize the EXPECTED SEQUENCE NUMBER to 1
	B_EXPECTEDSEQNUM = 1;
	// initalize the B_prev_packet
	B_prev_packet.acknum = 0;
	B_prev_packet.seqnum = 0;
	B_prev_packet.checksum = get_checksum(B_prev_packet);
}

