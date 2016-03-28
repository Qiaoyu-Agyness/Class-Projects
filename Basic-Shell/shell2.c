/*
*Ying LU ylu6@wpi.edu
*Qiaoyu Liao qliao@wpi.edu
*/

#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

//struct definition for tracking background processes
typedef struct task {
	int num;
	pid_t pid;
	char* com_arg;
	long long stime;
	struct task *next;
}Task;


int main(int argc, char* argv[]){
	//initiation
	pid_t pid;
	int status;
	struct timeval strTime, endTime;
	struct rusage strCPU,endCPU,c_usage;
	long long sysCPU, usrCPU;
	long elapse,taskFork;
	int maxsize = 128;
	int memoryread;
    int loopcounter = 0;
    int TRUE = 1;
    long long uCPU[2]={0,0};
    long stat[4]={0,0,0,0};
    int bgFlag=0;
    int jobNum=1;
    long forkTime;
    Task* tail=malloc(sizeof(Task));
    tail=NULL;
    Task* head=malloc(sizeof(Task));
    head=NULL;
    Task* prevPtr=malloc(sizeof(Task));
    prevPtr=NULL;
    Task* currPtr=malloc(sizeof(Task));
    currPtr=NULL;

    //prompt
	printf("\n==>");
	
	while(TRUE){

        if( loopcounter < 32){ //if # of command lines less than 32
			char command[128];
			char *temp;

			fgets(command,maxsize,stdin);

			if(command){//if succeeded in fetching a line
                int n = 128;
                int i;
                for(i = 0; i < 128; i++){
                    if(command[i] == '\0'){
                        n = i - 1;
                        break;
                    }
                }
                
                if(command[n-1] == '&'){
                    bgFlag=1;
                    command[n-1]='\0';
                }
                else{
                    bgFlag=0;
                }
                
        		loopcounter++;
	        	char* cline[100] = {};
    	    	int c = 0;
       			cline[c] =(char*)strtok(command," \n");
        		
        		char *bg="&";
      			while (cline[c] != NULL){ //break into tokens
         		    c++;
          		    cline[c] =(char*)strtok(NULL," \n");
        		}



        		char *shell_exit ="exit";
        		char *shell_jobs ="jobs";
        		char *shell_cd ="cd";        		

        		//exit case
        		if(strcmp(cline[0],shell_exit) == 0){
            		//if tail is NULL -- empty list
            		if(head==NULL){
            			exit(0);
            		}
            		else{

            			printf("\n Waiting for background processes to finish.");

            			while(head){
            				wait4(head->pid,&status,WNOHANG,&c_usage);

							//move onto the next task
            				head=head->next;
            			}
            			printf("\n Now exit\n");
            			exit(0);
            			
            		}
        		}

				//jobs case
  			    else if(strcmp(cline[0],shell_jobs)==0){ 
  			    	Task *temp=malloc(sizeof(Task));
  			    	temp=head;
  			    	if(!temp){
  			    		printf("\n list is empty.");
  			    	}
  			    	//if not empty list
  			    	else{
  			    		int i=0;
  			    		//printf("\n head pid %d, tail pid %d", head->pid, tail->pid);
  			    		do{
	  			    		printf("\n[%d] %d   		%s",temp->num,temp->pid,temp->com_arg);
	  			    		temp=temp->next;
	  			    	}while(temp);//end of while
	  			    	free(temp);
  			    	}
      			}

				//cd case
  			    else if(strcmp(cline[0],shell_cd)==0){ 
            
    		        if(chdir(cline[1]) == -1){
    		            printf("path is incorrect \n");
     		        }
      			}
				//other commands        
  			    else{

					pid=fork();
					//get start time
					gettimeofday(&strTime, NULL);
					forkTime=(strTime.tv_sec*1000000+strTime.tv_usec)/1000;

					// if is child
					if(pid==0){
					//command in the first field, array excluding ./runCommand in the second field

						if(execvp(cline[0], cline)==-1){
							printf("illegal command\n");
							return 0;
						}

					}

					//if is parent
					else if (pid>0){ 

						printf("pid=%d \n",pid);
						//if not background, waits
						if(!bgFlag){
							int wait = wait3(&status, 0, &c_usage);
							while(wait!=pid&&wait!=-1){
								//printf("child %d finishes\n",wait);	
								if(tail!=NULL){
									if(head->pid==wait){
										//printf("\nhead task dropped pid= %d",head->pid);
																				
										head=head->next;
									}
									else{
										prevPtr=head;
										currPtr=head->next; //move onto the next node
										//find the finished task in thelist
										while(currPtr!=NULL &&currPtr->pid!=wait){
											prevPtr=currPtr;
											currPtr=currPtr->next;
										}

										//delete the found node
										if(currPtr!=NULL){
											if(tail==currPtr){
												tail=prevPtr;
											}
											//printf("\n task dropped pid= %d",currPtr->pid);
											taskFork=currPtr->stime;
											prevPtr->next=currPtr->next;
										}
									}			
								}
								//print rusage stats
								printf("system cpu time is %ld millisends.\n",c_usage.ru_stime.tv_sec*1000+c_usage.ru_stime.tv_usec/1000);
								printf("user cpu time is %ld millisends.\n",c_usage.ru_utime.tv_sec*1000+c_usage.ru_utime.tv_usec/1000);
								printf("soft page fault is %ld \n hard page fault is %ld \n",c_usage.ru_minflt,c_usage.ru_majflt);
								printf("involuntarily context switched %ld times\n voluntarily context switched %ld times\n",c_usage.ru_nivcsw,c_usage.ru_nvcsw);

								//jumps out of the loop when no child finishes lately
								gettimeofday(&endTime, NULL);
								//getting wall-clock
								elapse=((endTime.tv_sec*1000000+endTime.tv_usec)-(strTime.tv_sec*1000000+strTime.tv_usec))/1000;
								//elapse=(endTime.tv_sec*1000000+endTime.tv_usec)-taskFork;
								//output
								printf("Time elapsed %ld milliseconds.\n",elapse);
								wait = wait3(&status, 0, &c_usage);
								
						    }
					    	//print rusage stats
					    	printf("child %d finishes\n",wait);
							printf("system cpu time is %ld millisends.\n",c_usage.ru_stime.tv_sec*1000+c_usage.ru_stime.tv_usec/1000);
							printf("user cpu time is %ld millisends.\n",c_usage.ru_utime.tv_sec*1000+c_usage.ru_utime.tv_usec/1000);
							printf("soft page fault is %ld \n hard page fault is %ld \n",c_usage.ru_minflt,c_usage.ru_majflt);
							printf("involuntarily context switched %ld times\n voluntarily context switched %ld times\n",c_usage.ru_nivcsw,c_usage.ru_nvcsw);
							//jumps out of the loop when no child finishes lately
							gettimeofday(&endTime, NULL);
							//getting wall-clock
							elapse=((endTime.tv_sec*1000000+endTime.tv_usec)-(strTime.tv_sec*1000000+strTime.tv_usec))/1000;
							//elapse=(endTime.tv_sec*1000000+endTime.tv_usec)-taskFork;
							//output
							printf("Time elapsed %ld milliseconds.\n",elapse);
						    int err=wait3(&status,WNOHANG,&c_usage);

							while(err!=-1 && err!=0){
								//printf("child %d finishes\n",pid);
								//taken the task off task list
								if(tail!=NULL){
									if(head->pid==err){
										//printf("\nhead task dropped pid= %d",head->pid);
										taskFork=head->stime;
										head=head->next;
									}
									else{
										prevPtr=head;
										currPtr=head->next; //move onto the next node
										//find the finished task in thelist
										while(currPtr!=NULL &&currPtr->pid!=err){
											prevPtr=currPtr;
											currPtr=currPtr->next;
										}

										//delete the found node
										if(currPtr!=NULL){
											if(tail==currPtr){
												tail=prevPtr;
											}
											//printf("\n task dropped pid= %d",currPtr->pid);
											taskFork=currPtr->stime;
											prevPtr->next=currPtr->next;
										}
									}
								}
								//print rusage stats
								printf("child %d finishes\n",err);
								printf("system cpu time is %ld millisends.\n",c_usage.ru_stime.tv_sec*1000+c_usage.ru_stime.tv_usec/1000);
								printf("user cpu time is %ld millisends.\n",c_usage.ru_utime.tv_sec*1000+c_usage.ru_utime.tv_usec/1000);
								printf("soft page fault is %ld \n hard page fault is %ld \n",c_usage.ru_minflt,c_usage.ru_majflt);
								printf("involuntarily context switched %ld times\n voluntarily context switched %ld times\n",c_usage.ru_nivcsw,c_usage.ru_nvcsw);
								//jumps out of the loop when no child finishes lately
								gettimeofday(&endTime, NULL);
								//getting wall-clock
								elapse=((endTime.tv_sec*1000000+endTime.tv_usec)-(strTime.tv_sec*1000000+strTime.tv_usec))/1000;
								//elapse=(endTime.tv_sec*1000000+endTime.tv_usec)-taskFork;
								//output
								printf("Time elapsed %ld milliseconds.\n",elapse);
						        err=wait3(&status,WNOHANG,&c_usage);
							    
							}
							
						} 
						//int test=wait3(&status,WNOHANG,&c_usage);
						
						//if background
						else{
							//create new task
							Task* newTask=malloc(sizeof(Task));
							newTask->num=jobNum;
							newTask->pid=pid;
							newTask->com_arg=cline[0];
							newTask->stime=forkTime;
							newTask->next=NULL;
							jobNum++;
							//if task list is empty
							if(tail==NULL && head==NULL){
								//set head and tail to newtask
								//printf("\n a new task added pid= %d",newTask->pid);
								tail=newTask;
								head=newTask;
							}
							else{
								tail->next=newTask;
								tail=newTask;
								//printf("\n new task added pid= %d",newTask->pid);
							}
							//when one of the children finishes
							int getPid=wait3(&status,WNOHANG,&c_usage);
							while(getPid){

								//printf("child %d finishes\n",getPid);
								//taken the task off task list
								if(tail!=NULL){
									if(head->pid==getPid){
										//printf("\nhead task dropped pid= %d",head->pid);
										taskFork=head->stime;
										head=head->next;
									}
									else{
										prevPtr=head;
										currPtr=head->next; //move onto the next node
										//find the finished task in thelist
										while(currPtr!=NULL &&currPtr->pid!=getPid){
											prevPtr=currPtr;
											currPtr=currPtr->next;
										}

										//delete the found node
										if(currPtr!=NULL){
											if(tail==currPtr){
												tail=prevPtr;
											}
											//printf("\n task dropped pid= %d",currPtr->pid);
											taskFork=currPtr->stime;
											prevPtr->next=currPtr->next;
										}
									}
								}

								//print rusage stats
								printf("system cpu time is %ld millisends.\n",c_usage.ru_stime.tv_sec*1000+c_usage.ru_stime.tv_usec/1000);
								printf("user cpu time is %ld millisends.\n",c_usage.ru_utime.tv_sec*1000+c_usage.ru_utime.tv_usec/1000);
								printf("soft page fault is %ld \n hard page fault is %ld \n",c_usage.ru_minflt,c_usage.ru_majflt);
								printf("involuntarily context switched %ld times\n voluntarily context switched %ld times\n",c_usage.ru_nivcsw,c_usage.ru_nvcsw);
								
								//jumps out of the loop when no child finishes lately
								gettimeofday(&endTime, NULL);
								//getting wall-clock
								elapse=((endTime.tv_sec*1000000+endTime.tv_usec)-(strTime.tv_sec*1000000+strTime.tv_usec))/1000;
								//output
								printf("Time elapsed %ld milliseconds.\n",elapse);								

					        	getPid=wait3(&status,WNOHANG,&c_usage);
							}//end of while


						
						}
					}
					//if failure
					else if(pid<0){
						printf("error\n");
					}
						//elapse=(endTime.tv_sec*1000000+endTime.tv_usec)-taskFork;

		        }//end of else
		    }//end of  if command != 1

			
        }//end of if loopcounter <32
					        
        else{
            printf("ERROR, number of commands exceeds 32 \n");
            TRUE = 0;
        }//end of else

		printf("\n\n==>");       
	}//end of while
	return 0;

}

