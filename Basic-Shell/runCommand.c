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


//#define TRUE 1


int main(int argc, char* argv[]){
	//initiation
	pid_t pid;
	int status;
	struct timeval strTime, endTime;
	int who=RUSAGE_SELF;
	struct rusage strCPU,endCPU;
	long long sysCPU, usrCPU;
	long long elapse;
//	long softPageFault, hardPageFault, vcsw, ivcsw;

	//get start time
	gettimeofday(&strTime, NULL);
	//getrusage(who,&strCPU);

	pid=fork();
	
	//while(TRUE){
	
	// if is child
	if(pid==0){
	//command in the first field, array excluding ./runCommand in the second field
		int a =0;
						while(argv[a]){
							printf("%s",argv[a]);
							printf("EOF");
							a++;
						}
		if(execvp(argv[1], argv+1)==-1){
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
	sysCPU=((endCPU.ru_stime.tv_sec*1000000+endCPU.ru_stime.tv_usec)-(strCPU.ru_stime.tv_sec*1000000+strCPU.ru_stime.tv_usec))/1000;
	usrCPU=((endCPU.ru_utime.tv_sec*1000000+endCPU.ru_utime.tv_usec)-(strCPU.ru_utime.tv_sec*1000000+strCPU.ru_utime.tv_usec))/1000;

	//output
	printf("Time elapsed %lld milliseconds.\n",elapse);
	//getrusage
	printf("system cpu time is %lld millisends.\n",sysCPU);
	printf("user cpu time is %lld millisends.\n",usrCPU);
	printf("soft page fault is %ld \n hard page fault is %ld \n",endCPU.ru_minflt,endCPU.ru_majflt);
	printf("involuntarily context switched %ld times\n voluntarily context switched %ld times\n",endCPU.ru_nivcsw,endCPU.ru_nvcsw);
	return 0;

}

