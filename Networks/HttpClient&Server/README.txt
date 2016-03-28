Qiaoyu Liao
qliao@wpi.edu
CS3516 Project 1
----------------------
Command Line Argument
----------------------
Compile both the server and client:
	make


Run the client:
	./httpclient [-options] server_url port_number

[-options]
	-p: prints the round-trip time for accessing the URL ont he terminal before server's response

Example:
	./httpclient -p www.cnn.com 80
	./httpclient www.oracle.com/downloads/index.html 80


Run the server:
	./httpserver port_number

Example:
	./httpserver 5721
	./httpserver 6849

--------------------------
File list
--------------------------
httpclient.c
httpserver.c
network.h
TMDG.html
index.html
makefile
README.txt
RTT.pdf

------------------------
The client can get a file from the an actual web server, and print it on the terminal
	example:
		www.cnn.com
		www.oracle.com/downloads/index.html


The server can send out the TDMG.html file when:
	1. the client request the TDMG.html file
	2. the client request the index.html file
	3. the client not requesting any file using request like "GET / HTTP1.1"

The browser should be type in command like:
	server_url:port_number
	server_url:port_number/TMDG.html
or	server_url:port_number/index.html

	example:
	130.215.223.229:7234/TMDG.html
Or send out the "HTTP1.1 404 NOT FOUND" message when the page is not found
And the server should be mutlithread since it is using fork to handle the requests
