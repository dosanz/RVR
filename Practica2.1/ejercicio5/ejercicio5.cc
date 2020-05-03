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

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char buffer[80];

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    struct sockaddr src_addr;
    socklen_t addrlen = sizeof(struct sockaddr);

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    int sd_server = connect(sd, res->ai_addr, res->ai_addrlen);

    bool run = true;

    while(run){
        std::cin >> buffer;

        if (strcmp("q", buffer) != 0)
        {
            send(sd, (void*)buffer, sizeof(buffer), 0);
            ssize_t bytes = recv(sd, (void*)buffer, sizeof(buffer), 0);
            std::cout << buffer << std::endl;
        }
        else{
            run = false;
        }

    }

    freeaddrinfo(res);

    return 0;
}