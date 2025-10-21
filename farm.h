#ifndef FARM_H
#define FARM_H

    #include "building.h"

    class farm : public building
    {
    private: 
        int location;
        int farmRate;
    public:
        farm(int location);

        void upgrade() override;

        int get_farmRate();

        // void attack(int enemy) override;

        int get_location();
    };

#endif