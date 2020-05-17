#include "Serializable.h"

#include <iostream>
#include <fstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador : public Serializable
{
public:
    Jugador(const char* _n, int16_t _x, int16_t _y) :pos_x(_x), pos_y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador() {};

    void to_bin()
    {
        alloc_data(MAX_NAME * sizeof(char) + sizeof(int16_t) + sizeof(int16_t));
        
        char* tmp = _data;

        memcpy(tmp, static_cast<void*>(name), sizeof(char)*MAX_NAME);
        tmp += MAX_NAME * sizeof(char);

        memcpy(tmp, &pos_x, sizeof(int16_t));
        tmp += sizeof(int16_t);

        memcpy(tmp, &pos_y, sizeof(int16_t));
        tmp += sizeof(int16_t);
    }

    int from_bin(char* data)
    {
        char* tmp = data;

        memcpy(name, tmp, MAX_NAME * sizeof(char));
        tmp += MAX_NAME * sizeof(char);

        memcpy(&pos_x, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);

        memcpy(&pos_y, tmp, sizeof(int16_t));

        return 0;
    }

    void printInfo() {
        std::cout << "name: " << name << " pos_x: " << pos_x << " pos_y: " << pos_y << std::endl;
    }


private:
    static const size_t MAX_NAME = 80;

    int16_t pos_x;
    int16_t pos_y;

    char name[MAX_NAME];

};

int main(int argc, char** argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    one_w.to_bin();
    std::fstream file;
    file.open("one_w", std::fstream::out);
    file.write(one_w.data(), one_w.size());
    file.close();

    one_r.from_bin(one_w.data());
    one_r.printInfo();

    //Serializar y escribir one_w en un fichero
    //Leer el fichero en un buffer y "deserializar" en one_r
    //Mostrar el contenido one_r

    return 0;
}