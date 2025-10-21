#ifndef PLAYER_H
#define PLAYER_H
class player
{
private:
    // int waveNumber = 1;
    int difficulty;
    int health;
    int money;
    // int multiplier;
public:
    player();

    player(int difficulty);

    //~player();    just in case

    // int get_waveNumber();

    int get_health() const;    
    int get_money() const;      
    int get_difficulty() const;

    // int get_multiplier();

    //void set_health(int health);      for if we add health potions or such

    // void next_wave();

    void add_money(int money);

    void add_health(int health);
    void set_money(int money);
    void set_health(int health);


};

#endif