/*
 CS3516-B01 Networks
 Qiaoyu Liao
 qliao@wpi.edu
 
 THIS is using the Alternating Bit Protocol
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

#define WAIT 50 //expire time in timer
#define MAXBUF 50// maximum length of A sender buffer


//global variable used in A
int A_STATE = 0; //state = 0 : waiting for call, state = 1 : waiting for ACK
int A_ACK = 0; //ACK number it is waiting for
int A_BUF = 0; //buffer length 
char buffer[MAXBUF][MESSAGE_LENGTH]; //input buffer of A
struct pkt A_prev_packet;

//global variable used in B
int B_STATE = 0;// state = 0: waiting for sequenece# 0 packet, state = 1: waiting for sequence# 1 packet
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

 
 //helper function, flip 0 to 1 and 1 to 0
int flip_num(int num){
	if(num == 0){return 1;}
	else return 0;
}


//helper function, generate the checksum of the packet using BSD checksum method
int get_checksum(struct pkt packet){
	const char *buf = packet.payload;
	int size = MESSAGE_LENGTH;

	int sum = 0;
	int i;
	//add seqnum in the checksum
	sum = (sum >> 1) + ((sum & 1) << 15);
	sum += packet.seqnum;
    sum &= 0xffff; 
    //add acknumin the checksum
    sum = (sum >> 1) + ((sum & 1) << 15);
	sum += packet.acknum;
    sum &= 0xffff; 

	//accumulate the checksum of the payload
	for(int i = 0; i < size -1; i+= 2){
		sum = (sum >> 1) + ((sum & 1) << 15);
		sum += (unsigned short) (buf[i] << 8 | buf[i + 1]); 
		sum &= 0xffff; 
	}
	//handle the odd size
	if(size & 1){
		sum = (sum >> 1) + ((sum & 1) << 15);
		sum += (unsigned char) buf[i];
		sum &= 0xffff; 
	}
	//return the checksum	
	return sum;
}


//helper function, check the checksum of the packet
int check_checksum(struct pkt packet){
	//compare the checksum to the calculated checksum
	return (packet.checksum == get_checksum(packet));
}


//process to make the packet from the message and sent the packet out
void A_process(struct msg message){
	//make the packet
	struct pkt packet;
	strncpy(packet.payload,message.data,MESSAGE_LENGTH);
	packet.seqnum = A_ACK;
	packet.acknum = flip_num(A_ACK);
	//get the checksum
	packet.checksum = get_checksum(packet);

	//set the prev packet to this packet
	A_prev_packet = packet;

	//state change to waiting for ACK
	A_STATE = flip_num(A_STATE);

	//send out the packet, start the timer
	tolayer3(AEntity,packet);
	
	if(getTimerStatus(AEntity)){
		stopTimer(AEntity);
	}
	startTimer(AEntity, WAIT);
}


/* 
 * A_output(message), where message is a structure of type msg, containing 
 * data to be sent to the B-side. This routine will be called whenever the 
 * upper layer at the sending side (A) has a message to send. It is the job 
 * of your protocol to insure that the data in such a message is delivered 
 * in-order, and correctly, to the receiving side upper layer.
 */
void A_output(struct msg message) {
	//first check if it is waiting for an ACK

	//if it is waiting for an ack
	if(A_STATE == 1){
		//as long as the message is not empty, store the message
		if(message.data[0] != 0){
			//if used up all 50 buffer space
			if(A_BUF >= MAXBUF){
				//exit program
				printf("\nUSED UP ALL 50 BUFFER SPACE, EXIT PROGRAM\n");
				exit(0);
			}
			strncpy(buffer[A_BUF],message.data,MESSAGE_LENGTH);		
			A_BUF++;
		}		
	}


	//if the sender is waiting to send data, the buffer is empty and it received a non-empty message
	//process the message
	else if(A_STATE == 0 && A_BUF == 0 && message.data[0] != 0){
		A_process(message);
	}

	//if the sender is waiting to send data, buffer is empty, message is empty
	//ignore
	else if(A_STATE == 0 && A_BUF == 0 && message.data[0] == 0){}

	//if the sender is waiting to send data, the buffer is not empty and it received a empty message
	//process the first message in the buffer
	else if(A_STATE == 0 && A_BUF > 0 && message.data[0] ==0){
		//construct the message from the buffer
		struct msg message_1;
       	strncpy(message_1.data,buffer[0],MESSAGE_LENGTH);

       	//if buffer length is larger than 1, loop to move the buffer forward
       	if(A_BUF > 1){
       		//loop around to move the buffer forward
       		for(int i=1; i < A_BUF; i++){
       			char temp[MESSAGE_LENGTH];
       			strncpy(temp,buffer[i],MESSAGE_LENGTH);
       			strncpy(buffer[i-1],temp,MESSAGE_LENGTH);
       		}
  		}
  		//clear the used buffer
  		memset(buffer[A_BUF-1],0,sizeof(char)*MESSAGE_LENGTH);
  		//decrement the buffer length
       	A_BUF--;
       	//send the message to process
      	A_process(message_1);
	}

	//if now the sender is waiting to send data, it receive a message but the buffer is not empty
	//put the message at the end of buffer and process the first message in the buffer
	else if(A_STATE == 0 && A_BUF >0 && message.data[0] != 0){

		//get the message at the head of buffer
       	struct msg message_1;
       	strncpy(message_1.data,buffer[0],MESSAGE_LENGTH);

       	//if buffer length is larger than 1, loop to move the buffer forward
       	if(A_BUF > 1){
       		//loop around to move the message forward
       		for(int i=1; i < A_BUF; i++){
       			char temp[MESSAGE_LENGTH];
       			strncpy(temp,buffer[i],MESSAGE_LENGTH);
       			strncpy(buffer[i-1],temp,MESSAGE_LENGTH);
       		}       		
  		}
  		//clear the used buffer
  		memset(buffer[A_BUF-1],0,sizeof(char)*MESSAGE_LENGTH);
  		//decrement buffer length
       	A_BUF--;

       	//cpoy the message to the tail of buffer
		strncpy(buffer[A_BUF],message.data,MESSAGE_LENGTH);
		//increment the buffer length
		A_BUF++;

       	//send the message to process
       	A_process(message_1);
	}

	//shouldn't enter this field, all occurance should be included
	else{
		printf("THIS IS A WRONG FIELD\n");
	}
}


