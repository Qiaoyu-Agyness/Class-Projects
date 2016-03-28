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





int main(int argc, char* argv[]){
	//initiation
	pid_t pid;
	int status;
	struct timeval strTime, endTime;
	int who=RUSAGE_SELF;
	struct rusage strCPU,endCPU;
	long long sysCPU, usrCPU;
	long long elapse;
	int maxsize = 128;
	int memoryread;
    int loopcounter = 0;
    int TRUE = 1;

	while(TRUE){

        if( loopcounter < 32){ //if # of command lines less than 32
			char command[128];
			char *temp;
			printf("\n==>");
			fgets(command,maxsize,stdin);
			//gstring initialization ???*************************
        

			if(command){//if succeeded in fetching a line
        		loopcounter++;
	        	char* cline[100];
    	    	int c = 0;
       			cline[c] =(char*)strtok(command," \n");
        
      			while (cline[c] != NULL){ //break into tokens
					//cline[c]!=EOF ???***************************
         		  //  printf("%s",cline[c]);
         		    c++;
          		    cline[c] =(char*)strtok(NULL," \n");
          		    //c++;
        		}
        		//cline[c]=(char*)NULL;

        		char *shell_exit ="exit";
        		char *shell_cd ="cd";
        		//exit case
        		if(strcmp(cline[0],shell_exit) == 0){
            		exit(0);
        		}

				//cd case
  			    else if(strcmp(cline[0],shell_cd)==0){ 
            
    		        if(chdir(cline[1]) == -1){
    		            printf("path is incorrect \n");
     		        }
      			}

				//other commands        
  			    else{
					//get start time
					gettimeofday(&strTime, NULL);
					//getrusage(who,&strCPU);

					pid=fork();
						
					// if is child
					if(pid==0){
					//command in the first field, array excluding ./runCommand in the second field
						/*int a =0;
						while(cline[a]){
							printf("%s",cline[a]);
							printf("EOF");
							a++;
						}*/
						if(execvp(cline[0], cline)==-1){
							printf("illegal command\n");
							return 0;
						}

					}

					//if is parent
					else if (pid>0){ 
						waitpid(-1,&status,0);

						//get end time
						gettimeofday(&endTime, NULL);
						getrusage(RUSAGE_CHILDREN,&endCPU);
					}
					
					//if failure
					else if(pid<0){
						printf("error\n");
					}
					
					//getting wall-clock
					elapse=((endTime.tv_sec*1000000+endTime.tv_usec)-(strTime.tv_sec*1000000+endTime.tv_usec))/1000;
					//getting CPU time
					sysCPU=(endCPU.ru_stime.tv_sec*1000000+endCPU.ru_stime.tv_usec)/1000;
					usrCPU=(endCPU.ru_utime.tv_sec*1000000+endCPU.ru_utime.tv_usec)/1000;

					//output
					printf("Time elapsed %lld milliseconds.\n",elapse);
					//getrusage
					printf("system cpu time is %lld millisends.\n",sysCPU);
					printf("user cpu time is %lld millisends.\n",usrCPU);
					printf("soft page fault is %ld \n hard page fault is %ld \n",endCPU.ru_minflt,endCPU.ru_majflt);
					printf("involuntarily context switched %ld times\n voluntarily context switched %ld times\n",endCPU.ru_nivcsw,endCPU.ru_nvcsw);
		        }//end of else
		    }//end of  if command != 1
        }//end of if loopcounter <32
					        
        else{
            printf("ERROR, number of commands exceeds 32 \n");
            TRUE = 0;
        }//end of else
		        
	}//end of while
	return 0;

}

