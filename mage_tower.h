#ifndef MAGE_TOWER_H
#define MAGE_TOWER_H

#include "building.h"

    class mage_tower : public building
    {
    protected: 
        int location;
    public:
        mage_tower(int location);

        void upgrade() override;

        // void attack(int enemy) override;

        int get_location();
    };

#endif