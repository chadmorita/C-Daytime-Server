#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int passive, session;
    struct sockaddr_in sin;
    struct sockaddr * sap = (struct sockaddr *) &sin;
    struct protoent * protocolentry;
    socklen_t adrsize;
    time_t timer;
    char buffer[27];
    struct tm* tm_info;

    // Check arguments
    if(argc < 2)
    {
      fprintf(stderr, "usage: port number\n");
      exit(1);
    }

    // Use TCP
    if((protocolentry = getprotobyname("tcp")) == NULL) {
      fprintf(stderr, "Error: getprotobyname\n");
      exit(1);
    }

    // Get a file descriptor for the TCP socket
    if((passive = socket(PF_INET, SOCK_STREAM, protocolentry->p_proto)) < 0) {
      fprintf(stderr, "Error: socket\n");
      exit(1);
    }

    // Clear everything to 0
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(atoi(argv[1]));

    // Bind to the socket
    if(bind(passive, sap, sizeof(sin)) != 0) {
      fprintf(stderr, "Error: bind\n");
      exit(1);
    }

    // Listen on the socket
    if(listen(passive, 1) < 0) {
      fprintf(stderr, "Error: listen\n");
      exit(1);
    }

    adrsize = sizeof(sin);
    // Accept connection
    if((session = accept(passive, sap, &adrsize)) < 0) {
      fprintf(stderr, "Error: accept\n");
      exit(1);
    };

    // Get the current time
    time(&timer);
    tm_info = localtime(&timer);

    // Format the time
    strftime(buffer, 27, "%Y/%m/%d %H:%M:%S\n", tm_info);

    // Send the time back to the client
    if(send(session, buffer, 27, 0) < 0) {
      fprintf(stderr, "Error: send\n");
    }

    return 0;
}
