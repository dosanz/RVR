#include <string>
#include <unistd.h>
#include <string.h>
#include "Game.h"
#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>

extern "C" void * _client_thread(void *arg)
{
    Client * server = static_cast<Client *>(arg);

    server->net_thread();

    return 0;
}

int main(int argc, char **argv)
{
    XInitThreads();

    sf::RenderWindow window(sf::VideoMode(300, 575), "Memorama");

    Client ec(argv[1], argv[2], &window);

    pthread_attr_t attr;
    pthread_t id;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&id, &attr, _client_thread, static_cast<void *>(&ec));

    ec.login();

    ec.input_thread();
}

