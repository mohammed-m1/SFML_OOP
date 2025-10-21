#ifndef OGRE_H
#define OGRE_H

    #include "enemy.h"
    #include <SFML/Graphics.hpp>

    class ogre : public enemy
    {
    private:
        int quantity;
    public:
        ogre(int quantity);

        // int get_quantity();
    };
    
#endif