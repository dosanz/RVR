#include <string.h>

#include "Serializable.h"
#include "Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    struct addrinfo hints;
    struct addrinfo * res;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(address, port, &hints, &res);

    if ( rc != 0 )
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rc) << std::endl;
    }

    sd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    sa =  *res->ai_addr; 
    sa_len = res->ai_addrlen;

    freeaddrinfo(res);

}

int Socket::recv(Serializable &obj, Socket * &sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &sa, &sa_len);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    obj.to_bin();
    
    int bytes = sendto(sd, obj.data(), MAX_MESSAGE_SIZE, 0, &sock.sa, sock.sa_len);

    if (bytes <= 0)
        return -1;
    
    return 0;
}

bool operator== (const Socket &s1, const Socket &s2)
{
    struct sockaddr_in *_s1 = (struct sockaddr_in*)(&s1.sa);
    struct sockaddr_in *_s2 = (struct sockaddr_in*)(&s2.sa);

    return (_s1->sin_family == _s2->sin_family && _s1->sin_addr.s_addr == _s2->sin_addr.s_addr &&
        _s1->sin_port == _s2->sin_port);
    
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};

