#include <string>
#include <unistd.h>
#include <string.h>
#include <vector>

#include "Serializable.h"
#include "Socket.h"
#include "Tablero.h"

#include <SFML/Graphics.hpp>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class Message: public Serializable
{
public:
    static const size_t MESSAGE_SIZE = sizeof(uint8_t) + 2 * sizeof(int);

    enum MessageType
    {
        LOGIN   = 0,
        MESSAGE = 1,
        LOGOUT  = 2
    };

    Message(){};

    Message(const int& r, const int& c):row(r), col(c){};

    void to_bin();

    int from_bin(char * bobj);

    uint8_t type;

    std::string message;
    
    
    int row;
    int col;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el servidor de chat
 */
class Server
{
public:
    Server(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };

    /**
     *  Thread principal del servidor recibe mensajes en el socket y
     *  lo distribuye a los clientes. Mantiene actualizada la lista de clientes
     */
    void do_messages();

private:

    int players = 0;

    /**
     *  Lista de clientes conectados al servidor, representados por
     *  su socket
     */
    std::vector<Socket *> clients;

    /**
     * Socket del servidor
     */
    Socket socket;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Clase para el cliente de chat
 */
class Client
{
public:
    /**
     * @param s dirección del servidor
     * @param p puerto del servidor
     */
    Client(const char * s, const char * p, sf::RenderWindow* w):socket(s, p), window(w){};

    /**
     *  Envía el mensaje de login al servidor
     */
    void login();

    /**
     *  Envía el mensaje de logout al servidor
     */
    void logout();

    /**
     *  Rutina principal para el Thread de E/S.
     */
    void input_thread();

    /**
     *  Rutina del thread de Red. Recibe datos de la red y los "renderiza"
     *  en STDOUT
     */
    void net_thread();

private:

    int puntuacion = 0;

    int cuentaClick = 0;

    Tablero* t;

    sf::RenderWindow* window;

    /**
     * Socket para comunicar con el servidor
     */

    Socket socket;
};