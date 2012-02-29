/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <assert.h>
#include <pthread.h>
#include <fstream>

using namespace std;
struct timeval tp;
typedef struct _server_struct
{
	int newsockfd;
	long bsize;
} server_struct;
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void *handleRequest(void *argument) {
	server_struct struc = *((server_struct*) argument);
	int newsockfd = struc.newsockfd;
	long i,recvd_size, iteration;
	long blocksize = 64000;
    char size_buffer[30];
    char* file_buffer;
    long n =1;



	bzero(size_buffer,30);

    cout<< "error!"<<endl;
			n = read(newsockfd,size_buffer,30);
			// if it's size, then you have to send fail
			if (n < 0) error("ERROR reading from socket");
			printf("Here is the message:%s\n",size_buffer);
			printf("Here is the message:%li\n",n);
			recvd_size = atol(size_buffer);
			printf("Here is the message:%li\n",recvd_size);
			file_buffer = (char*) malloc(sizeof(char)*recvd_size);
			bzero(file_buffer,recvd_size);
			iteration = recvd_size/blocksize;
			for (i = 0; i < iteration; ++i) {
				n = read(newsockfd,file_buffer +(i*blocksize),blocksize);
			    cout<< i<<endl;
				// if it's size, then you have to send fail
			}
			//send success ack
			n= send(newsockfd, "1", blocksize, 0);



    close(newsockfd);

	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char *argv[]) {
//	long blocksize = atol(argv[1]);
	server_struct struc[100];
	pthread_t threads[100];
	int thread_args[100];
	int rc, i;

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;


     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi("2001");
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     i = 0;

     //
     while (1){
         clilen = sizeof(cli_addr);
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
              error("ERROR on accept");
    	 struc[i].newsockfd = newsockfd;
    	 struc[i].bsize = 0;// changed to something. not used anymore
 		rc = pthread_create(&threads[i], NULL, handleRequest, (void *) &struc[i]);
 		assert(0 == rc);
        i++;
     }
     printf("after while loop");
     close(sockfd);
     return 0;
}
