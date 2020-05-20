#include "Chat.h"

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
        
    char* tmp = _data;

    memcpy(tmp, static_cast<void*>(&type), sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(tmp, nick.c_str(), sizeof(char)*8);
    tmp += sizeof(char)*8;

    memcpy(tmp, message.c_str(), sizeof(char)*80);
    tmp += sizeof(char)*80;
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    char* tmp = _data;

    memcpy(&type, tmp, sizeof(uint8_t));
    tmp += sizeof(uint8_t);

    memcpy(&nick, tmp, sizeof(char)*8);
    tmp += sizeof(char)*8;

    memcpy(&message, tmp, sizeof(char)*80);
    tmp += sizeof(char)*80;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        ChatMessage msg;
        Socket* s;
        socket.recv(msg, s);
        switch (msg.type){
            case ChatMessage::LOGIN:
            {
                clients.push_back(s);
                std::cout << msg.nick.c_str() << " se ha conectado." << std::endl;
                break;
            }

            case ChatMessage::LOGOUT:
            {
                auto c = clients.begin();

                while (c != clients.end() && (*c) != s)
                    c++;

                    if (c != clients.end()){
                        std::cout << msg.nick.c_str() << " se ha desconectado." << std::endl;
                        delete *c;
                        *c = nullptr;
                        clients.erase(c);
                    }
                break;
            }

            case ChatMessage::MESSAGE:
            {
                for (auto c : clients)
                    if(!(*c == *s)) socket.send(msg, *c);
                break;
            }
        }
    }
}

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);

}

void ChatClient::input_thread()
{
    while (true)
    {

        std::string msg;
        std::getline(std::cin, msg);

        if (msg == "q")
            logout();

        ChatMessage em(nick, msg);
        em.type = ChatMessage::MESSAGE;

        socket.send(em, socket);
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        ChatMessage em;
        socket.recv(em);

        if (em.type == ChatMessage::MESSAGE)
            std::cout << em.nick << ": " << em.message << std::endl;
    }
}