/*
 * Just like A_output, but residing on the B side.  USED only when the 
 * implementation is bi-directional.
 */
void B_output(struct msg message) {
}


/* 
 * A_input(packet), where packet is a structure of type pkt. This routine 
 * will be called whenever a packet sent from the B-side (i.e., as a result 
 * of a tolayer3() being done by a B-side procedure) arrives at the A-side. 
 * packet is the (possibly corrupted) packet sent from the B-side.
 */
void A_input(struct pkt packet) {
    
    //if the packet has the right ack and right checksum
    //also the sender is in accepting ACK state
    if(packet.acknum == A_ACK  && check_checksum(packet) && A_STATE == 1){
    	//stop the timer
        stopTimer(AEntity);
        //change the ACK number waiting for
        A_ACK = flip_num(A_ACK);
        //change the state to waiting to send
        A_STATE = flip_num(A_STATE);

        //send an empty message to A_output incase the buffer is not empty
        struct msg message = {0};
        A_output(message);
    }
    //if received a wrong ACK or corrpted packet
    //ignore, waiting until the timer interrupted
    else{}
}


/*
 * A_timerinterrupt()  This routine will be called when A's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */
void A_timerinterrupt() {
	//resend the previous packet
    tolayer3(AEntity,A_prev_packet);
    //restart the timer
    if(getTimerStatus(AEntity)){
		stopTimer(AEntity);
	}
    startTimer(AEntity,WAIT);

}  

/* The following routine will be called once (only) before any other    */
/* entity A routines are called. You can use it to do any initialization */
void A_init() {
	//initialize the state, expected ack number
	A_STATE = 0;
	A_ACK = 0;
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
	//if the packet received has the right seqence number and not corrupted
	//since the checksum algorithm is not perfect, corrupted packet still might pass
	if(packet.seqnum == B_STATE  && check_checksum(packet) ){
		//construct the message to send to layer5
		struct msg message;
		strncpy(message.data,packet.payload,MESSAGE_LENGTH);

		//construct the ACK packet to send back to A
		struct pkt ack_packet;
		ack_packet.seqnum = packet.seqnum;//packet.seqnum == B_STATE
		ack_packet.acknum = B_STATE;
		strncpy(ack_packet.payload,packet.payload,MESSAGE_LENGTH);
		ack_packet.checksum = get_checksum(ack_packet);

		//set the B's previous packet to the current ACK packet
		B_prev_packet = ack_packet;

		// B now waiting for next sequence number
		B_STATE = flip_num(B_STATE);

		//send back the ACK packet
		tolayer3(BEntity,ack_packet);
		//deliver the message
		tolayer5(BEntity,message);
	}
	//if the packet is corrupted or has the wrong sequnece number
	else{
		//resend the previous packet
		tolayer3(BEntity,B_prev_packet);
	}
		
}

/*
 * B_timerinterrupt()  This routine will be called when B's timer expires 
 * (thus generating a timer interrupt). You'll probably want to use this 
 * routine to control the retransmission of packets. See starttimer() 
 * and stoptimer() in the writeup for how the timer is started and stopped.
 */

//this should not happen, but incase it happened from the project2.c
void  B_timerinterrupt() {
    tolayer3(BEntity,B_prev_packet);
}

/* 
 * The following routine will be called once (only) before any other   
 * entity B routines are called. You can use it to do any initialization 
 */
void B_init() {
	//set the B_state to waiting for ACK 0
	B_STATE = 0;
	//set up the B_prev_packet
	B_prev_packet.acknum = 1;
	B_prev_packet.seqnum = 1;
	B_prev_packet.checksum = get_checksum(B_prev_packet);
}

