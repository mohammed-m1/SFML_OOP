#ifndef BALLISTA_H
#define BALLISTA_H

#include "building.h"

    class ballista : public building
    {
    private: 
        int location;
    public:
        ballista(int location);

        void upgrade() override;

        // void attack(int enemy) override;

        int get_location();
    };

#endif