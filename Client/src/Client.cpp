#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
# include <iostream>
using namespace std;

struct timeval tp;

double getTime_usec() {
	gettimeofday(&tp, NULL);
	return static_cast<double>(tp.tv_sec) * 1E6 + static_cast<double>(tp.tv_usec);
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void *sendRequest(void *argument) {
	char *file_name = ((char *) argument);
	printf("e43");
    char *buffer;
    char ack[1];
    int ack_flag;
    long file_size = 1000000; // take this from file in folder
    long blocksize = 64000;
    long iteration = file_size/blocksize;
    int i;
	//sockfd = *((int *) argument);
//    sprintf(filename, "%s%d", base, number);
    char send_size[15];
    bzero(send_size,15);
    sprintf(send_size,"%li",file_size);
	// set port number manually
	int portno = 2001;
    int sockfd,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    // set server address manually. later u have to change this
    server = gethostbyname("localhost");//216.47.152.77
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");


    // set a buffer to send.
	buffer = (char*) malloc(sizeof(char)*file_size);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	memset(buffer,'5',file_size);


		n= send(sockfd, send_size, 30, 0);
	    if (n < 0)
	         error("send size");
	    for (i = 0; i < iteration; ++i) {
			n= send(sockfd, buffer+(blocksize*i), blocksize, 0);
		    cout<< i<<endl;
			if (n < 0)
		         error("send file");

		}

		n = read(sockfd,ack,1);
	    if (n < 0)
	         error("receive ack:");
	    ack_flag = atoi(ack);
	    if (ack_flag == 1){
	        close(sockfd);
	    } else{
	    	n= send(sockfd, send_size, 30, 0);
	    		    if (n < 0)
	    		         error("send size again:");
	    		    for (i = 0; i < iteration; ++i) {
	    				n= send(sockfd, buffer+(blocksize*i), blocksize, 0);
	    			    if (n < 0)
	    			         error("send file again:");
	    			}
	    }

	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char *argv[]) {
	char * file_name = (char *) argv[1];
	int rc, i;
	i=21;
	pthread_t threads[1];

		rc = pthread_create(&threads[0], NULL, sendRequest, (void *) &file_name);
		assert(0 == rc);

	pthread_exit(NULL);

    return 0;
}
