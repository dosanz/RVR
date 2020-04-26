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

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

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

    bool run = true;

    while (run)
    {

        char buffer[80];
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        struct sockaddr client_addr;
        socklen_t client_len = sizeof(struct sockaddr);

        ssize_t bytes = recvfrom(sd, (void*)buffer, sizeof(buffer), 0, &client_addr,
        &client_len);

        if (bytes == -1)
        {
            std::cerr << "recvfrom: " << std::endl;
            return -1;
        }

        getnameinfo(&client_addr, client_len, host, NI_MAXHOST, service,
        NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        time_t t = time(NULL);
        struct tm* tiempo;
        char bufferTiempo[10];
        tiempo = localtime(&t);

        switch (buffer[0]) {
        case 't':
            strftime(bufferTiempo, sizeof(bufferTiempo), "%T", tiempo);
            sendto(sd, bufferTiempo, sizeof(bufferTiempo), 0, &client_addr, client_len);
            break;
        case 'd':
            strftime(bufferTiempo, sizeof(bufferTiempo), "%D", tiempo);
            sendto(sd, bufferTiempo, sizeof(bufferTiempo), 0, &client_addr, client_len);
            break;
        case 'q':
            run = false;
            std::cout << "Saliendo..." << std::endl;
            break;
        default:
            std::cout << "Comando no soportado: " << buffer[0] << std::endl;
            break;
        }

        if (run) {
            std::cout << bytes << " bytes de " << host << ":" << service << std::endl;
        }

    }

    freeaddrinfo(res);

    return 0;
}