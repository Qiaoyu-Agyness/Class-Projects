/* The server */
/* author Qiaoyu Liao 
    qliao@wpi.edu 
    */
#include "network.h"

void sigchld_handler(int s);
void *get_in_addr(struct sockaddr *sa);
void parse_request(char *buf, int buflen, char *file_name);
void processing(int sock);



/* the main server function */ 
int main(int argc, char *argv[]){
	int rv, sockfd, new_fd;
	int yes=1;
	struct addrinfo hints;
	struct addrinfo *servinfo, *p;  // will point to the results
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
	char s[INET6_ADDRSTRLEN];
	char port[HOST_LENGTH];

	/* check if the input is right */
	if(argc != 2){
		printf("Wrong command format!\n Example ./httpclient 1234\n");
		return 1;
	}
	/* copy the port number */
	strcpy(port,argv[1]);

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me


    /* if not getting the address info */
	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));
    	return 1;
	}



	/* loop through the result to find the first one to bind */
	for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    /* all done with this structure */
    freeaddrinfo(servinfo); 
	
    /* if no addressinfo found */
	if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 1;
    }



    /* listen to the socket */
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        return 1;
    }


    /* to catch zoombie running process */
    /* reap all dead processes */
    sa.sa_handler = sigchld_handler; 
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }



    printf("server: waiting for connections...\n");


    /*  main accept loop */
    while(1) {  
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);


        int pid = fork();

        /* if the fork id is <0 */
        if(pid < 0){
        	perror("ERROR on fork");
            close(sockfd);
        	return 1;
        }

        /* if sucessfully forked */
        if(pid ==0){
        	/*this is the client process */
        	close(sockfd);
        	processing(new_fd);
        	return 0;
        }

        else{
        	close(new_fd);
        }
    }

    return 0;
}/* end of main */




/* child signal handler */
void sigchld_handler(int s){
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}/* end of sigchild_handler */



/*  get socket address , IPv4 or IPv6 */
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}/* end of get_in_addr */




/* parse the html1.1 request */
void parse_request(char *buf, int buflen, char *file_name){
    int length = 0;
    char *begin, *end;

    /* search for the place the file name start */
    begin = strchr(buf, '/');
    begin += 1;

    /* search for the place the file name end */
    end = strchr(begin, ' ');
    *end = 0;

    /* get the file name */
    length = end - begin;


    if(length > 0){
        memcpy(file_name, begin, length);
        file_name[length] = 0;
        printf("The file name is:%s\n",file_name);
    }
    /* if no file name, then fetch index.html */
    else if(length == 0){
        printf("No file name, autmatically fetch %s\n", file_name);
    }

    /* just to catch whichever left, put the error message in the file name */
    else{
        strcpy(file_name,"error when reading filename");
    }


    return;
}/* end of parse_request */


/* process the socket connection with single client */
void processing(int sock){
    int n, file_len, hdr_len, send_len, read_len;
    char buf[BUF_SIZE], header[HOST_LENGTH],file_name[HOST_LENGTH] = "index.html";
    char read_buf[BUF_SIZE];
    bzero(buf,BUF_SIZE);
    FILE *res_file;


    /* receive the message from the client */
    n = recv(sock,buf,BUF_SIZE,0);
    if(n <= 0){
        perror("ERROR reading from socket");
        close(sock);
        exit(1);
    }


    /* print out the message */
    printf("Here is the message:%s\n",buf);
    /* parse the request to get the filename */
    parse_request(buf,n,file_name);


    /* open the request file */
    res_file = fopen(file_name,"r");
    /* if no file opened */
    if(res_file == NULL){
        /* send the 404 not found message */
        send(sock,"HTTP/1.1 404 NOT FOUND\r\n",26,0);
        printf("HTTP/1.1 404 NOT FOUND\r\n");
        close(sock);
        return;
    }


    /* find the length of the file */
    fseek(res_file,0,SEEK_END);
    file_len = ftell(res_file);
    fseek(res_file,0,SEEK_SET);


    /* construct the HTTP header */
    hdr_len = sprintf(header, "HTTP/1.1 200 OK\r\n"
        "Server: Qiaoyu's Server <1.0>\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Length: %d\r\n"
        "Accept-Ranges: bytes\r\n"
        "Connection: close\r\n\r\n"
   , file_len);
    send_len = send(sock, header, hdr_len,0);


    /* if the sending message length is less than 1 */
    if(send_len <= 1){
        /* close the reading document */
        fclose(res_file);
        /* close the socket */
        close(sock);
        return;
    }


    /* while still reading from the file and still something left in the file */
    do{
        //read from the file
        read_len = fread(read_buf, sizeof(char), BUF_SIZE,res_file);
        //send out what the buffer just read
        if(read_len > 0){
            send_len = send(sock, read_buf, read_len,0);
            file_len -= read_len;
        }
    }while((read_len>0)&&(file_len>0));

    /* close the reading document */
    fclose(res_file);

    /* close the socket */
    close(sock);
    return;

}  /* end of processing */











