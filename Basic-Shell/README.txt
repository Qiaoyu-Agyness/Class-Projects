

************************************
Ying Lu <ylu6@wpi.edu>
Qiaoyu Liao <qliao@wpi.edu>

*************************************
This project is to implement a basic shell.
It contains three phases:
	1. single command
	2. basic shell
	3. command shell with background execution
	
run 'make' to compile three phases.
Below also includes our data.

===================PHASE 1=====================
run 'make runCommand' to compile only this phase

to execute, type <./runCommand [command]>, where
you substitute the bracket with desired command.

e.g.
./runCommand ls

this will return all the files in the current 
directory. 

runCommand will only execute one command every
execution.


===================PHASE 2=====================
run 'make shell' to compile only this phase

to execute, type <./shell >, then follow the prompts

e.g. 
./shell
==>ls
/* shows files in current directory */

==>pwd 
/* shows current location */

shell allows user to execute multiple commands at one execution
of shell.

test2.txt provides more detailed illustration

The result from ./shell <test2.txt is as follows:
-------------------------------------------------------
yl@yl-VirtualBox:~/proj1$ ./shell <test2.txt

aa	    Makefile~	 runCommand	shell2	    shell-a.c  shell-r	   testhandle.c
aa~	    README~	 runCommand.c	shell2.c    shell.c    test1.txt   testproj1.zip
file2.txt~  README.txt	 runCommand.c~	shell2-r    shell.c~   test1.txt~  test.txt
Makefile    README.txt~  shell		shell2-r.c  shellq.c   test2.txt   test.txt~
==>Time elapsed 0 milliseconds.
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 265 
 hard page fault is 0 
involuntarily context switched 32 times
 voluntarily context switched 1 times

==>Time elapsed 0 milliseconds.
system cpu time is 0 millisends.
user cpu time is 12 millisends.
soft page fault is 1934 
 hard page fault is 0 
involuntarily context switched 80 times
 voluntarily context switched 18 times

/home/yl/proj1
==>Time elapsed 0 milliseconds.
system cpu time is 0 millisends.
user cpu time is 12 millisends.
soft page fault is 2123 
 hard page fault is 0 
involuntarily context switched 82 times
 voluntarily context switched 19 times

==>
-------------------------------------------------------


===================PHASE 3======================
run 'make shell2' to compile only this phase

to execute, type <./shell2>, then follow the prompts

based on shell, shell2 allows user to run background processes.
the processes information will be stored in a linked list, ordered
by their fork time. if a process 4 ends, the upcoming process will 
be number 5, not replacing the finished process. The process 4 will
then be deleted from the linked list, by parsing the list and finding 
its information.

test.txt, testexit.txt, test32.txt, test128.txt provides more detailed illustration

The result from ./shell <test2.txt is as follows:
explainations are written in /*...*/
--------------------------------------------------------
yl@yl-VirtualBox:~/proj1$ ./shell2 <test.txt

==>pid=4145 					/* sleep for 1 sec, background test */


==>pid=4146 					/* ls--blocking test for background tasks */
aa	    Makefile~	README.txt~    shell	   shell2-r   shellq.c	  test2.txt	 test.txt
aa~	    Makefile2	runCommand     shell2	   shell-a.c  shell-r	  testexit.txt	 test.txt~
file2.txt~  README~	runCommand.c   shell2-a.c  shell.c    test1.txt   testhandle.c
Makefile    README.txt	runCommand.c~  shell2.c    shell.c~   test1.txt~  testproj1.zip
child 4146 finishes				/* rusage stats output */
system cpu time is 4 millisends.
user cpu time is 0 millisends.
soft page fault is 267 
 hard page fault is 0 
involuntarily context switched 34 times
 voluntarily context switched 1 times
Time elapsed 9 milliseconds.


==>						/* jobs--show current background task */
[1] 4145   		jobs

==>pid=4147 					/* illegal command test */
==>illegal command
child 4147 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 56 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 1 milliseconds.


==>						/* cd test */

==>pid=4148 					/* pwd to see if cd suceeds */
/home/yl
child 4148 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 190 
 hard page fault is 0 
involuntarily context switched 3 times
 voluntarily context switched 1 times
Time elapsed 9 milliseconds.


==>path is incorrect 				/* incorrect cd path test */


==>pid=4149 					/* start a second background task */


==>						/* see if background task info is stored */
[1] 4145   		jobs
[2] 4149   		jobs

==>pid=4150 				/* execute a new command to pick up former background tasks */
system cpu time is 0 millisends.		/* outputs for finished background tasks */
user cpu time is 0 millisends.
soft page fault is 195 
 hard page fault is 0 
