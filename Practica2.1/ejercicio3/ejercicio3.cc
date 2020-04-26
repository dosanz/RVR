#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo* res;

    memset((void*)&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    char buffer[80];

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    int sd = socket(res->ai_family, res->ai_socktype, 0);
    bind(sd, res->ai_addr, res->ai_addrlen);

    socklen_t addrlen = sizeof(struct sockaddr);

    sendto(sd, argv[3], sizeof(argv[3]), 0, res->ai_addr, res->ai_addrlen);
    
    ssize_t bytes = recvfrom(sd, buffer, sizeof(buffer), 0, res->ai_addr, &addrlen);
    buffer[0] = '\n';

    std::cout << buffer << std::endl;

    freeaddrinfo(res);

    return 0;
}