#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <random>
#include <iostream>
#include <cstdlib>
#include "bot.hpp"
#include <string>
using namespace std;

class Player{
    public:
        bool defending = false;
        int hp = 100;
        int atk = 50;
        int def = 20;
        int agl = 30;
        int defIncrement;
        string name = "You";

        /*void attack(AIClass& ai){
            // Calculate agility ratio for dodge chance
            double dynamicDamage;
            double dodgeChance = static_cast<double>(ai.agl) / (ai.agl + agl);

            // Check if defender dodges the attack
            if (static_cast<double>(rand()) / RAND_MAX < dodgeChance) {
                dynamicDamage = 0; // Defender dodges the attack
                cout<<ai.name<<" dodged the attack!";

            }else{

            // Calculate base damage
            double baseDamage = atk - ai.def;

            if(ai.defending == true){
                ai.defending = false;
                ai.def -= ai.defIncrement;
                ai.defIncrement = 0;
            }

            // Ensure there's at least a base damage multiplier
            if (baseDamage <= 0) {
                baseDamage = 1;
            }

            // Scale damage based on HP
            double hpRatio = static_cast<double>(hp) / (hp + ai.hp);
            dynamicDamage = baseDamage * hpRatio;
            int finalDamage = static_cast<int>(round(dynamicDamage));
            ai.hp -= finalDamage;

            cout<<"You dealt "<<to_string(finalDamage)<<" damage to "<<ai.name<<"!"<<endl;
            }
        }*/

        void defend(){
            random_device rd;
            mt19937 gen(rd());

            //Distribution, like in range()
            uniform_int_distribution<> dist(7, 14);
            defIncrement = dist(gen); //choose random number
            defending = true;
            def += defIncrement;
            cout<<"Increased defense by "<<to_string(defIncrement)<<" points!"<<endl<<endl;
        }

        void run(){
            hp = 0;
            cout<<"Leaving the battlefield..."<<endl<<endl;
        }

        void skip(){
            cout<<"Skipped the turn!"<<endl<<endl;
        }

        void show(){
            cout<<endl<<"--PLAYER STATS--"<<endl;
            cout<<"HP: "<<hp<<endl;
            cout<<"ATK: "<<atk<<endl;
            cout<<"DEF: "<<def<<endl;
            cout<<"AGL: "<<agl<<endl;
            cout<<endl;
        }
};

#endif