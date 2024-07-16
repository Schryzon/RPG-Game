#ifndef BOT_HPP
#define BOT_HPP
#include <random>
#include <iostream>
#include <cstdlib>
#include "player.hpp"
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class AIClass{
    public:
        bool defending = false;
        string name;
        string desc;
        int hp;
        int atk;
        int def;
        int agl;
        int defIncrement;

        /*void attack(Player& player){
            // Calculate agility ratio for dodge chance
            double dynamicDamage;
            double dodgeChance = static_cast<double>(player.agl) / (player.agl + agl);

            // Check if defender dodges the attack
            if (static_cast<double>(rand()) / RAND_MAX < dodgeChance) {
                dynamicDamage = 0; // Defender dodges the attack
                cout<<"You dodged the attack!";

            }else{

            // Calculate base damage
            double baseDamage = atk - player.def;

            if(player.defending == true){
                player.defending = false;
                player.def -= player.defIncrement;
                player.defIncrement = 0;
            }

            // Ensure there's at least a base damage multiplier
            if (baseDamage <= 0) {
                baseDamage = 1;
            }

            // Scale damage based on HP
            double hpRatio = static_cast<double>(hp) / (hp + player.hp);
            dynamicDamage = baseDamage * hpRatio;
            int finalDamage = static_cast<int>(round(dynamicDamage));
            player.hp -= finalDamage;

            cout<<name<<" dealt "<<to_string(finalDamage)<<" damage to you!"<<endl;
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

        void setup(const json& enemy){
            size_t array_size = enemy.size();

            // Initialize random number generator
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib(0, array_size - 1);

            // Generate a random index
            int random_index = distrib(gen);

            // Accessing the randomly chosen object
            json result = enemy[random_index];

            name = result["name"];
            desc = result["desc"];
            hp = result["hp"];
            atk = result["atk"];
            def = result["def"];
            agl = result["agl"];
        }

        void show(){
            cout<<endl<<"--AI STATS--"<<endl;
            cout<<"NAME: "<<name<<endl;
            cout<<"DESC: "<<desc<<endl;
            cout<<"HP: "<<hp<<endl;
            cout<<"ATK: "<<atk<<endl;
            cout<<"DEF: "<<def<<endl;
            cout<<"AGL: "<<agl<<endl;
        }

        string decide(Player& player){
            int attackMood = 0;
            int defendMood = 0;
            int escapeMood = 0;
            int skipMood = 0;

            if(player.hp >= hp){
                defendMood += 10;
                escapeMood += 5;
            }else{
                attackMood += 20;
                defendMood += 7;
                skipMood += 8;
            }

            if(player.atk > atk){
                attackMood += 4;
                defendMood += 12;
                escapeMood += 15;
            }else{
                attackMood += 15;
                defendMood += 10;
                skipMood += 3;
            }

            vector<pair<int, string>> moods = {
                {attackMood, "attack"},
                {defendMood, "defend"},
                {escapeMood, "run"},
                {skipMood, "skip"}
            };

            // Sort moods in descending order
            std::sort(moods.begin(), moods.end(), [](const auto& a, const auto& b) {
                return a.first > b.first;
            });

            // Check if there are at least two moods to choose from
            if (moods.size() >= 3) {
                // Randomly pick one of the two highest moods
                srand(static_cast<unsigned int>(time(0))); // Seed for randomness
                int randomIndex = rand() % 3;
                return moods[randomIndex].second;
            } else {
                return moods[0].second;
            }
        }
};

#endif