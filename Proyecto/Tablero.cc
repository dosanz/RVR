#include "Tablero.h"

Tablero::Tablero(){
    int count = 0;
    int incrX= 0, incrY=0;

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            baraja[i][j].id = count % 10;
            baraja[i][j].flipped = 0;
            baraja[i][j].posX = incrX + (70 * j);
            baraja[i][j].posY = incrY + (110 * i);
            count++;
            incrX += 5;
        }
        incrX = 0;
        incrY += 5;
    }
}

void Tablero::barajaCartas(){
    
    srand(time(NULL));

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            int m = rand()%5;
            int n = rand()%4;
            int tempid = baraja[i][j].id;
            baraja[i][j].id = baraja[m][n].id;
            baraja[m][n].id = tempid;
        }
    }
}

bool Tablero::volteaCarta(int x, int y, int click){
    if (baraja[x][y].flipped == 0){
        baraja[x][y].flipped = 1;

        cartasVoleadas[click].x = x;
        cartasVoleadas[click].y = y;
        cartasVoleadas[click].z = baraja[x][y].id;

        return true;
    }

    return false;
}

bool Tablero::compruebaCartas(){
    if (cartasVoleadas[0].z == cartasVoleadas[1].z){
        return true;
    }
    else{
        baraja[cartasVoleadas[0].x][cartasVoleadas[0].y].flipped = 0;
        baraja[cartasVoleadas[1].x][cartasVoleadas[1].y].flipped = 0;
        return false;
    }
}

void Tablero::render(sf::RenderWindow &window){
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            shape->setPosition(baraja[i][j].posX, baraja[i][j].posY);
            if (baraja[i][j].flipped == 0){
                shape->setFillColor(sf::Color::White);
            }
            
            else{
                shape->setFillColor(colorArray[baraja[i][j].id]);
            }

            window.draw(*shape);
            
        }
    }
}

void Tablero::to_bin(){

    alloc_data(BOARD_SIZE);
    char* tmp = _data;
    memset(tmp, 0, BOARD_SIZE);


    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            memcpy(tmp, static_cast<void*>(&baraja[i][j].id), sizeof(int));
            tmp += sizeof(int);
            
            memcpy(tmp, static_cast<void*>(&baraja[i][j].flipped), sizeof(int));
            tmp += sizeof(int);
        }
    }

    memcpy(tmp, static_cast<void*>(&turn), sizeof(bool));
    tmp += sizeof(bool);
}

int Tablero::from_bin(char * bobj){

    char* tmp = bobj;

    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 4; j++){
            int auxId, auxFlipped;

            memcpy(static_cast<int*>(&auxId), tmp, sizeof(int));
            baraja[i][j].id = auxId;

            tmp += sizeof(int);

            memcpy(static_cast<int*>(&auxFlipped), tmp, sizeof(int));
            baraja[i][j].flipped = auxFlipped;

            tmp += sizeof(int);
        }
    }

    bool auxTurn;

    memcpy(static_cast<bool*>(&auxTurn), tmp, sizeof(bool));
    turn = auxTurn;
    tmp += sizeof(bool);

    return 0;
}

sf::Vector2i Tablero::findCoordinates(sf::Vector2i pos){
    sf::Vector2i coordinates (-1, -1);
    bool found = false;
    int i = 0;
    while (i < 5 && !found){
        int j = 0;
        while (j < 4 && !found){
            if (pos.x >= baraja[i][j].posX && pos.x <= baraja[i][j].posX + 70 &&
            pos.y >= baraja[i][j].posY && pos.y <= baraja[i][j].posY + 110){
                found = true;
                coordinates.x = j;
                coordinates.y = i;
            }
                j++;
        }
        i++;
    }

    return coordinates;
}

bool Tablero::getTurn(){
    return turn;
}

void Tablero::swapTurn(){
    turn = !turn;
}