/*processEvent*/
#include "globals.h"
void processEvent(){
  Link tmpPtr = eventQueueHead;//take the next thing off the list
  eventQueueHead = eventQueueHead->link; //move the event queue to the next element
  int currentTime = tmpPtr->arrival_time; //initialize the current time
  switch(tmpPtr->state){//check the state of the arriving packet
    case Arriving: //if the element was arriving
	  printf("Process %d arrived at the simulation at time %d \n", tmpPtr->process_id, currentTime);//tell the user what time this process arrived
	  if(isCPUBusy)
	  {//checks if the CPU is busy
	    tmpPtr->state = Computing;//if it is, set the event's state to Computing and add it to the CPU queue
		addToQueue(&CPUQueueHead, &CPUQueueTail, tmpPtr);
	  }else
	  {//if the CPU is not busy
	    printf("Process %d beginning compute at time %d \n", tmpPtr->process_id, currentTime);//print the time the packet began computing for this cycle
		tmpPtr->state = Computing;//set the packets state to computing
		int processing = MIN(tmpPtr->cpu_time, timeSlice);//figure out if it needs a full time slice, if it doesn't, set its processing time to be however long it needs
		tmpPtr->arrival_time += processing;//increment its arrival time by its processing time
		tmpPtr->cpu_time -= processing;//decrement its CPU time required by its processing time
		tmpPtr->totalTime += processing;//increment its total time in the simulation by its processing time
		add_event(tmpPtr);//re-add it to the event list
		isCPUBusy = 1;//set the CPU to busy
	  }//end else statement
	  break;//end arrival case
	case Computing: //if the element was computing
	  if (tmpPtr->cpu_time == 0)//this task is done computing
	  {
		if(isIOBusy)//check if the IO is busy
		{ 
		  tmpPtr->state = Outputting;//if the IO is busy, set the packet's state to Outputting and add it to the IO queue
		  addToQueue(&IOQueueHead, &IOQueueTail, tmpPtr);
		} else //if the IO is not busy
		{
		  printf("Process %d beginning output at time %d \n", tmpPtr->process_id, currentTime);//print that the packet is outputting at this time
		  tmpPtr->state = Outputting;//set the packet's state to Outputting
		  tmpPtr->arrival_time += tmpPtr->io_time;//increment the packet's arrival time by the amount of time it would take to output
		  tmpPtr->totalTime += tmpPtr->io_time;//increment the packet's total time in the simulation by it's output time
		  add_event(tmpPtr);//add the packet back to the event list
		  isIOBusy = 1;//set the IO to busy
		}//end else statement
		if(CPUQueueHead != NULL)//if the CPU queue has something in it
		{
		  Link nextTask = CPUQueueHead;//take the first packet off of the CPU queue
		  CPUQueueHead = CPUQueueHead->link;
		  nextTask->queueTime += (currentTime - nextTask->arrival_time);//remember how long this task was waiting in queue
		  printf("Process %d beginning compute at time %d \n", nextTask->process_id, currentTime);//print out that the next computing packet is computing
		  int processing = MIN(nextTask->cpu_time, timeSlice); //figure out if it needs a full time slice, if it doesn't, set its processing time to be however long it needs
		  nextTask->arrival_time = currentTime + processing;//set it's arrival time to be the current time plus how long it needs to process
		  nextTask->cpu_time -= processing;//decrement it's CPU time by how long it is processing
		  nextTask->totalTime += processing;//increment it's total time in the simulation
		  add_event(nextTask);//put the next task onto the event queue
		} else 
		{//if the CPU queue doesn't have something in it, set it's state to not busy
		  isCPUBusy = 0;
		}
	  } else if (CPUQueueHead != NULL)
	  {//this task is not done and tasks waiting
	    addToQueue(&CPUQueueHead, &CPUQueueTail, tmpPtr); //Add this task to the CPU queue
		
		Link nextTask = CPUQueueHead; //remove next task waiting for CPU
		CPUQueueHead = CPUQueueHead->link;
		nextTask->queueTime += (currentTime - nextTask->arrival_time); //remember how long this task was waiting in queue
		printf("Process %d beginning compute at time %d \n", nextTask->process_id, currentTime);//print out what time it was that this packet began computing
		int processing = MIN(nextTask->cpu_time, timeSlice); //figure out if it needs a full time slice, if it doesn't, set its processing time to be however long it needs
		nextTask->arrival_time = currentTime + processing;//set it's arrival time to be the current time plus how long it needs to process
		nextTask->cpu_time -= processing;//decrement its CPU time by how long it is processing
		nextTask->totalTime += processing;//increment its time in the simulation
		add_event(nextTask);//add the event back onto the event list
		
	  } else {//this task is not done but is only task in queue
		printf("Process %d beginning compute at time %d \n", tmpPtr->process_id, currentTime);//print out what time it was that this packet began computing
		int processing = MIN(tmpPtr->cpu_time, timeSlice);//check if it needs a full time slice, if it doesn't, set its processing time to be however long it needs
		tmpPtr->arrival_time += processing;//increment its arrival time by processing time
		tmpPtr->cpu_time -= processing;//decrement its required CPU time by its processing time
		tmpPtr->totalTime += processing;//increment its total time in the simulation
		add_event(tmpPtr);//add it back into the event list
	  }
	  break;//end computing case
	case Outputting: //This task is completely done!
	  totalTime += tmpPtr->totalTime;//increment the total time by all events in the simulation by how long this event took
	  totalQueueTime += tmpPtr->queueTime;//increment the total queue time by all events in the simulation by how long this event waited
	  printf("Process %d complete at time %d \n", tmpPtr->process_id, currentTime);//print out the time the event completed
	  if(IOQueueHead == NULL){//if there is nothing in the IO queue, set it to not busy
	    isIOBusy = 0;
	  } else {//otherwise
	    Link nextTask = IOQueueHead; //remove next task waiting for IO
		IOQueueHead = IOQueueHead->link;
		nextTask->queueTime += (currentTime - nextTask->arrival_time); //remember how long this task was waiting in queue
	    printf("Process %d beginning output at time %d \n", nextTask->process_id, currentTime);//print out what time this task was waiting for output
		nextTask->state = Outputting;//set its state to outputting
		nextTask->arrival_time = currentTime + nextTask->io_time;//set its arrival time by how long it needs to output plus the current time
		nextTask->totalTime += nextTask->io_time;//increment the total time it has spent in the simulation by the amount of time it needs to output
		add_event(nextTask);//add it back to the event list
	  }
	  break;//end outputting case
  }//end switch statement
}
