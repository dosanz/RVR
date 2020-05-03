#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

int main(int argc, char **argv)
{
    struct addrinfo hints;
    struct addrinfo * res;

    memset((void*)&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
        return -1;
    }

    int sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if ( bind(sd, res->ai_addr, res->ai_addrlen) != 0 )
    {
        std::cerr << "bind: " << std::endl;
        return -1;
    }

    freeaddrinfo(res);

    listen(sd, 16);

    bool run = true;

    while (true){
        char buffer[80];

        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        struct sockaddr client_addr;
        socklen_t client_len = sizeof(struct sockaddr);

        int sd_client = accept(sd, &client_addr, &client_len);

        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        std::cout << "conexion desde: " << host << " " << service << std::endl;

        run = true;

        while (run){
            ssize_t bytes = recv(sd_client, (void *) buffer, sizeof(char)*80, 0);

            if (buffer[0] == 'q' && bytes <= 2 || bytes <= 1) {
                run = false;
            }
            else
                send(sd_client, (void*)buffer, bytes, 0);
        }

        std::cout << "Conexion terminada" << std::endl;
    }

    return 0;
}