#include "Game.h"


void Message::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    char* tmp = _data;
    memset(tmp, 0, MESSAGE_SIZE);

    memcpy (tmp, static_cast<void*>(&type), sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, static_cast<void*>(&row), sizeof(int));
    tmp += sizeof(int);

    memcpy(tmp, static_cast<void*>(&col), sizeof(int));
    tmp += sizeof(int);    
}

int Message::from_bin(char * bobj)
{
    char* tmp = bobj;

    memcpy (static_cast<void*>(&type), tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    int auxRow;
    memcpy(static_cast<void*>(&auxRow), tmp, sizeof(int));
    tmp += sizeof(int);
    row = auxRow;

    int auxCol;
    memcpy(static_cast<void*>(&auxCol), tmp, sizeof(int));
    tmp += sizeof(int);
    col = auxCol;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void Server::do_messages()
{
    while (true)
    {
        Message msg;
        Socket* s;
        socket.recv(msg, s);
        switch (msg.type){
            case Message::LOGIN:
            {
                clients.push_back(s);
                players++;

                if (players == 2)
                {
                    Tablero * t = new Tablero();
                    t->barajaCartas();
                    socket.send(*t, *clients.front());

                    t->swapTurn();
                    socket.send(*t, *clients.back());
                }

                break;
            }

            case Message::LOGOUT:
            {
                auto c = clients.begin();

                while (c != clients.end() && (*c) != s)
                    {c++;}

                if (c != clients.end()){
                    delete *c;
                    *c = nullptr;
                    clients.erase(c);
                }

                players--;
                break;
            }

            case Message::MESSAGE:
            {
                for (auto c : clients){
                    socket.send(msg, *c);
                }

                break;
            }
        }
    }
}

void Client::login()
{
    t = new Tablero();

    Message em(-1, -1);
    em.type = Message::LOGIN;

    socket.send(em, socket);
}

void Client::logout()
{
    std::cout << "Puntuacion: " << puntuacion << std::endl;

    Message em(-1, -1);
    em.type = Message::LOGOUT;

    socket.send(em, socket);

}

void Client::input_thread()
{

    while (window->isOpen())
    {
        sf::Event event;

        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window->close();
            }

            else if (window->isOpen() && event.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed && t->getTurn()){
                sf::Vector2i v = t->findCoordinates(sf::Mouse::getPosition(*window));
                Message em(v.x, v.y);
                em.type = Message::MESSAGE;
                socket.send(em, socket);
            }
        }
        
    }

    logout();
}

void Client::net_thread()
{

    socket.recv(*t);

    window->clear();
    t->render(*window);
    window->display();

    while (window->isOpen())
    {

        Message em;
        socket.recv(em);
        

        switch (em.type){
            case Message::MESSAGE:
            {
                if (cuentaClick < 2){
                        if (em.col != -1){
                            bool volteada = t->volteaCarta(em.col, em.row, cuentaClick);

                            if (volteada){
                                cuentaClick++;
                            }
                        }
                    }

                else if (cuentaClick == 2){
                    bool correcto = t->compruebaCartas();
                    cuentaClick = 0;

                    if (!correcto){
                        t->swapTurn();
                    }

                    else if (t->getTurn())
                    {
                        puntuacion++;
                    }
                }
                break;
            }
        }

        window->clear();
        t->render(*window);
        window->display();

    }
}