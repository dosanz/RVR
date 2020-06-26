#include "Serializable.h"

#include <SFML/Graphics.hpp>
#include "string.h"

struct Carta {
    int id;
    int flipped;
    int posX, posY;
};

class Tablero : public Serializable
{
    public:
        Tablero();

        void barajaCartas();

        bool volteaCarta(int x, int y, int click);

        bool compruebaCartas();

        void render(sf::RenderWindow &window);

        void to_bin();

        int from_bin(char * bobj);

        bool getTurn();

        void swapTurn();

        sf::Vector2i findCoordinates(sf::Vector2i pos);
        

    private:

        bool turn = false;

        size_t BOARD_SIZE = 40 * sizeof(int) + sizeof(bool);

        Carta baraja[5][4];

        sf::Vector3i cartasVoleadas[2];

        sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(70, 110));

        sf::Color colorArray[9] { sf::Color(96, 169, 23), sf::Color(0, 171, 169), sf::Color(0, 80, 239), sf::Color(170, 0, 255), sf::Color(244, 114, 208),
                                    sf::Color(229, 9, 0), sf::Color(250, 104, 0), sf::Color(50, 200, 1), sf::Color(166, 194, 0)};
};