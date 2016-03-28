/* The clienet */
/* author: Qiaoyu Liao 
    qliao@wpi.edu
    */
#include "network.h"
#include <sys/time.h>

void parse_url(char *buf, char *host, char *file_name);
int hostname_to_ip(char *hostname , char *ip);
int recv_timeout(int s , int timeout);



/* main function of the http client */
int main(int argc, char *argv[]){
	int result, sock = 0;
	int rtt_flag=0;
	struct sockaddr_in serv_addr;
	int port;	
	char message[BUF_SIZE];
	char host[HOST_LENGTH] = "127.0.0.1";
	char addr[HOST_LENGTH];
	char file_name[HOST_LENGTH];
	struct timeval before , after;
	double timediff;

    /* if it is not the right argument numbers */
	if(argc != 3 && argc != 4){
		fprintf(stderr,"wrong input, try again\n");
		return 1;
	}


	/* if the command wihtout options -p */
	if(argc == 3){
		/* take the third arg string as port number */
		port=atoi(argv[2]);
		printf("The port number is %d\n", port);

		/* check the first letter of the input string */
		/* if it is a number, then treat it as ip address */
		if(isdigit(*argv[1])){
            /* seperate the ip address and file name */
            parse_url(argv[1],host,file_name);
			strcpy(addr,host);
		}

		/* if not, parse the string */
		else if(isalpha(*argv[1])){
            /* seperate the domain name and file name */
			parse_url(argv[1],host,file_name);
			printf("The host is %s\n", host);
			hostname_to_ip(host,addr);
		}	

		printf("The address is %s\n", addr);
	}/* end of if */



    /* if the command is with the -p option */
	if(argc == 4){

        /* if the option is not -p */
		if(strcmp(argv[1],"-p") != 0){
			printf("invalid input in the options\n");
		}

        /* set the rtt_flag to 1 */
		rtt_flag = 1;

		/* take the fourth arg string as port number */
		port=atoi(argv[3]);
		printf("The port number is %d\n", port);

		/* check the first letter of the input string */
		/* if it is a number, then treat it as ip address */
		if(isdigit(*argv[2])){
            /* seperate the ip address and file name */
			parse_url(argv[2],host,file_name);
            strcpy(addr,host);
		}

		/* if not, parse the string */
		else if(isalpha(*argv[2])){
            /* seperate the domain name and file name */
			parse_url(argv[2],host,file_name);
			printf("The host is %s\n", host);
			hostname_to_ip(host,addr);
		}

		printf("The address is %s\n", addr);
	}/* end of if */



	/* set up server address */
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(addr);

    /* create the socket */
    sock = socket(AF_INET, SOCK_STREAM, 0); 
    /* if the socket cannot be created */
    if(sock == -1){
        printf("Could not create the socket\n");
        return 1;
    }



    /* check the time before */
    gettimeofday(&before , NULL);

    /* connect to the server */
    result = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    /* check the time after */
    gettimeofday(&after , NULL);

    /* calculate the RTT */
    timediff = 1000 * (after.tv_sec - before.tv_sec) + 0.001 * (after.tv_usec - before.tv_usec);

    /* if not connected to the socket */
    if (result == -1) {
        close(sock);
        printf("fail to connect, error\n");
        return 1; 
    }
    printf("Connected\n");

    /* if the rtt time needed to be printed */
    if(rtt_flag == 1){
    	printf("The RTT time is %f milliseconds\n", timediff);
    }




    /* construct the HTTP/1.1 GET message */
    strcpy(message,"GET /");
    printf("%s\n",message);
    if(strcmp(file_name,"") != 0){
    	strcat(message, file_name);
	}
    strcat(message, " HTTP/1.1\r\n");
    strcat(message, "Host:");
    strcat(message, host);
    strcat(message,"\r\n\r\n");

    printf("%s\n",message);



    /* send message */
    if( send(sock , message , strlen(message) , 0) == -1)
    {
        puts("Send failed");
        return 1;
    }
    puts("Data Send\n");



    /* Receive a reply from the server */
    int total_recv = recv_timeout(sock, 2);
    printf("\n\nDone. Received a total of %d bytes\n\n" , total_recv);

    /*close the socket */ 
    close(sock);
    return 0;
}/* end of main */





/* parse the url to seperate the domain name and file name */
void parse_url(char *buf, char *host, char *file_name){
    int length = 0;
    char *buf_end = (char *)(buf + strlen(buf));
    char *begin, *host_end, *file;

    /* the beginning of the domain name should be at the start of the string */
    begin = buf;


    /* the end of the domain name is at '/'*/
    host_end = strchr(begin, '/');
    if(host_end == NULL){
        host_end = buf_end;
    }


    /* get the file name */
    else{
        /* find end of the file name at the last '/' */
        file = strrchr(host_end,'/');
        /* if the file end is not null and the character after it is not null */
        if(file && (file + 1) != buf_end)
            strcpy(file_name, file + 1);
    }


    /* get the domain name */
    length = host_end - begin;
    memcpy(host,begin,length);
    host[length] = 0;
}/* end of parse_url */



/* translate the host name url to ip address */
int hostname_to_ip(char *hostname , char *ip)
{
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
    int rv;
 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM;
 
    /* if not get the address information correctly */
    if ( (rv = getaddrinfo( hostname , "http" , &hints , &servinfo)) != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
 
    /* loop through all the results and connect to the first */
    for(p = servinfo; p != NULL; p = p->ai_next) {
        h = (struct sockaddr_in *) p->ai_addr;
        strcpy(ip , inet_ntoa( h->sin_addr ) );
    }
     
    /* all done with this structure */
    freeaddrinfo(servinfo);
    return 0;
}/* end of hostname_to_ip */



/* loop to receive  */
int recv_timeout(int s , int timeout)
{
    int size_recv , total_size= 0;
    struct timeval begin , now;
    char chunk[BUF_SIZE];
    double timediff;
     
    /* make socket non blocking*/
    fcntl(s, F_SETFL, O_NONBLOCK);
     
    /* get the beginning time */
    gettimeofday(&begin , NULL);
     
    while(1)
    {
        /* get the time right now */
        gettimeofday(&now , NULL);
         
        /* time elapsed in seconds */
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
         
        /* if you got some data, then break after timeout */
        if( total_size > 0 && timediff > timeout){
            break;
        }
         
        /* if you got no data at all, wait a little longer, twice the timeout */
        else if( timediff > timeout*2){
            break;
        }
         
        /* clear the variable */
        memset(chunk ,0 , BUF_SIZE);  
        /* if nothing was received then we want to wait a little before trying again, 0.1 seconds */
        if((size_recv =  recv(s , chunk , BUF_SIZE , 0) ) <= 0){
            usleep(100000);
        }
        /* if receive something */
        else{
            /* increment the total size */
            total_size += size_recv;
            /* print out themessage */
            printf("%s" , chunk);
            /* reset beginning time */
            gettimeofday(&begin , NULL);
        }
    }/* end of while */
     
    return total_size;
}/* end of recv_timeout */




