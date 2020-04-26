#include <sys/types.h> 
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

int main(int argc, char** argv){
    
    struct addrinfo hints;
    memset((void*)&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    struct addrinfo *res;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if(rc != 0){
        std::cout << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    for(struct addrinfo *it = res; it != nullptr ; it = it->ai_next){
        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];

        getnameinfo(it->ai_addr, it->ai_addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, 
        NI_NUMERICHOST | NI_NUMERICSERV);

        std::cout << host << " " << it->ai_family << " " << it->ai_socktype << std::endl;
    }
    freeaddrinfo(res);

    return 0;
}