involuntarily context switched 0 times
 voluntarily context switched 2 times
Time elapsed 983 milliseconds.
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 193 
 hard page fault is 0 
involuntarily context switched 1 times
 voluntarily context switched 2 times
Time elapsed 4010 milliseconds.
child 4150 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 194 
 hard page fault is 0 
involuntarily context switched 0 times
 voluntarily context switched 2 times
Time elapsed 5002 milliseconds.


==>						/* empty background task list test */
 list is empty.

==>pid=4151 					/* foreground test */
code.c~  Documents  Dropbox	      hello.c~	 Music	   proj0  Public     test
Desktop  Downloads  examples.desktop  Makefile~  Pictures  proj1  Templates  Videos
child 4151 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 266 
 hard page fault is 0 
involuntarily context switched 0 times
 voluntarily context switched 1 times
Time elapsed 1 milliseconds.


==>						/* jobs list test */
 list is empty.
						/* exit test -- all background tasks are finished */
----------------------------------------------------------


The result from ./shell <test2.txt is as follows:
explainations are written in /*...*/
--------------------------------------------------------
yl@yl-VirtualBox:~/proj1$ ./shell2 <testexit.txt

==>pid=4191 					/* start first background task */


==>pid=4192 					/* start second background task */


==>						/* jobs list test */
[1] 4191   		jobs
[2] 4192   		jobs

==>						/* exit on background tasks still running */
 Waiting for background processes to finish.
 Now exit
yl@yl-VirtualBox:~/proj1$ aa	    Makefile2	 runCommand.c	shell2.c   shellq.c    testexit.txt   test.txt~
aa~	    README~	 runCommand.c~	shell2-r   shell-r     testexit.txt~
file2.txt~  README.txt	 shell		shell-a.c  test1.txt   testhandle.c
Makefile    README.txt~  shell2		shell.c    test1.txt~  testproj1.zip
Makefile~   runCommand	 shell2-a.c	shell.c~   test2.txt   test.txt		/* outputs from ls */

--------------------------------------------------------

The result from ./shell <test32.txt is as follows:
explainations are written in /*...*/
--------------------------------------------------------
yl@yl-VirtualBox:~/proj1$ ./shell2 <test32.txt

==>pid=4229 
==>illegal command
child 4229 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 56 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4230 
No manual entry for ls
child 4230 finishes
system cpu time is 4 millisends.
user cpu time is 8 millisends.
soft page fault is 1343 
 hard page fault is 0 
involuntarily context switched 7 times
 voluntarily context switched 2 times
Time elapsed 59 milliseconds.


==>pid=4231 
==>illegal command
child 4231 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4232 
==>illegal command
child 4232 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4233 
==>illegal command
child 4233 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4234 
==>illegal command
child 4234 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4235 
==>illegal command
child 4235 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4236 
==>illegal command
child 4236 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4237 
==>illegal command
child 4237 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4238 
==>illegal command
child 4238 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4239 
: No such file or directory
child 4239 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 206 
 hard page fault is 0 
involuntarily context switched 5 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4240 
==>illegal command
child 4240 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4241 
==>illegal command
child 4241 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4242 
==>illegal command
child 4242 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4243 
==>illegal command
child 4243 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4244 
==>illegal command
child 4244 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 2 milliseconds.


==>pid=4245 
==>illegal command
child 4245 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4246 
==>illegal command
child 4246 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4247 
==>illegal command
child 4247 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4248 
==>illegal command
child 4248 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4249 
==>illegal command
child 4249 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4250 
==>illegal command
child 4250 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4251 
==>illegal command
child 4251 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4252 
==>illegal command
child 4252 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4253 
==>illegal command
child 4253 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4254 
==>illegal command
child 4254 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4255 
==>illegal command
child 4255 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4256 
==>illegal command
child 4256 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4257 
==>illegal command
child 4257 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4258 
==>illegal command
child 4258 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 2 milliseconds.


==>pid=4259 
==>illegal command
child 4259 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>pid=4260 
==>illegal command
child 4260 finishes
system cpu time is 0 millisends.
user cpu time is 0 millisends.
soft page fault is 55 
 hard page fault is 0 
involuntarily context switched 2 times
 voluntarily context switched 1 times
Time elapsed 0 milliseconds.


==>ERROR, number of commands exceeds 32 	/* test for too many inputs (>32 commands) */
						/* no need to test for >128 characters case */
						/* as there's upper bound for fgets */
-----------------------------------------------------

end of README
thank you for your time! ^^
***********************************************************

